#include "image.h"
#include "Control.h"

uint8_t data_ready_flag_for_core0=0;//图像双核缓冲标志

//存储各种处理之后的图像
uint8 imgZip[my_camera_h][my_camera_w];
uint8 img_binary[my_camera_h][my_camera_w];
uint8_t img_copy[my_camera_h][my_camera_w];

//////////////////SPI图传内容/////////////////////
uint8 img_SPI[my_camera_h][my_camera_w];
uint8 xy_x1_boundary[BOUNDARY_NUM], xy_x2_boundary[BOUNDARY_NUM], xy_x3_boundary[BOUNDARY_NUM];
uint8 xy_y1_boundary[BOUNDARY_NUM], xy_y2_boundary[BOUNDARY_NUM], xy_y3_boundary[BOUNDARY_NUM];
//////////////////SPI图传内容/////////////////////

//经过方差法计算出的最佳阈值
uint8_t nowThreshold;

//中线偏置
int16_t Middle_Line_x = my_camera_w / 2;
int16_t Middle_Line_x_left = my_camera_w / 4;
int16_t Middle_Line_x_right = (my_camera_w * 3) / 4;
int16_t center_point_x_average = 47;

//陀螺仪补偿值
int16_t Gyr_Sum = 0;
uint8_t Gyr_dir = 0;
//计时器
uint32_t tim;

//判断标志位
uint8_t state_flag = 1;
uint8_t turning_state;
uint8_t end_turning_state = 1;
uint8_t protect_flag = 0;   //0为未保护 1为保护
uint8_t ban_transisitor = 0;
uint8_t Direction_Planning = 0;

//中线参数
#define Boundary_point_gap 50

//转弯参数
uint8_t Img_Gap_left = 22;
uint8_t Img_Gap_right = 22;
#define frame_judge 1


/***************************************************
  * @brief    取绝对值
  * @param    value
  * @return   |value|
  * @note     无
  *************************************************/
int my_abs(int value)
{
    return value >= 0 ? value : -value;
}

/***************************************************
  * @brief    压缩2倍图像
  * @param    压缩前图像:Image_src[MT9V03X_H][MT9V03X_W] 压缩后图像:uint8 Image_tar[my_camera_h][my_camera_w]
  * @return   无
  * @note     优化了计算效率和内存访问
  *************************************************/
void Get_Zip_Image_Factor2(uint8 Image_src[MT9V03X_H][MT9V03X_W],
                           uint8 Image_tar[my_camera_h][my_camera_w])
{
    const int tar_w = MT9V03X_W / 2;
    const int tar_h = MT9V03X_H / 2;
    for (int i = 0; i < tar_h; i++)
    {
        uint8 *src_row0 = (uint8*)&Image_src[i*2][0];
        uint8 *src_row1 = src_row0 + MT9V03X_W;
        uint8 *tar_row = (uint8*)&Image_tar[i][0];
        for (int j = 0; j < tar_w; j++)
        {
            uint16 sum = src_row0[0] + src_row0[1] + src_row1[0] + src_row1[1];
            tar_row[j] = sum >> 2;  // 除以4用移位
            src_row0 += 2;
            src_row1 += 2;
        }
    }
}

/***************************************************
  * @brief    使用直方图法求阈值
  * @param    image[my_camera_h][my_camera_w]  求阈值的图片
  * @return   Threshold
  * @note     优化了计算效率和内存访问
  *************************************************/
uint8_t Iterative_Threshold_Fast(unsigned char image[][my_camera_w],
                                 uint8 start_y, uint8 start_x,
                                 uint8 end_h, uint8 end_w)
{
    uint32_t hist[256] = {0};
    // 一次遍历，统计直方图
    for (int i = start_y; i < end_h; i++) {
        for (int j = start_x; j < end_w; j++) {
            hist[image[i][j]]++;
        }
    }

    uint8_t threshold = 128;
    uint8_t new_threshold;
    do {
        uint32_t sum1 = 0, count1 = 0;
        uint32_t sum2 = 0, count2 = 0;
        for (int g = 0; g < 256; g++) {
            if (g > threshold) {
                sum1 += g * hist[g];
                count1 += hist[g];
            } else {
                sum2 += g * hist[g];
                count2 += hist[g];
            }
        }
        uint8_t mean1 = count1 ? sum1 / count1 : 0;
        uint8_t mean2 = count2 ? sum2 / count2 : 0;
        new_threshold = (mean1 + mean2) / 2;
        if (abs(new_threshold - threshold) <= 1) break;
        threshold = new_threshold;
    } while (1);
    return threshold;
}

/***************************************************
  * @brief    大津法求阈值
  * @param
  * @return   无
  * @note     优化了计算效率和内存访问
  *************************************************/
