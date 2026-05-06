#include "image.h"

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

//计时器
uint32_t tim;

//判断标志位
uint8_t state_flag = 1;
uint8_t turning_state;
uint8_t end_turning_state = 1;
uint8_t T = 0;

//中线参数
#define Boundary_point_gap 15

//转弯参数
#define Img_Gap_left 14
#define Img_Gap_right 14
#define Special_Img_Gap 14
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
  * @brief    压缩n倍图像
  * @param    压缩前图像:Image_src[MT9V03X_H][MT9V03X_W] 压缩后图像:uint8 Image_tar[my_camera_h][my_camera_w] 压缩倍数:factor
  * @return   无
  * 注:       这个函数运行起来比较耗时，如果没有太高压缩要求的话尽量不使用
  * @note     优化了计算效率和内存访问
  *************************************************/
//void Get_Zip_Image(uint8 Image_src[MT9V03X_H][MT9V03X_W], uint8 Image_tar[my_camera_h][my_camera_w], int factor)
//{
//    const int tar_w = MT9V03X_W / factor;
//    const int tar_h = MT9V03X_H / factor;
//    const int area = factor * factor;
//    // 预计算除法优化
//    int use_shift = 0;
//    int shift = 0;
//    if (area > 0 && (area & (area - 1)) == 0) {
//        use_shift = 1;
//        int temp = area;
//        while (temp > 1) {
//            temp >>= 1;
//            shift++;
//        }
//    }
//    // 使用指针运算优化内存访问
//    uint8 *src_ptr = (uint8 *)Image_src;
//    uint8 *tar_ptr = (uint8 *)Image_tar;
//    const int src_stride = MT9V03X_W;
//    const int tar_stride = tar_w;
//
//    // 循环分块处理，提高缓存命中率
//    const int block_size = 16; // 缓存友好的块大小
//    const int blocks_h = (tar_h + block_size - 1) / block_size;
//    const int blocks_w = (tar_w + block_size - 1) / block_size;
//
//    for (int block_i = 0; block_i < blocks_h; block_i++) {
//        for (int block_j = 0; block_j < blocks_w; block_j++) {
//            // 计算当前块的边界
//            int i_start = block_i * block_size;
//            int i_end = (i_start + block_size < tar_h) ? i_start + block_size : tar_h;
//            int j_start = block_j * block_size;
//            int j_end = (j_start + block_size < tar_w) ? j_start + block_size : tar_w;
//
//            // 处理当前块
//            for (int i = i_start; i < i_end; i++) {
//                int src_row_start = i * factor * src_stride;
//                int tar_row = i * tar_stride;
//
//                for (int j = j_start; j < j_end; j++) {
//                    int src_col_start = j * factor;
//                    int sum = 0;
//
//                    // 使用指针运算避免重复乘法计算
//                    uint8 *block_ptr = src_ptr + src_row_start + src_col_start;
//
//                    // 平均池化：对factor×factor块求和
//                    for (int k = 0; k < factor; k++) {
//                        // 部分循环展开，减少循环开销
//                        int l = 0;
//                        for (; l <= factor - 4; l += 4) {
//                            sum += block_ptr[l] + block_ptr[l+1] +
//                                   block_ptr[l+2] + block_ptr[l+3];
//                        }
//                        // 处理剩余像素
//                        for (; l < factor; l++) {
//                            sum += block_ptr[l];
//                        }
//                        block_ptr += src_stride; // 移动到下一行
//                    }
//
//                    // 优化除法计算
//                    if (use_shift) {
//                        tar_ptr[tar_row + j] = (uint8)(sum >> shift);
//                    } else {
//                        tar_ptr[tar_row + j] = (uint8)(sum / area);
//                    }
//                }
//            }
//        }
//    }
//}

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

/********************************/
//图像二值化变量
uint8 img_threshold_group[3];    //划分阈值区域   上中下

uint8 close_Threshold = 0;
uint8 mid_Threshold = 0;  //根据需求调整，目前不用调整阈值都很好
uint8 far_Threshold = 0;

