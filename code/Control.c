#include "zf_common_headfile.h"
#include "Control.h"
#include "IfxGpt12_IncrEnc.h"

/* ======================== Global Variables ======================== */

uint16_t turn_time_count = 0;

TASK_POINT task_point = TASK_STRAIGHT;
LIMIT_CHECK limit_check = {0};              // Limit overflow counter
PID_SPEED speed_pid = {0};                  // Speed loop PID

// Direction PID -- 3 sets for straight, left turn, right turn
PID_DIR pid_dir_pos = {0};
PID_DIR pid_dir_gyro = {0};
PID_DIR pid_dir_pos_left = {0};
PID_DIR pid_dir_gyro_left = {0};
PID_DIR pid_dir_pos_right = {0};
PID_DIR pid_dir_gyro_right = {0};

Encoder_Data encoder_left = {0};            // Left encoder data
Encoder_Data encoder_right = {0};           // Right encoder data

SpeedRamp_t ramp = {0};

PID_SPEED speed_left_pid_new = {0};         // Fully closed-loop speed PID (left)
PID_SPEED speed_right_pid_new = {0};        // Fully closed-loop speed PID (right)

volatile int speed_target = 0;
volatile int speed_target_max = 150;
volatile int speed_dis_original = 0;        // Raw speed difference
volatile int speed_dis_feedback = 0;        // Feedback speed difference
int speed_final_average = 0;                // Average speed
volatile float speed_dis_out = 0;           // Direction loop output (speed compensation)
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
volatile float expect_gyro = 0;

uint8_t zuo_ing = 0;
uint8_t you_ing = 0;
float yaw_turn_first = 0.0f;

uint8_t turn_count = 0;

#define guandao 0

/* ======================== Helper: Reset all PID states ======================== */

static void reset_all_pid_state(void)
{
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

/* ======================== Initialization ======================== */

// Pre-start initialization
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
    speed_target = 0;
    speed_target_max = 190;

    speed_pid.Kp = 70;
    speed_pid.Ki = 0.56;
    speed_pid.err = 0;
    speed_pid.err_last = 0;
    speed_pid.out = 0;
    speed_pid.dec_limit = 5000;
    speed_pid.out_limit = 6500;

    pid_dir_gyro.Kp = 2.5;
    pid_dir_gyro.Kd = 0.15;
    pid_dir_gyro.Ki = 0;
    pid_dir_gyro.err = 0;
    pid_dir_gyro.err_last = 0;
    pid_dir_gyro.out = 0;

    pid_dir_pos.Kp = 18.10;
    pid_dir_pos.Kd = 0.66;
    pid_dir_pos.err = 0;
    pid_dir_pos.err_last = 0;
    pid_dir_pos.out = 0;

    pid_dir_gyro_left.Kp = 3.08;
    pid_dir_gyro_left.Kd = 0.50;
    pid_dir_gyro_left.Ki = 0;
    pid_dir_gyro_left.err = 0;
    pid_dir_gyro_left.err_last = 0;
    pid_dir_gyro_left.out = 0;

    pid_dir_pos_left.Kp = 22.10;
    pid_dir_pos_left.Kd = 0.86;
    pid_dir_pos_left.err = 0;
    pid_dir_pos_left.err_last = 0;
    pid_dir_pos_left.out = 0;

    pid_dir_gyro_right.Kp = 3.08;
    pid_dir_gyro_right.Kd = 0.70;
    pid_dir_gyro_right.Ki = 0;
    pid_dir_gyro_right.err = 0;
    pid_dir_gyro_right.err_last = 0;
    pid_dir_gyro_right.out = 0;

    pid_dir_pos_right.Kp = 22.10;
    pid_dir_pos_right.Kd = 0.86;
    pid_dir_pos_right.err = 0;
    pid_dir_pos_right.err_last = 0;
    pid_dir_pos_right.out = 0;

    ramp.speed_target = 0.0f;
    ramp.speed_target_max = 190;
    ramp.ramp_time = 0.2;
    ramp.elapsed_time = 0.0f;

    speed_turn = 195.0f;
}

/* ======================== Speed PID (Incremental PI) ======================== */

