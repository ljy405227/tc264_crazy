#ifndef CODE_PID_H_
#define CODE_PID_H_

typedef struct
{
    float Kf;               // Feed-forward coefficient (optional)
    float Kp;               // Proportional coefficient
    float Ki;               // Integral coefficient
    float Kd;               // Derivative coefficient

    float Error;            // Current error (target - feedback)
    float Last_Error;       // Previous error
    float Last_Feedback;    // Previous feedback value (for derivative)
    float Integral;         // Accumulated integral value
    float Differential;     // Derivative value (filtered)

    float OutPut;           // PID output
    float MAX_Error;        // Error limit (optional, anti-windup)
    float MAX_Integral;     // Integral limit (anti-windup)
    float MAX_OutPut;       // Output limit (actuator protection)
} Pos_PID;                  // Position PID structure

typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float Error;            // Current error
    float Last_Error;       // Previous error
    float Prev_Error;       // Error before last

    float Pout;
    float Iout;
    float Dout;
    float OutPut;

    float MAX_OutPut;
} Increment_PID;            // Incremental PID structure

typedef struct {
    float Kp1;
    float Kp2;
    float Kd;

    float Error;            // Current error
    float Last_Error;       // Previous error

    float P1out;
    float P2out;
    float Dout;
    float OutPut;

    float MAX_OutPut;
} DIR_PID;                  // Direction PID structure

void PID_Init(void);

float Pos_PID_Calculate(Pos_PID *pid, float expect, float feedback, float dt);

float Increment_PID_Caculate(Increment_PID *pid, int16_t Target, int16_t feedback);

#endif /* CODE_PID_H_ */