uint8 white_num_row[my_camera_h];    //存储每行白点数
uint8 white_num_col[my_camera_w];    //存储每列白点数
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
//                    for(uint8_t k = 0; k<8; k++)
//                    {
//                        int8_t nx = j + Loop[k][1];
//                        int8_t ny = i + Loop[k][0];
//                        if(nx >= 0 && nx <= my_camera_w-1 && ny >= 0 && ny <= my_camera_h-1)
//                        {
//                            if(img_binary[ny][nx] == Black)  //检查八领域是否为黑点 此处为腐蚀操作
//                            {
//                                Black_Num ++;
//                            }
//                        }
//                    }
//                    if(Black_Num >= 8)
//                    {
//                        img_binary[i][j] = Black_Expansion;
//                    }
                }
//                else   //此处为膨胀操作 当黑点的周围有四个白点后，就膨胀该点也为白
//                {
//                    for(uint8_t k = 0; k<8; k++)
//                    {
//                        int8_t nx = j + Loop[k][1];
//                        int8_t ny = i + Loop[k][0];
//                        if(nx >= 0 && nx <= my_camera_w-1 && ny >= 0 && ny <= my_camera_h-1)
//                        {
//                            if(img_binary[ny][nx] == White)  //检查八领域是否为白点
//                            {
//                                White_Num ++;
//                            }
//                        }
//                    }
//                    if(White_Num >= 3)
//                    {
//                        img_binary[i][j] = White_Expansion;
//                        white_num_row[i]++;  //计算每行白点数
//                        white_num_col[j]++;  //计算每列白点数
//                    }
//                }
            }
            else if(i >= 20 && i < 40)  //当位于中景时
            {
                threshold_group_num = 1;
                if(imgZip[i][j] >= img_threshold_group[threshold_group_num])
                {
                    img_binary[i][j] = White;
                    white_num_row[i]++;  //计算每行白点数
                    white_num_col[j]++;  //计算每列白点数
//                    for(uint8_t k = 0; k<8; k++)
//                    {
//                        int8_t nx = j + Loop[k][1];
//                        int8_t ny = i + Loop[k][0];
//                        if(nx >= 0 && nx <= my_camera_w-1 && ny >= 0 && ny <= my_camera_h-1)
//                        {
//                            if(img_binary[ny][nx] == Black)  //检查八领域是否为黑点 此处为腐蚀操作
//                            {
//                                Black_Num ++;
//                            }
//                        }
//                    }
//                    if(Black_Num >= 8)
//                    {
//                        img_binary[i][j] = Black_Expansion;
//                    }
                }
//                else   //此处为膨胀操作 当黑点的周围有四个白点后，就膨胀该点也为白
//                {
//                    for(uint8_t k = 0; k<8; k++)
//                    {
//                        int8_t nx = j + Loop[k][1];
//                        int8_t ny = i + Loop[k][0];
//                        if(nx >= 0 && nx <= my_camera_w-1 && ny >= 0 && ny <= my_camera_h-1)
//                        {
//                            if(img_binary[ny][nx] == White)  //检查八领域是否为白点
//                            {
//                                White_Num ++;
//                            }
//                        }
//                    }
//                    if(White_Num >= 3)
//                    {
//                        img_binary[i][j] = White_Expansion;
//                        white_num_row[i]++;  //计算每行白点数
//                        white_num_col[j]++;  //计算每列白点数
//                    }
//                }
            }
            else if(i >= 40 && i < 60)  //当位于近景时
            {
                threshold_group_num = 2;
                if(imgZip[i][j] >= img_threshold_group[threshold_group_num])
                {
                    img_binary[i][j] = White;
                    white_num_row[i]++;  //计算每行白点数
                    white_num_col[j]++;  //计算每列白点数
//                    for(uint8_t k = 0; k<8; k++)
//                    {
//                        int8_t nx = j + Loop[k][1];
//                        int8_t ny = i + Loop[k][0];
//                        if(nx >= 0 && nx <= my_camera_w-1 && ny >= 0 && ny <= my_camera_h-1)
//                        {
//                            if(img_binary[ny][nx] == Black)  //检查八领域是否为黑点 此处为腐蚀操作
//                            {
//                                Black_Num ++;
//                            }
//                        }
//                    }
//                    if(Black_Num >= 8)
//                    {
//                        img_binary[i][j] = Black_Expansion;
//                    }
                }
//                else   //此处为膨胀操作 当黑点的周围有四个白点后，就膨胀该点也为白
//                {
//                    for(uint8_t k = 0; k<8; k++)
//                    {
//                        int8_t nx = j + Loop[k][1];
//                        int8_t ny = i + Loop[k][0];
//                        if(nx >= 0 && nx <= my_camera_w-1 && ny >= 0 && ny <= my_camera_h-1)
//                        {
//                            if(img_binary[ny][nx] == White)  //检查八领域是否为白点
//                            {
//                                White_Num ++;
//                            }
//                        }
//                    }
//                    if(White_Num >= 3)
//                    {
//                        img_binary[i][j] = White_Expansion;
//                        white_num_row[i]++;  //计算每行白点数
//                        white_num_col[j]++;  //计算每列白点数
//                    }
//                }
            }
        }
    }

}

