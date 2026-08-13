#include "zf_common_headfile.h"
#include "Control.h"

uint16_t turn_time_count = 0;

TASK_POINT task_point = TASK_STRAIGHT;
LIMIT_CHECK limit_check = {0};          // 限幅溢出函数
PID_SPEED speed_pid = {0};              // 速度环PID

// 方向环PID -- 包含直行和左右转弯3套参数
PID_DIR pid_dir_pos = {0};
PID_DIR pid_dir_gyro = {0};
PID_DIR pid_dir_pos_left = {0};
PID_DIR pid_dir_gyro_left = {0};
PID_DIR pid_dir_pos_right = {0};
PID_DIR pid_dir_gyro_right = {0};


Encoder_Data encoder_left = {0};              // 左编码器数据
Encoder_Data encoder_right = {0};             // 右编码器数据

SpeedRamp_t ramp = {0};

PID_SPEED speed_left_pid_new = {0};              // 速度环PID=============================完全闭环=============================
PID_SPEED speed_right_pid_new = {0};             // 速度环PID=============================完全闭环=============================

volatile int speed_target = 0;
volatile int speed_target_max = 150;
volatile int speed_dis_original = 0;    // 速度差原始值
volatile int speed_dis_feedback = 0;    // 速度差反馈值
int speed_final_average = 0;            // 速度平均值
volatile float speed_dis_out = 0;       // 角度环的输出，用于速度差补偿
volatile float turn_yaw_begin = 0.0f;
volatile float turn_yaw_now = 0.0f;

volatile float dec = 0;
volatile float pos_d_err = 0;
volatile float d_gyro = 0;
volatile float dir_dec = 0;
volatile int yichu_count = 0;
volatile float encoder_all_left = 0;
volatile float encoder_all_right = 0;
volatile float speed_turn = 35.0f;

// 发车前初始化
void varyble_init(void)
{
    Pid_Init();
    encoder_right.encoder_count_original = 0;
    encoder_right.encoder_count = 0;
    encoder_left.encoder_count_original = 0;
    encoder_left.encoder_count = 0;
    encoder_clear_count(ENCODER_1);
    encoder_clear_count(ENCODER_2);
    
    speed_dis_original = 0;
    speed_dis_feedback = 0;
    speed_final_average = 0;
    speed_dis_out = 0;
    yichu_count = 0;
}

