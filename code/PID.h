#ifndef CODE_PID_H_
#define CODE_PID_H_


typedef struct
{
    float Kf;               // ǰ��ϵ����ѡ�ã�
    float Kp;               // ����ϵ��
    float Ki;               // ����ϵ��
    float Kd;               // ΢��ϵ��

    float Error;            // ��ǰ��Ŀ��ֵ - ����ֵ��
    float Last_Error;       // ��һ�����
    float Last_Feedback;    // ��һ�η���ֵ�����ڼ���΢�֣�
    float Integral;         // �������ۼ�ֵ
    float Differential;     // ΢�������ֵ

    float OutPut;           // PID���ֵ
    float MAX_Error;        // ���������ƣ���ѡ����ֹ������
    float MAX_Integral;     // �����޷�����ֹ���ֱ��ͣ�
    float MAX_OutPut;       // ����޷�������ִ���������Χ��
}Pos_PID;       // �ṹ������



typedef struct{
        float Kp;
        float Ki;
        float Kd;

        float Error;            //���
        float Last_Error;       //�ϴ����
        float Prev_Error;

        float Pout;
        float Iout;
        float Dout;
        float OutPut;

        float MAX_OutPut;

}Increment_PID;

typedef struct{
        float Kp1;
        float Kp2;
        float Kd;

        float Error;            //���
        float Last_Error;       //�ϴ����

        float P1out;
        float P2out;
        float Dout;
        float OutPut;

        float MAX_OutPut;

}DIR_PID;

//extern PID Speed;

void PID_Init(void);

float Pos_PID_Calculate(Pos_PID *pid, float expect, float feedback, float dt);

float Increment_PID_Caculate(Increment_PID *pid, int16_t Target, int16_t feedback);

#endif /* CODE_PID_H_ */
