#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_
#include "zf_common_headfile.h"

#if mode_fuya
 #define Motor_left_dir                   (P00_8)
 #define Motor_right_dir                  (P00_9)
 #define Motor_left_pwm                   (ATOM2_CH2_P11_3)
 #define Motor_right_pwm                  (ATOM2_CH0_P13_3)

 #define ENCODER_2                   (TIM3_ENCODER)
 #define ENCODER_2_A                 (TIM3_ENCODER_CH1_P02_6)
 #define ENCODER_2_B                 (TIM3_ENCODER_CH2_P02_7)

 #define ENCODER_1                   (TIM5_ENCODER)
 #define ENCODER_1_A                 (TIM5_ENCODER_CH1_P21_7)
 #define ENCODER_1_B                 (TIM5_ENCODER_CH2_P21_6)
#endif

#if !mode_fuya
//  #define Motor_left_dir                   (P00_9)
//  #define Motor_right_dir                  (P00_8)
//  #define Motor_left_pwm                   (ATOM2_CH0_P13_3)
//  #define Motor_right_pwm                  (ATOM2_CH2_P11_3)

//  #define ENCODER_1                   (TIM3_ENCODER)
//  #define ENCODER_1_A                 (TIM3_ENCODER_CH1_P02_6)
//  #define ENCODER_1_B                 (TIM3_ENCODER_CH2_P02_7)

//  #define ENCODER_2                   (TIM2_ENCODER)
//  #define ENCODER_2_A                 (TIM2_ENCODER_CH1_P33_7)
//  #define ENCODER_2_B                 (TIM2_ENCODER_CH2_P33_6)
// #endif

//  #define fuya_left_pwm                    (ATOM2_CH1_P33_5)
//  #define fuya_wushua_pwm                  (ATOM0_CH0_P21_2)

 #define Motor_left_dir                   (P00_8)
 #define Motor_right_dir                  (P00_9)
 #define Motor_left_pwm                   (ATOM2_CH2_P11_3)
 #define Motor_right_pwm                  (ATOM2_CH0_P13_3)

 #define ENCODER_1                   (TIM3_ENCODER)
 #define ENCODER_1_A                 (TIM3_ENCODER_CH1_P02_6)
 #define ENCODER_1_B                 (TIM3_ENCODER_CH2_P02_7)

 #define ENCODER_2                   (TIM2_ENCODER)
 #define ENCODER_2_A                 (TIM2_ENCODER_CH1_P33_7)
 #define ENCODER_2_B                 (TIM2_ENCODER_CH2_P33_6)
#endif

 #define fuya_left_pwm                    (ATOM2_CH1_P33_5)
 #define fuya_wushua_pwm                  (ATOM0_CH0_P21_2)



//#define Motor_left_dir                   (P13_3)
//#define Motor_right_dir                  (P11_6)
//#define Motor_left_pwm                   (ATOM2_CH1_P11_2)
//#define Motor_right_pwm                  (ATOM2_CH2_P11_3)
//#define ENCODER_2                   (TIM6_ENCODER)
//#define ENCODER_2_A                 (TIM6_ENCODER_CH1_P20_3)
//#define ENCODER_2_B                 (TIM6_ENCODER_CH2_P20_0)
//
//#define ENCODER_1                   (TIM5_ENCODER)
//#define ENCODER_1_A                 (TIM5_ENCODER_CH1_P21_7)
//#define ENCODER_1_B                 (TIM5_ENCODER_CH2_P21_6)

//������pwmֵ�޷�
#define MOTOR_OUT_MAX         10000
#define MOTOR_OUT_MIN        -10000

typedef struct
{
   float Pulse;     //������������
   float encoder_temp;  //���߲���
   float PulseSum;  //���������ۼ�
   int16_t Count;
}encoder_type;



void Motor_Init(void);
void Encoder_Init(void);
void Ljy_set_motor_pwm(int l_pwm, int r_pwm);
void DRV_Set_PWM(int fLeftVoltage,int fRightVoltage);

void Brushless_Set_PWM(float Left_PWM, float Right_PWM);

void Motor_Stop(void);

#endif /* CODE_MOTOR_H_ */