/********************************/
// 起点信息
uint16_t start_center_y = 0;
uint16_t start_point_l_x = 0,start_point_r_x = 0;
uint16_t deal_bottom=my_camera_h-1;
uint16_t l_point_x = 0, r_point_x = 0;
uint16_t bottom_center_x;
uint16_t bottom_center_y;
/********************************/
/******************************************************************************
* 函数名称     : find_track_start_point
* 描述         : 在图像底部寻找赛道左右边界的起点
* 进入参数     : void
* 返回参数     : uint8_t 1:成功找到起点 0:未找到起点
******************************************************************************/
uint8_t find_track_start_point(void)
{
    bottom_center_x = 47;
    bottom_center_y = 59;

    for(uint16_t i = deal_bottom; i > deal_bottom - 30 ; i--)    // 从底部向上搜索合适的起始行（使用行白点数作为判断条件）
    {
        uint8_t l_found = 0, r_found = 0;
        start_center_y = i;
        for(uint16_t j = 1; j < my_camera_w - 2; j++) // 从左向右寻找左边界（黑到白跳变）
        {
            if(img_binary[i][j] == Black && img_binary[i][j+1] == White && img_binary[i][j+2] == White)
            {
                l_point_x = j + 1;
                l_found = 1;
                break;
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
//        uint16_t Start_Point_Gap = my_abs(r_point_x - l_point_x);   //在赛道比较脏的情况下使用
//        if(l_found && r_found && Start_Point_Gap < 15)
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
uint16_t left_boundary_average_x;
uint16_t right_boundary_x_sum;
uint16_t right_boundary_average_x;
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
uint16_t right_corner_point_y;
uint16_t left_corner_point_y;
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
    uint16_t tmp_left_point_y = 59;
    uint16_t tmp_right_point_y = 59;
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
                }
                if(ny_l == my_camera_h - 2)
                {
                    total_touch_flags.touch_bottom = 1;   //下
                }
                if(nx_l == 2)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_l < tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_l;
                        left_edge_point.x = nx_l;
                        left_edge_point.y = ny_l;
                    }
                }
                if(nx_l == my_camera_w - 2)
                {
                    total_touch_flags.touch_right = 1;   //右
                    if(ny_l < tmp_right_point_y)
                    {
                        tmp_right_point_y = ny_l;
                        right_edge_point.x = nx_l;
                        right_edge_point.y = ny_l;
                    }
                }

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
            if(nx_r < 0 || nx_r >= my_camera_w || ny_r < 0 || ny_r >= my_camera_h)
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
                }
                if(ny_r == my_camera_h - 2)
                {
                    total_touch_flags.touch_bottom = 1;   //下
                }
                if(nx_r == 2)
                {
                    total_touch_flags.touch_left = 1;   //左
                    if(ny_r < tmp_left_point_y)
                    {
                        tmp_left_point_y = ny_r;
                        left_edge_point.x = nx_r;
                        left_edge_point.y = ny_r;
                    }
                }
                if(nx_r == my_camera_w - 2)
                {
                    total_touch_flags.touch_right = 1;   //右
                    if(ny_r < tmp_right_point_y)
                    {
                        tmp_right_point_y = ny_r;
                        right_edge_point.x = nx_r;
                        right_edge_point.y = ny_r;
                    }
                }
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
int16_t center_point_x_average = 47;
uint8 center_point_num_row[my_camera_h];    //判断每行的中点数量
/********************************/
/******************************************************************************
* 函数名称     : find_center_point
* 描述         : 双指针获取爬线后的中点坐标,同时记录最低点和最高点
* 进入参数     : void
* 返回参数     : void
******************************************************************************/