uint8 otsuThreshold_fast(void)
{
    #define GrayScale 256

    int width = MT9V03X_W / 2;
    int height = MT9V03X_H / 2;
    int bottom_start = height / 2;
    int start_x = 2;
    int end_x = width - 2;

    uint16 pixelCount[GrayScale] = {0};
    uint16 bottomPixelCount[GrayScale] = {0};
    uint32 pixelSum = 0;
    uint32 gray_sum = 0;
    uint32 bottomGraySum = 0;
    uint32 bottomGraySumSq = 0;
    uint32 bottomPixelSum = 0;
    uint8 Pixel_Max = 0;
    uint8 Pixel_Min = 255;


    for (int i = 0; i < height; i += 2)
    {
        uint8 *row = imgZip[i];
        int is_bottom = (i >= bottom_start);
        for (int j = start_x; j < end_x; j += 2)
        {
            uint8 val = row[j];
            pixelCount[val]++;
            gray_sum += val;
            pixelSum++;

            if (is_bottom)
            {
                bottomPixelCount[val]++;
                bottomPixelSum++;
                bottomGraySum += val;
                bottomGraySumSq += (uint32)val * val;
            }

            if (val > Pixel_Max) Pixel_Max = val;
            if (val < Pixel_Min) Pixel_Min = val;
        }
    }

    if (pixelSum == 0) return 100;

    // ---------- 前缀锟酵ｏ拷uint32 锟姐够锟斤拷锟斤拷锟揭讹拷锟杰猴拷 ~1.13M锟斤拷----------
    uint16 prefixCount[GrayScale];
    uint32 prefixGraySum[GrayScale];
    uint32 cumCount = 0;
    uint32 cumGray = 0;
    for (int g = Pixel_Min; g <= Pixel_Max; g++)
    {
        cumCount += pixelCount[g];
        cumGray += (uint32)g * pixelCount[g];
        prefixCount[g] = (uint16)cumCount;
        prefixGraySum[g] = cumGray;
    }
    uint32 totalCount = prefixCount[Pixel_Max];
    uint32 totalGray = prefixGraySum[Pixel_Max];
    float invN = 1.0f / totalCount;

    // ---------- Otsu锟斤拷float锟斤拷TC1.6 硬锟斤拷 FPU锟斤拷----------
    uint8 bestTh = 0;
    float maxVar = 0.0f;

    for (int th = Pixel_Min; th < Pixel_Max; th++)
    {
        uint32 cnt0 = prefixCount[th];
        if (cnt0 == 0 || cnt0 == totalCount) continue;

        uint32 cnt1 = totalCount - cnt0;
        uint32 sum0 = prefixGraySum[th];
        uint32 sum1 = totalGray - sum0;
        float diff = (float)sum0 / cnt0 - (float)sum1 / cnt1;
        float var = (float)cnt0 * invN * (float)cnt1 * invN * diff * diff;

        if (var > maxVar)
        {
            maxVar = var;
            bestTh = th;
        }
    }

    // ---------- Otsu 锟斤拷锟叫ｏ拷锟  & 锟阶诧拷锟斤拷锟斤拷统锟斤拷 ----------
    uint32 cnt0 = prefixCount[bestTh];
    uint32 sum0 = prefixGraySum[bestTh];
    float mean0 = (float)sum0 / cnt0;
    float mean1 = (float)(totalGray - sum0) / (totalCount - cnt0);
    float class_gap = mean1 - mean0;

    uint32 white_cnt = totalCount - cnt0;
    float white_ratio = (float)white_cnt * invN;

    float bg_std = 0.0f;
    uint8 bg_ref = 100;
    if (bottomPixelSum > 0)
    {
        uint32 cum = 0;
        for (int g = 0; g < GrayScale; g++)
        {
            cum += bottomPixelCount[g];
            if (cum >= bottomPixelSum * 0.95f)
            {
                bg_ref = g;
                break;
            }
        }
        float bg_mean = (float)bottomGraySum / bottomPixelSum;
        float bg_var = (float)bottomGraySumSq / bottomPixelSum - bg_mean * bg_mean;
        if (bg_var > 0) bg_std = sqrtf(bg_var);
    }

    uint8 final_th = bestTh;

    // Otsu 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟阶碉拷太锟斤拷 OR 锟斤拷锟斤拷值锟斤拷太小锟斤拷锟街碉拷锟角★拷锟斤拷锟斤拷 vs 锟斤拷锟解”锟斤拷
    int otsu_unreliable = (white_ratio < 0.04f) || (class_gap < 30.0f);

    if (otsu_unreliable && bottomPixelSum > 0)
    {
        // 锟斤拷锟斤拷应偏锟狡ｏ拷锟斤拷锟斤拷越强锟斤拷bg_std 越锟襟）ｏ拷偏锟斤拷越锟斤拷
        int offset = 20 + (int)(bg_std * 0.5f);
        if (offset > 40) offset = 40;
        int new_th = bg_ref + offset;
        final_th = (new_th > 70) ? new_th : 70;
    }

    // 锟睫凤拷
    if (final_th < 50) final_th = 50;
    if (final_th > 160) final_th = 160;

    // 帧锟斤拷平锟斤拷
    static uint8 prev_th = 100;
    final_th = (prev_th * 3 + final_th) / 4;
    prev_th = final_th;

    return final_th;
}

/********************************/
//图像二值化变量
uint8 img_threshold_group[3];    //划分阈值区域   上中下

uint8 close_Threshold = 0;
uint8 mid_Threshold = 0;  //根据需求调整
uint8 far_Threshold = 0;

uint8 white_num_row[my_camera_h];    //存储每行白点数
uint8 white_num_col[my_camera_w];    //存储每列白点数
uint16_t white_num_sum;              //整个屏幕的总白点数
/********************************/
/******************************************************************************
* 函数名称     : Get_img_binary
* 描述         : 进行图像二值化并进行(可选)噪点去除
* 进入参数     : void
* 注:         腐蚀和扩张操作耗费很长时间，谨慎添加使用
* 返回参数     : void
******************************************************************************/
void Get_img_binary(void)
{
    // 1. 初始化数组
    memset(img_binary, Black, sizeof(img_binary));  //先把屏幕全黑
    memset(img_SPI, Black, sizeof(img_SPI));
    memset(white_num_row, 0, sizeof(white_num_row));
    memset(white_num_col, 0, sizeof(white_num_col));
    white_num_sum = 0;

    img_threshold_group[0] = nowThreshold - far_Threshold;      //远景       ///远景多降，能清晰识别更远
    img_threshold_group[1] = nowThreshold - mid_Threshold;      //中景
    img_threshold_group[2] = nowThreshold + close_Threshold;    //近景       //近景少加，能看清最近

    uint8_t threshold_group_num = 0;
    int Loop[9][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};  //一个像素点的八个邻域点

    for(int8_t i = 0;i <= my_camera_h-1 ;i++)
    {
        for(int8_t j = 0;j <= my_camera_w-1 ;j++)
        {
            uint8_t White_Num = 0;
            uint8_t Black_Num = 0;
            if(i >= 0 && i < 20)  //当位于远景时
            {
                threshold_group_num = 0;
                if(imgZip[i][j] >= img_threshold_group[threshold_group_num])
                {
                    img_binary[i][j] = White;
                    white_num_row[i]++;  //计算每行白点数
                    white_num_col[j]++;  //计算每列白点数
                    white_num_sum++;

                }

            }
            else if(i >= 20 && i < 40)  //当位于中景时
            {
                threshold_group_num = 1;
                if(imgZip[i][j] >= img_threshold_group[threshold_group_num])
                {
                    img_binary[i][j] = White;
                    white_num_row[i]++;  //计算每行白点数
                    white_num_col[j]++;  //计算每列白点数
                    white_num_sum++;
                }
            }
            else if(i >= 40 && i < 60)  //当位于近景时
            {
                threshold_group_num = 2;
                if(imgZip[i][j] >= img_threshold_group[threshold_group_num])
                {
                    img_binary[i][j] = White;
                    white_num_row[i]++;  //计算每行白点数
                    white_num_col[j]++;  //计算每列白点数
                    white_num_sum++;
                }
            }
        }
    }
//////////////////////////////保护小车///////////////////////////////////////////
    if(white_num_sum <= 100)
    {
        protect_flag = 1;
    }
    else if(white_num_sum > 100)
    {
        protect_flag = 0;
    }
//////////////////////////////保护小车///////////////////////////////////////////
}

