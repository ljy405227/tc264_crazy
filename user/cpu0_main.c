#include "zf_common_headfile.h"
#include "cpu0_main.h"
#pragma section all "cpu0_dsram"

volatile uint16_t pit_count_ms = 500;
volatile uint8_t pit_control_flag = 0;
uint8_t ips_show_time = 0;
volatile uint8_t first_flag = 1;


void printf_limit()
{
    //      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
    printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n"
            , encoder_left.encoder_count            // 0
            , encoder_right.encoder_count           // 1
            , Final_Sum                             // 2
            , speed_target                          // 3
            , limit_check.speed_dec                 // 4
            , limit_check.speed_out                 // 5
            , limit_check.dir_d_err                 // 6
            , limit_check.dir_expect_gyro           // 7
            , limit_check.d_gyro                    // 8
            , limit_check.dir_out                   // 9
            , limit_check.final_pwm                 //10
            , (int)speed_dis_out                    //11
            , (int)speed_pid.out                    //12
            , expect_gyro                           //13
            , turn_count                            //14
            , (int)yaw
            , (int)yaw_turn_first
        );
}

void printf_deal(void)
{
    printf("%d, %d, %d, %d, %d, %f\r\n"
            , encoder_left.encoder_count
            , encoder_right.encoder_count
            , Final_Sum
//            , end_turning_state
//            , speed_target
//            , turn_time_count
//            , you_ing
//            , zuo_ing
//            , Gyr_dir
//            , state_flag
            , (int)speed_dis_out
            , (int)speed_pid.out
            , expect_gyro
        );
}
volatile uint16_t time_debug = 0;
uint8_t car_on_flag = 0;

#if mode_fuya
void pwm_ramp_up(uint32_t pwm, uint16_t target)
{
    uint16_t duty = 1000;
    while (duty < target)
    {
        duty = duty + 20;                 // 每次增加的步长（可调）
        if (duty > target)
            duty = target;

        pwm_set_duty(pwm, duty);
        system_delay_ms(10);                // 每步之间的延时（决定上升速度）
    }
}
#endif


#if !mode_fuya
void pwm_ramp_up(uint32_t pwm, uint16_t target)
{
    uint16_t duty = 600;
    while (duty < target)
    {
        duty = duty ++;                 // 每次增加的步长（可调）
        if (duty > target)
            duty = target;

        pwm_set_duty(pwm, duty);
        system_delay_ms(20);                // 每步之间的延时（决定上升速度）
    }
}
#endif

//uint16 duty;
int core0_main(void)
{
    clock_init();
    debug_init();
    Device_Init();
    system_delay_ms(1000);
    gyro_amend = 500; // 陀螺仪校准修正值
    pit_ms_init(CCU60_CH0, 10);
    pit_ms_init(CCU60_CH1, 1);
    while (Gyro_Flag == 0)
    {
//        system_delay_ms(10);
    }
    // pwm_init(fuya_wushua_pwm, 50, 0);
    // pwm_ramp_up(fuya_wushua_pwm, 800);
#if run
#if mode_fuya
    pwm_init(fuya_left_pwm, 17000, 0);
    pwm_ramp_up(fuya_left_pwm, 8500);
#endif
#if !mode_fuya
    pwm_init(fuya_wushua_pwm, 50, 0);
    pwm_ramp_up(fuya_wushua_pwm, 700);
#endif
#endif

#if 0
    Flash_Init();
    Flash_Value_Write();
#endif 
    pit_control_flag = 0;
    car_on_flag = 1;
//    Ljy_set_motor_pwm(3000,3000);
    cpu_wait_event_ready();

    while (1)
    {
//        gpio_set_level(P33_7, 0);
        if (Gyro_Flag == 1 && car_on_flag == 1)
        {
            pit_control_flag = 1;
            car_on_flag = 0;
        }
        menu_control();
//       ips_show();
         ips_show_time ++;
         if (ips_show_time > 10)
         {
             ips_show_time = 0;
             if (main_choose_item < 5 || (main_choose_item == 5 && menu_state == 0))
             {
                 ljy_ips200_data_printf(1, "Encoder1:%05d|Encoder2:%05d", encoder_left.encoder_count, encoder_right.encoder_count);
                 ljy_ips200_data_printf(2, "yaw:%.2f, yaw_b:%.2f    ", yaw, yaw_turn_first);
                 ljy_ips200_data_printf(3, "gyro_lim:%d", limit_check.dir_expect_gyro);
                 ljy_ips200_data_printf(4, "dir_lim:%5d, pwm_lim:%3d", limit_check.dir_out, limit_check.final_pwm);
                 ljy_ips200_data_printf(5, "beg:%d|end:%d|zuo:%d|you:%d", turning_state, end_turning_state, zuo_ing, you_ing);
                 ljy_ips200_data_printf(6, "turn_count:%d", turn_count);
                 ljy_ips200_data_printf(7, "final_sum:%d     ", Final_Sum);
             }
         }
        if (pit_control_flag == 1)
        {
//            printf_deal();
            printf_limit();
            time_debug++;
#if !run
             if (time_debug <= 70000)
             {
                 if (first_flag == 1)
                 {
                     varyble_init();
                     first_flag = 0;
                 }
             }
             else if (time_debug > 70000)
             {
                 pit_control_flag = 0;
                 Ljy_set_motor_pwm(0,0);
                 pwm_set_duty(fuya_wushua_pwm, 0);

             }
#endif
#if run
            //  if (time_debug <= 80)
            //  {
            //      if (first_flag == 1)
            //      {
            //          varyble_init();
            //          first_flag = 0;
            //      }
            //  }
            //  else if (time_debug > 80)
            //  {
            //      pit_control_flag = 0;
            //      Ljy_set_motor_pwm(0,0);
            //      pwm_set_duty(fuya_wushua_pwm, 0);

            //  }
            if (time_debug <= 3600 && protect_flag == 0 && turn_count < 30)
            {
                if (first_flag == 1)
                {
                    varyble_init();
                    first_flag = 0;
                }
            }
            else if (time_debug > 3600 || protect_flag == 1 || turn_count >= 30)
            {
                pit_control_flag = 0;
                Ljy_set_motor_pwm(0,0);
                #if mode_fuya
                pwm_set_duty(fuya_left_pwm, 0);
                #endif
                #if !mode_fuya
                pwm_set_duty(fuya_wushua_pwm, 0);
                #endif
            }
#endif
        }
//        gpio_set_level(P33_7, 1);
    }
}




#pragma section all restore
