#include "zf_common_headfile.h"


void ips_show(void)
{
//       ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
//       ips200_show_gray_image(0, 0, (const uint8 *)img_bin, my_camera_h, my_camera_h, MT9V03X_W, MT9V03X_H, 0);//鍓嶄袱涓珮瀹芥槸鍥惧儚鐨勯珮瀹斤紝鍚庨潰鐨勬槸鏄剧ず楂樺
//       ips200_show_gray_image(my_camera_w *2, (my_camera_h *2 +5), (const uint8 *)imgZip, my_camera_w, my_camera_h, my_camera_w *2, my_camera_h *2, 0);//鍓嶄袱涓珮瀹芥槸鍥惧儚鐨勯珮瀹斤紝鍚庨潰鐨勬槸鏄剧ず楂樺

         ips200_show_gray_image(0, 150, (const uint8 *)imgZip, my_camera_w, my_camera_h, my_camera_w *2, my_camera_h *2, 0);//鍘熷鍥惧儚
         ips200_show_gray_image(0, 0, (const uint8 *)img_copy, my_camera_w, my_camera_h, my_camera_w *2, my_camera_h *2, 0);//浜屽 煎寲鍥惧儚

//       ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);//鍓嶄袱涓珮瀹芥槸鍥惧儚鐨勯珮瀹斤紝鍚庨潰鐨勬槸鏄剧ず楂樺
         draw_boundary_points();
         draw_center_points();
//       ips200_draw_line(Right_tangle_top_x * 2,Right_tangle_top_y * 2,bottom_center_x * 2,bottom_center_y *2 , RGB565_RED);
//       draw_Corner_points();
//
        ips200_show_int(200,1,Final_Sum,3);
        ips200_show_int(200,50,center_point_x_average,3);
//        ips200_show_uint(200,100,right_corner_point_y,3);
//        ips200_show_uint(200,120,left_corner_point_y,3);
        ips200_show_int(200,80,right_edge_point.y,3);
        ips200_show_int(200,100,left_edge_point.y,3);
        ips200_show_uint(200,120,transistor_Num,3);
        ips200_show_int(200,140,boundary_gap,3);

        ips200_show_uint(200,170,total_touch_flags.touch_top,3);
        ips200_show_uint(190,190,total_touch_flags.touch_left,3);
        ips200_show_uint(210,190,total_touch_flags.touch_right,3);
        ips200_show_uint(200,210,total_touch_flags.touch_bottom,3);


//    if(BM1 == up)
//    {
//       ips200_clear();
//       pit_count_ms = 500;
//    }
}

/******************************************************************************
* 函数名称     : draw_boundary_points
* 描述         : 绘制左右边界点（蓝色）
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void draw_boundary_points(void)
{
    uint16_t i,j,k;
//    for (i = 0; i < boundary_point_num; i++)
//    {
//        ips200_draw_point(Total_boundary[i].x *2, Total_boundary[i].y *2, RGB565_BLUE);
//    }
    for (j = 0; j < left_boundary_len; j++)
    {
        ips200_draw_point(left_boundary[j].x *2, left_boundary[j].y *2, RGB565_BLUE);
    }
    for (k = 0; k < right_boundary_len; k++)
    {
        ips200_draw_point(right_boundary[k].x *2, right_boundary[k].y *2, RGB565_BLUE);
    }

}

/******************************************************************************
* 函数名称     : draw_center_points
* 描述         : 绘制赛道中点线(红色)
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void draw_center_points(void)
{
    for (uint8_t i = 0; i < center_point_num; i++)
    {
        ips200_draw_point(center_points_new[i].x *2, center_points_new[i].y *2, RGB565_RED);
    }

}