/********************************/
// 起点信息
uint16_t start_center_y = 0;
uint16_t start_point_l_x = 0,start_point_r_x = 0;
uint16_t deal_bottom=my_camera_h-1;
uint16_t l_point_x = 0, r_point_x = 0;
uint16_t bottom_center_x;
uint16_t bottom_center_y;
int16_t left_point[10];
int16_t right_point[10];
uint16_t left_point_num = 0;
uint16_t right_point_num = 0;
int16_t left_Point_Gap = 0;
int16_t right_Point_Gap = 0;
int16_t current_left_Point_Gap = 60;
int16_t current_right_Point_Gap = 60;
/********************************/
/******************************************************************************
* 函数名称     : find_track_start_point
* 描述         : 在图像底部寻找赛道左右边界的起点
* 进入参数     : void
* 返回参数     : uint8_t 1:成功找到起点 0:未找到起点
******************************************************************************/
uint8_t find_track_start_point(void)
{
    memset(left_point, 0, sizeof(left_point));
    memset(right_point, 0, sizeof(right_point));
    bottom_center_x = 47;
    bottom_center_y = 59;
    left_point_num = 0;
    right_point_num = 0;
    left_Point_Gap = 0;
    right_Point_Gap = 0;
    current_left_Point_Gap = 60;
    current_right_Point_Gap = 60;

    for(uint16_t i = deal_bottom; i > deal_bottom - 15 ; i--)    // 从底部向上搜索合适的起始行（使用行白点数作为判断条件）
    {
        uint8_t l_found = 0, r_found = 0;
        start_center_y = i;
        if(transistor_Num == 40)  //特殊运放处巡线处理
        {
            for(uint16_t j = 1; j < my_camera_w - 2; j++) // 从左向右寻找左边界（黑到白跳变）
            {
                if(img_binary[i][j] == Black && img_binary[i][j+1] == White && img_binary[i][j+2] == White)
                {
                    left_point[left_point_num] = j + 1;
                    left_point_num ++;
                    l_found = 1;
                }
            }
            for(uint16_t m = 0; m < left_point_num; m++)
            {
                left_Point_Gap = my_abs(Middle_Line_x - left_point[m]);
                if(left_Point_Gap < current_left_Point_Gap)
                {
                    current_left_Point_Gap = left_Point_Gap;
                    l_point_x = left_point[m];
                }
            }
        }
        else
        {
            for(uint16_t j = 1; j < my_camera_w - 2; j++) // 从左向右寻找左边界（黑到白跳变）
            {
                if(img_binary[i][j] == Black && img_binary[i][j+1] == White && img_binary[i][j+2] == White)
                {
                    l_point_x = j + 1;
                    l_found = 1;
                    break;
                }
            }
        }

        for(uint16_t j = my_camera_w - 2; j > 1; j--) // 从右向左寻找右边界（白到黑跳变）
        {
            if(img_binary[i][j-1] == White && img_binary[i][j] == White && img_binary[i][j+1] == Black)
            {
                r_point_x = j;
                r_found = 1;
                break;
            }
        }
        uint16_t Start_Point_Gap = my_abs(r_point_x - l_point_x);   //在赛道比较脏的情况下使用
//        if(l_found && r_found && Start_Point_Gap < 30)
          if(l_found && r_found)
        {
               start_point_l_x = l_point_x; //此处获取到左扫右点的起始点的X坐标
               start_point_r_x = r_point_x; //此处获取到右扫左点的起始点的X坐标
               bottom_center_x = (l_point_x + r_point_x) / 2;
               bottom_center_y = i;
               return 1;
        }
    }
    return 0;
}

/********************************/
//8领域变量区域
uint8_t img_boundary[my_camera_h][my_camera_w]; //边界点
BoundaryPoint Total_boundary[2000];   // 边界点数组
BoundaryPoint left_boundary[1000];   // 左边界点数组
BoundaryPoint right_boundary[1000];  // 右边界点数组
Corner_Point corner_points[100];     //拐点数组
uint16_t left_boundary_len = 0;      // 左边界点数量
uint16_t right_boundary_len = 0;     // 右边界点数量

uint16_t left_boundary_x_sum;
int16_t left_boundary_average_x;
uint16_t right_boundary_x_sum;
int16_t right_boundary_average_x;
int16_t boundary_gap;
uint16_t last_white_x_l,last_white_y_l,last_white_x_r,last_white_y_r,boundary_point_num,corner_point_num;
BoundaryTouchFlags total_touch_flags;
int start_dir,enter_dir;

Four_Edge_Point left_edge_point;
Four_Edge_Point right_edge_point;
Four_Edge_Point up_edge_point;
Four_Edge_Point down_edge_point;


uint16_t tmp_right_corner_point_x;
uint16_t tmp_left_corner_point_x;
uint16_t tmp_right_corner_point_y;
uint16_t tmp_left_corner_point_y;

uint16_t right_corner_point_x;
uint16_t left_corner_point_x;
uint16_t right_corner_point_y = 0;
uint16_t left_corner_point_y = 0;
/********************************/
/******************************************************************************
* 函数名称     : trace_boundary
* 描述         : 赛道8邻域爬线 + 四方位判断
* 进入参数     : start_x_l, start_y_l,start_x_r,start_y_r - 起始点坐标
* 返回参数     : void
******************************************************************************/

