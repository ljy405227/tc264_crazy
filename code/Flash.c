#include "zf_common_headfile.h"

uint32 Flash_Write_Buffer[Flash_Buffer_NUM] = {0};          // Buffer for writing to Flash
uint32 Flash_Read_Buffer_int[Flash_Buffer_NUM] = {0};       // Buffer for reading integer values from Flash
float Flash_Read_Buffer_float[Flash_Buffer_NUM] = {0};      // Buffer for reading float values from Flash

void Flash_Value_Updata(void)
{
    Flash_Write_Buffer[0] = float_conversion_uint32(Flash.tar_speed);
    Flash_Write_Buffer[1] = float_conversion_uint32(Flash.speed_kp);
    Flash_Write_Buffer[2] = float_conversion_uint32(Flash.speed_ki);

    Flash_Write_Buffer[3] = float_conversion_uint32(Flash.gyro_kp);
    Flash_Write_Buffer[4] = float_conversion_uint32(Flash.gyro_kd);
    Flash_Write_Buffer[5] = float_conversion_uint32(Flash.dir_kp);
    Flash_Write_Buffer[6] = float_conversion_uint32(Flash.dir_kd);

    Flash_Write_Buffer[7] = float_conversion_uint32(Flash.gyro_kp_left);
    Flash_Write_Buffer[8] = float_conversion_uint32(Flash.gyro_kd_left);
    Flash_Write_Buffer[9] = float_conversion_uint32(Flash.dir_kp_left);
    Flash_Write_Buffer[10] = float_conversion_uint32(Flash.dir_kd_left);

    Flash_Write_Buffer[11] = float_conversion_uint32(Flash.gyro_kp_right);
    Flash_Write_Buffer[12] = float_conversion_uint32(Flash.gyro_kd_right);
    Flash_Write_Buffer[13] = float_conversion_uint32(Flash.dir_kp_right);
    Flash_Write_Buffer[14] = float_conversion_uint32(Flash.dir_kd_right);

    Flash_Write_Buffer[15] = float_conversion_uint32(Flash.other_ramp_time);
    Flash_Write_Buffer[16] = float_conversion_uint32(Flash.other_turn_speed);

    Flash_Write_Buffer[17] = float_conversion_uint32(Flash.speed_kp_right);
    Flash_Write_Buffer[18] = float_conversion_uint32(Flash.speed_ki_right);
    Flash_Write_Buffer[19] = float_conversion_uint32(Flash.speed_kd);
}

// Write data to Flash
void Flash_Value_Write(void)
{
    int i;
    // Read latest data before writing
    Flash_Value_Updata();
    // Erase sector 0
    new_eeprom_erase_sector(0);

    // One sector = 1024 pages, write one value per page
    for (i = 0; i < Flash_Buffer_NUM; i++)
    {
        new_eeprom_page_program(0, i, &Flash_Write_Buffer[i]);
    }
    printf("ljy_Flash Write OK!\n");
}

// Read data from Flash
void Flash_Value_Read(void)
{
    Flash_Read_Buffer_float[0] = flash_read(0, 0, float);
    Flash_Read_Buffer_float[1] = flash_read(0, 1, float);
    Flash_Read_Buffer_float[2] = flash_read(0, 2, float);
    Flash_Read_Buffer_float[3] = flash_read(0, 3, float);
    Flash_Read_Buffer_float[4] = flash_read(0, 4, float);
    Flash_Read_Buffer_float[5] = flash_read(0, 5, float);
    Flash_Read_Buffer_float[6] = flash_read(0, 6, float);
    Flash_Read_Buffer_float[7] = flash_read(0, 7, float);
    Flash_Read_Buffer_float[8] = flash_read(0, 8, float);
    Flash_Read_Buffer_float[9] = flash_read(0, 9, float);
    Flash_Read_Buffer_float[10] = flash_read(0, 10, float);
    Flash_Read_Buffer_float[11] = flash_read(0, 11, float);
    Flash_Read_Buffer_float[12] = flash_read(0, 12, float);
    Flash_Read_Buffer_float[13] = flash_read(0, 13, float);
    Flash_Read_Buffer_float[14] = flash_read(0, 14, float);
    Flash_Read_Buffer_float[15] = flash_read(0, 15, float);

    Flash_Read_Buffer_float[16] = flash_read(0, 16, float);
    Flash_Read_Buffer_float[17] = flash_read(0, 17, float);
    Flash_Read_Buffer_float[18] = flash_read(0, 18, float);
    Flash_Read_Buffer_float[19] = flash_read(0, 19, float);
    Flash_Read_Buffer_float[20] = flash_read(0, 20, float);
    Flash_Read_Buffer_float[21] = flash_read(0, 21, float);
    Flash_Read_Buffer_float[22] = flash_read(0, 22, float);
    Flash_Read_Buffer_float[23] = flash_read(0, 23, float);
    Flash_Read_Buffer_float[24] = flash_read(0, 24, float);
    Flash_Read_Buffer_float[25] = flash_read(0, 25, float);
    Flash_Read_Buffer_float[26] = flash_read(0, 26, float);
    Flash_Read_Buffer_float[27] = flash_read(0, 27, float);
    Flash_Read_Buffer_float[28] = flash_read(0, 28, float);
    Flash_Read_Buffer_float[29] = flash_read(0, 29, float);
}

void Startingup_Flash_Value_Updata(void)
{
    Flash_Value_Read();     // Read data from Flash

    Flash.tar_speed = Flash_Read_Buffer_float[0];
    Flash.speed_kp = Flash_Read_Buffer_float[1];
    Flash.speed_ki = Flash_Read_Buffer_float[2];

    Flash.gyro_kp = Flash_Read_Buffer_float[3];
    Flash.gyro_kd = Flash_Read_Buffer_float[4];
    Flash.dir_kp = Flash_Read_Buffer_float[5];
    Flash.dir_kd = Flash_Read_Buffer_float[6];

    Flash.gyro_kp_left = Flash_Read_Buffer_float[7];
    Flash.gyro_kd_left = Flash_Read_Buffer_float[8];
    Flash.dir_kp_left = Flash_Read_Buffer_float[9];
    Flash.dir_kd_left = Flash_Read_Buffer_float[10];

    Flash.gyro_kp_right = Flash_Read_Buffer_float[11];
    Flash.gyro_kd_right = Flash_Read_Buffer_float[12];
    Flash.dir_kp_right = Flash_Read_Buffer_float[13];
    Flash.dir_kd_right = Flash_Read_Buffer_float[14];

    Flash.other_ramp_time = Flash_Read_Buffer_float[15];
    Flash.other_turn_speed = Flash_Read_Buffer_float[16];

    Flash.speed_kp_right = Flash_Read_Buffer_float[17];
    Flash.speed_ki_right = Flash_Read_Buffer_float[18];

    Flash.speed_kd = Flash_Read_Buffer_float[19];
}

void flash(void)
{
    // Placeholder function (currently unused)
}