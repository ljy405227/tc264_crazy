#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"



void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);
    cpu_wait_event_ready();

    while (1)
    {
        if(mt9v03x_finish_flag)
        {
            //  gpio_set_level(P33_7, 0);
             mt9v03x_finish_flag = 0;
             ljw_camera_deal();
             
            //  gpio_set_level(P33_7, 1);
        }
        
    }
}



#pragma section all restore