void trace_boundary(uint16_t start_x_l, uint16_t start_y_l,uint16_t start_x_r,uint16_t start_y_r)
{
    int Loop_left[8][2] =  {{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}}; //一个像素点的八个邻域点 (x,y) 这个是逆时针扫描左边界的
    int Loop_right[8][2] = {{-1,0}, {-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}}; //一个像素点的八个邻域点 (x,y) 这个是顺时针扫描右边界的

    memset(img_boundary, 0, sizeof(img_boundary));
    memset(Total_boundary, 0, sizeof(Total_boundary));

    total_touch_flags.touch_top = 0;
    total_touch_flags.touch_bottom = 0;
    total_touch_flags.touch_left = 0;
    total_touch_flags.touch_right = 0;
    uint16_t tmp_left_point_y = 0;
    uint16_t tmp_right_point_y = 0;
    up_edge_point.x = 47;
    up_edge_point.y = 0;
    left_edge_point.x = 47;
    left_edge_point.y = 0;
    right_edge_point.x = 47;
    right_edge_point.y = 0;

    left_boundary_x_sum = 0;
    left_boundary_average_x = 0;
    right_boundary_x_sum = 0;
    right_boundary_average_x = 0;
    boundary_gap = 0;

    boundary_point_num = 0;
    left_boundary_len = 0;
    right_boundary_len = 0;
    corner_point_num = 0;

    last_white_x_l = start_x_l;
    last_white_y_l = start_y_l;
    Total_boundary[boundary_point_num].x = start_x_l;
    Total_boundary[boundary_point_num].y = start_y_l;
//    img_boundary[start_y_l][start_x_l] = Yellow; // 标记已访问
    boundary_point_num++;

    last_white_x_r = start_x_r;
    last_white_y_r = start_y_r;
    Total_boundary[boundary_point_num].x = start_x_r;
    Total_boundary[boundary_point_num].y = start_y_r;
//    img_boundary[start_y_r][start_x_r] = Yellow; // 标记已访问
    boundary_point_num++;

    int found_radius = 0;
    int First_dir = -1;

    for(uint16_t m = 0;m < 150; m++)
    {
        uint8_t find_point_flag = 0;
        if(First_dir == -1)
        {
            start_dir = 0;
        }
        else
        {
            enter_dir = (First_dir + 4) % 8;  // 初始进入方向：从起点到第一个白色点的方向的反方向
            start_dir = (enter_dir + 1) % 8;
        }
        for(uint8_t k = 0; k < 8; k++)
        {
            int16_t nx_l = last_white_x_l + Loop_left[(start_dir+k) % 8][0];
            int16_t ny_l = last_white_y_l + Loop_left[(start_dir+k) % 8][1];
            if(nx_l < 0 || nx_l >= my_camera_w || ny_l < 0 || ny_l >= my_camera_h)
                continue;
            int prev_k = ((start_dir+k) % 8 == 0) ? 7 : ((start_dir+k) % 8 - 1);
            int16_t last_nx_l = last_white_x_l + Loop_left[prev_k][0];
            int16_t last_ny_l = last_white_y_l + Loop_left[prev_k][1];

            if(img_binary[last_ny_l][last_nx_l] == White && img_binary[ny_l][nx_l] == Black)
            {
                last_white_x_l = nx_l;
                last_white_y_l = ny_l;
                Total_boundary[boundary_point_num].x = nx_l;       //记录边界点的X坐标
                Total_boundary[boundary_point_num].y = ny_l;       //记录边界点的Y坐标
                img_SPI[ny_l][nx_l] = White;
//                xy_x1_boundary[boundary_point_num] = nx_l;         //SPI图传记录边界点的X坐标
//                xy_y1_boundary[boundary_point_num] = ny_l;         //SPI图传记录边界点的y坐标

                boundary_point_num ++;
                left_boundary[left_boundary_len].x = nx_l;
                left_boundary[left_boundary_len].y = ny_l;
                left_boundary_x_sum += left_boundary[left_boundary_len].x;
                left_boundary_len ++;


                if(ny_l == 3)
                {
                    total_touch_flags.touch_top = 1;   //上
                    up_edge_point.x = nx_l;
                    up_edge_point.y = ny_l;
                }
                if(ny_l == my_camera_h - 5)
                {
                    total_touch_flags.touch_bottom = 1;   //下
                }
                if(nx_l == 2 && ny_l >= 5 && ny_l <= 55 && (transistor_Num < 19 || transistor_Num > 23) && turn_count != 27)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_l >= tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_l;
                        left_edge_point.x = nx_l;
                        left_edge_point.y = ny_l;
                    }
                }
                if(nx_l == my_camera_w - 2 && ny_l >= 5 && ny_l <= 55)
                {
                    total_touch_flags.touch_right = 1;   //右
                    if(ny_l >= tmp_right_point_y)
                    {
                        tmp_right_point_y = ny_l;
                        right_edge_point.x = nx_l;
                        right_edge_point.y = ny_l;
                    }
                }
////////////////////////////////////////////边界特殊处理///////////////////////////////////////////////
                if(nx_l == 2 && ny_l >= 20 && ny_l <= 55 && transistor_Num >= 19 && transistor_Num <= 23)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_l >= tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_l;
                        left_edge_point.x = nx_l;
                        left_edge_point.y = ny_l;
                    }
                }
                if(nx_l == 15 && ny_l >= 5 && ny_l <= 55 && turn_count == 27)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_l >= tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_l;
                        left_edge_point.x = nx_l;
                        left_edge_point.y = ny_l;
                    }
                }