void find_center_point(void)
{
  memset(center_point, 0, sizeof(center_point));
  memset(center_point_num_row, 0, sizeof(center_point_num_row));

  center_point_x_sum = 0;
  center_point_num = 0;

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

          img_SPI[left_y][(left_x + right_x) / 2] = White; //图传标记中线坐标
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
     center_point_x_average = my_abs(center_point_x_sum / center_point_num); //计算整条线的平均X值
///////////////////////////////查找左右拐点/////////////////////////////////
     tmp_right_corner_point_x = 0;
     tmp_left_corner_point_x = 0;
     tmp_right_corner_point_y = 0;
     tmp_left_corner_point_y = 0;
     right_corner_point_x = 0;
     right_corner_point_y = 0;
     left_corner_point_x = 0;
     left_corner_point_y = 0;

     if(right_edge_point.x != 0)
     {
         tmp_right_corner_point_x = (right_edge_point.x + bottom_center_x) / 2;
     }
     if(left_edge_point.x != 0)
     {
         tmp_left_corner_point_x = (left_edge_point.x + bottom_center_x) / 2;
     }

     for(uint16_t n = 0; n < boundary_point_num; n++)
     {
         if(tmp_right_corner_point_x != 0 && Total_boundary[n].x == tmp_right_corner_point_x && Total_boundary[n].y > tmp_right_corner_point_y)
         {
             tmp_right_corner_point_y = Total_boundary[n].y;
             right_corner_point_x = Total_boundary[n].x;
             right_corner_point_y = Total_boundary[n].y;

         }
         if(tmp_left_corner_point_x != 0 && Total_boundary[n].x == tmp_left_corner_point_x && Total_boundary[n].y > tmp_left_corner_point_y)
         {
             tmp_left_corner_point_y = Total_boundary[n].y;
             left_corner_point_x = Total_boundary[n].x;
             left_corner_point_y = Total_boundary[n].y;
         }
     }

//////////////////////////////////////////////////////////////////////////
}

/********************************/
//直角拐弯处理函数
Corner_Point fit_points[150];
uint8_t fit_point_count = 0;
uint8_t Right_angle_l = 0;
uint8_t Right_angle_r = 0;
uint8_t find_right_top = 0;
uint8_t find_right_middle = 0;
uint8_t find_right_bottom = 0;
uint8_t Right_frames_judge_r = frame_judge;
uint8_t Right_frames_judge_l = frame_judge;
int16_t Right_current_point_x = 0;
/********************************/

/******************************************************************************
* 函数名称     : trace_Right_angle
* 描述         : 直角拐弯处理
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void trace_Right_angle(void)
{
      find_right_top = 0;
      find_right_middle = 0;
      find_right_bottom = 0;
      Right_current_point_x = bottom_center_x;

      if(total_touch_flags.touch_top == 0 && total_touch_flags.touch_left == 0 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_bottom == 1 && right_corner_point_y >= Img_Gap_right && state_flag == 1 && end_turning_state == 1)
      {
        Right_frames_judge_r --;
        if(Right_frames_judge_r == 0 && T == 0)
        {
          turning_state = 2; //标志着此时是需要直角右拐弯
          state_flag = 2;
          end_turning_state = 0;
          Right_frames_judge_r = frame_judge;
        }
      }
      else
      {
          Right_frames_judge_r = frame_judge;
      }
      if(total_touch_flags.touch_top == 0 && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 0 && total_touch_flags.touch_bottom == 1 && left_corner_point_y >= Img_Gap_left && state_flag == 1 && end_turning_state == 1)
      {
        Right_frames_judge_l --;
        if(Right_frames_judge_l == 0 && T == 0)
        {
          state_flag = 3;
          turning_state = 1; //标志着此时是需要直角左拐弯
          end_turning_state = 0;
          Right_frames_judge_l = frame_judge;
        }
      }
      else
      {
          Right_frames_judge_l = frame_judge;
      }
}

/********************************/
//三极管处理函数
uint8_t transisitor_left_flag = 0;
uint8_t transisitor_right_flag = 0;
uint8_t transisitor_left_and_right_flag = 0;

