#include "zf_common_headfile.h"

uint32  Flash_Write_Buffer[Flash_Buffer_NUM]={0}; //存放写入Flash的值
uint32  Flash_Read_Buffer_int[Flash_Buffer_NUM]={0};  //存放从Flash读取的整型值
float   Flash_Read_Buffer_float[Flash_Buffer_NUM]={0};//存放从Flash读取的浮点型值

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




//1
//      Flash_Write_Buffer[0] = float_conversion_uint32(Flash.img_kp);
//      Flash_Write_Buffer[1] = float_conversion_uint32(Flash.img_kp2);
//      Flash_Write_Buffer[2] = float_conversion_uint32(Flash.img_max_out);
//      Flash_Write_Buffer[3] = float_conversion_uint32(Flash.dis_kf);
//      Flash_Write_Buffer[4] = float_conversion_uint32(Flash.dis_kp);
//      Flash_Write_Buffer[5] = float_conversion_uint32(Flash.dis_ki);
//      Flash_Write_Buffer[6] = float_conversion_uint32(Flash.dis_kd);
//      Flash_Write_Buffer[7] = float_conversion_uint32(Flash.dis_max_intel);
//      Flash_Write_Buffer[8] = float_conversion_uint32(Flash.dis_max_out);

// //     Flash_Write_Buffer[5] = float_conversion_uint32(Flash.speed_max_intel);
// //     Flash_Write_Buffer[6] = float_conversion_uint32(Flash.speed_max_out);
// //     Flash_Write_Buffer[7] = float_conversion_uint32(Flash.tar_speed);


// //16
//      Flash_Write_Buffer[16] = float_conversion_uint32(Flash.speed_row);

//      Flash_Write_Buffer[17] = float_conversion_uint32(Flash.speed_kf);
//      Flash_Write_Buffer[18] = float_conversion_uint32(Flash.speed_kp);
//      Flash_Write_Buffer[19] = float_conversion_uint32(Flash.speed_ki);
//      Flash_Write_Buffer[20] = float_conversion_uint32(Flash.speed_kd);
//      Flash_Write_Buffer[21] = float_conversion_uint32(Flash.speed_max_intel);

//      Flash_Write_Buffer[22] = float_conversion_uint32(Flash.speedl_kp);
//      Flash_Write_Buffer[23] = float_conversion_uint32(Flash.speedl_ki);

//      Flash_Write_Buffer[24] = float_conversion_uint32(Flash.speedr_kp);
//      Flash_Write_Buffer[25] = float_conversion_uint32(Flash.speedr_ki);

//      Flash_Write_Buffer[26] = float_conversion_uint32(Flash.speed_max_out);

//      Flash_Write_Buffer[27] = float_conversion_uint32(Flash.tar_speed);
//      Flash_Write_Buffer[28] = float_conversion_uint32(Flash.Brushless_duty);
//      Flash_Write_Buffer[29] = float_conversion_uint32(Flash.Sped_dec);


// //32

}

// 向Flash写入数据
void Flash_Value_Write(void)
{
    int i;
    //写入数据前读取当前最新数据
    Flash_Value_Updata();
    //擦除扇区0
    new_eeprom_erase_sector(0);

    //一个扇区有1024页，一个数据写一页都够我写    我就一页写一个数据进去
    for(i=0;i<Flash_Buffer_NUM;i++)
    {
        new_eeprom_page_program(0, i, &Flash_Write_Buffer[i]);
    }
    printf("ljy_Flash Write OK!\n");
}

// 读取flash数据
void Flash_Value_Read(void)
{

    Flash_Read_Buffer_float[0]=flash_read(0,0,float);
    Flash_Read_Buffer_float[1]=flash_read(0,1,float);
    Flash_Read_Buffer_float[2]=flash_read(0,2,float);
    Flash_Read_Buffer_float[3]=flash_read(0,3,float);
    Flash_Read_Buffer_float[4]=flash_read(0,4,float);
    Flash_Read_Buffer_float[5]=flash_read(0,5,float);
    Flash_Read_Buffer_float[6]=flash_read(0,6,float);
    Flash_Read_Buffer_float[7]=flash_read(0,7,float);
    Flash_Read_Buffer_float[8]=flash_read(0,8,float);
    Flash_Read_Buffer_float[9]=flash_read(0,9,float);
    Flash_Read_Buffer_float[10]=flash_read(0,10,float);
    Flash_Read_Buffer_float[11]=flash_read(0,11,float);
    Flash_Read_Buffer_float[12]=flash_read(0,12,float);
    Flash_Read_Buffer_float[13]=flash_read(0,13,float);
    Flash_Read_Buffer_float[14]=flash_read(0,14,float);
    Flash_Read_Buffer_float[15]=flash_read(0,15,float);

//    Flash_Read_Buffer_float[5]=flash_read(0,5,float);

    Flash_Read_Buffer_float[16]=flash_read(0,16,float);
    Flash_Read_Buffer_float[17]=flash_read(0,17,float);
    Flash_Read_Buffer_float[18]=flash_read(0,18,float);
    Flash_Read_Buffer_float[19]=flash_read(0,19,float);
    Flash_Read_Buffer_float[20]=flash_read(0,20,float);
    Flash_Read_Buffer_float[21]=flash_read(0,21,float);
    Flash_Read_Buffer_float[22]=flash_read(0,22,float);
    Flash_Read_Buffer_float[23]=flash_read(0,23,float);
    Flash_Read_Buffer_float[24]=flash_read(0,24,float);
    Flash_Read_Buffer_float[25]=flash_read(0,25,float);
    Flash_Read_Buffer_float[26]=flash_read(0,26,float);
    Flash_Read_Buffer_float[27]=flash_read(0,27,float);
    Flash_Read_Buffer_float[28]=flash_read(0,28,float);
    Flash_Read_Buffer_float[29]=flash_read(0,29,float);
    //Flash_Read_Buffer_float[16]=flash_read(0,16,float);



}

