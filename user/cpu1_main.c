/*********************************************************************************************************************
* TC264 Opensourec Library 锟斤拷锟斤拷TC264 锟斤拷源锟解）锟斤拷一锟斤拷锟斤拷锟节官凤拷 SDK 锟接口的碉拷锟斤拷锟斤拷锟斤拷源锟斤拷
* Copyright (c) 2022 SEEKFREE 锟斤拷煽萍锟
*
* 锟斤拷锟侥硷拷锟斤拷 TC264 锟斤拷源锟斤拷锟揭伙拷锟斤拷锟
*
* TC264 锟斤拷源锟斤拷 锟斤拷锟斤拷锟斤拷锟斤拷锟
* 锟斤拷锟斤拷锟皆革拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷岱拷锟斤拷锟  GPL锟斤拷GNU General Public License锟斤拷锟斤拷 GNU通锟矫癸拷锟斤拷锟斤拷锟斤拷证锟斤拷锟斤拷锟斤拷锟斤拷
* 锟斤拷 GPL 锟侥碉拷3锟芥（锟斤拷 GPL3.0锟斤拷锟斤拷锟斤拷选锟斤拷模锟斤拷魏魏锟斤拷锟斤拷陌姹撅拷锟斤拷锟斤拷路锟斤拷锟斤拷锟 /锟斤拷锟睫革拷锟斤拷
*
* 锟斤拷锟斤拷源锟斤拷姆锟斤拷锟斤拷锟较ｏ拷锟斤拷锟斤拷芊锟斤拷锟斤拷锟斤拷茫锟斤拷锟斤拷锟轿达拷锟斤拷锟斤拷锟斤拷魏蔚谋锟街
* 锟斤拷锟斤拷没锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟皆伙拷锟绞猴拷锟截讹拷锟斤拷途锟侥憋拷证
* 锟斤拷锟斤拷细锟斤拷锟斤拷渭锟  GPL
*
* 锟斤拷应锟斤拷锟斤拷锟秸碉拷锟斤拷锟斤拷源锟斤拷锟酵憋拷盏锟揭伙拷锟  GPL 锟侥革拷锟斤拷
* 锟斤拷锟矫伙拷校锟斤拷锟斤拷锟斤拷<https://www.gnu.org/licenses/>
*
* 锟斤拷锟斤拷注锟斤拷锟斤拷
* 锟斤拷锟斤拷源锟斤拷使锟斤拷 GPL3.0 锟斤拷源锟斤拷锟斤拷证协锟斤拷 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷为锟斤拷锟侥版本
* 锟斤拷锟斤拷锟斤拷锟斤拷英锟侥帮拷锟斤拷 libraries/doc 锟侥硷拷锟斤拷锟铰碉拷 GPL3_permission_statement.txt 锟侥硷拷锟斤拷
* 锟斤拷锟斤拷证锟斤拷锟斤拷锟斤拷 libraries 锟侥硷拷锟斤拷锟斤拷 锟斤拷锟斤拷锟侥硷拷锟斤拷锟铰碉拷 LICENSE 锟侥硷拷
* 锟斤拷迎锟斤拷位使锟矫诧拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 锟斤拷锟睫革拷锟斤拷锟斤拷时锟斤拷锟诫保锟斤拷锟斤拷煽萍锟斤拷陌锟饺拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟
*
* 锟侥硷拷锟斤拷锟斤拷          cpu1_main
* 锟斤拷司锟斤拷锟斤拷          锟缴讹拷锟斤拷煽萍锟斤拷锟斤拷薰锟剿
* 锟芥本锟斤拷息          锟介看 libraries/doc 锟侥硷拷锟斤拷锟斤拷 version 锟侥硷拷 锟芥本说锟斤拷
* 锟斤拷锟斤拷锟斤拷锟斤拷          ADS v1.10.2
* 锟斤拷锟斤拷平台          TC264D
* 锟斤拷锟斤拷锟斤拷锟斤拷          https://seekfree.taobao.com/
*
* 锟睫改硷拷录
* 锟斤拷锟斤拷              锟斤拷锟斤拷                锟斤拷注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"



void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);
/////////////////////////////串口2初始化////////////////////..///////////////////////
//    fifo_init(&spi_uart_data_fifo, FIFO_DATA_8BIT, spi_uart_get_data, 64);              // 初始化 fifo 挂载缓冲区
//    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 初始化串口
//    uart_rx_interrupt(UART_INDEX, 1);                                           // 打开串口2的接收中断
/////////////////////////////串口2初始化//////////////////////////////////////////

    // Image_Send_SPI();

    cpu_wait_event_ready();
    while (1)
    {
//        ljy_ips200_data_printf(7, "turn_count:%d", turn_count);
//        ips200_show_int(200,1,111,3);
        if(mt9v03x_finish_flag)
        {
//             gpio_set_level(P02_6, 0);
             mt9v03x_finish_flag = 0;
//             ljw_camera_deal();
             ljy_camera_deal_flag = 1;
//
//            ips_show();
//             gpio_set_level(P02_6, 1);
            //  memcpy(image_copy[0], img_SPI[0], MY_CAMERA_SIZE);
            //  seekfree_assistant_camera_send();
            //  WIFI_SPI_Data();
        }
        
    }
}



#pragma section all restore
// **************************** 锟斤拷锟斤拷锟斤拷锟斤拷 ****************************