////////////////////////////////////////////边界特殊处理///////////////////////////////////////////////
                find_point_flag = 1;
                if(First_dir == -1)
                {
                    First_dir = k;
                }
                else
                {
                    First_dir = (start_dir+k) % 8;
                }
                break;   //当找到八领域的相邻白点时跳出循环
            }
            else
            {

            }
         }
        if(find_point_flag == 0) break;
    }

    First_dir = -1;
    for(uint16_t m = 0;m < 150; m++)
    {
        uint8_t find_point_flag = 0;
        if(First_dir == -1)
        {
            start_dir = 0;
        }
        else
        {
            enter_dir = (First_dir + 4) % 8;  // 初始进入方向：从起点到第一个白色点的方向的反方向
            start_dir = (enter_dir + 1) % 8;
        }
        for(uint8_t k = 0; k < 8; k++)
        {
            int16_t nx_r = last_white_x_r + Loop_right[(start_dir+k) % 8][0];
            int16_t ny_r = last_white_y_r + Loop_right[(start_dir+k) % 8][1];
            if(nx_r < 0 || nx_r >= (my_camera_w - 1) || ny_r < 0 || ny_r >= my_camera_h)
                continue;
            int prev_k = ((start_dir+k) % 8 == 0) ? 7 : ((start_dir+k) % 8 - 1);
            int16_t last_nx_r = last_white_x_r + Loop_right[prev_k][0];
            int16_t last_ny_r = last_white_y_r + Loop_right[prev_k][1];
            if(img_binary[last_ny_r][last_nx_r] == White && img_binary[ny_r][nx_r] == Black)
            {
                last_white_x_r = nx_r;
                last_white_y_r = ny_r;
                Total_boundary[boundary_point_num].x = nx_r;       //记录边界点的X坐标
                Total_boundary[boundary_point_num].y = ny_r;       //记录边界点的Y坐标
                img_SPI[ny_r][nx_r] = White;
//                xy_x1_boundary[boundary_point_num] = nx_r;         //SPI图传记录边界点的X坐标
//                xy_y1_boundary[boundary_point_num] = ny_r;         //SPI图传记录边界点的y坐标

                boundary_point_num ++;
                right_boundary[right_boundary_len].x = nx_r;
                right_boundary[right_boundary_len].y = ny_r;
                right_boundary_x_sum += right_boundary[right_boundary_len].x;
                right_boundary_len ++;

                if(ny_r == 3)
                {
                    total_touch_flags.touch_top = 1;   //上
                    up_edge_point.x = nx_r;
                    up_edge_point.y = ny_r;
                }
                if(ny_r == my_camera_h - 2)
                {
                    total_touch_flags.touch_bottom = 1;   //下
                }
                if(nx_r == 2 && ny_r >= 5 && ny_r <= 55 && (transistor_Num < 19 || transistor_Num > 23) && turn_count != 27)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_r >= tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_r;
                        left_edge_point.x = nx_r;
                        left_edge_point.y = ny_r;
                    }
                }
                if(nx_r == my_camera_w - 2 && ny_r >= 5 && ny_r <= 55)
                {
                    total_touch_flags.touch_right = 1;   //右
                    if(ny_r >= tmp_right_point_y)
                    {
                        tmp_right_point_y = ny_r;
                        right_edge_point.x = nx_r;
                        right_edge_point.y = ny_r;
                    }
                }
////////////////////////////////////////////边界特殊处理///////////////////////////////////////////////
                if(nx_r == 2 && ny_r >= 20 && ny_r <= 55 && transistor_Num >= 19 && transistor_Num <= 23)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_r >= tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_r;
                        left_edge_point.x = nx_r;
                        left_edge_point.y = ny_r;
                    }
                }
                if(nx_r == 15 && ny_r >= 5 && ny_r <= 55 && turn_count == 27)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_r >= tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_r;
                        left_edge_point.x = nx_r;
                        left_edge_point.y = ny_r;
                    }
                }
////////////////////////////////////////////边界特殊处理///////////////////////////////////////////////
                find_point_flag = 1;
                if(First_dir == -1)
                {
                    First_dir = k;
                }
                else
                {
                    First_dir = (start_dir+k) % 8;
                }
                break;   //当找到八领域的相邻白点时跳出循环
            }
         }
        if(find_point_flag == 0) break;
     }
    left_boundary_average_x = left_boundary_x_sum / left_boundary_len;
    right_boundary_average_x = right_boundary_x_sum / right_boundary_len;
    boundary_gap = my_abs(right_boundary_average_x - left_boundary_average_x);
}


/********************************/
//中心点变量区域
Center_Point center_points_new[150];
uint8_t center_point[my_camera_h][my_camera_w];
uint16_t center_point_num;
uint16_t center_point_x_sum;
uint8_t center_point_num_row[my_camera_h];    //判断每行的中点数量

int8_t top_center_x_gap;
uint8_t top_center_x = 47;
uint8_t top_center_y = 59;
/********************************/
/******************************************************************************
* 函数名称     : find_center_point
* 描述         : 双指针获取爬线后的中点坐标,同时记录最低点和最高点
* 进入参数     : void
* 返回参数     : void
******************************************************************************/

void find_center_point(void)
{
        memset(center_points_new, 0, sizeof(center_points_new));
        memset(center_point_num_row, 0, sizeof(center_point_num_row));

        top_center_x_gap = 0;
        center_point_x_sum = 0;
        center_point_num = 0;
        top_center_x = 47;
        top_center_y = 59;

        uint16_t left_idx = 0;
        uint16_t right_idx = 0;

        while (left_idx < left_boundary_len && right_idx < right_boundary_len)
        {
            uint16_t left_y = left_boundary[left_idx].y;
            uint16_t right_y = right_boundary[right_idx].y;
            if (left_y == right_y)
            {
            // 相同Y坐标，计算中点
            uint16_t left_x = left_boundary[left_idx].x;
            uint16_t right_x = right_boundary[right_idx].x;
            int16_t Point_Gap = my_abs(right_x - left_x);
            // 检查有效性
            if (right_x > left_x)  // 宽度合理检查
            {
            if (center_point_num < 150 && Point_Gap < Boundary_point_gap)
            {
                  center_points_new[center_point_num].y = left_y;
                  center_points_new[center_point_num].x = (left_x + right_x) / 2;
                  center_point_x_sum += center_points_new[center_point_num].x;
                  if(center_points_new[center_point_num].y <= top_center_y)
                  {
                      top_center_x = center_points_new[center_point_num].x;
                      top_center_y = center_points_new[center_point_num].y;
                  }
                  center_point_num++;
            }
            }
            left_idx++;
            right_idx++;
            }
            else if (left_y > right_y)
            {
             left_idx++;           // 左边界Y更大（更靠下），移动左指针
            }
            else if (left_y < right_y)
            {
             right_idx++;          // 右边界Y更大，移动右指针
            }
        }
        center_point_x_average = center_point_x_sum / center_point_num;
        top_center_x_gap = my_abs(top_center_x - Middle_Line_x);

}

/********************************/
//直角拐弯处理函数
uint8_t Right_frames_judge_r = frame_judge;
uint8_t Right_frames_judge_l = frame_judge;
uint8_t Right_ban_flag = 0;
uint8_t Right_flag1 = 0;
uint8_t Right_flag2 = 0;
uint8_t Right_flag3 = 0;
/********************************/

