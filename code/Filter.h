#ifndef CODE_FILTER_H_
#define CODE_FILTER_H_

typedef struct{

    float fd;
    float fs;

    float ohm;
    float c;

    float _b0;
    float _b1;
    float _b2;

    float _a1;
    float _a2;

    float IIR_Delay_Element_0;
    float IIR_Delay_Element_1;
    float IIR_Delay_Element_2;

}IIR2_Filter;
extern IIR2_Filter IIR2_Gyrox, IIR2_Gyroy, IIR2_Gyroz;


typedef struct {
    float q;    // ��������Э����
    float r;    // ��������Э����
    float x;    // ����ֵ
    float p;    // �������Э����
    float k;    // ����������
}KalmanFilter;
//extern KalmanFilter ;

typedef struct {
    float out_f;   // �˲����ڲ�״̬
} LPF_State;
extern LPF_State lpf_encoder_left;
extern LPF_State lpf_encoder_right;
extern LPF_State lpf_encoder_dis;


void My_LPF_1(float k,float in,float *out);

void LPF_1(float hz,float time,float in,float *out);

void LPF_1_int(float hz, float dt, int in, int *out, LPF_State *state);

void LPF_1_db(float hz,float time,double in,double *out);

void IIR_Filter(IIR2_Filter *fliter,float sample,float *Output);

void All_Filter_Init(void);

float KalmanFilter_Update(KalmanFilter* kf, float measurement, float acceleration);


#endif /* CODE_FILTER_H_ */
