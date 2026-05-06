#   include "zf_common_headfile.h"
#include "cpu0_main.h"
#pragma section all "cpu0_dsram"

volatile uint16_t pit_count_ms = 500;
volatile uint8_t gogogo_flag = 0; // 启用控制中断
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
#endif
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
}
volatile uint16_t time_debug = 0;

uint8_t linshi_flag = 0;

// 带延时，循环中断里面慎用
void pwm_ramp_up(uint32_t pwm, uint16_t target)
{
    uint16_t duty = 1000;
    while (duty < target)
    {
        duty = duty + 50;
        if (duty > target)
            duty = target;

        pwm_set_duty(pwm, duty);
        system_delay_ms(10);
    }
}

int core0_main(void)
{
    clock_init();
    debug_init();
    Device_Init();
    pwm_init(fuya_left_pwm, 17000, 0);
//    pwm_ramp_up(fuya_left_pwm, 7000);
#if 0
    Flash_Init();
    Flash_Value_Write();
#endif 
    pit_ms_init(CCU60_CH0, 10);
    pit_ms_init(CCU60_CH1, 1);


    gogogo_flag = 0;
    linshi_flag = 1;
    cpu_wait_event_ready();

    while (1)
    {
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
                , turn_count
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

#pragma section all restore
