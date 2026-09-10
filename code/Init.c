#include "zf_common_headfile.h"

struct FlashInformation Flash;

void Device_Init(void)
{
#if 1
    ips200_init(IPS200_TYPE_SPI);
    mt9v03x_init();
    Motor_Init();
    Encoder_Init();
    Key_Init();
    Startingup_Flash_Value_Updata();
    Pid_Init();
    imu660rb_init();

#if Wifi_mode
    while (wifi_uart_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST, WIFI_UART_STATION))
    {
        printf("\r\n Init error, try again.");
        system_delay_ms(100);
    }
    if (3 != WIFI_UART_AUTO_CONNECT)
    {
        while (wifi_uart_entry_tcp_servers(WIFI_UART_LOCAL_PORT))
        {
            printf("\r\n Entry tcp servers error, try again.");
            system_delay_ms(100);
        }
    }
    printf("\r\nmodule version:%s", wifi_uart_information.wifi_uart_version);
#endif

#if Battery_Valtage_mode
    adc_init(VOLTAGE_PORT, ADC_12BIT);
#endif
#endif
    uart_init(UART_2, 115200, UART2_TX_P10_5, UART2_RX_P10_6);

    All_Filter_Init();
}

#define wushua_150 0
#define wushua_210 1
#define wushua_kuai 0

void Flash_Init(void)
{
#if wushua_150
    Flash.tar_speed = 150.0f;
    Flash.speed_kp = 43.0f;
    Flash.speed_ki = 0.18f;
    Flash.speed_kd = 1.0f;
    Flash.gyro_kp = 5.70f;
    Flash.gyro_kd = 0.5f;
    Flash.dir_kp = 12.20f;
    Flash.dir_kd = 0.86f;

    Flash.gyro_kp_left = 14.48f;
    Flash.gyro_kd_left = 0.2f;
    Flash.dir_kp_left = 19.70f;
    Flash.dir_kd_left = 2.06f;

    Flash.gyro_kp_right = 12.40f;
    Flash.gyro_kd_right = 0.2f;
    Flash.dir_kp_right = 20.80f;
    Flash.dir_kd_right = 2.06f;

    Flash.other_ramp_time = 0.2f;
    Flash.other_turn_speed = 150.0f;

    Flash.speed_kp_right = 17.80f;
    Flash.speed_ki_right = 2.06f;
#endif
#if wushua_210
    Flash.tar_speed = 210.0f;
    Flash.speed_kp = 43.0f;
    Flash.speed_ki = 0.18f;
    Flash.speed_kd = 1.0f;
    Flash.gyro_kp = 10.70f;
    Flash.gyro_kd = 1.0f;
    Flash.dir_kp = 23.20f;
    Flash.dir_kd = 0.86f;

    Flash.gyro_kp_left = 14.48f;
    Flash.gyro_kd_left = 0.2f;
    Flash.dir_kp_left = 19.70f;
    Flash.dir_kd_left = 2.06f;

    Flash.gyro_kp_right = 12.40f;
    Flash.gyro_kd_right = 0.2f;
    Flash.dir_kp_right = 20.80f;
    Flash.dir_kd_right = 2.06f;

    Flash.other_ramp_time = 0.2f;
    Flash.other_turn_speed = 190.0f;

    Flash.speed_kp_right = 17.80f;
    Flash.speed_ki_right = 2.06f;
#endif
#if wushua_kuai
    Flash.tar_speed = 210.0f;
    Flash.speed_kp = 43.0f;
    Flash.speed_ki = 0.18f;
    Flash.speed_kd = 1.0f;
    Flash.gyro_kp = 10.70f;
    Flash.gyro_kd = 1.0f;
    Flash.dir_kp = 23.20f;
    Flash.dir_kd = 0.86f;

    Flash.gyro_kp_left = 14.48f;
    Flash.gyro_kd_left = 0.2f;
    Flash.dir_kp_left = 19.70f;
    Flash.dir_kd_left = 2.06f;

    Flash.gyro_kp_right = 12.40f;
    Flash.gyro_kd_right = 0.2f;
    Flash.dir_kp_right = 20.80f;
    Flash.dir_kd_right = 2.06f;

    Flash.other_ramp_time = 0.2f;
    Flash.other_turn_speed = 190.0f;

    Flash.speed_kp_right = 17.80f;
    Flash.speed_ki_right = 2.06f;
#endif
}