void Pid_Init(void)
{
    // speed_target = 0;
    // speed_target_max = (int)Flash.tar_speed;

    // speed_pid.Kp = Flash.speed_kp;
    // speed_pid.Ki = Flash.speed_ki;
    // speed_pid.Kd = Flash.speed_kd;
    // speed_pid.err = 0;
    // speed_pid.err_last = 0;
    // speed_pid.out = 0;
    // speed_pid.dec_limit = 6500;
    // speed_pid.out_limit = 6500;


    // pid_dir_gyro.Kp =  Flash.gyro_kp;
    // pid_dir_gyro.Kd =  Flash.gyro_kd;
    // pid_dir_gyro.Ki = 0;
    // pid_dir_gyro.err = 0;
    // pid_dir_gyro.err_last = 0;
    // pid_dir_gyro.out = 0;

    // pid_dir_pos.Kp =  Flash.dir_kp;
    // pid_dir_pos.Kd =  Flash.dir_kd;
    // pid_dir_pos.err = 0;
    // pid_dir_pos.err_last = 0;
    // pid_dir_pos.out = 0;

    // pid_dir_gyro_left.Kp =  Flash.gyro_kp_left;
    // pid_dir_gyro_left.Kd =  Flash.gyro_kd_left;
    // pid_dir_gyro_left.Ki = 0;
    // pid_dir_gyro_left.err = 0;
    // pid_dir_gyro_left.err_last = 0;
    // pid_dir_gyro_left.out = 0;

    // pid_dir_pos_left.Kp =  Flash.dir_kp_left;
    // pid_dir_pos_left.Kd =  Flash.dir_kd_left;
    // pid_dir_pos_left.err = 0;
    // pid_dir_pos_left.err_last = 0;
    // pid_dir_pos_left.out = 0;

    // pid_dir_gyro_right.Kp =  Flash.gyro_kp_right;
    // pid_dir_gyro_right.Kd =  Flash.gyro_kd_right;
    // pid_dir_gyro_right.Ki = 0;
    // pid_dir_gyro_right.err = 0;
    // pid_dir_gyro_right.err_last = 0;
    // pid_dir_gyro_right.out = 0;

    // pid_dir_pos_right.Kp =  Flash.dir_kp_right;
    // pid_dir_pos_right.Kd =  Flash.dir_kd_right;
    // pid_dir_pos_right.err = 0;
    // pid_dir_pos_right.err_last = 0;
    // pid_dir_pos_right.out = 0;


    // ramp.speed_target = 0.0f;
    // ramp.speed_target_max = Flash.tar_speed;
    // ramp.ramp_time = Flash.other_ramp_time;
    // ramp.elapsed_time = 0.0f;

    // speed_turn = Flash.other_turn_speed;


    speed_target = 0;
    speed_target_max = 190;

    speed_pid.Kp = 70;
    speed_pid.Ki = 0.56;
    speed_pid.err = 0;
    speed_pid.err_last = 0;
    speed_pid.out = 0;
    speed_pid.dec_limit = 5000;
    speed_pid.out_limit = 6500;


    pid_dir_gyro.Kp =  2.5;
    pid_dir_gyro.Kd =  0.15;
    pid_dir_gyro.Ki = 0;
    pid_dir_gyro.err = 0;
    pid_dir_gyro.err_last = 0;
    pid_dir_gyro.out = 0;

    pid_dir_pos.Kp =  18.10;
    pid_dir_pos.Kd =  0.66;
    pid_dir_pos.err = 0;
    pid_dir_pos.err_last = 0;
    pid_dir_pos.out = 0;

    pid_dir_gyro_left.Kp = 3.08;
    pid_dir_gyro_left.Kd =  0.50;
    pid_dir_gyro_left.Ki = 0;
    pid_dir_gyro_left.err = 0;
    pid_dir_gyro_left.err_last = 0;
    pid_dir_gyro_left.out = 0;

    pid_dir_pos_left.Kp =  22.10;
    pid_dir_pos_left.Kd =  0.86;
    pid_dir_pos_left.err = 0;
    pid_dir_pos_left.err_last = 0;
    pid_dir_pos_left.out = 0;

    pid_dir_gyro_right.Kp =  3.08;
    pid_dir_gyro_right.Kd =  0.70;
    pid_dir_gyro_right.Ki = 0;
    pid_dir_gyro_right.err = 0;
    pid_dir_gyro_right.err_last = 0;
    pid_dir_gyro_right.out = 0;

    pid_dir_pos_right.Kp =  22.10;
    pid_dir_pos_right.Kd =  0.86;
    pid_dir_pos_right.err = 0;
    pid_dir_pos_right.err_last = 0;
    pid_dir_pos_right.out = 0;


    ramp.speed_target = 0.0f;
    ramp.speed_target_max = 190;
    ramp.ramp_time = 0.2;
    ramp.elapsed_time = 0.0f;

    speed_turn = 195.0f;
}

// 速度环增量式PID公式
void PID_Speed_Control(PID_SPEED *pid, int target, int measure)
{

    //1 保存上次误差
    pid->err_last = pid->err;
    //2 当前误差
    pid->err = target - measure;
    //3 增量PI
    dec = pid->Kp * (pid->err - pid->err_last)
        + pid->Ki * pid->err;
    //4 增量限幅
    if(dec > pid->dec_limit) 
    {
        dec = pid->dec_limit;
        limit_check.speed_dec += 1;
    }
    if(dec < -pid->dec_limit) 
    {
        dec = -pid->dec_limit;
        limit_check.speed_dec += 1;
    }
    //5 输出累加
    pid->out += dec;
    //6 输出限幅
    if(pid->out > pid->out_limit)  
    {
        pid->out = pid->out_limit;
        limit_check.speed_out += 1;
    }
    if(pid->out < -pid->out_limit) 
    {
        pid->out = -pid->out_limit;
        limit_check.speed_out += 1;
    }
}