/******************************************************************************
* 函数名称     : trace_Right_angle
* 描述         : 直角拐弯处理
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void trace_Right_angle(void)
{

      if(transistor_Num == 42 && Right_ban_flag == 1 && Right_flag2 == 1)
      {
         Right_ban_flag = 0;
      }
      if((total_touch_flags.touch_top == 0 || (total_touch_flags.touch_top == 1 && up_edge_point.x >= 47 && transistor_Num == 40)) && total_touch_flags.touch_left == 0 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_bottom == 1 && right_edge_point.y >= Img_Gap_right && state_flag == 1 && end_turning_state == 1 && boundary_gap < 12)
      {
        Right_frames_judge_r --;
        if(Right_frames_judge_r == 0 && (ban_transisitor == 0 || (ban_transisitor == 1 && transistor_Num == 3)) && transistor_Num != 29)
        {
          turning_state = 1; //标志着此时是需要直角右拐弯
          state_flag = 2;
          end_turning_state = 0;
          Right_frames_judge_r = frame_judge;
        }
        if(transistor_Num == 35 && Right_ban_flag == 1 && Right_flag1 == 1)
        {
            Right_ban_flag = 0;
        }
        if(transistor_Num == 35 && Right_ban_flag == 0 && Right_flag1 == 0)
        {
            Right_ban_flag = 1;
            Right_flag1 = 1;
        }
      }
      else
      {
          Right_frames_judge_r = frame_judge;
      }
      if((total_touch_flags.touch_top == 0 || (total_touch_flags.touch_top == 1 && up_edge_point.x <= 47 && turn_count == 27))  && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 0 && total_touch_flags.touch_bottom == 1 && left_edge_point.y >= Img_Gap_left && state_flag == 1 && end_turning_state == 1 && boundary_gap < 12)
      {
        Right_frames_judge_l --;
        if(Right_frames_judge_l == 0 && ban_transisitor == 0 && Right_ban_flag != 1)
        {
          turning_state = 1; //标志着此时是需要直角左拐弯
          state_flag = 3;
          end_turning_state = 0;
          Right_frames_judge_l = frame_judge;
        }
        if(transistor_Num == 41 && Right_ban_flag == 0 && Right_flag2 == 0)
        {
            Right_ban_flag = 1;
            Right_flag2 = 1;
        }
      }
      else
      {
          Right_frames_judge_l = frame_judge;
      }
}

/********************************/
//MOS管断路处理函数
uint8_t MOS_Left_Flag_over;
uint8_t MOS_Right_Flag_over;
uint8_t MOS_Judge_Flag = 0;
uint8_t MOS_of_Curves_Sign = 0;
uint8_t ban_MOS = 0;
uint8_t MOS_Series_of_Curves_Sign = 0;

uint8_t MOS_Left_Flag;
uint8_t MOS_Right_Flag;

uint16_t MOS_Right_Top_point_y;
uint16_t MOS_Right_Bottom_point_y;
uint16_t MOS_Left_Top_point_y;
uint16_t MOS_Left_Bottom_point_y;
uint8_t MOS_Right_Gap;
uint8_t MOS_Left_Gap;

uint8_t MOS_Judge_frames = 2;
/********************************/

/******************************************************************************
* 函数名称     :MOS_Disconnect
* 描述        :MOS管断路处理
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void MOS_Disconnect(void)
{
    MOS_Left_Flag = 0;
    MOS_Right_Flag = 0;

    MOS_Left_Flag_over = 0;
    MOS_Right_Flag_over = 0;

    MOS_Right_Top_point_y = 0;
    MOS_Right_Bottom_point_y = 0;
    MOS_Left_Top_point_y = 0;
    MOS_Left_Bottom_point_y = 0;
    MOS_Right_Gap = 0;
    MOS_Left_Gap = 0;

    //CCD扫线判断MOS管
    for(uint16_t i = 5; i < 55; i++)    // 从顶部向下搜索左侧
    {
        if(img_binary[i][2] == Black && img_binary[i+1][2] == Black && img_binary[i+2][2] == White)  //先找到最底部点
        {
            MOS_Left_Top_point_y = i+1;
            break;
        }
    }
    for(uint16_t m = 55; m > 5; m--)    // 从底部向上搜索左侧
    {
        if(img_binary[m][2] == Black && img_binary[m-1][2] == Black && img_binary[m-2][2] == White)  //先找到最底部点
        {
            MOS_Left_Bottom_point_y = m-1;
            break;
        }
    }
    for(uint16_t j = 5; j < 55; j++)    // 从顶部向下搜索右侧
    {
        if(img_binary[j][91] == Black && img_binary[j+1][91] == Black && img_binary[j+2][91] == White)  //先找到最底部点
        {
            MOS_Right_Top_point_y = j+1;
            break;
        }
    }

    for(uint16_t n = 55; n > 5; n--)    // 从底部向上搜索右侧
    {
        if(img_binary[n][91] == Black && img_binary[n-1][91] == Black && img_binary[n-2][91] == White)  //先找到最底部点
        {
            MOS_Right_Bottom_point_y = n-1;
            break;
        }
    }

    if(MOS_Left_Top_point_y != 0 && MOS_Left_Bottom_point_y != 0)
    {
        MOS_Left_Gap = MOS_Left_Bottom_point_y - MOS_Left_Top_point_y;
        if(MOS_Left_Gap <= 10 && MOS_Left_Gap > 1)
        {
            MOS_Left_Flag = 1;
        }
    }
    if(MOS_Right_Top_point_y != 0 && MOS_Right_Bottom_point_y != 0)
    {
        MOS_Right_Gap = MOS_Right_Bottom_point_y - MOS_Right_Top_point_y;
        if(MOS_Right_Gap <= 10 && MOS_Right_Gap > 1)
        {
            MOS_Right_Flag = 1;
        }

    }
    //CCD扫线判断MOS管

    if((MOS_Left_Flag == 1 || MOS_Right_Flag == 1) && MOS_Series_of_Curves_Sign == 0 && (MOS_Left_Bottom_point_y >= Img_Gap_left || MOS_Right_Bottom_point_y >= Img_Gap_right) && state_flag == 1 && end_turning_state == 1 && MOS_Judge_Flag == 1 && Direction_Planning == 0)
    { //MOS管直行处理
        ban_transisitor = 1;
        MOS_Series_of_Curves_Sign = 1;
    }
    if(total_touch_flags.touch_bottom == 1 && MOS_Left_Flag == 0 && MOS_Right_Flag == 0 && MOS_Series_of_Curves_Sign == 1)
    {
        MOS_Judge_frames --;
        if(MOS_Judge_frames == 0)
        {
            transistor_Num++;
            ban_transisitor = 0;
            MOS_Series_of_Curves_Sign = 0;
            MOS_Judge_Flag = 0;
        }
    }
    else
    {
        MOS_Judge_frames = 3;
    }


    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_left == 0 && total_touch_flags.touch_right == 0 && total_touch_flags.touch_bottom == 1 && MOS_Right_Bottom_point_y >= Img_Gap_right && MOS_Right_Flag == 1 && state_flag == 1 && end_turning_state == 1 && MOS_Judge_Flag == 1)
    { //MOS管右拐处理
        MOS_Judge_frames --;
        if(Gyr_dir == 2 && MOS_Judge_frames == 0 && ban_transisitor == 0)
        {
            MOS_Judge_frames = frame_judge;
            transistor_Num ++;
            end_turning_state = 0;
            turning_state = 1;
            state_flag = 2;
            MOS_Judge_Flag = 0;
        }
    }
    else
    {
        MOS_Judge_frames = frame_judge;
    }

    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_left == 0 && total_touch_flags.touch_right == 0 && total_touch_flags.touch_bottom == 1 && MOS_Left_Bottom_point_y >= Img_Gap_left && MOS_Left_Flag == 1 && state_flag == 1 && end_turning_state == 1 && MOS_Judge_Flag == 1)
    { //MOS管左拐处理
        MOS_Judge_frames --;
        if(Gyr_dir == 1 && MOS_Judge_frames == 0 && ban_transisitor == 0)
        {

            MOS_Judge_frames = frame_judge;
            transistor_Num ++;
            end_turning_state = 0;
            turning_state = 1;
            state_flag = 3;
            MOS_Judge_Flag = 0;
        }
     }
    else
    {
        MOS_Judge_frames = frame_judge;
    }

    if(total_touch_flags.touch_top == 0 && total_touch_flags.touch_bottom == 1 && state_flag == 1 && end_turning_state == 1 && MOS_Judge_Flag == 1 && ban_MOS == 0)
    {
        if(Gyr_dir == 1 && MOS_Left_Bottom_point_y >= Img_Gap_left)  //MOS管左拐
        {
            MOS_Judge_frames --;
            if(MOS_Judge_frames == 0 && MOS_Left_Flag == 1 && MOS_Right_Flag == 1 && ban_transisitor == 0)
            {
                MOS_Judge_frames = frame_judge;
                transistor_Num ++;
                end_turning_state = 0;
                turning_state = 1;
                state_flag = Direction_Planning;
                MOS_Judge_Flag = 0;
            }
        }
        if(Gyr_dir == 2 && MOS_Right_Bottom_point_y >= Img_Gap_right)  //MOS管右拐
        {
            MOS_Judge_frames --;
            if(MOS_Judge_frames == 0 && MOS_Left_Flag == 1 && MOS_Right_Flag == 1 && ban_transisitor == 0)
            {
                MOS_Judge_frames = frame_judge;
                transistor_Num ++;
                end_turning_state = 0;
                turning_state = 1;
                state_flag = Direction_Planning;
                MOS_Judge_Flag = 0;
            }
        }

    }
    else
    {
        MOS_Judge_frames = frame_judge;
    }


}

/********************************/
//三极管处理函数
uint8_t transistor_Num = 0;

