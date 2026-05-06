#include "zf_common_headfile.h"

//Pos_PID Speed;
Pos_PID speed_left_pid = {0};
Pos_PID speed_right_pid = {0};
DIR_PID direction_pid = {0};
Increment_PID direction_pid_2 = {0};

/*速度环PID初始化*/
void Speed_PID_Init(void)
{

}

/*方向环PID初始化*/
void Direction_PID_Init(void)
{

}

/*PID最终初始化*/
void PID_Init(void)
{
    Speed_PID_Init();

    Direction_PID_Init();
}

/****************************PID计算核心函数************************************/

/*
 功能：位置式PID
 参数：pid - PID结构体指针 | expect - 目标值 | feedback - 反馈值 | dt - 时间间隔（单位：秒）
 返回值：计算得到的PID输出
*/
float Pos_PID_Calculate(Pos_PID *pid, float expect, float feedback, float dt)
{
    float Points_ratio = 0.0f;  // 积分权重系数，用于动态调整积分贡献

    // 计算当前误差（目标值 - 反馈值）
    pid->Error = expect - feedback;

    // 是否发车，防止未发车前积分累加
    if(1)
    {
        // 计算积分权重：误差越小，权重越大（非线性积分，减少大误差时的积分积累）
        // 公式：1 - (当前误差^2)/(最大误差^2)，限制在0~1之间
        Points_ratio = MAX((1-(pid->Error*pid->Error)/(pid->MAX_Error*pid->MAX_Error)),0);

        // 积分项累加（带权重，防止大误差时积分饱和）
        pid->Integral += Points_ratio * pid->Error * dt;

        // 积分限幅：限制积分项在[-MAX_Integral, MAX_Integral]范围内
        pid->Integral = LIMIT(pid->Integral, -pid->MAX_Integral, pid->MAX_Integral);
    }
    else
    {
        pid->Integral = 0.0f;
    }

    // 计算微分项：对反馈值的变化率（反馈值的导数）进行一阶低通滤波（LPF_1）
    // 作用：减少噪声对微分环节的影响
    LPF_1(10, dt, ((feedback - pid->Last_Feedback)/dt), &pid->Differential);

    // 计算PID输出：前馈项 + 比例项 + 积分项 - 微分项（微分作用于反馈变化率）
    pid->OutPut = pid->Kf * expect + pid->Kp * pid->Error + pid->Ki * pid->Integral - pid->Kd * pid->Differential;

    // 输出限幅：限制输出在[-MAX_OutPut, MAX_OutPut]范围内，保护执行器
    pid->OutPut = LIMIT(pid->OutPut, -pid->MAX_OutPut, pid->MAX_OutPut);

    // 保存当前反馈值，用于下一次计算微分
    pid->Last_Feedback = feedback;

    return pid->OutPut;
}

/*
 功能：增量式式PID
*/
float Increment_PID_Caculate(Increment_PID *pid, int16_t Target, int16_t feedback)
{
    pid->Error = Target - feedback;

    pid->Pout = pid->Kp*(pid->Error - pid->Last_Error);
    pid->Iout = pid->Ki*pid->Error;
    pid->Dout = pid->Kd*(pid->Error - 2*pid->Last_Error + pid->Prev_Error);

    pid->OutPut += pid->Pout + pid->Iout + pid->Dout;
    pid->OutPut = LIMIT(pid->OutPut, -pid->MAX_OutPut, pid->MAX_OutPut);    //限幅

    pid->Prev_Error = pid->Last_Error;
    pid->Last_Error = pid->Error;

    return pid->OutPut;
}
