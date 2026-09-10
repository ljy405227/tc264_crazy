#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_

typedef enum
{
    TASK_STRAIGHT,
    TASK_TURN_RIGHT,
    TASK_TURN_LEFT,
    TASK_STOP,
} TASK_POINT;

typedef struct
{
    int speed_dec;
    int speed_out;
    int dir_d_err;
    int dir_expect_gyro;
    int d_gyro;
    int dir_out;
    int final_pwm;
} LIMIT_CHECK;

typedef struct
{
    int16 encoder_count_original;
    int16 encoder_count;
} Encoder_Data;         // Encoder data structure

typedef struct
{
    float Kp;
    float Ki;
    float Kd;

    int16 err;
    int16 err_last;
    int16 err_last2;

    float out;

    float dec_limit;
    float out_limit;
} PID_SPEED;            // Speed PID structure

typedef struct
{
    float Kp;
    float Ki;
    float Kd;

    float err;
    float err_last;

    float out;
    float out_limit;
} PID_DIR;              // Direction PID structure

typedef struct
{
    float kp_cam;
    float Kp_pos;
    float Kd_pos;
} PID_TURN;

typedef struct
{
    float speed_target;       // Current target
    float speed_target_max;   // Maximum target
    float ramp_time;          // Ramp-up time (seconds)
    float elapsed_time;       // Elapsed time
} SpeedRamp_t;


#define limit_gyro 2000


extern TASK_POINT task_point;

void Pid_Init(void);
void PID_Speed_Control(PID_SPEED *pid, int target, int measure);
void PID_Direction_Control(PID_DIR *pos_pid, PID_DIR *gyro_pid, float err_position);
void Speed_Control_begin(float Left_Target_Speed, float Right_Target_Speed);
void Speed_Control(float Left_Target_Speed, float Right_Target_Speed);
void Dir_Control(void);
void Run_Control(void);


#endif /* CODE_CONTROL_H_ */