#include "zf_common_headfile.h"


MenuState menu_state = main_menu;
uint8_t main_choose_item = 0;
uint8_t secondary_choose_item = 0;
uint8_t data_change_flag = 0;
float data_original_value = 0.0f;
MenuItem main_menu_items[MENU_ITEM_COUNT] = {
    {"speed", NULL, NULL},
    {"dir_str", NULL, NULL},
    {"dir_left", NULL, NULL},
    {"dir_right", NULL, NULL},
    {"other", NULL, NULL},
    {"go", NULL, NULL}
};
MenuItem speed_adjust_menu[secondary_menu_count] = {
    {"tar_speed:%.2f", NULL, &Flash.tar_speed},
    {"speed_kp: %.2f", NULL, &Flash.speed_kp},
    {"speed_ki: %.2f", NULL, &Flash.speed_ki},
    {"speed_kd: %.2f", NULL, &Flash.speed_kd},
    {"speed_i: %.2f", NULL, &Flash.speed_ki_right},
    
    // {"go", NULL}
};

MenuItem direction_adjust_menu[secondary_menu_count] = {
    {"tar_speed: %.2f", NULL, &Flash.tar_speed},
    {"gyro_kp: %.2f", NULL, &Flash.gyro_kp},
    {"gyro_kd: %.2f", NULL, &Flash.gyro_kd},
    {"dir_kp: %.2f", NULL, &Flash.dir_kp},
    {"dir_kd: %.2f", NULL, &Flash.dir_kd},
    // {"go", NULL}
};

MenuItem dir_left_adjust_menu[secondary_menu_count] = {
    {"tar_speed: %.2f", NULL, &Flash.tar_speed},
    {"gyro_kp_left: %.2f", NULL, &Flash.gyro_kp_left},
    {"gyro_kd_left: %.2f", NULL, &Flash.gyro_kd_left},
    {"dir_kp_left: %.2f", NULL, &Flash.dir_kp_left},
    {"dir_kd_left: %.2f", NULL, &Flash.dir_kd_left},
    // {"go", NULL}
};

MenuItem dir_right_adjust_menu[secondary_menu_count] = {
    {"tar_speed: %.2f", NULL, &Flash.tar_speed},
    {"gyro_kp_right: %.2f", NULL, &Flash.gyro_kp_right},
    {"gyro_kd_right: %.2f", NULL, &Flash.gyro_kd_right},
    {"dir_kp_right: %.2f", NULL, &Flash.dir_kp_right},
    {"dir_kd_right: %.2f", NULL, &Flash.dir_kd_right},
    // {"go", NULL}
};

MenuItem other_adjust_menu[secondary_menu_count] = {
    {"ramp_time: %.2f", NULL, &Flash.other_ramp_time},
    {"turn_speed: %.2f", NULL, &Flash.other_turn_speed},

    {"gyro_kd_right: %.2f", NULL, &Flash.gyro_kd_right},
    {"dir_kp_right: %.2f", NULL, &Flash.dir_kp_right},
    {"dir_kd_right: %.2f", NULL, &Flash.dir_kd_right},
    // {"go", NULL}
};


void menu_choose(uint8_t *menu_item, uint8_t menu_count)
{
    ips200_show_string( 0 , 20*(*menu_item),"-->");
    if (key_flag[1] == key_short_down)
    {
        // uint8_t last_main_choose_item = *menu_item;
        ips200_show_string( 0 , 20**menu_item,"   ");
        *menu_item = (*menu_item + 1) % menu_count;
        key_res();
    }
    else if (key_flag[0] == key_short_down)
    {
        // uint8_t last_main_choose_item = *menu_item;
        ips200_show_string( 0 , 20**menu_item,"   ");
        *menu_item = (*menu_item + menu_count - 1) % menu_count;
        key_res();
    }
}

void menu_control(void)
{
    ips200_draw_line(0, 121, 239, 121, RGB565_RED);
    switch (menu_state)
    {
        case main_menu:
            main_menu_handle();
            break;
        case secondary_menu:
            secondary_menu_handle();
            break;
        case three_level_menu:
            three_level_task_menu_handle();
            break;
        default:
            menu_state = main_menu;
            break;
    }
}

void main_menu_handle(void)
{
    for ( int i = 0; i < MENU_ITEM_COUNT; i++)
    {
        ips200_show_string( 30 , 20*i, main_menu_items[i].name);
    }
    menu_choose(&main_choose_item, MENU_ITEM_COUNT);
    if (key_flag[2] == key_short_down)
    {
        ljy_ips200_clear();
        menu_state = secondary_menu;
        key_res();
    }
}

