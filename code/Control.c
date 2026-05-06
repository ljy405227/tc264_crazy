#include "zf_common_headfile.h"
#include "Control.h"
#define mode_1 1
#define mode_2 0

#define mode_adjust_speed 0

TASK_POINT task_point = TASK_STRAIGHT;
LIMIT_CHECK limit_check = {0};
PID_SPEED speed_pid = {0};              // 速度环PID
PID_SPEED speed_right_pid_yuan = {0};              // 速度环PID

PID_SPEED speed_left_pid_new = {0};              // 速度环PID=============================完全闭环=============================
PID_SPEED speed_right_pid_new = {0};             // 速度环PID=============================完全闭环=============================

PID_DIR pid_dir_pos = {0};
PID_DIR pid_dir_gyro = {0};
PID_DIR pid_dir_pos_left = {0};
PID_DIR pid_dir_gyro_left = {0};
PID_DIR pid_dir_pos_right = {0};
PID_DIR pid_dir_gyro_right = {0};


PID_DIR pid_dir_pos_turn = {0};
PID_DIR pid_dir_gyro_turn = {0};
PID_TURN pid_dir_turn = {0};
Encoder_Data encoder_left = {0};              // 左编码器数据
Encoder_Data encoder_right = {0};             // 右编码器数据
volatile int speed_target = 0;
volatile int speed_target_max = 30;
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
SpeedRamp_t ramp;

void Pid_Init(void)
{
    speed_target = 0;
    speed_target_max = (int)Flash.tar_speed;

    // speed_pid.Kp = 5;
    // speed_pid.Ki = 0.04;
    speed_pid.Kp = Flash.speed_kp;
    speed_pid.Ki = Flash.speed_ki;
    speed_pid.Kd = Flash.speed_kd;
    speed_pid.err = 0;
    speed_pid.err_last = 0;
    speed_pid.out = 0;
    speed_pid.dec_limit = 6000;
    speed_pid.out_limit = 6000;

    
    speed_right_pid_yuan.Kp = Flash.speed_kp_right;
    speed_right_pid_yuan.Ki = Flash.speed_ki_right;
    speed_right_pid_yuan.err = 0;
    speed_right_pid_yuan.err_last = 0;
    speed_right_pid_yuan.out = 0;
    speed_right_pid_yuan.dec_limit = 5000;
    speed_right_pid_yuan.out_limit = 5000;

    pid_dir_gyro.Kp =  Flash.gyro_kp;
    pid_dir_gyro.Kd =  Flash.gyro_kd;
    pid_dir_gyro.Ki = 0;
    pid_dir_gyro.err = 0;
    pid_dir_gyro.err_last = 0;
    pid_dir_gyro.out = 0;


    pid_dir_pos.Kp =  Flash.dir_kp;
    pid_dir_pos.Kd =  Flash.dir_kd;
    pid_dir_pos.err = 0;
    pid_dir_pos.err_last = 0;
    pid_dir_pos.out = 0;

    pid_dir_gyro_left.Kp =  Flash.gyro_kp_left;
    pid_dir_gyro_left.Kd =  Flash.gyro_kd_left;
    pid_dir_gyro_left.Ki = 0;
    pid_dir_gyro_left.err = 0;
    pid_dir_gyro_left.err_last = 0;
    pid_dir_gyro_left.out = 0;


    pid_dir_pos_left.Kp =  Flash.dir_kp_left;
    pid_dir_pos_left.Kd =  Flash.dir_kd_left;
    pid_dir_pos_left.err = 0;
    pid_dir_pos_left.err_last = 0;
    pid_dir_pos_left.out = 0;

    pid_dir_gyro_right.Kp =  Flash.gyro_kp_right;
    pid_dir_gyro_right.Kd =  Flash.gyro_kd_right;
    pid_dir_gyro_right.Ki = 0;
    pid_dir_gyro_right.err = 0;
    pid_dir_gyro_right.err_last = 0;
    pid_dir_gyro_right.out = 0;


    pid_dir_pos_right.Kp =  Flash.dir_kp_right;
    pid_dir_pos_right.Kd =  Flash.dir_kd_right;
    pid_dir_pos_right.err = 0;
    pid_dir_pos_right.err_last = 0;
    pid_dir_pos_right.out = 0;


    ramp.speed_target = 0.0f;
    ramp.speed_target_max = Flash.tar_speed;
    ramp.ramp_time = Flash.other_ramp_time;      // 固定1秒
    ramp.elapsed_time = 0.0f;

    speed_turn = Flash.other_turn_speed;


    // pid_direction.Kp = 12.0f;
    // pid_direction.Kd = 2.0f;
    // pid_direction.Kp = Flash.direction_kp;
    // pid_direction.Kd = Flash.direction_kd;
    // pid_direction.err = 0.0f;
    // pid_direction.err_last = 0.0f;
    // pid_gyro.Kp_gyro = 0.0f;
    // pid_gyro.Kd_gyro = 0.0f;
    // pid_gyro.err = 0.0f;
    // pid_gyro.err_last = 0.0f;
}

// 年后
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
// void PID_Speed_Control(PID_SPEED *pid, int target, int measure)
// {
//     // 1. 更新误差
//     pid->err_last2 = pid->err_last; // 新增：前前次误差
//     pid->err_last  = pid->err;      // 上一次误差
//     pid->err       = target - measure;

//     // 2. 增量PID公式
//     int delta_err1 = pid->err - pid->err_last;
//     int delta_err2 = pid->err - 2*pid->err_last + pid->err_last2;

//     dec = pid->Kp * delta_err1                     // 比例增量
//         + pid->Ki * pid->err                       // 积分
//         + pid->Kd * delta_err2;                    // 微分增量，抑制震荡

