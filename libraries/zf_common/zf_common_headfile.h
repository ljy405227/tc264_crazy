#ifndef _zf_common_headfile_h_
#define _zf_common_headfile_h_

//===================================================C锟斤拷锟斤拷 锟斤拷锟斤拷锟斤拷===================================================
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
//===================================================C锟斤拷锟斤拷 锟斤拷锟斤拷锟斤拷===================================================

//===================================================芯片 SDK 锟阶诧拷===================================================
#include "ifxAsclin_reg.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxCcu6_Timer.h"
#include "IfxScuEru.h"
//===================================================芯片 SDK 锟阶诧拷===================================================

//====================================================锟斤拷源锟解公锟斤拷锟斤拷====================================================
#include "zf_common_typedef.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
#include "isr_config.h"
//====================================================锟斤拷源锟解公锟斤拷锟斤拷====================================================

//===================================================芯片锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_dma.h"
#include "zf_driver_encoder.h"
#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
#include "zf_driver_soft_iic.h"
#include "zf_driver_spi.h"
#include "zf_driver_soft_spi.h"
#include "zf_driver_uart.h"
#include "zf_driver_timer.h"
#include "zf_eeprom.h"
//===================================================芯片锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷===================================================

//===================================================锟斤拷锟斤拷璞革拷锟斤拷锟斤拷锟�===================================================
#include "zf_device_absolute_encoder.h"
#include "zf_device_ble6a20.h"
#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_gnss.h"
#include "zf_device_camera.h"
#include "zf_device_dl1a.h"
#include "zf_device_dl1b.h"
#include "zf_device_icm20602.h"
#include "zf_device_imu660ra.h"
#include "zf_device_imu660rb.h"
#include "zf_device_imu660rx.h"
#include "zf_device_imu963ra.h"
#include "zf_device_ips114.h"
#include "zf_device_ips200.h"
#include "zf_device_ips200pro.h"
#include "zf_device_key.h"
#include "zf_device_menc15a.h"
#include "zf_device_mpu6050.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_ov7725.h"
#include "zf_device_scc8660.h"
#include "zf_device_tft180.h"
#include "zf_device_tsl1401.h"
#include "zf_device_type.h"
#include "zf_device_uart_receiver.h"
#include "zf_device_virtual_oscilloscope.h"
#include "zf_device_wifi_uart.h"
#include "zf_device_wifi_spi.h"
#include "zf_device_wireless_uart.h"
//===================================================锟斤拷锟斤拷璞革拷锟斤拷锟斤拷锟�===================================================

//====================================================应锟斤拷锟斤拷锟斤拷锟�====================================================
#include "seekfree_assistant.h"
#include "seekfree_assistant_interface.h"
//====================================================应锟斤拷锟斤拷锟斤拷锟�====================================================

//=====================================================锟矫伙拷锟斤拷======================================================
#include "Init.h"
#include "image.h"
#include "Control.h"
#include "ips_show.h"
#include "SWJ.h"
#include "Filter.h"
#include "VOFA.h"
#include "isr.h"
#include "Flash.h"
#include "PID.h"
#include "Motor.h"
#include "IMU.h"
#include "My_math.h"
#include "ljy_key.h"
#include "ljy_menu.h"

extern volatile uint16_t pit_count_ms;
extern volatile uint8_t gogogo_flag;
extern volatile uint8_t first_flag;
extern int16 encoder_data_1;
extern int16 encoder_data_2;

/******************锟斤拷锟斤拷模锟介开锟斤拷 */
#define Battery_Valtage_mode 0
#define Wifi_mode 0
/******************锟斤拷锟斤拷模锟介开锟斤拷 */
#define VOLTAGE_PORT    ADC0_CH11_A11 // 锟斤拷锟斤拷锟斤拷丝锟
#define WIFI_SSID_TEST          "YiMeng"
#define WIFI_PASSWORD_TEST      "123456789"
#define VOLTAGE_PORT    ADC0_CH11_A11 // 锟斤拷锟斤拷锟斤拷丝锟

