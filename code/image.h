#ifndef CODE_IMG_H_
#define CODE_IMG_H_

#include "zf_common_headfile.h"

#define Black 0             // Black
#define Black_Expansion 1   // Dilated black point
#define Blue 2              // Line-fill color
#define Yellow 3            // Track boundary color
#define Green 4             // Track centerline color
#define White 255           // White
#define White_Expansion 254 // Dilated white point

#define XM MT9V03X_W        // Image width in pixels
#define YM MT9V03X_H        // Image height in pixels
#define XX XM-1             // Max X index
#define YY YM-1             // Max Y index
#define DEAL_BOTTOM     (my_camera_h - 1)  // Bottom row index
#define DEAL_TOP        0                 // Top row index

extern uint8_t nowThreshold_flag;
extern uint8_t img_deal_ok_flag;

void Get_Zip_Image(uint8 Image_src[MT9V03X_H][MT9V03X_W], uint8 Image_tar[my_camera_h][my_camera_w], int factor);
uint8_t Get_Threshold(unsigned char image[my_camera_h][my_camera_w], uint8 start_y, uint8 start_x, uint8 end_h, uint8 end_w);
void Get_img_binary(void);
uint8_t is_edge_point(uint16_t x, uint16_t y);
void trace_boundary(uint16_t start_x_l, uint16_t start_y_l, uint16_t start_x_r, uint16_t start_y_r);
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
void Road_Judge(uint8_t Road_Num, uint8_t Road_Turn_Judge);
void ljw_camera_deal(void);

#endif // DEAL_IMG_H