uint8_t transistor_left_Num = 0;
uint8_t transistor_right_Num = 0;
uint8_t transistor_left_and_right_Num = 0;

uint8_t transistor_Judge_frames_r = frame_judge;
uint8_t transistor_Judge_frames_l = frame_judge;
uint8_t transistor_Judge_frames_t = frame_judge;

uint8_t transisitor_lock = 0;
uint8_t ban_transisitor = 0;
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

    Final_Road();

    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_left == 0 && total_touch_flags.touch_bottom == 1 && right_corner_point_y >= Img_Gap_right && state_flag == 1 && end_turning_state == 1 && Special_flag == 0  && T == 0 && boundary_gap >= 12) //右转的T字口
    {
        transistor_Judge_frames_r --;
        if(transistor_Judge_frames_r == 0 && transisitor_lock == 0)
        {
            transistor_right_Num ++;          // 经过的三极管数量
            transistor_Judge_frames_r = frame_judge;
            if(ban_transisitor == 0)
            {
                end_turning_state = 0;
                turning_state = 2;
                state_flag = 4;
            }
        }
    }
    else
    {
        transistor_Judge_frames_r = frame_judge;
    }

    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 0 && total_touch_flags.touch_bottom == 1 && left_corner_point_y >= Img_Gap_left && state_flag == 1 && end_turning_state == 1 && Special_flag == 0 && T == 0 && boundary_gap >= 12) //左转的T字口
    {
        transistor_Judge_frames_l --;
        if(transistor_Judge_frames_l == 0 && transisitor_lock == 0)
        {
            transistor_left_Num++;          // 经过的三极管数量
            transistor_Judge_frames_l = frame_judge;

            if(ban_transisitor == 0)
            {
                end_turning_state = 0;
                turning_state = 1;
                state_flag = 5;
            }
        }
    }
    else
    {
        transistor_Judge_frames_l = frame_judge;
    }

    if(total_touch_flags.touch_top == 0 && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_bottom == 1 && (left_corner_point_y >= Img_Gap_left || right_corner_point_y >= Img_Gap_right) && state_flag == 1 && end_turning_state == 1 && Special_flag == 0 && boundary_gap >= 12) //左右转的T字口
    {
        transistor_Judge_frames_t --;
        if(transistor_Judge_frames_t == 0 && transisitor_lock == 0)
        {
            transistor_left_and_right_Num++;
            transistor_Judge_frames_t = frame_judge;
            end_turning_state = 0;
            if(ban_transisitor == 0)
            {
                state_flag = 6;
            }
        }
    }
    else
    {
        transistor_Judge_frames_t = frame_judge;
    }

    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_left == 0 && total_touch_flags.touch_bottom == 1 && right_corner_point_y >= Special_Img_Gap && state_flag == 1 && end_turning_state == 1 && Special_flag == 1 && T == 0 && boundary_gap >= 12) //右转的T字口
    {
        transistor_Judge_frames_r --;
        if(transistor_Judge_frames_r == 0 && transisitor_lock == 0)
        {
            transistor_right_Num ++;          // 经过的三极管数量
            transistor_Judge_frames_r = frame_judge;
            if(ban_transisitor == 0)
            {
                end_turning_state = 0;
                turning_state = 2;
                state_flag = 4;
            }
        }
    }
    else
    {
        transistor_Judge_frames_r = frame_judge;
    }

    if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 0 && total_touch_flags.touch_bottom == 1 && left_corner_point_y >= Special_Img_Gap && state_flag == 1 && end_turning_state == 1 && Special_flag == 1 && T == 0 && boundary_gap >= 12) //左转的T字口
    {
        transistor_Judge_frames_l --;
        if(transistor_Judge_frames_l == 0 && transisitor_lock == 0)
        {
            transistor_left_Num++;          // 经过的三极管数量
            transistor_Judge_frames_l = frame_judge;
            if(ban_transisitor == 0)
            {
                end_turning_state = 0;
                turning_state = 1;
                state_flag = 5;
            }
        }
    }
    else
    {
        transistor_Judge_frames_l = frame_judge;
    }

    if(total_touch_flags.touch_top == 0 && total_touch_flags.touch_left == 1 && total_touch_flags.touch_right == 1 && total_touch_flags.touch_bottom == 1 && (left_corner_point_y >= Special_Img_Gap || right_corner_point_y >= Special_Img_Gap) && state_flag == 1 && end_turning_state == 1 && ban_transisitor == 0 && boundary_gap >= 12) //左右转的T字口
    {
        transistor_Judge_frames_t --;
        if(transistor_Judge_frames_t == 0 && transisitor_lock == 0)
        {
            transistor_left_and_right_Num++;
            transistor_Judge_frames_t = frame_judge;
            if(ban_transisitor == 0)
            {
                end_turning_state = 0;
                state_flag = 6;
            }
        }
    }
    else
    {
        transistor_Judge_frames_t = frame_judge;
    }

    if(state_flag == 6 && ban_transisitor == 0)
    {
        if(transistor_left_and_right_Num == 1)
        {
            turning_state = 2;
            state_flag = 4;
            T = 0;
        }
         if(transistor_left_and_right_Num == 2)
         {
             turning_state = 1;
             state_flag = 5;
             T = 0;
         }
         if(transistor_left_and_right_Num == 3)
         {
             turning_state = 2;
             state_flag = 4;
             T = 0;
         }
    }
}

