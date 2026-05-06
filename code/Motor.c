#include "zf_common_headfile.h"
#include "Motor.h"



void Motor_Init(void)
{
    gpio_init(Motor_left_dir, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(Motor_right_dir, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(P15_7, GPO, GPIO_LOW   , GPO_PUSH_PULL);
    gpio_init(P02_6, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    pwm_init(Motor_left_pwm, 17000, 0);
    pwm_init(Motor_right_pwm, 17000, 0);

    
    


}
void Encoder_Init(void)
{
    encoder_dir_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                      // ��ʼ��������ģ�������� ������������ģʽ
    encoder_dir_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                      // ��ʼ��������ģ�������� ������������ģʽ
}

void Ljy_set_motor_pwm(int l_pwm,int r_pwm)
{
    int Left_PWM, Right_PWM = 0;

    Left_PWM = l_pwm;
    Right_PWM = r_pwm;

    Left_PWM = LIMIT(Left_PWM,MOTOR_OUT_MIN,MOTOR_OUT_MAX);
    Right_PWM = LIMIT(Right_PWM,MOTOR_OUT_MIN,MOTOR_OUT_MAX);

    if(Left_PWM < 0)
    {
        gpio_set_level(Motor_left_dir, 0);
        pwm_set_duty(Motor_left_pwm, -Left_PWM);
    }
    else
    {
        gpio_set_level(Motor_left_dir, 1);
        pwm_set_duty(Motor_left_pwm, Left_PWM);
    }

    if(Right_PWM < 0)
    {
        gpio_set_level(Motor_right_dir, 1);
        pwm_set_duty(Motor_right_pwm, -Right_PWM);
    }
    else
    {
        gpio_set_level(Motor_right_dir, 0);
        pwm_set_duty(Motor_right_pwm, Right_PWM);
    }
}

void Brushless_Set_PWM(float Left_PWM, float Right_PWM)//������ˢ�����pwm
{
    float Left_P, Right_P = 0;

    Left_P = Left_PWM + 500;
    Right_P = Right_PWM + 500;

    Left_P = LIMIT(Left_P, 500, 1000);
    Right_P = LIMIT(Right_P, 500, 1000);

    pwm_set_duty(ATOM1_CH5_P02_5, Left_P);
    pwm_set_duty(ATOM1_CH4_P02_4, Right_P);

}

void Motor_Stop(void)
{
    Ljy_set_motor_pwm(0,0);
}