uint8_t transistor_Judge_frames_r = frame_judge;
uint8_t transistor_Judge_frames_l = frame_judge;
uint8_t transistor_Judge_frames_t = frame_judge;

uint8_t Special_flag = 0;
/********************************/
/******************************************************************************
* 函数名称     :trace_transistor
* 描述         : 三极管拐弯处理
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void trace_transistor(void)
{

    Final_Road();           //路径规划

    MOS_Disconnect();      //MOS管断路处理

    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_left == 0 && total_touch_flags.touch_bottom == 1 && right_edge_point.y >= Img_Gap_right && state_flag == 1 && end_turning_state == 1 && boundary_gap >= 12) //右转的T字口
    {
        transistor_Judge_frames_r --;
        if(transistor_Judge_frames_r == 0 && ban_transisitor == 0 && Right_ban_flag != 1)
        {
            transistor_Judge_frames_r = frame_judge;
            transistor_Num ++;          // 经过的三极管数量
            end_turning_state = 0;
            turning_state = 1;
            state_flag = Direction_Planning;
            MOS_Judge_Flag = 0;
        }
    }
    else
    {
        transistor_Judge_frames_r = frame_judge;
    }

    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 0 && total_touch_flags.touch_bottom == 1 && left_edge_point.y >= Img_Gap_left && state_flag == 1 && end_turning_state == 1 && boundary_gap >= 12) //左转的T字口
    {
        transistor_Judge_frames_l --;
        if(transistor_Judge_frames_l == 0 && ban_transisitor == 0 && Right_ban_flag != 1)
        {
            transistor_Judge_frames_l = frame_judge;
            transistor_Num++;          // 经过的三极管数量
            end_turning_state = 0;
            turning_state = 1;
            state_flag = Direction_Planning;
            MOS_Judge_Flag = 0;

        }
    }
    else
    {
        transistor_Judge_frames_l = frame_judge;
    }

    if(total_touch_flags.touch_top == 0 && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_bottom == 1  && state_flag == 1 && end_turning_state == 1 && boundary_gap >= 12) //左右转的T字口
    {
        if(Gyr_dir == 1 && left_edge_point.y >= Img_Gap_left)   //T字形左转
        {
            transistor_Judge_frames_t --;
            if(transistor_Judge_frames_t == 0 && ban_transisitor == 0 && Right_ban_flag != 1)
            {
                transistor_Judge_frames_t = frame_judge;
                transistor_Num++;
                end_turning_state = 0;
                turning_state = 1;
                state_flag = Direction_Planning;
                MOS_Judge_Flag = 0;

            }
        }
        if(Gyr_dir == 2 && right_edge_point.y >= Img_Gap_right)   //T字形右转
        {
            transistor_Judge_frames_t --;
            if(transistor_Judge_frames_t == 0 && ban_transisitor == 0 && Right_ban_flag != 1)
            {
                transistor_Judge_frames_t = frame_judge;
                transistor_Num++;
                end_turning_state = 0;
                turning_state = 1;
                state_flag = Direction_Planning;
                MOS_Judge_Flag = 0;
            }
        }
    }
    else
    {
        transistor_Judge_frames_t = frame_judge;
    }

}

/********************************/
//最终路径规划函数
uint8_t transistor_frames_judge;
uint8_t Series_of_Curves_Sign = 0;
uint8_t End_Judge_frames = 2;
uint8_t Road_Planning[100] = {0,2,2,0,0,2,0,2,0,2,0,2,2,1,2,0,0,1,1,0,0,0,0,0,2,0,5,3,1,0,0,0,1,1,2,0,0,0,0,2,0,1,2,0,2,1,1,1,0,0};   //0为直走,1为左转,2为右转，3为MOS管左转，4为MOS管右转,5为MOS管直走

