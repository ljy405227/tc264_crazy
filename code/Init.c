#include "zf_common_headfile.h"

struct FlashInformation Flash;

void Device_Init(void)
{

#if 1
    ips200_init(IPS200_TYPE_SPI);
//    mt9v03x_init();
    Motor_Init();
    Encoder_Init();
    Key_Init();
    Startingup_Flash_Value_Updata();
    Pid_Init();
    imu660rb_init();
    // gyroOffset_init();
    
#if Wifi_mode
    while(wifi_uart_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST, WIFI_UART_STATION))
    {
        printf("\r\n Init error, try again.");
        system_delay_ms(100);                                                   // 锟斤拷始锟斤拷失锟斤拷 锟饺达拷 500ms
    }
    if(3 != WIFI_UART_AUTO_CONNECT)                                             // 锟斤拷锟矫伙拷锌锟斤拷锟斤拷远锟斤拷锟斤拷锟� 锟斤拷锟斤拷要锟街讹拷锟斤拷锟斤拷目锟斤拷 IP
    {
        while(wifi_uart_entry_tcp_servers(WIFI_UART_LOCAL_PORT))                // 锟斤拷锟斤拷 TCP Server 模式 使锟斤拷锟皆讹拷锟斤拷锟斤拷时使锟矫的端口猴拷
        {
            // 锟斤拷锟揭恢憋拷锟斤拷锟绞э拷锟� 锟斤拷锟皆革拷硬锟斤拷锟较碉拷锟斤拷锟斤拷锟斤拷系绺次�
            printf("\r\n Entry tcp servers error, try again.");
            system_delay_ms(100);                                               // 锟斤拷锟斤拷锟斤拷锟斤拷失锟斤拷 锟饺达拷 500ms
        }
    }
    printf("\r\nmodule version:%s", wifi_uart_information.wifi_uart_version);   // 模锟斤拷碳锟斤拷姹�

#endif

#if Battery_Valtage_mode
    adc_init(VOLTAGE_PORT, ADC_12BIT);
    
#endif
//    imu660ra_init();
#endif
    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);


#if 0

#endif

    // adc_init(ADC0_CH0_A0, ADC_12BIT);
    All_Filter_Init();
}

void Flash_Init(void)
{
    Flash.tar_speed = 120.0f;
    Flash.speed_kp = 103.0f;
    Flash.speed_ki = 1.0f;
    Flash.speed_kd = 1.0f;
    // 3.28
    Flash.gyro_kp = 4.70f;
    Flash.gyro_kd = 2.0f;
    Flash.dir_kp = 18.50f;
    Flash.dir_kd = 2.06f;

    Flash.gyro_kp_left = 6.50f;
    Flash.gyro_kd_left = 2.0f;
    Flash.dir_kp_left = 19.20f;
    Flash.dir_kd_left = 2.06f;

    Flash.gyro_kp_right = 4.70f;
    Flash.gyro_kd_right = 2.0f;
    Flash.dir_kp_right = 18.50f;
    Flash.dir_kd_right = 2.06f;

    Flash.other_ramp_time = 0.3f;
    Flash.other_turn_speed = 95.0f;

    Flash.speed_kp_right = 20.0f;
    Flash.speed_ki_right = 0.0f;


    // Flash.gyro_kp = 15.0f;
    // Flash.gyro_kd = 2.6f;

    // Flash.dir_kp = 10.5f;
    // Flash.dir_kd = 0.72f;

    // Flash.turn_cam = -1.18f;
    // Flash.turn_kp = 1.58f;
    // Flash.turn_pd = 2.04f;

    // Flash.gyro_kp_turn = 15.0f;
    // Flash.gyro_kd_turn = 2.6f;

    // Flash.dir_kp_turn = 10.5f;
    // Flash.dir_kd_turn = 0.72f;










//     Flash.speed_kf = 5.10f;
//     Flash.speed_kp = 3.20f;
//     Flash.speed_ki = 0.05f;
//     Flash.speed_kd = 0.0f;

//     Flash.direction_kp1 = 0;
//     Flash.direction_kp2 = 0;
//     Flash.direction_kd = 0;
//     Flash.direction_max_out = 200;


//     Flash.dis_kf = 0;
//     Flash.dis_kp = 140;
//     Flash.dis_ki = 4.0;
//     Flash.dis_kd = 0.0;
//     Flash.dis_max_intel = 40.0;
//     Flash.dis_max_out = 110;

//     Flash.img_kp = 3.6;
//     Flash.img_kp2 = 10;
//     Flash.img_max_out = 180;

//     Flash.speed_row = 26;
// /*锟劫度伙拷锟斤拷锟斤拷*/
    
//     Flash.speed_max_intel = 1000.f;

//     Flash.speedl_kp = 130;
//     Flash.speedl_ki = 2.2;

//     Flash.speedr_kp = 130;
//     Flash.speedr_ki = 2.2;

//     Flash.speed_max_out = 50;
// /*转锟津环诧拷锟斤拷*/
    

    


//     Flash.Brushless_duty = 0;

//     Flash.Sped_dec = 0;
}












