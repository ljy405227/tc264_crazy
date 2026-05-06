#ifndef CODE_MY_MATH_H_
#define CODE_MY_MATH_H_

extern float Raw_accel,Ori_accel;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define LIMIT(input, low, upper)    MIN(MAX(input, low), upper)//限幅
#define ABS(a) ((a >= 0) ? (a) : (-a))//取绝对值

float distance(float ax, float ay, float bx, float by);

float xielv_sideline(int x1, int y1, int x2, int y2, char data);

float Get_line_x(int y, int x1, int y1, int x2, int y2);

float Get_line_y(int x, int x1, int y1, int x2, int y2);

void bodyToWorldAccelerationZUp(double PITCH, double ROLL, double YAW);

#endif /* CODE_MY_MATH_H_ */
