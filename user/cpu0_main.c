#   include "zf_common_headfile.h"
#include "cpu0_main.h"
#pragma section all "cpu0_dsram"

volatile uint16_t pit_count_ms = 500;
volatile uint8_t gogogo_flag = 0;
uint8_t ips_show_time = 0;
volatile uint8_t first_flag = 1;

void printf_deal(void)
{
#if Battery_Valtage_mode
    if (pit_count_ms > 500)
    {
        uint16 valtage_adc = adc_convert(ADC0_CH11_A11);
        float valtage_now = 36.3 * valtage_adc / 4096;
        ljy_ips200_data_printf(1, "valtage:%.2fV", valtage_now);
        pit_count_ms = 0;
    }
#endif`
//    printf("%d, %d, %d, %f, %f, %f\r\n", encoder_left.encoder_count, encoder_right.encoder_count, Final_Sum, speed_dis_out, avl_gyro_z, expect_gyro);

    printf("%d, %d, %d, %f, %f, %f, %f, %f, %f,%f, %d, %d, %d, %d\r\n"
            , encoder_left.encoder_count
            , encoder_right.encoder_count
            , Final_Sum
            , speed_dis_out
            , pid_dir_pos.err
            , pos_d_err
            , expect_gyro
            , pid_dir_gyro.err
            , d_gyro
            , pid_dir_gyro.out
            , task_point
            ,transistor_right_Num
            ,transistor_left_Num
            ,transistor_left_and_right_Num
        );
//    printf("%f, %f, %d\r\n"
//            , turn_yaw_begin
//            , yaw
//            , Final_Sum);
    // printf("%d, %f, %f\r\n", Final_Sum, speed_dis_out, pid_direction.err);
    
}
// #define UART_INDEX              (DEBUG_UART_INDEX   )                           // 默锟斤拷 UART_0
// #define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // 默锟斤拷 115200
// #define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // 默锟斤拷 UART0_TX_P14_0
// #define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // 默锟斤拷 UART0_RX_P14_1
// uint8 uart_get_data[64];                                                        // 锟斤拷锟节斤拷锟斤拷锟斤拷锟捷伙拷锟斤拷锟斤拷
// uint8 fifo_get_data[64];                                                        // fifo 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
// uint8  get_data = 0;                                                            // 锟斤拷锟斤拷锟斤拷锟捷憋拷锟斤拷
// uint32 fifo_data_count = 0;                                                     // fifo 锟斤拷锟捷革拷锟斤拷

volatile uint16_t time_debug = 0;
// fifo_struct uart_data_fifo;





uint8 image_copy[MT9V03X_H / 2][MT9V03X_W / 2];

// **************************** 锟斤拷锟斤拷锟斤拷锟斤拷 ****************************

#define LED1                    (P20_9)
#define UART_INDEX              (UART_2)
#define UART_BAUDRATE           (115200)                // 默认 115200
#define UART_TX_PIN             (UART2_TX_P10_5)
#define UART_RX_PIN             (UART2_RX_P10_6)

void Image_Send_SPI(void);
void WIFI_SPI_Data(void);

uint8_t Camera_Send_Flag = 0;
uint8_t wifi_spi_get_data_buffer[256];
uint8_t wifi_spi_send_data_buffer[256];
uint32 wifi_spi_recieve_data;
uint32 wifi_spi_send_data;
uint8_t spi_uart_get_data[64];             // 串口接收数据缓冲区
fifo_struct spi_uart_data_fifo;


uint8_t ljy_camera_deal_flag = 0;

uint8_t linshi_flag = 0;


void pwm_ramp_up(uint32_t pwm, uint16_t target)
{
    uint16_t duty = 1000;

    while (duty < target)
    {
        duty = duty + 50;                 // 每次增加的步长（可调）
        if (duty > target)
            duty = target;

        pwm_set_duty(pwm, duty);
        system_delay_ms(10);                // 每步之间的延时（决定上升速度）
    }
}

