#ifndef CODE_IMG_H_
#define CODE_IMG_H_

#include "zf_common_headfile.h"

#define Black 0  //黑色
#define Black_Expansion 1 //膨胀操作后的拓宽黑点
#define Blue 2 //补线颜色
#define Yellow 3 //赛道边界线颜色
#define Green 4 //赛道中线颜色
#define White 255  //白色
#define White_Expansion 254 //膨胀操作后的拓宽白点

#define XM MT9V03X_W       //处理图像 像素X
#define YM MT9V03X_H        //处理图像 像素Y
#define XX XM-1             //处理图像 下标X
#define YY YM-1             //处理图像 下标Y
#define DEAL_BOTTOM     (my_camera_h - 1)  // 处理底部行
#define DEAL_TOP        0                 // 处理顶部行

extern uint8_t nowThreshold_flag;
extern uint8_t img_deal_ok_flag;

void Get_Zip_Image(uint8 Image_src[MT9V03X_H][MT9V03X_W], uint8 Image_tar[my_camera_h][my_camera_w], int factor);
uint8_t Get_Threshold(unsigned char image[my_camera_h][my_camera_w], uint8 start_y, uint8 start_x, uint8 end_h, uint8 end_w);
void Get_img_binary(void);
uint8_t is_edge_point(uint16_t x, uint16_t y);
void trace_boundary(uint16_t start_x_l, uint16_t start_y_l,uint16_t start_x_r,uint16_t start_y_r);
void find_center_point(void);
void trace_both_boundaries(void);
void trace_Right_angle(void);
void img_copy_for_show(void);
void trace_center_point(void);
void Four_boundary_flag(void);
void Error_Gap(void);
void Disconnected_Judge(void);
void Disconnected_Handle(void);
void trace_transistor(void);
void Final_Road(void);
void Road_Judge(uint8_t Road_Num,uint8_t Road_Turn_Judge);  //其中Road_Turn_Judge 为1时即转弯，为0时为不转弯
void ljw_camera_deal(void);

#endif // DEAL_IMG_H