#define my_camera_h    60
#define my_camera_w    94
#define MY_CAMERA_SIZE      (my_camera_w * my_camera_h)
#define Camera_H MT9V03X_H
#define Camera_W MT9V03X_W
#define Camera_H_YM  MT9V03X_H
#define Camera_W_XM  MT9V03X_W
#define BOUNDARY_NUM            (MT9V03X_H * 5)   //确保存储的点够用

#define Control_line  1

extern uint8 white_num_row[my_camera_h];
extern uint8 white_num_col[my_camera_w];
extern uint8 imgZip[my_camera_h][my_camera_w];
extern uint8 img_binary[my_camera_h][my_camera_w];
extern uint8 img_copy[my_camera_h][my_camera_w];
extern uint8 img_SPI[my_camera_h][my_camera_w];

extern uint8 xy_x1_boundary[BOUNDARY_NUM], xy_x2_boundary[BOUNDARY_NUM], xy_x3_boundary[BOUNDARY_NUM];
extern uint8 xy_y1_boundary[BOUNDARY_NUM], xy_y2_boundary[BOUNDARY_NUM], xy_y3_boundary[BOUNDARY_NUM];
extern uint8_t data_ready_flag_for_core0 ;
extern uint32_t tim;
extern uint16_t l_point_x ;
extern uint16_t r_point_x ;
extern uint16_t bottom_center_x;
extern uint16_t start_point_l_x;
extern uint16_t start_point_r_x;
extern uint8_t state_flag;
extern uint8_t turning_state;
extern uint8_t end_turning_state;
// 锟竭斤拷锟结构锟斤拷
typedef struct {
    int16_t x;
    int16_t y;
    uint8_t direction;  // 锟斤拷锟斤拷锟斤拷锟斤拷 (0-7锟斤拷锟斤拷应8锟斤拷锟斤拷)
} BoundaryPoint;

typedef struct {
    uint8_t touch_top : 1;     // 锟斤拷锟斤拷锟斤拷锟较边斤拷
    uint8_t touch_bottom : 1;  // 锟斤拷锟斤拷锟斤拷锟铰边斤拷
    uint8_t touch_left : 1;    // 锟斤拷锟斤拷锟斤拷锟斤拷呓锟
    uint8_t touch_right : 1;   // 锟斤拷锟斤拷锟斤拷锟揭边斤拷
} BoundaryTouchFlags;

typedef struct {
    uint16_t x;            // 锟斤拷锟斤拷锟斤拷锟絰锟斤拷锟斤拷
    uint16_t y;            // 锟斤拷锟斤拷锟斤拷锟統锟斤拷锟斤拷
    uint8_t boundary_side; // 锟斤拷锟斤拷锟侥边界：0-锟斤拷, 1-锟斤拷, 2-锟斤拷, 3-锟斤拷
} TouchPoint;

// 锟斤拷锟侥碉拷峁癸拷锟
typedef struct {
    int16_t x;
    int16_t y;
} Center_Point;

typedef struct {
    int16_t x;
    int16_t y;
} Corner_Point;

typedef struct {
    int16_t x;
    int16_t y;
} Four_Edge_Point;

typedef struct {
    int16_t x;
    int16_t y;
} Disconnect_point;

//爬线数组
extern BoundaryPoint Total_boundary[];
extern BoundaryPoint left_boundary[];
extern BoundaryPoint right_boundary[];

//四方位点
extern Four_Edge_Point left_edge_point;
extern Four_Edge_Point right_edge_point;
extern Four_Edge_Point up_edge_point;
extern Four_Edge_Point down_edge_point;

//拐点
extern uint16_t tmp_right_corner_point_x;
extern uint16_t tmp_left_corner_point_x;
extern uint16_t tmp_right_corner_point_y;
extern uint16_t tmp_left_corner_point_y;
extern uint16_t right_corner_point_x;
extern uint16_t left_corner_point_x;
extern uint16_t right_corner_point_y;
extern uint16_t left_corner_point_y;

