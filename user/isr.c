#include "isr_config.h"
#include "isr.h"
#include "zf_common_headfile.h"

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);
    pit_clear_flag(CCU60_CH0);
    key_scan(); 
    pit_count_ms++;
}



IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{
    
    interrupt_global_enable(0);
    pit_clear_flag(CCU60_CH1);
//    gpio_set_level(P02_6, 0);
    Process_Gyro();
    Updata_IMU(gyro[0].data,gyro[1].data,gyro[2].data,accel[0].data,accel[1].data,accel[2].data);

    
    
    if (gogogo_flag == 1)
    {
        ljy_isr_headle();
    }
     
//   gpio_set_level(P02_6, 1);
}

IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);
    pit_clear_flag(CCU61_CH0);
}

IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);
    pit_clear_flag(CCU61_CH1);
}
// **************************** PIT锟叫断猴拷锟斤拷 ****************************


// **************************** 锟解部锟叫断猴拷锟斤拷 ****************************
IFX_INTERRUPT(exti_ch0_ch4_isr, 0, EXTI_CH0_CH4_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    if(exti_flag_get(ERU_CH0_REQ0_P15_4))           // 通锟斤拷0锟叫讹拷
    {
        exti_flag_clear(ERU_CH0_REQ0_P15_4);

    }

    if(exti_flag_get(ERU_CH4_REQ13_P15_5))          // 通锟斤拷4锟叫讹拷
    {
        exti_flag_clear(ERU_CH4_REQ13_P15_5);




    }
}

IFX_INTERRUPT(exti_ch1_ch5_isr, 0, EXTI_CH1_CH5_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷

    if(exti_flag_get(ERU_CH1_REQ10_P14_3))          // 通锟斤拷1锟叫讹拷
    {
        exti_flag_clear(ERU_CH1_REQ10_P14_3);

        tof_module_exti_handler();                  // ToF 模锟斤拷 INT 锟斤拷锟斤拷锟叫讹拷

    }

    if(exti_flag_get(ERU_CH5_REQ1_P15_8))           // 通锟斤拷5锟叫讹拷
    {
        exti_flag_clear(ERU_CH5_REQ1_P15_8);


    }
}

// 锟斤拷锟斤拷锟斤拷锟斤拷头pclk锟斤拷锟斤拷默锟斤拷占锟斤拷锟斤拷 2通锟斤拷锟斤拷锟斤拷锟节达拷锟斤拷DMA锟斤拷锟斤拷锟斤拷锟斤拷锊伙拷俣锟斤拷锟斤拷卸虾锟斤拷锟�
// IFX_INTERRUPT(exti_ch2_ch6_isr, 0, EXTI_CH2_CH6_INT_PRIO)
// {
//  interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
//  if(exti_flag_get(ERU_CH2_REQ7_P00_4))           // 通锟斤拷2锟叫讹拷
//  {
//      exti_flag_clear(ERU_CH2_REQ7_P00_4);
//  }
//  if(exti_flag_get(ERU_CH6_REQ9_P20_0))           // 通锟斤拷6锟叫讹拷
//  {
//      exti_flag_clear(ERU_CH6_REQ9_P20_0);
//  }
// }
IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    if(exti_flag_get(ERU_CH3_REQ6_P02_0))           // 通锟斤拷3锟叫讹拷
    {
        exti_flag_clear(ERU_CH3_REQ6_P02_0);
        camera_vsync_handler();                     // 锟斤拷锟斤拷头锟斤拷锟斤拷锟缴硷拷统一锟截碉拷锟斤拷锟斤拷
    }
    if(exti_flag_get(ERU_CH7_REQ16_P15_1))          // 通锟斤拷7锟叫讹拷
    {
        exti_flag_clear(ERU_CH7_REQ16_P15_1);




    }
}
// **************************** 锟解部锟叫断猴拷锟斤拷 ****************************


// **************************** DMA锟叫断猴拷锟斤拷 ****************************
IFX_INTERRUPT(dma_ch5_isr, 0, DMA_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    camera_dma_handler();                           // 锟斤拷锟斤拷头锟缴硷拷锟斤拷锟酵骋伙拷氐锟斤拷锟斤拷锟�
}
// **************************** DMA锟叫断猴拷锟斤拷 ****************************


// **************************** 锟斤拷锟斤拷锟叫断猴拷锟斤拷 ****************************
// 锟斤拷锟斤拷0默锟斤拷锟斤拷为锟斤拷锟皆达拷锟斤拷
IFX_INTERRUPT(uart0_tx_isr, 0, UART0_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷



}
IFX_INTERRUPT(uart0_rx_isr, 0, UART0_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    // uart_rx_interrupt_handler();
#if DEBUG_UART_USE_INTERRUPT                        // 锟斤拷锟斤拷锟斤拷锟� debug 锟斤拷锟斤拷锟叫讹拷
    debug_interrupr_handler();                  // 锟斤拷锟斤拷 debug 锟斤拷锟节斤拷锟秸达拷锟斤拷锟斤拷锟斤拷 锟斤拷锟捷会被 debug 锟斤拷锟轿伙拷锟斤拷锟斤拷锟斤拷取
#endif                                              // 锟斤拷锟斤拷薷锟斤拷锟� DEBUG_UART_INDEX 锟斤拷锟斤拷未锟斤拷锟斤拷锟揭拷诺锟斤拷锟接︼拷拇锟斤拷锟斤拷卸锟饺�
}




// 锟斤拷锟斤拷1默锟斤拷锟斤拷锟接碉拷锟斤拷锟斤拷头锟斤拷锟矫达拷锟斤拷
IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷




}
IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    camera_uart_handler();                          // 锟斤拷锟斤拷头锟斤拷锟斤拷锟斤拷锟斤拷统一锟截碉拷锟斤拷锟斤拷
}

// 锟斤拷锟斤拷2默锟斤拷锟斤拷锟接碉拷锟斤拷锟斤拷转锟斤拷锟斤拷模锟斤拷
IFX_INTERRUPT(uart2_tx_isr, 0, UART2_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷



}

IFX_INTERRUPT(uart2_rx_isr, 0, UART2_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    wireless_module_uart_handler();                 // 锟斤拷锟斤拷模锟斤拷统一锟截碉拷锟斤拷锟斤拷



}
// 锟斤拷锟斤拷3默锟斤拷锟斤拷锟接碉拷GPS锟斤拷位模锟斤拷
IFX_INTERRUPT(uart3_tx_isr, 0, UART3_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷



}

IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    gnss_uart_callback();                           // GNSS锟斤拷锟节回碉拷锟斤拷锟斤拷



}

// 锟斤拷锟斤拷通讯锟斤拷锟斤拷锟叫讹拷
IFX_INTERRUPT(uart0_er_isr, 0, UART0_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    IfxAsclin_Asc_isrError(&uart0_handle);
}
IFX_INTERRUPT(uart1_er_isr, 0, UART1_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    IfxAsclin_Asc_isrError(&uart1_handle);
}
IFX_INTERRUPT(uart2_er_isr, 0, UART2_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    IfxAsclin_Asc_isrError(&uart2_handle);
}
IFX_INTERRUPT(uart3_er_isr, 0, UART3_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 锟斤拷锟斤拷锟叫讹拷嵌锟斤拷
    IfxAsclin_Asc_isrError(&uart3_handle);
}