void Startingup_Flash_Value_Updata(void)
{
    Flash_Value_Read(); //将数据从FLASH中读出来

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




    // Flash.img_kp = Flash_Read_Buffer_float[0];
    // Flash.img_kp2 = Flash_Read_Buffer_float[1];
    // Flash.img_max_out = Flash_Read_Buffer_float[2];

    // Flash.dis_kf = Flash_Read_Buffer_float[3];
    // Flash.dis_kp = Flash_Read_Buffer_float[4];
    // Flash.dis_ki = Flash_Read_Buffer_float[5];
    // Flash.dis_kd = Flash_Read_Buffer_float[6];
    // Flash.dis_max_intel = Flash_Read_Buffer_float[7];
    // Flash.dis_max_out = Flash_Read_Buffer_float[8];

    // /*********速度环**********/
    // Flash.speed_row = Flash_Read_Buffer_float[16];

    // Flash.speed_kf = Flash_Read_Buffer_float[17];
    // Flash.speed_kp = Flash_Read_Buffer_float[18];
    // Flash.speed_ki = Flash_Read_Buffer_float[19];
    // Flash.speed_kd = Flash_Read_Buffer_float[20];
    // Flash.speed_max_intel = Flash_Read_Buffer_float[21];

    // Flash.speedl_kp = Flash_Read_Buffer_float[22];
    // Flash.speedl_ki = Flash_Read_Buffer_float[23];

    // Flash.speedr_kp = Flash_Read_Buffer_float[24];
    // Flash.speedr_ki = Flash_Read_Buffer_float[25];

    // Flash.speed_max_out = Flash_Read_Buffer_float[26];

    // Flash.tar_speed = Flash_Read_Buffer_float[27];
    // Flash.Brushless_duty = Flash_Read_Buffer_float[28];
    // Flash.Sped_dec = Flash_Read_Buffer_float[29];
}

void flash(void)
{
    // Flash.img_kp = Flash_Read_Buffer_float[0];
    // Flash.img_kp2 = Flash_Read_Buffer_float[1];
    // Flash.img_max_out = Flash_Read_Buffer_float[2];

    // Flash.dis_kf = Flash_Read_Buffer_float[3];
    // Flash.dis_kp = Flash_Read_Buffer_float[4];
    // Flash.dis_ki = Flash_Read_Buffer_float[5];
    // Flash.dis_kd = Flash_Read_Buffer_float[6];
    // Flash.dis_max_intel = Flash_Read_Buffer_float[7];
    // Flash.dis_max_out = Flash_Read_Buffer_float[8];

    // /*********速度环**********/
    // Flash.speed_row = Flash_Read_Buffer_float[16];

    // Flash.speed_kf = Flash_Read_Buffer_float[17];
    // Flash.speed_kp = Flash_Read_Buffer_float[18];
    // Flash.speed_ki = Flash_Read_Buffer_float[19];
    // Flash.speed_kd = Flash_Read_Buffer_float[20];
    // Flash.speed_max_intel = Flash_Read_Buffer_float[21];

    // Flash.speedl_kp = Flash_Read_Buffer_float[22];
    // Flash.speedl_ki = Flash_Read_Buffer_float[23];

    // Flash.speedr_kp = Flash_Read_Buffer_float[24];
    // Flash.speedr_ki = Flash_Read_Buffer_float[25];

    // Flash.speed_max_out = Flash_Read_Buffer_float[26];

    // Flash.tar_speed = Flash_Read_Buffer_float[27];
    // Flash.Brushless_duty = Flash_Read_Buffer_float[28];
    // Flash.Sped_dec = Flash_Read_Buffer_float[29];
}