/********************************/
/******************************************************************************
* 函数名称     : Final_Road
* 描述         : 最终路径规划 目前的路径规划是遇到的前三个三极管忽视
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void Final_Road(void)
{

     if(Road_Planning[transistor_Num] == 3)
     {
         MOS_Judge_Flag = 1;
         Direction_Planning = 3;
         Gyr_dir = 1;
     }
     if(Road_Planning[transistor_Num] == 4)
     {
         MOS_Judge_Flag = 1;
         Direction_Planning = 2;
         Gyr_dir = 2;
     }
     if(Road_Planning[transistor_Num] == 5)
      {
          MOS_Judge_Flag = 1;
          Direction_Planning = 0;
          Gyr_dir = 0;
      }

    if(Road_Planning[transistor_Num] == 0 && MOS_Judge_Flag == 0)
    {
        if((total_touch_flags.touch_right == 1 || total_touch_flags.touch_left == 1) && Series_of_Curves_Sign == 0 && (left_edge_point.y >= Img_Gap_left || right_edge_point.y >= Img_Gap_right) && state_flag == 1 && boundary_gap >= 12 && turn_count != 26)
        {
            ban_transisitor = 1;
            Series_of_Curves_Sign = 1;
        }

        if(total_touch_flags.touch_bottom == 1 && total_touch_flags.touch_right == 0 && (total_touch_flags.touch_left == 0 || (total_touch_flags.touch_left == 1 && transistor_Num == 40)) && Series_of_Curves_Sign == 1 )
        {
            End_Judge_frames --;
            if(End_Judge_frames == 0)
            {
                transistor_Num++;
                ban_transisitor = 0;
                Series_of_Curves_Sign = 0;
            }
        }
        else
        {
            End_Judge_frames = 2;
        }

    }
    else if(Road_Planning[transistor_Num] == 1 && MOS_Judge_Flag == 0)
    {
        Direction_Planning = 3;
        Gyr_dir = 1;

    }
    else if(Road_Planning[transistor_Num] == 2 && MOS_Judge_Flag == 0)
    {
        Direction_Planning = 2;
        Gyr_dir = 2;
    }


}

///******************************************************************************
//* 函数名称     : End_of_turning_state
//* 描述         : 转弯结束标志
//* 进入参数     : void
//* 返回参数     : void
//******************************************************************************/
void End_of_turning_state(void)
{
   if(end_turning_state == 1 && top_center_x_gap <= 30)
//   if(end_turning_state == 1)
   {
       state_flag = 1;
   }
}

/********************************/
//误差处理函数
int16_t Final_Sum = 0;
int16_t Last_Final_Sum = 0;
/********************************/
/******************************************************************************
* 函数名称     : Error_Gap
* 描述         : 图像误差处理
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void Error_Gap(void)
{
    switch(state_flag)
    {
        //这里是需要区分直线，直角拐弯，三极管拐弯然后计算不同的误差 这个的前提也是必须清晰才行
        case(1): //根据四方位判断此时为直线处理
        {
           task_point = TASK_STRAIGHT;
           if(center_point_x_average <= 0)
           {
               center_point_x_average = 47;
           }
           if(right_boundary_average_x <= 0)
           {
               right_boundary_average_x = 47;
           }
           if(left_boundary_average_x <= 0)
           {
               left_boundary_average_x = 47;
           }
           Final_Sum = center_point_x_average - Middle_Line_x;  //默认循中线
           if(transistor_Num == 41)
           {
               Final_Sum = (right_boundary_average_x - Middle_Line_x) * 4;
           }
           if(transistor_Num == 20 || transistor_Num == 23 || transistor_Num == 29 || transistor_Num == 31 || turn_count == 28)
           {
               if(Final_Sum < -10)
               {
                   Final_Sum = -10;
               }
               if(Final_Sum > 10)
               {
                   Final_Sum = 10;
               }
           }

           break;
        }
        case(2): //根据四方位判断此时为右拐弯处理
        {
           task_point = TASK_TURN_RIGHT;
           Final_Sum = 45;
           break;
        }
        case(3): //根据四方位判断此时为左拐弯处理
        {
           task_point = TASK_TURN_LEFT;
           Final_Sum = -45;
           break;
        }
    }

}


//总函数
void ljw_camera_deal(void)
{
    data_ready_flag_for_core0 = 0;
    Get_Zip_Image_Factor2(mt9v03x_image, imgZip);    //把mt9v03x_image获取的原始图像压缩成imgZip,已优化，目前380us跑完
    nowThreshold = otsuThreshold_fast();
//    nowThreshold = Iterative_Threshold_Fast(imgZip, 0, 0, my_camera_h, my_camera_w);    //利用直方图法来计算出当前最佳的灰度阈值，已优化,目前900us跑完
    Get_img_binary();     //将灰度图进行二值化处理,灰度阈值就是用到这里的
    memcpy(img_copy[0], img_binary[0], MY_CAMERA_SIZE); //复制图像来显示

    if(find_track_start_point())
    {
        trace_boundary(start_point_l_x, start_center_y,start_point_r_x, start_center_y);   //优化后的爬线代码
        End_of_turning_state();   //用来接收陀螺仪拐弯结束标志
        find_center_point();    //找中线以及中线的最高，中间，最低点
        trace_Right_angle();   //直角拐弯处理
        trace_transistor();    //三极管 + MOS管拐弯处理
        Error_Gap();  //计算不同状态下的小车误差
    }
    data_ready_flag_for_core0 = 1;          //图像处理标志位，避免1核0和抢数据

/////////////////////测试代码运行时间////////////////////////
//        system_start();
//        tim = system_getval_us();
/////////////////////测试代码运行时间////////////////////////
}
