#include "zf_common_headfile.h"

KEY_STATE key_state[KEY_COUNT];
uint8_t key_flag[KEY_COUNT];
volatile uint8_t key_time[KEY_COUNT];
uint8_t key_lock[KEY_COUNT];

static const gpio_pin_enum  key_gpio_pin[KEY_COUNT] = KEY_LIST;
static const gpio_pin_enum  BM_gpio_pin[BM_COUNT] = BM_LIST;
static uint32 key_long_time = 100;
static uint32 key_short_time = 8;

void Key_Init(void)
{
    for (int i = 0; i < KEY_COUNT; i++)
    {
        gpio_init(key_gpio_pin[i], GPI, 1, GPI_PULL_UP);
        gpio_init(BM_gpio_pin[i], GPI, 1, GPI_PULL_UP);
        key_state[i] = key_check;
        key_flag[i] = key_up;
        key_lock[i] = 0;
        key_time[i] = 0;
    }
}

void key_scan(void)
{
    for (int i = 0; i < KEY_COUNT; i++)
    {
        switch (key_state[i])
        {
            case key_check:
            {
                if (!key_read(i))
                {
                    key_state[i] = key_confirm;
                    key_lock[i] = 0;
                    key_time[i] = 0;
                    
                }
            } break;
            
            case key_confirm:
            {
                if (!key_read(i)) 
                {
                    if (!key_lock[i]) key_lock[i] = 1;
                    key_time[i]++;
                    if (key_time[i] > key_long_time) 
                    {
                        key_flag[i] = key_long_down;
                        key_lock[i] = 0;
                        key_state[i] = key_release;
                        
                    }
                }
                else
                {
                    if (key_lock[i] == 1 && key_time[i] > key_short_time)
                    {
                        key_flag[i] = key_short_down;
                        key_state[i] = key_release;
                    }
                    else
                    {
                        key_state[i] = key_check; 
                    }
                }
            } break;
            
            case key_release:
            {
                if (key_read(i))
                {
                    key_state[i] = key_check;
                }
            } break;
            
            default: break;
        }
    }
}

uint8_t key_read(int key_index)
{
    if (key_index == 0)
    {
        return gpio_get_level(key_gpio_pin[0]);  // key1
    }
    else if (key_index == 1)
    {
        return gpio_get_level(key_gpio_pin[1]);  // key2
    }
    else if (key_index == 2)
    {
        return gpio_get_level(key_gpio_pin[2]);  // key3
    }
    else if (key_index == 3)
    {
        return gpio_get_level(key_gpio_pin[3]);  // key4
    }
    // else if (key_index == 4)
    // {
    //     return gpio_get_level(key_gpio_pin[4]);  // key5
    // }
    return 1; 
}

bool is_key_dowm(uint8_t key_index) {
    if (key_flag[key_index] == key_short_down) {
        return true;
    }
    return false;
}

void key_res(){
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        key_flag[i] = 0;
    }
}


// volatile uint16_t time_debug = 0;
// void BM_Control(void)
// {
// // UP 为发车
//     if (BM1 == up)
//     {
//         menu_control();
//     }
//     else if(BM1==down)
//     {
//         if (data_ready_flag_for_core0)
//         {
//             data_ready_flag_for_core0 = 0;
//             ips_show();
//         }
//     }

//     if(BM2 == up&& BM4 == up)
//     {
//         time_debug++;
//         if (time_debug <= 800)
//         {
            
//             if (first_flag == 1)
//             {
//                 system_delay_ms(1000);
//                 Pid_Init();
//                 varyble_init();
//                 first_flag = 0;
//             }
//             gogogo_flag = 1;
//             printf_deal();
            
//         }
//         else
//         {
//             Ljy_set_motor_pwm(0,0);
//             gogogo_flag = 0;
//         }
//     }
//     else if(BM2 == down || BM4 == down)
//     {
//         Ljy_set_motor_pwm(0,0);
//         gogogo_flag = 0;
//         if (first_flag == 0)
//         {
//             time_debug = 0;
//             Pid_Init();
//             first_flag = 1;
//         }
//     }
    
// }