/********************************/
//最终路径规划函数
uint8_t transistor_frames_judge;
/********************************/
/******************************************************************************
* 函数名称     : Final_Road
* 描述         : 最终路径规划 目前的路径规划是遇到的前三个三极管忽视
* 进入参数     : void
* 返回参数     : void
******************************************************************************/
void Final_Road(void)
{
    if(transistor_left_Num == 0 && transistor_right_Num == 1 && transistor_left_and_right_Num == 0 && ban_transisitor == 0)
    {
        Special_flag = 1;
    }
    if(transistor_left_Num == 1 && transistor_right_Num == 1 && transistor_left_and_right_Num == 0 && ban_transisitor == 0)
    {
        Special_flag = 0;
        T = 1;
    }
    if(transistor_left_Num == 2 && transistor_right_Num == 3 && transistor_left_and_right_Num == 1 && ban_transisitor == 0)
    {
        T = 1;
    }
    if(transistor_left_Num == 2 && transistor_right_Num == 3 && transistor_left_and_right_Num == 2 && ban_transisitor == 0)
    {
        ban_transisitor = 1;
    }
    if(transistor_left_Num == 3 && transistor_right_Num == 3 && transistor_left_and_right_Num == 2 && ban_transisitor == 1)
    {
        transisitor_lock = 1;
        if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_bottom == 1 && total_touch_flags.touch_left == 0)
        {
            ban_transisitor = 0;
            transisitor_lock = 0;
        }
    }
    if(transistor_left_Num == 4 && transistor_right_Num == 3 && transistor_left_and_right_Num == 2 && ban_transisitor == 0)
    {
        ban_transisitor = 1;
    }
    if(transistor_left_Num == 4 && transistor_right_Num == 4 && transistor_left_and_right_Num == 2 && ban_transisitor == 1)
    {
        transisitor_lock = 1;
        if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_bottom == 1 && total_touch_flags.touch_right == 0)
        {
            ban_transisitor = 0;
            transisitor_lock = 0;
            T = 1;
        }
    }
    if(transistor_left_Num == 4 && transistor_right_Num == 4 && transistor_left_and_right_Num == 3 && ban_transisitor == 0)
    {
        ban_transisitor = 1;
    }
    if(transistor_left_Num == 4 && transistor_right_Num == 5 && transistor_left_and_right_Num == 3 && ban_transisitor == 1)
    {
        transisitor_lock = 1;
        if(total_touch_flags.touch_top == 1 && total_touch_flags.touch_bottom == 1 && total_touch_flags.touch_right == 0)
        {
            ban_transisitor = 0;
            transisitor_lock = 0;
            transistor_left_Num = 0;
            transistor_right_Num = 0;
            transistor_left_and_right_Num = 0;
        }
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
   if(end_turning_state == 1)
   {
       state_flag = 1;
   }
}