volatile float expect_gyro = 0;
// 方向环PD公式
void PID_Direction_Control(PID_DIR *pos_pid, PID_DIR *gyro_pid , float err_position)
{
    // static float dec = 0;
    /*==================== 1. 位置环（PD） ====================*/
    // 保存上次误差
    pos_pid->err_last = pos_pid->err;
    // 当前误差
    pos_pid->err = err_position;
    // 微分（带简单抗突变）
    pos_d_err = pos_pid->err - pos_pid->err_last;
    if(pos_d_err > 20.3f)
    {
        limit_check.dir_d_err += 1;
        pos_d_err = 20.3f;   // 抑制突变
    }

    if(pos_d_err < -20.3f)
    {
        limit_check.dir_d_err += 1;
        pos_d_err = -20.3f;   // 抑制突变
    }
    // PD 输出（目标角速度）
    expect_gyro = pos_pid->Kp * pos_pid->err
                + pos_pid->Kd * pos_d_err;
    // 限幅
    if(expect_gyro > limit_gyro)  {expect_gyro = limit_gyro;limit_check.dir_expect_gyro += 1;}
    if(expect_gyro < -limit_gyro) {expect_gyro = -limit_gyro;limit_check.dir_expect_gyro += 1;}
     if (turn_count == 27 )
     {
        if (task_point == TASK_TURN_RIGHT) expect_gyro = 1180;
        else if (task_point == TASK_TURN_LEFT) expect_gyro = -1180;
     }
     if (turn_count == 28 || turn_count == 29)
     {
        if (task_point == TASK_TURN_RIGHT) expect_gyro = 1500;
        else if (task_point == TASK_TURN_LEFT) expect_gyro = -1500;
     }
     else
     {
         if (task_point == TASK_TURN_RIGHT) expect_gyro = 1350;
         else if (task_point == TASK_TURN_LEFT) expect_gyro = -1350;
     }

    /*==================== 2. 角速度环（PD） ====================*/
    // 保存上次误差
    gyro_pid->err_last = gyro_pid->err;
    // 当前误差
    gyro_pid->err = - expect_gyro - avl_gyro_z;  // 左偏了expect_gyro为正
    // pid_dir_gyro.err = 0 - avl_gyro_z; 
    // PD（这里也可以改成 PI，看需求） 
    d_gyro = gyro_pid->err - gyro_pid->err_last;
    if(d_gyro > 300)  {d_gyro = 300;limit_check.d_gyro += 1;}
    if(d_gyro < -300) {d_gyro = -300;limit_check.d_gyro += 1;}
    dir_dec = gyro_pid->Kp * gyro_pid->err
            + gyro_pid->Kd * d_gyro;
    // 输出
    gyro_pid->out = dir_dec;
    // 限幅
    if(gyro_pid->out > 6500)  {gyro_pid->out = 6500;limit_check.dir_out += 1;}
    if(gyro_pid->out < -6500) {gyro_pid->out = -6500;limit_check.dir_out += 1;}

    // 最终输出
    speed_dis_out = gyro_pid->out;
}

// 每1ms调用一次
float SpeedRamp_Update(SpeedRamp_t *ramp)
{
    // 1ms = 0.001s
    ramp->elapsed_time += 0.001f;

    float t = ramp->elapsed_time / ramp->ramp_time;
    if (t >= 1.0f)
    {
        t = 1.0f;
        ramp->speed_target = ramp->speed_target_max;
    }
    else
    {
        ramp->speed_target = ramp->speed_target_max * (3.0f * t * t - 2.0f * t * t * t);
    }

    return ramp->speed_target;
}