//uint16 duty;
int core0_main(void)
{
    clock_init();
    debug_init();
    Device_Init();
    pwm_init(fuya_left_pwm, 17000, 0);
//    pwm_ramp_up(fuya_left_pwm, 7000);
    system_delay_ms(100);

#if 0
    Flash_Init();
    Flash_Value_Write();
#endif 
    pit_ms_init(CCU60_CH0, 10);
    pit_ms_init(CCU60_CH1, 1);

    // fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 锟斤拷始锟斤拷 fifo 锟斤拷锟截伙拷锟斤拷锟斤拷
    // uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 锟斤拷始锟斤拷锟斤拷锟斤拷
    // uart_rx_interrupt(UART_INDEX, 1);                                           // 锟斤拷锟斤拷 UART_INDEX 锟侥斤拷锟斤拷锟叫讹拷
    // uart_write_string(UART_INDEX, "UART Text.");                                // 锟斤拷锟斤拷锟斤拷锟斤拷锟较�
    // uart_write_byte(UART_INDEX, '\r');                                          // 锟斤拷锟斤拷爻锟�
    // uart_write_byte(UART_INDEX, '\n');                                          // 锟斤拷锟斤拷锟斤拷锟�

    gogogo_flag = 0;
    linshi_flag = 1;
//    Ljy_set_motor_pwm(2000,2000);
    cpu_wait_event_ready();

    while (1)
    {
//         Ljy_set_motor_pwm(2000,2000);
        if (Gyro_Flag == 1 && linshi_flag == 1)
        {
            gogogo_flag = 1;
            linshi_flag = 0;
        }
        menu_control();
        ips_show_time ++;
        if (ips_show_time > 10)
        {
            ips_show_time = 0;
            if (main_choose_item != 5)
            {
                ljy_ips200_data_printf(1, "Encoder1:%05d|Encoder2:%05d", encoder_left.encoder_count, encoder_right.encoder_count);
                ljy_ips200_data_printf(2, "yaw:%.2f, yaw_b:%.2f", yaw, yaw_turn_first);
//                ljy_ips200_data_printf(2, "z:%02f", avl_gyro_z);
                ljy_ips200_data_printf(3, "out:%d", (int)speed_pid.out);
                ljy_ips200_data_printf(4, "dis_out:%5d, final:%3d", (int)speed_dis_out, (int)Final_Sum);
                ljy_ips200_data_printf(5, "beg:%d, end:%d", turning_state, end_turning_state);
                ljy_ips200_data_printf(6, "zuo:%d, you:%d", zuo_ing, you_ing);
                ljy_ips200_data_printf(7, "turn_count:%d", turn_count);
            }
        }

        if (gogogo_flag == 1)
        {
            printf("%d, %d, %d, %d, %d, %d\r\n"
                    , encoder_left.encoder_count
                    , encoder_right.encoder_count
                    , Final_Sum
                    , (int)speed_dis_out
                    , (int)speed_pid.out
//                    , turning_state
//                    , end_turning_state
//                    , task_point
//                    , zuo_ing
//                    , you_ing
                   , turn_count
//                    , transistor_right_Num
//                    , transistor_left_Num
//                    , transistor_left_and_right_Num
                );
            time_debug++;
            if (time_debug <= 800)
            {
                if (first_flag == 1)
                {
                    varyble_init();
                    first_flag = 0;
                }
            }
            else
            {
                pwm_set_duty(fuya_left_pwm, 0);
                Ljy_set_motor_pwm(0,0);
                gogogo_flag = 0;
            }
        }


        // fifo_data_count = fifo_used(&uart_data_fifo);                           // 锟介看 fifo 锟角凤拷锟斤拷锟斤拷锟斤拷
        // if(fifo_data_count != 0)                                                // 锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷
        // {
        //     fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 锟斤拷 fifo 锟斤拷锟斤拷锟捷讹拷锟斤拷锟斤拷锟斤拷锟� fifo 锟斤拷锟截的伙拷锟斤拷
        //     if(fifo_data_count > 0 && fifo_get_data[0] == 'A')
        //     {
        //         gogogo_flag = 0;
        //         Ljy_set_motor_pwm( 0, 0);`
        //         varyble_init();
        //         // if (first_flag == 0)
        //         // {
        //         //     time_debug = 0;
        //         //     Pid_Init();
        //         //     first_flag = 1;
        //         // }
        //         uart_write_string(UART_INDEX, "\r\nUART get data:");
        //         uart_write_buffer(UART_INDEX, fifo_get_data, fifo_data_count);

        //     }
        //     if(fifo_data_count > 0 && fifo_get_data[0] == 'B')
        //     {
        //         gogogo_flag = 1;
        //         time_debug = 0;
        //         uart_write_string(UART_INDEX, "\r\nUART get data:");
        //         uart_write_buffer(UART_INDEX, fifo_get_data, fifo_data_count);
        //     }
        // }


    }
}


