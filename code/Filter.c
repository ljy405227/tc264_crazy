#include "zf_common_headfile.h"
#include "Filter.h"

LPF_State lpf_encoder_left = {.out_f = 0.0f};
LPF_State lpf_encoder_right = {.out_f = 0.0f};
LPF_State lpf_encoder_dis = {.out_f = 0.0f};

//���幹�岢��ʼ������
IIR2_Filter IIR2_Gyrox, IIR2_Gyroy, IIR2_Gyroz;

//һ�׵�ͨ�˲�
void My_LPF_1(float k,float in,float *out)
{
    *out = k*in+(1-k)*(*out);
}

/*************************************************************************
 *  �������ƣ�void LPF_1(float hz,float time,float in,float *out)
 *  ����˵���������ݽ���һ�׵�ͨ�˲�
 *  ����˵����hz:����Ƶ�ʣ�time:����ʱ�䣬in:���룬out:���
 *  �������أ���
 *  ��    ע����float�������˲�
 *
 *************************************************************************/
void LPF_1(float hz,float time,float in,float *out)
{
    *out += ( 1 / ( 1 + 1 / ( hz *6.28f *time ) ) ) *( in - *out );
}


// �˲�������ʹ�ö���״̬
void LPF_1_int(float hz, float dt, int in, int *out, LPF_State *state)
{
    float tau = 1.0f / (2.0f * 3.1415926f * hz);
    float alpha = dt / (tau + dt);
    state->out_f += alpha * (in - state->out_f);
    *out = (int)state->out_f;
}

/*************************************************************************
 *  �������ƣ�void LPF_1_db(float hz,float time,double in,double *out)
 *  ����˵���������ݽ���һ�׵�ͨ�˲�
 *  ����˵����hz:����Ƶ�ʣ�time:����ʱ�䣬in:���룬out:���
 *  �������أ���
 *  ��    ע����double�������˲�
 *
 *************************************************************************/
void LPF_1_db(float hz,float time,double in,double *out)
{
    *out += ( 1 / ( 1 + 1 / ( hz *6.28f *time ) ) ) *( in - *out );
}



/*************************************************************************
 *  �������ƣ�void IIR_Filter_Init(IIR2_Filter *fliter, float hz, float time)
 *  ����˵��������IIR�˲���ʼ��
 *  ����˵��������IIR�˲�(������˹�˲�),hz:����Ƶ�ʣ�time:����ʱ��
 *  �������أ���
 *************************************************************************/
void IIR_Filter_Init(IIR2_Filter *fliter, float hz, float time)
{
    fliter->ohm = tanf(3.1416*hz*time);

    fliter->c = 1 + 1.414f*fliter->ohm + fliter->ohm*fliter->ohm;

    fliter->_b0 = fliter->ohm*fliter->ohm / fliter->c;

    fliter->_b1 = 2.0f * fliter->_b0;

    fliter->_b2 = fliter->_b0;

    fliter->_a1 = 2.0f*(fliter->ohm*fliter->ohm - 1.0f) / fliter->c;

    fliter->_a2 = (1.0f - 1.414f*fliter->ohm + fliter->ohm*fliter->ohm) / fliter->c;
}

/*************************************************************************
 *  �������ƣ�void IIR_Filter(IIR2_Filter *fliter,float in,float *out)
 *  ����˵��������IIR�˲�(������˹�˲�)
 *  ����˵����in:���룬out:���
 *  �������أ���
 *************************************************************************/
void IIR_Filter(IIR2_Filter *fliter,float in,float *out)
{
    fliter->IIR_Delay_Element_0 = in - fliter->IIR_Delay_Element_1*fliter->_a1 - fliter->IIR_Delay_Element_2*fliter->_a2;

    if(0)//���˲�ֵ�����жϣ�����˲�����������ԭʼֵ
    {
        fliter->IIR_Delay_Element_0 = in;
    }

    *out = fliter->IIR_Delay_Element_0*fliter->_b0 + fliter->IIR_Delay_Element_1*fliter->_b1 + fliter->IIR_Delay_Element_2*fliter->_b2;

    fliter->IIR_Delay_Element_2 = fliter->IIR_Delay_Element_1;
    fliter->IIR_Delay_Element_1 = fliter->IIR_Delay_Element_0;
}

/*************************************************************************
 *  �������ƣ�void KalmanFilter_Init(KalmanFilter* kf, float Q, float R)
 *  ����˵�����������˲���ʼ��
 *  ����˵����Q:����������R:��������
 *  �������أ���
 *************************************************************************/
//KalmanFilter xxxx;    //�������˲������ṹ��
void KalmanFilter_Init(KalmanFilter* kf, float Q, float R)
{
    kf->q = Q;
    kf->r = R;
    kf->x = 0;
    kf->p = 100;
    kf->k = 0;
}



// �������˲����£�Ԥ��Ͳ������£�
float KalmanFilter_Update(KalmanFilter* kf, float measurement, float acceleration)
{
    // Ԥ�ⲽ��
    // ״̬Ԥ��: x = x + (acceleration * dt)
    kf->x = kf->x + acceleration * 0.005;

    // ���Э����Ԥ��: p = p + q
    kf->p = kf->p + kf->q;

    // ���²���
    // �������������: k = p / (p + r)
    kf->k = kf->p / (kf->p + kf->r);

    // ״̬����: x = x + k * (measurement - x)
    kf->x = kf->x + kf->k * (measurement - kf->x);

    // ���Э�������: p = (1 - k) * p
    kf->p = (1 - kf->k) * kf->p;

    return kf->x;
}

//�����˲�����ʼ��
void All_Filter_Init(void)
{

//  KalmanFilter_Init(&xxxx, 0.01, 1.0);
}