void PID_Speed_Control(PID_SPEED *pid, int target, int measure)
{
    // 1. Save last error
    pid->err_last = pid->err;
    // 2. Current error
    pid->err = target - measure;
    // 3. Incremental PI
    dec = pid->Kp * (pid->err - pid->err_last)
        + pid->Ki * pid->err;
    // 4. Incremental limit
    if (dec > pid->dec_limit)
    {
        dec = pid->dec_limit;
        limit_check.speed_dec += 1;
    }
    if (dec < -pid->dec_limit)
    {
        dec = -pid->dec_limit;
        limit_check.speed_dec += 1;
    }
    // 5. Accumulate output
    pid->out += dec;
    // 6. Output limit
    if (pid->out > pid->out_limit)
    {
        pid->out = pid->out_limit;
        limit_check.speed_out += 1;
    }
    if (pid->out < -pid->out_limit)
    {
        pid->out = -pid->out_limit;
        limit_check.speed_out += 1;
    }
}

/* ======================== Direction PID (Cascaded PD) ======================== */

void PID_Direction_Control(PID_DIR *pos_pid, PID_DIR *gyro_pid, float err_position)
{
    /*==================== 1. Position Loop (PD) ====================*/
    pos_pid->err_last = pos_pid->err;
    pos_pid->err = err_position;

    // Derivative with spike suppression
    pos_d_err = pos_pid->err - pos_pid->err_last;
    if (pos_d_err > 40.3f)
    {
        limit_check.dir_d_err += 1;
        pos_d_err = 40.3f;
    }
    if (pos_d_err < -40.3f)
    {
        limit_check.dir_d_err += 1;
        pos_d_err = -40.3f;
    }

    // PD output (target angular velocity)
    expect_gyro = pos_pid->Kp * pos_pid->err
                + pos_pid->Kd * pos_d_err;
    if (expect_gyro > limit_gyro)
    {
        expect_gyro = limit_gyro;
        limit_check.dir_expect_gyro += 1;
    }
    if (expect_gyro < -limit_gyro)
    {
        expect_gyro = -limit_gyro;
        limit_check.dir_expect_gyro += 1;
    }

    // Override for turns (fixed angular velocity)
    if (task_point == TASK_TURN_RIGHT)
        expect_gyro = 1600;
    else if (task_point == TASK_TURN_LEFT)
        expect_gyro = -1600;

    /*==================== 2. Angular Velocity Loop (PD) ====================*/
    gyro_pid->err_last = gyro_pid->err;
    gyro_pid->err = -expect_gyro - avl_gyro_z;  // Positive expect_gyro = left deviation

    d_gyro = gyro_pid->err - gyro_pid->err_last;
    if (d_gyro > 700)
    {
        d_gyro = 700;
        limit_check.d_gyro += 1;
    }
    if (d_gyro < -700)
    {
        d_gyro = -700;
        limit_check.d_gyro += 1;
    }

    dir_dec = gyro_pid->Kp * gyro_pid->err
            + gyro_pid->Kd * d_gyro;
    gyro_pid->out = dir_dec;

    if (gyro_pid->out > 6500)
    {
        gyro_pid->out = 6500;
        limit_check.dir_out += 1;
    }
    if (gyro_pid->out < -6500)
    {
        gyro_pid->out = -6500;
        limit_check.dir_out += 1;
    }

    // Final output
    speed_dis_out = gyro_pid->out;
}

/* ======================== Speed Ramp (S-curve) ======================== */

float SpeedRamp_Update(SpeedRamp_t *ramp)
{
    // Called every 1ms (dt = 0.001s)
    ramp->elapsed_time += 0.001f;

    float t = ramp->elapsed_time / ramp->ramp_time;
    if (t >= 1.0f)
    {
        t = 1.0f;
        ramp->speed_target = ramp->speed_target_max;
    }
    else
    {
        // S-curve: 3*t^2 - 2*t^3
        ramp->speed_target = ramp->speed_target_max * (3.0f * t * t - 2.0f * t * t * t);
    }

    return ramp->speed_target;
}

/* ======================== Angle Utilities ======================== */

float angle_diff(float current, float target)
{
    float diff = current - target;
    while (diff > 180)
        diff -= 360;
    while (diff < -180)
        diff += 360;
    return diff;
}