extern Center_Point center_points_new[150];
extern Center_Point center_points[];
extern Corner_Point fit_points[];
extern Disconnect_point Disconnect_Point[];
extern uint16_t center_point_num;
extern uint16_t center_point_x_sum;
extern int16_t center_point_x_average;

extern uint16_t boundary_point_num;
extern int16_t Middle_Line_x;
extern int16_t Middle_Line_x_left;
extern int16_t Middle_Line_x_right;
extern int16_t Final_Sum;


extern uint16_t left_boundary_len;
extern uint16_t right_boundary_len;
extern uint16_t left_boundary_x_sum;
extern uint16_t left_boundary_average_x;
extern uint16_t right_boundary_x_sum;
extern uint16_t right_boundary_average_x;
extern int16_t boundary_gap;

extern BoundaryTouchFlags left_touch_flags;  // 宸﹁竟鐣岃Е纰版爣蹇
extern BoundaryTouchFlags right_touch_flags; // 鍙宠竟鐣岃Е纰版爣蹇
extern BoundaryTouchFlags total_touch_flags; // 鎬昏竟鐣岃Е纰版爣蹇
extern TouchPoint left_touch_points[4];     // 宸﹁竟鐣岃Е纰扮偣锛堜笂涓嬪乏鍙筹級
extern TouchPoint right_touch_points[4];    // 鍙宠竟鐣岃Е纰扮偣锛堜笂涓嬪乏鍙筹級

extern uint16_t current_white_num_col;
extern uint16_t top_point_x;
extern uint16_t top_point_y;

extern uint8_t fit_point_count;
extern uint8_t Right_angle_l;
extern uint8_t Right_angle_r;

extern uint8_t transisitor_left_flag;
extern uint8_t transisitor_right_flag;
extern uint8_t transisitor_left_and_right_flag;
extern uint8_t transistor_left_Num;
extern uint8_t transistor_right_Num;
extern uint8_t transistor_left_and_right_Num;
extern uint8_t Special_flag;
extern uint8_t T;

//=====================================================锟矫伙拷锟斤拷======================================================

void printf_deal(void);
extern Pos_PID speed_left_pid;
extern Pos_PID speed_right_pid;

extern DIR_PID direction_pid;
extern Increment_PID direction_pid_2;
extern volatile uint16_t time_debug;



// Control.c
extern PID_SPEED speed_pid;
extern LIMIT_CHECK limit_check;
extern PID_SPEED speed_left_pid_new;
extern PID_SPEED speed_right_pid_new;
extern PID_DIR pid_dir_pos;
extern PID_DIR pid_dir_gyro;
extern Encoder_Data encoder_left;
extern Encoder_Data encoder_right;
extern volatile int speed_target;           // 閫熷害鐩爣鍊�
extern volatile int speed_target_max;       // 閫熷害鐩爣鏈�澶у��
extern volatile int speed_dis_original;     // 閫熷害宸師濮嬪��
extern volatile int speed_dis_feedback;     // 閫熷害宸弽棣堝��
extern int speed_final_average;             // 閫熷害骞冲潎鍊�
extern volatile float speed_dis_out;
extern volatile float dec;
extern volatile float dir_dec;
extern volatile float expect_gyro;
extern TASK_POINT task_point;
extern volatile float turn_yaw_begin;
extern volatile int yichu_count;
extern volatile float encoder_all_left;
extern volatile float encoder_all_right;
extern volatile float pos_d_err;
extern volatile float d_gyro;
extern SpeedRamp_t ramp;


// IMU.c
extern IMU gyro[3],accel[3];
extern float pitch,roll,yaw;
extern float avl_gyro_z;



extern volatile uint64 start, end, diff;


extern uint8_t main_choose_item;

extern uint8_t ljy_camera_deal_flag;

extern float yaw_turn_first;
extern uint8_t zuo_ing;
extern uint8_t you_ing;
extern uint8_t Gyro_Flag;
extern uint8_t turn_count;
#endif

