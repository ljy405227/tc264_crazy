#include "zf_common_headfile.h"
#include "stdio.h"
/******VOFA+示波器**
 *
 * 用法：VOFA_Send(数据);
 *                       一个数据调用一次，可显示无限个数据
 *      VOFA_draw();在上位机上显示出数据波形
 *
 *      这两句放在主函数的while里
 * ******/



uint8 t_test=0;
uint8 send_date[4]={0};
uint8 byte[4]={0};
/****************************************************
将浮点数f转化为4个字节数据存放在byte[4]中
*****************************************************/

void Float_to_Byte(float f,unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}


void Byte_to_Float(float *f,unsigned char byte[])
{
    FloatLongType fl;
    fl.ldata=0;
    fl.ldata=byte[3];
    fl.ldata=(fl.ldata<<8)|byte[2];
    fl.ldata=(fl.ldata<<8)|byte[1];
    fl.ldata=(fl.ldata<<8)|byte[0];
    *f=fl.fdata;
}


/*************VOFA发送数据***********************/
void VOFA_Send(float t)
{
    Float_to_Byte(t*1.0,byte);

    for(t_test=0;t_test<4;t_test++)
    {
       uart_write_byte(UART_2, byte[t_test]);//向串口1发送数据
    }
}

/*************VOFA绘制图像***********************/
void VOFA_draw(void)
{
    /*曲线绘制协议后缀*/
    send_date[0]=0X00;send_date[1]=0X00;
    send_date[2]=0X80;send_date[3]=0X7f;

    for(t_test=0;t_test<4;t_test++)
    {
       uart_write_byte(UART_2, send_date[t_test]);         //向串口2发送数据
    }
}

void My_Printf(float Data)
{
    VOFA_Send(Data);
    VOFA_draw();
}


void My_Vofa_Send(void)
{
    VOFA_Send(1);
    VOFA_Send(2);

    VOFA_draw();
}