// void uart_rx_interrupt_handler (void)
// {
// //    get_data = uart_read_byte(UART_INDEX);                                      // 锟斤拷锟斤拷锟斤拷锟斤拷 while 锟饺达拷式 锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷使锟斤拷
//     uart_query_byte(UART_INDEX, &get_data);                                     // 锟斤拷锟斤拷锟斤拷锟斤拷 锟斤拷询式 锟斤拷锟斤拷锟捷会返锟斤拷 TRUE 没锟斤拷锟斤拷锟捷会返锟斤拷 FALSE
//     fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           // 锟斤拷锟斤拷锟斤拷写锟斤拷 fifo 锟斤拷
// }

void Image_Send_SPI(void)
{
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST))
    {
        system_delay_ms(100);                                                   // 初始化失败 等待 100ms
    }
    // zf_device_wifi_spi.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接
    if(0 == WIFI_SPI_AUTO_CONNECT)                                              // 如果没有开启自动连接 就需要手动连接目标 IP
    {
        while(wifi_spi_socket_connect(                                          // 向指定目标 IP 的端口建立 TCP 连接
            "TCP",                                                              // 指定使用TCP方式通讯
            WIFI_SPI_TARGET_IP,                                                      // 指定远端的IP地址，填写上位机的IP地址
            WIFI_SPI_TARGET_PORT,                                                    // 指定远端的端口号，填写上位机的端口号，通常上位机默认是8080
            WIFI_SPI_LOCAL_PORT))                                                   // 指定本机的端口号
        {
            // 如果一直建立失败 考虑一下是不是没有接硬件复位
            system_delay_ms(100);                                               // 建立连接失败 等待 100ms
        }
    }

    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W / 2, MT9V03X_H / 2);
//    seekfree_assistant_camera_boundary_config(XY_BOUNDARY,BOUNDARY_NUM,xy_x1_boundary,NULL,NULL,xy_y1_boundary,NULL,NULL);
//     gogogo_flag = 1;
}
void WIFI_SPI_Data(void)
{
    wifi_spi_recieve_data = wifi_spi_read_buffer(wifi_spi_get_data_buffer, sizeof(wifi_spi_get_data_buffer));  //接收数据

    if(wifi_spi_recieve_data)                                                     // 如果接收到数据 则进行数据类型判断
    {
        if (wifi_spi_get_data_buffer[0] == 'A')                                                 // 如果接收到的数据以 'A' 开头 则认为是控制数据
        {
                gogogo_flag = 0;
                time_debug = 0;
                Ljy_set_motor_pwm( 0, 0);
                varyble_init();
        }
        else if (wifi_spi_get_data_buffer[0] == 'B')                                            // 如果接收到的数据以 'B' 开头 则认为是调试数据
        {
            gogogo_flag = 1;
            time_debug = 0;
        }
        else
        {
        }
    }

}

#pragma section all restore