//     // 3. 增量限幅
//     if(dec >  pid->dec_limit) dec =  pid->dec_limit;
//     if(dec < -pid->dec_limit) dec = -pid->dec_limit;

//     // 4. 积分抗饱和：输出饱和时停止继续积分累积
//     float temp_out = pid->out + dec;
//     if(temp_out >  pid->out_limit || temp_out < -pid->out_limit)
//     {
//         // 直接丢弃溢出增量，防止积分堆积
//         dec = 0;
//         limit_check.speed_out += 1;
//     }
//     else
//     {
//         pid->out = temp_out;
//     }
// }

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
    /*==================== 2. 角速度环（PD） ====================*/
    // 保存上次误差
    gyro_pid->err_last = gyro_pid->err;
    // 当前误差
    gyro_pid->err = - expect_gyro - avl_gyro_z;  // 左偏了expect_gyro为正
    // pid_dir_gyro.err = 0 - avl_gyro_z; 
    // PD（这里也可以改成 PI，看需求） 
    d_gyro = gyro_pid->err - gyro_pid->err_last;
    if(d_gyro > 100)  d_gyro = 100;
    if(d_gyro < -100) d_gyro = -100;
    dir_dec = gyro_pid->Kp * gyro_pid->err
            + gyro_pid->Kd * d_gyro;
    // 输出
    gyro_pid->out = dir_dec;
    // 限幅
    if(gyro_pid->out > 2000)  {gyro_pid->out = 2000;limit_check.dir_out += 1;}
    if(gyro_pid->out < -2000) {gyro_pid->out = -2000;limit_check.dir_out += 1;}

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


void ljy_isr_headle(void)
{
    if (turning_state == 1)
    {
        yaw_turn_first = yaw;
        zuo_ing = 1;
        turn_count++;
        turning_state = 0;

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
        task_point = TASK_TURN_LEFT;
    }
    else if (turning_state == 2)
    {
        yaw_turn_first = yaw;
        you_ing = 1;
        turn_count++;
        turning_state = 0;

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
        task_point = TASK_TURN_RIGHT;
    }
 
    if (zuo_ing == 1)
    {
        float diff = angle_diff(yaw, yaw_turn_first);
        if (diff >= 58)  // 左转达到 60 度
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
            task_point = TASK_STRAIGHT;
        }
    }

    // 右转判断（正确角度计算）
    if (you_ing == 1)
    {
        float diff = angle_diff(yaw, yaw_turn_first);
        if (diff <= -58)
        {
            end_turning_state = 1;
            you_ing = 0;

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
            task_point = TASK_STRAIGHT;
        }
    }
    

#if !mode_adjust_speed
    // Dir_Control();
#endif
    switch (task_point)
    {
    case TASK_STRAIGHT:
    {
        ramp.speed_target_max = Flash.tar_speed;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos, &pid_dir_gyro, Final_Sum);
        Speed_Control_begin(speed_target + speed_dis_out, speed_target - speed_dis_out);
//       Ljy_set_motor_pwm((int)speed_pid.out - (int)speed_dis_out, (int)speed_pid.out + (int)speed_dis_out);
        break;
    }
    case TASK_TURN_RIGHT:
    {
        ramp.speed_target_max = Flash.other_turn_speed;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos_right, &pid_dir_gyro_right, Final_Sum);
        Speed_Control_begin(speed_target * 1 + speed_dis_out, speed_target * 1 - speed_dis_out);
    //    Ljy_set_motor_pwm((int)speed_pid.out - (int)speed_dis_out, (int)speed_pid.out + (int)speed_dis_out);
//        Ljy_set_motor_pwm((int)(speed_pid.out*0.9 - (int)speed_dis_out), (int)(speed_pid.out * 0.9) + (int)speed_dis_out);
        break;
    }

    case TASK_TURN_LEFT:
    {
        ramp.speed_target_max = Flash.other_turn_speed;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos_left, &pid_dir_gyro_left, Final_Sum);
        Speed_Control_begin(speed_target * 1 + speed_dis_out, speed_target * 1 - speed_dis_out);
    //    Ljy_set_motor_pwm((int)speed_pid.out - (int)speed_dis_out, (int)speed_pid.out + (int)speed_dis_out);
//        Ljy_set_motor_pwm((int)(speed_pid.out*0.9 - (int)speed_dis_out), (int)(speed_pid.out * 0.9) + (int)speed_dis_out);
        break;
    }
    default:
        break;
    }
}
#include "IfxGpt12_IncrEnc.h"
void Speed_Control_begin(float Left_Target_Speed, float Right_Target_Speed)
{

    encoder_left.encoder_count_original = (int16)IfxGpt12_T3_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, 0);
    LPF_1_int(20, 1.0e-3, encoder_left.encoder_count_original, &encoder_left.encoder_count, &lpf_encoder_left);
    
    encoder_right.encoder_count_original = -(int16)IfxGpt12_T5_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T5_setTimerValue(&MODULE_GPT120, 0);
    LPF_1_int(20, 1.0e-3, encoder_right.encoder_count_original, &encoder_right.encoder_count, &lpf_encoder_right);

    // encoder_all_left += encoder_left.encoder_count;
    // encoder_all_right += encoder_right.encoder_count;
    speed_final_average = (encoder_left.encoder_count + encoder_right.encoder_count) / 2;
    // PID_Speed_Control(&speed_pid, speed_target, encoder_left.encoder_count); // 得到speed_pid.out
    // PID_Speed_Control(&speed_right_pid_yuan, speed_target, encoder_right.encoder_count);

    PID_Speed_Control(&speed_pid, speed_target, speed_final_average);
}