uint8_t zuo_ing = 0;
uint8_t you_ing = 0;
float yaw_turn_first = 0.0f;

uint8_t turn_count = 0;

float angle_diff(float current, float target)
{
    float diff = current - target;
    while (diff > 180)  diff -= 360;
    while (diff < -180) diff += 360;
    return diff;
}

#define guandao 0


void ljy_isr_headle(void)
{
#if !guandao
    if (turning_state == 1)
    {
        turning_state = 0;
        turn_time_count = 0;
        yaw_turn_first = yaw;
        zuo_ing = 1;
        turn_count++;

        pid_dir_gyro.err = 0;
        pid_dir_gyro.err_last = 0;
        pid_dir_gyro.out = 0;
        pid_dir_pos.err = 0;
        pid_dir_pos.err_last = 0;
        pid_dir_pos.out = 0;
        pid_dir_gyro_left.err = 0;
        pid_dir_gyro_left.err_last = 0;
        pid_dir_gyro_left.out = 0;
        pid_dir_pos_left.err = 0;
        pid_dir_pos_left.err_last = 0;
        pid_dir_pos_left.out = 0;

        pid_dir_gyro_right.err = 0;
        pid_dir_gyro_right.err_last = 0;
        pid_dir_gyro_right.out = 0;
        pid_dir_pos_right.err = 0;
        pid_dir_pos_right.err_last = 0;
        pid_dir_pos_right.out = 0;
    }
    if (zuo_ing == 1)
    {
        float diff = angle_diff(yaw, yaw_turn_first);
        if (turn_count == 27)
        {
            if (diff >= 77 || diff <= -77 )  // 左转达到 65 度
            {
                end_turning_state = 1;
                zuo_ing = 0;

                pid_dir_gyro.err = 0;
                pid_dir_gyro.err_last = 0;
                pid_dir_gyro.out = 0;
                pid_dir_pos.err = 0;
                pid_dir_pos.err_last = 0;
                pid_dir_pos.out = 0;

                pid_dir_gyro_left.err = 0;
                pid_dir_gyro_left.err_last = 0;
                pid_dir_gyro_left.out = 0;
                pid_dir_pos_left.err = 0;
                pid_dir_pos_left.err_last = 0;
                pid_dir_pos_left.out = 0;

                pid_dir_gyro_right.err = 0;
                pid_dir_gyro_right.err_last = 0;
                pid_dir_gyro_right.out = 0;
                pid_dir_pos_right.err = 0;
                pid_dir_pos_right.err_last = 0;
                pid_dir_pos_right.out = 0;
            }
        }
        else
        {
            if (diff >= 62 || diff <= -62 )  // 左转达到 65 度
            {
                end_turning_state = 1;
                zuo_ing = 0;

                pid_dir_gyro.err = 0;
                pid_dir_gyro.err_last = 0;
                pid_dir_gyro.out = 0;
                pid_dir_pos.err = 0;
                pid_dir_pos.err_last = 0;
                pid_dir_pos.out = 0;

                pid_dir_gyro_left.err = 0;
                pid_dir_gyro_left.err_last = 0;
                pid_dir_gyro_left.out = 0;
                pid_dir_pos_left.err = 0;
                pid_dir_pos_left.err_last = 0;
                pid_dir_pos_left.out = 0;

                pid_dir_gyro_right.err = 0;
                pid_dir_gyro_right.err_last = 0;
                pid_dir_gyro_right.out = 0;
                pid_dir_pos_right.err = 0;
                pid_dir_pos_right.err_last = 0;
                pid_dir_pos_right.out = 0;
            }
        }

    }
#endif

    switch (task_point)
    {
    case TASK_STRAIGHT:
    {
        if (turn_count == 26)
        {
            Img_Gap_left = 20;
            Img_Gap_right = 20;
        }
        else if (turn_count == 27)
        {
            Img_Gap_left = 15;
            Img_Gap_right = 15;
        }
        else if (turn_count == 28)
        {
            Img_Gap_left = 19;
            Img_Gap_right = 19;
        }
        else
        {
            Img_Gap_left = 24;
            Img_Gap_right = 24;
        }


        ramp.speed_target_max = speed_target_max;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos, &pid_dir_gyro, Final_Sum);
        Speed_Control_begin(speed_target + speed_dis_out, speed_target - speed_dis_out);
#if run
       Ljy_set_motor_pwm((int)speed_pid.out - (int) speed_dis_out, (int)speed_pid.out + (int)speed_dis_out);
#endif
        break;
    }
    case TASK_TURN_RIGHT: 
    {
        turn_time_count++;
        ramp.speed_target_max = speed_turn;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos_right, &pid_dir_gyro_right, Final_Sum);
        Speed_Control_begin(speed_target * 1 + speed_dis_out, speed_target * 1 - speed_dis_out);
#if run
       Ljy_set_motor_pwm((int)speed_pid.out - (int) speed_dis_out, (int)speed_pid.out + (int)(speed_dis_out));
#endif
       break;
    }

    case TASK_TURN_LEFT:
    {
        turn_time_count++;
        ramp.speed_target_max = speed_turn;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos_left, &pid_dir_gyro_left, Final_Sum);
        Speed_Control_begin(speed_target * 1 + speed_dis_out, speed_target * 1 - speed_dis_out);
