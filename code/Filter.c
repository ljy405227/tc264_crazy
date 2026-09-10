#include "zf_common_headfile.h"
#include "Filter.h"

LPF_State lpf_encoder_left = {.out_f = 0.0f};
LPF_State lpf_encoder_right = {.out_f = 0.0f};
LPF_State lpf_encoder_dis = {.out_f = 0.0f};

// IIR filter instances
IIR2_Filter IIR2_Gyrox, IIR2_Gyroy, IIR2_Gyroz;

// First-order low-pass filter (simple)
void My_LPF_1(float k, float in, float *out)
{
    *out = k * in + (1 - k) * (*out);
}

/*************************************************************************
 *  Function : LPF_1
 *  Description : First-order low-pass filter for float data
 *  Parameters : hz - cutoff frequency, time - sampling interval,
 *               in - input value, out - filtered output pointer
 *  Return : None
 *************************************************************************/
void LPF_1(float hz, float time, float in, float *out)
{
    *out += (1 / (1 + 1 / (hz * 6.28f * time))) * (in - *out);
}

// First-order low-pass filter (integer variant with state tracking)
void LPF_1_int(float hz, float dt, int in, int *out, LPF_State *state)
{
    float tau = 1.0f / (2.0f * 3.1415926f * hz);
    float alpha = dt / (tau + dt);
    state->out_f += alpha * (in - state->out_f);
    *out = (int)state->out_f;
}

/*************************************************************************
 *  Function : LPF_1_db
 *  Description : First-order low-pass filter for double data
 *  Parameters : hz - cutoff frequency, time - sampling interval,
 *               in - input value, out - filtered output pointer
 *  Return : None
 *************************************************************************/
void LPF_1_db(float hz, float time, double in, double *out)
{
    *out += (1 / (1 + 1 / (hz * 6.28f * time))) * (in - *out);
}

/*************************************************************************
 *  Function : IIR_Filter_Init
 *  Description : Initialize IIR Butterworth filter
 *  Parameters : fliter - filter struct, hz - cutoff freq, time - sample time
 *  Return : None
 *************************************************************************/
void IIR_Filter_Init(IIR2_Filter *fliter, float hz, float time)
{
    fliter->ohm = tanf(3.1416 * hz * time);

    fliter->c = 1 + 1.414f * fliter->ohm + fliter->ohm * fliter->ohm;

    fliter->_b0 = fliter->ohm * fliter->ohm / fliter->c;

    fliter->_b1 = 2.0f * fliter->_b0;

    fliter->_b2 = fliter->_b0;

    fliter->_a1 = 2.0f * (fliter->ohm * fliter->ohm - 1.0f) / fliter->c;

    fliter->_a2 = (1.0f - 1.414f * fliter->ohm + fliter->ohm * fliter->ohm) / fliter->c;
}

/*************************************************************************
 *  Function : IIR_Filter
 *  Description : Apply IIR Butterworth filter
 *  Parameters : fliter - filter struct, in - input, out - filtered output
 *  Return : None
 *************************************************************************/
void IIR_Filter(IIR2_Filter *fliter, float in, float *out)
{
    fliter->IIR_Delay_Element_0 = in - fliter->IIR_Delay_Element_1 * fliter->_a1 - fliter->IIR_Delay_Element_2 * fliter->_a2;

    if (0)  // If filter value is abnormal, output raw value
    {
        fliter->IIR_Delay_Element_0 = in;
    }

    *out = fliter->IIR_Delay_Element_0 * fliter->_b0 + fliter->IIR_Delay_Element_1 * fliter->_b1 + fliter->IIR_Delay_Element_2 * fliter->_b2;

    fliter->IIR_Delay_Element_2 = fliter->IIR_Delay_Element_1;
    fliter->IIR_Delay_Element_1 = fliter->IIR_Delay_Element_0;
}

/*************************************************************************
 *  Function : KalmanFilter_Init
 *  Description : Initialize Kalman filter
 *  Parameters : kf - Kalman filter struct, Q - process noise, R - measurement noise
 *  Return : None
 *************************************************************************/
void KalmanFilter_Init(KalmanFilter *kf, float Q, float R)
{
    kf->q = Q;
    kf->r = R;
    kf->x = 0;
    kf->p = 100;
    kf->k = 0;
}

// Kalman filter update: prediction and measurement update
float KalmanFilter_Update(KalmanFilter *kf, float measurement, float acceleration)
{
    // Prediction step
    // State prediction: x = x + (acceleration * dt)
    kf->x = kf->x + acceleration * 0.005;

    // Covariance prediction: p = p + q
    kf->p = kf->p + kf->q;

    // Update step
    // Kalman gain: k = p / (p + r)
    kf->k = kf->p / (kf->p + kf->r);

    // State update: x = x + k * (measurement - x)
    kf->x = kf->x + kf->k * (measurement - kf->x);

    // Covariance update: p = (1 - k) * p
    kf->p = (1 - kf->k) * kf->p;

    return kf->x;
}

// Initialize all filters
void All_Filter_Init(void)
{
//  KalmanFilter_Init(&xxxx, 0.01, 1.0);
}