int abs_val(int x)
{
    return x >= 0 ? x : -x;
}

/* ======================== Turn State Machine ======================== */

static void handle_turn_speed_config(void)
{
    if (turn_count == 42)
    {
        ramp.ramp_time = 0.3;
        speed_target_max = 170;
        ramp.speed_target_max = 170;
        speed_turn = 175;
    }
    else if (turn_count == 50)
    {
        ramp.ramp_time = 0.2;
        speed_target_max = 190;
        ramp.speed_target_max = 190;
        speed_turn = 195;
    }
    else if (turn_count == 29)
    {
        ramp.ramp_time = 0.2;
        speed_target_max = 180;
        ramp.speed_target_max = 180;
        speed_turn = 185;
    }
    else if (turn_count == 33)
    {
        ramp.ramp_time = 0.4;
        speed_target_max = 220;
        ramp.speed_target_max = 220;
        speed_turn = 225;
    }
}

/* ======================== Main 1ms ISR Handler ======================== */

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

        reset_all_pid_state();
    }
    if (zuo_ing == 1)
    {
        float diff = angle_diff(yaw, yaw_turn_first);
        if (diff >= 50 || diff <= -50)
        {
            end_turning_state = 1;
            zuo_ing = 0;

            reset_all_pid_state();
            handle_turn_speed_config();
        }
    }
#endif

    switch (task_point)
    {
    case TASK_STRAIGHT:
    {
        ramp.speed_target_max = speed_target_max;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos, &pid_dir_gyro, Final_Sum);
        Speed_Control_begin(speed_target + speed_dis_out, speed_target - speed_dis_out);
#if run
        Ljy_set_motor_pwm((int)speed_pid.out - (int)speed_dis_out, (int)speed_pid.out + (int)speed_dis_out);
#endif
        break;
    }
    case TASK_TURN_RIGHT:
    {
        turn_time_count++;
        ramp.speed_target_max = speed_turn;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos_right, &pid_dir_gyro_right, Final_Sum);
        Speed_Control_begin(speed_target + speed_dis_out, speed_target - speed_dis_out);
#if run
        Ljy_set_motor_pwm((int)speed_pid.out - (int)speed_dis_out, (int)speed_pid.out + (int)speed_dis_out);
#endif
        break;
    }
    case TASK_TURN_LEFT:
    {
        turn_time_count++;
        ramp.speed_target_max = speed_turn;
        speed_target = (int)SpeedRamp_Update(&ramp);
        PID_Direction_Control(&pid_dir_pos_left, &pid_dir_gyro_left, Final_Sum);
        Speed_Control_begin(speed_target + speed_dis_out, speed_target - speed_dis_out);
#if run
        Ljy_set_motor_pwm((int)speed_pid.out - (int)speed_dis_out, (int)speed_pid.out + (int)speed_dis_out);
#endif
        break;
    }
    default:
        break;
    }

    // Final PWM overflow check
    if ((int)speed_pid.out - (int)speed_dis_out > 9900 || (int)speed_pid.out + (int)speed_dis_out > 9900)
    {
        limit_check.final_pwm += 1;
    }
}

/* ======================== Speed Control (Encoder + Filter + PID) ======================== */

void Speed_Control_begin(float Left_Target_Speed, float Right_Target_Speed)
{
#if mode_fuya
    encoder_left.encoder_count_original = -(int16)IfxGpt12_T3_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, 0);
    LPF_1_int(20, 1.0e-3, encoder_left.encoder_count_original, &encoder_left.encoder_count, &lpf_encoder_left);

    encoder_right.encoder_count_original = -(int16)IfxGpt12_T5_getTimerValue(&MODULE_GPT120);
    IfxGpt12_T5_setTimerValue(&MODULE_GPT120, 0);
    encoder_right.encoder_count_original = abs_val(encoder_right.encoder_count_original);
    LPF_1_int(20, 1.0e-3, encoder_right.encoder_count_original, &encoder_right.encoder_count, &lpf_encoder_right);

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

    speed_final_average = (encoder_left.encoder_count + encoder_right.encoder_count) / 2;
    PID_Speed_Control(&speed_pid, speed_target, speed_final_average);
#endif
}