#if run
       Ljy_set_motor_pwm((int)speed_pid.out - (int)(speed_dis_out), (int)speed_pid.out + (int)speed_dis_out);
#endif
       break;
    }
    default:
        break;
    }
    if((int)speed_pid.out - (int)(speed_dis_out) > 9900 || (int)speed_pid.out + (int)(speed_dis_out)> 9900)
    {
        limit_check.final_pwm += 1;
    }
}


int abs_val(int x) {
    return x >= 0 ? x : -x;
}

#include "IfxGpt12_IncrEnc.h"
void Speed_Control_begin(float Left_Target_Speed, float Right_Target_Speed)
{

#if mode_fuya
    encoder_left.encoder_count_original = - (int16)IfxGpt12_T3_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, 0);
//    encoder_left.encoder_count_original =  abs_val(encoder_left.encoder_count_original);
    LPF_1_int(20, 1.0e-3, encoder_left.encoder_count_original, &encoder_left.encoder_count, &lpf_encoder_left);
    
    encoder_right.encoder_count_original = -(int16)IfxGpt12_T5_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T5_setTimerValue(&MODULE_GPT120, 0);
    encoder_right.encoder_count_original =  abs_val(encoder_right.encoder_count_original);
    LPF_1_int(20, 1.0e-3, encoder_right.encoder_count_original, &encoder_right.encoder_count, &lpf_encoder_right);

    // encoder_all_left += encoder_left.encoder_count;
    // encoder_all_right += encoder_right.encoder_count;
    speed_final_average = (encoder_left.encoder_count + encoder_right.encoder_count) / 2;
    PID_Speed_Control(&speed_pid, speed_target, speed_final_average);
#endif

#if !mode_fuya
    encoder_left.encoder_count_original = -(int16)IfxGpt12_T3_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, 0);
    LPF_1_int(20, 1.0e-3, encoder_left.encoder_count_original, &encoder_left.encoder_count, &lpf_encoder_left);

    encoder_right.encoder_count_original = (int16)IfxGpt12_T2_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T2_setTimerValue(&MODULE_GPT120, 0);
    LPF_1_int(20, 1.0e-3, encoder_right.encoder_count_original, &encoder_right.encoder_count, &lpf_encoder_right);

    // encoder_all_left += encoder_left.encoder_count;
    // encoder_all_right += encoder_right.encoder_count;
    speed_final_average = (encoder_left.encoder_count + encoder_right.encoder_count) / 2;
    PID_Speed_Control(&speed_pid, speed_target, speed_final_average);
#endif
}