void secondary_menu_handle(void)
{
    switch (main_choose_item)
    {
    case 0:
    {
        for (int i = 0; i < secondary_menu_count; i++)
        {
            ljy_ips200_printf(30, i*20,  speed_adjust_menu[i].name, *speed_adjust_menu[i].value);
        }
        
        if(data_change_flag == 1)
        {
            ips200_show_string( 0 , 20*secondary_choose_item,"   ");
            ips200_show_string( 200 , 20*secondary_choose_item,"<--");
            if (key_flag[1] == key_short_down)
            {
                if (secondary_choose_item == 0 || secondary_choose_item == 1)
                {
                    *speed_adjust_menu[secondary_choose_item].value -= 1.0f;
                }
                else
                {
                    *speed_adjust_menu[secondary_choose_item].value -= 0.02f;
                }
                key_res();
            }
            else if (key_flag[0] == key_short_down)
            {
                if (secondary_choose_item == 0 || secondary_choose_item == 1)
                {
                    *speed_adjust_menu[secondary_choose_item].value += 1.0f;
                }
                else
                {
                    *speed_adjust_menu[secondary_choose_item].value += 0.02f;
                }
                
                key_res();
            }
            else if (key_flag[2] == key_short_down)
            {
                ljy_ips200_clear();
                Flash_Value_Write();
                Pid_Init();
                data_change_flag = 0;
                key_res();
            }
            else if (key_flag[3] == key_short_down)
            {
                ljy_ips200_clear();
                *speed_adjust_menu[secondary_choose_item].value = data_original_value;
                data_change_flag = 0;
                key_res();
            }
            
        }
        break;
    }
    case 1:
    {
        for (int i = 0; i < secondary_menu_count; i++)
        {
            ljy_ips200_printf(30, i*20,  direction_adjust_menu[i].name, *direction_adjust_menu[i].value);
        }
        
        
        if(data_change_flag == 1)
        {
            ips200_show_string( 0 , 20*secondary_choose_item,"   ");
            ips200_show_string( 200 , 20*secondary_choose_item,"<--");
            if (key_flag[1] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *direction_adjust_menu[secondary_choose_item].value -= 5.0f;
                }
                else if (secondary_choose_item == 4)
                {
                    *direction_adjust_menu[secondary_choose_item].value -= 0.1f;
                }
                else
                {
                    *direction_adjust_menu[secondary_choose_item].value -= 0.1f;
                }
                key_res();
            }
            else if (key_flag[0] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *direction_adjust_menu[secondary_choose_item].value += 5.0f;
                }
                else if (secondary_choose_item == 4)
                {
                    *direction_adjust_menu[secondary_choose_item].value += 0.1f;
                }
                else
                {
                    *direction_adjust_menu[secondary_choose_item].value += 0.1f;
                }
                
                key_res();
            }
            else if (key_flag[2] == key_short_down)
            {
                ljy_ips200_clear();
                Flash_Value_Write();
                Pid_Init();
                data_change_flag = 0;
                key_res();
            }
            else if (key_flag[3] == key_short_down)
            {
                ljy_ips200_clear();
                *direction_adjust_menu[secondary_choose_item].value = data_original_value;
                data_change_flag = 0;
                key_res();
            }
            
        }
        break;
    }

    case 2:
    {
        for (int i = 0; i < secondary_menu_count; i++)
        {
            ljy_ips200_printf(30, i*20,  dir_left_adjust_menu[i].name, *dir_left_adjust_menu[i].value);
        }
        
        
        if(data_change_flag == 1)
        {
            ips200_show_string( 0 , 20*secondary_choose_item,"   ");
            ips200_show_string( 200 , 20*secondary_choose_item,"<--");
            if (key_flag[1] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *dir_left_adjust_menu[secondary_choose_item].value -= 5.0f;
                }
                else if (secondary_choose_item == 4)
                {
                    *dir_left_adjust_menu[secondary_choose_item].value -= 0.1f;
                }
                else
                {
                    *dir_left_adjust_menu[secondary_choose_item].value -= 0.1f;
                }
                key_res();
            }
            else if (key_flag[0] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *dir_left_adjust_menu[secondary_choose_item].value += 5.0f;
                }
                else if (secondary_choose_item == 4)
                {
                    *dir_left_adjust_menu[secondary_choose_item].value += 0.1f;
                }
                else
                {
                    *dir_left_adjust_menu[secondary_choose_item].value += 0.1f;
                }
                
                key_res();
            }
            else if (key_flag[2] == key_short_down)
            {
                ljy_ips200_clear();
                Flash_Value_Write();
                Pid_Init();
                data_change_flag = 0;
                key_res();
            }
            else if (key_flag[3] == key_short_down)
            {
                ljy_ips200_clear();
                *dir_left_adjust_menu[secondary_choose_item].value = data_original_value;
                data_change_flag = 0;
                key_res();
            }
            
        }
        break;
    }

    case 3:
    {
        for (int i = 0; i < secondary_menu_count; i++)
        {
            ljy_ips200_printf(30, i*20,  dir_right_adjust_menu[i].name, *dir_right_adjust_menu[i].value);
        }
        
        
        if(data_change_flag == 1)
        {
            ips200_show_string( 0 , 20*secondary_choose_item,"   ");
            ips200_show_string( 200 , 20*secondary_choose_item,"<--");
            if (key_flag[1] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *dir_right_adjust_menu[secondary_choose_item].value -= 5.0f;
                }
                else if (secondary_choose_item == 4)
                {
                    *dir_right_adjust_menu[secondary_choose_item].value -= 0.1f;
                }
                else
                {
                    *dir_right_adjust_menu[secondary_choose_item].value -= 0.1f;
                }
                key_res();
            }
            else if (key_flag[0] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *dir_right_adjust_menu[secondary_choose_item].value += 5.0f;
                }
                else if (secondary_choose_item == 4)
                {
                    *dir_right_adjust_menu[secondary_choose_item].value += 0.1f;
                }
                else
                {
                    *dir_right_adjust_menu[secondary_choose_item].value += 0.1f;
                }
                
                key_res();
            }
            else if (key_flag[2] == key_short_down)
            {
                ljy_ips200_clear();
                Flash_Value_Write();
                Pid_Init();
                data_change_flag = 0;
                key_res();
            }
            else if (key_flag[3] == key_short_down)
            {
                ljy_ips200_clear();
                *dir_right_adjust_menu[secondary_choose_item].value = data_original_value;
                data_change_flag = 0;
                key_res();
            }
            
        }
        break;
    }

    case 4:
    {
        for (int i = 0; i < secondary_menu_count; i++)
        {
            ljy_ips200_printf(30, i*20,  other_adjust_menu[i].name, *other_adjust_menu[i].value);
        }
        
        
        if(data_change_flag == 1)
        {
            ips200_show_string( 0 , 20*secondary_choose_item,"   ");
            ips200_show_string( 200 , 20*secondary_choose_item,"<--");
            if (key_flag[1] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *other_adjust_menu[secondary_choose_item].value -= 0.05f;
                }
                else
                {
                    *other_adjust_menu[secondary_choose_item].value -= 1.0f;
                }
                key_res();
            }
            else if (key_flag[0] == key_short_down)
            {
                if (secondary_choose_item == 0)
                {
                    *other_adjust_menu[secondary_choose_item].value += 0.05f;
                }
                else
                {
                    *other_adjust_menu[secondary_choose_item].value += 1.0f;
                }
                
                key_res();
            }
            else if (key_flag[2] == key_short_down)
            {
                ljy_ips200_clear();
                Flash_Value_Write();
                Pid_Init();
                data_change_flag = 0;
                key_res();
            }
            else if (key_flag[3] == key_short_down)
            {
                ljy_ips200_clear();
                *other_adjust_menu[secondary_choose_item].value = data_original_value;
                data_change_flag = 0;
                key_res();
            }
            
        }
        break;
    }

    case 5:
    {
        
        ips_show();
        if (key_flag[3] == key_short_down)
        {
            // gogogo_flag = 1;
            system_delay_ms(500);
            ljy_ips200_clear();
            menu_state = main_menu;
            key_res();
        }
        break;
    }
    default:
        break;
    }
    
    if (data_change_flag == 0 && main_choose_item != 5)
    {
        menu_choose(&secondary_choose_item, secondary_menu_count);
    }
    {

        menu_choose(&secondary_choose_item, secondary_menu_count);
        if (key_flag[2] == key_short_down)
        {
            switch (main_choose_item)
            {
            case 0:
                data_original_value = *speed_adjust_menu[secondary_choose_item].value;
                break;
            case 1:
                data_original_value = *direction_adjust_menu[secondary_choose_item].value;
                break;
            case 2:
                data_original_value = *dir_left_adjust_menu[secondary_choose_item].value;
                break;
            case 3:
                data_original_value = *dir_right_adjust_menu[secondary_choose_item].value;
                break;
            
            default:
                break;
            }
            
            data_change_flag = 1;
            key_res();
        }
        if (key_flag[3] == key_short_down)
        {
            ljy_ips200_clear();
            menu_state = main_menu;
            key_res();
        }
    }
}

void three_level_task_menu_handle(void)
{
    // switch (task_choose_item)
    // {
    // case 0: /* speed */
    //     ljy_ips200_show_str_flo(0, 0, "speed:", Flash.tar_speed);
    //     break;
    
    // default:
    //     break;
    // }
}