/********************************/
//误差处理函数
int16_t Middle_Line_x = my_camera_w / 2;
int16_t Middle_Line_x_left = my_camera_w / 4;
int16_t Middle_Line_x_right = (my_camera_w * 3) / 4;
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
            if(center_point_x_average <= 0)
            {
                center_point_x_average = 47;
            }
           Final_Sum = center_point_x_average - Middle_Line_x;
           break;
        }
        case(2): //根据四方位判断此时为右拐弯处理
        {
           Final_Sum = 70 - (center_point_x_average - Middle_Line_x_left);
           if(Final_Sum < 40)
           {
               Final_Sum = 40;
           }
           if(Final_Sum > 50)
           {
              Final_Sum = 50;
           }
           if(center_point_x_average < 3 && center_point_x_average > -3)
           {
              Final_Sum = Last_Final_Sum;
           }
           if (center_point_x_average >= 3)
           {
              Last_Final_Sum = Final_Sum;
           }
           break;
        }
        case(3): //根据四方位判断此时为左拐弯处理
        {
            Final_Sum = -70 - (center_point_x_average - Middle_Line_x_right);
            if(Final_Sum > -40)
            {
               Final_Sum = -40;
            }
            if(Final_Sum < -50)
            {
               Final_Sum = -50;
            }
            if(center_point_x_average < 3 && center_point_x_average > -3)
            {
               Final_Sum = Last_Final_Sum;
            }
            if (center_point_x_average >= 3)
            {
               Last_Final_Sum = Final_Sum;
            }
            break;
        }
        case(4): //根据四方位判断此时为三极管右转处理 这个的前提是识别的图像必须清晰才行。得让线能连接到顶部，不然不起作用
        {
            Final_Sum = 70 - (center_point_x_average - Middle_Line_x_left);

            if(Final_Sum < 40)
            {
               Final_Sum = 40;
            }
            if(Final_Sum > 50)
            {
               Final_Sum = 50;
            }
            if(center_point_x_average < 3 && center_point_x_average > -3)
            {
               Final_Sum = Last_Final_Sum;
            }
            if (center_point_x_average >= 3)
            {
               Last_Final_Sum = Final_Sum;
            }
            break;

        }
        case(5): //根据四方位判断此时为三极管左转处理
        {
            Final_Sum = -70 - (center_point_x_average - Middle_Line_x_right);
            if(Final_Sum > -40)
            {
               Final_Sum = -40;
            }
            if(Final_Sum < -50)
            {
               Final_Sum = -50;
            }
            if(center_point_x_average < 3 && center_point_x_average > -3)
            {
               Final_Sum = Last_Final_Sum;
            }
            if (center_point_x_average >= 3)
            {
               Last_Final_Sum = Final_Sum;
            }
            break;
        }
    }

}

//总函数
void ljw_camera_deal(void)
{
    data_ready_flag_for_core0 = 0;

    Get_Zip_Image_Factor2(mt9v03x_image, imgZip);    //把mt9v03x_image获取的原始图像压缩成imgZip,已优化，目前380us跑完
    nowThreshold = Iterative_Threshold_Fast(imgZip, 0, 0, my_camera_h, my_camera_w);    //利用直方图法来计算出当前最佳的灰度阈值，已优化,目前900us跑完
    Get_img_binary();     //将灰度图进行二值化处理,灰度阈值就是用到这里的
    memcpy(img_copy[0], img_binary[0], MY_CAMERA_SIZE); //复制图像来显示

    if(find_track_start_point())
    {
        trace_boundary(start_point_l_x, start_center_y,start_point_r_x, start_center_y);   //优化后的爬线代码
        End_of_turning_state();   //用来接收陀螺仪拐弯结束标志
        find_center_point();  //找中线以及中线的最高，中间，最低点
        trace_Right_angle();   //直角拐弯处理
        trace_transistor();    //三极管拐弯处理
        Error_Gap();  //计算不同状态下的小车误差
    }
    data_ready_flag_for_core0 = 1;          //图像处理标志位，避免1核0和抢数据

/////////////////////测试代码运行时间////////////////////////
//        system_start();
//        tim = system_getval_us();
/////////////////////测试代码运行时间////////////////////////
}
