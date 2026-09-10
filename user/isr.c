#include "isr_config.h"
#include "isr.h"
#include "zf_common_headfile.h"

// **************************** PIT Timer Interrupts ****************************
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
    Process_Gyro();
    Updata_IMU(gyro[0].data, gyro[1].data, gyro[2].data, accel[0].data, accel[1].data, accel[2].data);
    if (pit_control_flag == 1)
    {
        ljy_isr_headle();
    }
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
// **************************** PIT Timer Interrupts ****************************


// **************************** External Interrupts ****************************
IFX_INTERRUPT(exti_ch0_ch4_isr, 0, EXTI_CH0_CH4_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    if (exti_flag_get(ERU_CH0_REQ0_P15_4))          // Channel 0 trigger
    {
        exti_flag_clear(ERU_CH0_REQ0_P15_4);
    }

    if (exti_flag_get(ERU_CH4_REQ13_P15_5))         // Channel 4 trigger
    {
        exti_flag_clear(ERU_CH4_REQ13_P15_5);
    }
}

IFX_INTERRUPT(exti_ch1_ch5_isr, 0, EXTI_CH1_CH5_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts

    if (exti_flag_get(ERU_CH1_REQ10_P14_3))         // Channel 1 trigger
    {
        exti_flag_clear(ERU_CH1_REQ10_P14_3);
        tof_module_exti_handler();                  // ToF module INT handler
    }

    if (exti_flag_get(ERU_CH5_REQ1_P15_8))          // Channel 5 trigger
    {
        exti_flag_clear(ERU_CH5_REQ1_P15_8);
    }
}

// Camera pclk interrupt (default occupies 2 channels, triggers DMA at high speed)
IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    if (exti_flag_get(ERU_CH3_REQ6_P02_0))          // Channel 3 trigger
    {
        exti_flag_clear(ERU_CH3_REQ6_P02_0);
        camera_vsync_handler();                     // Camera VSYNC unified handler
    }
    if (exti_flag_get(ERU_CH7_REQ16_P15_1))         // Channel 7 trigger
    {
        exti_flag_clear(ERU_CH7_REQ16_P15_1);
    }
}
// **************************** External Interrupts ****************************


// **************************** DMA Interrupts ****************************
IFX_INTERRUPT(dma_ch5_isr, 0, DMA_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    camera_dma_handler();                           // Camera DMA unified handler
}
// **************************** DMA Interrupts ****************************


// **************************** UART Interrupts ****************************
// UART0 defaults to debug port
IFX_INTERRUPT(uart0_tx_isr, 0, UART0_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
}

IFX_INTERRUPT(uart0_rx_isr, 0, UART0_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
#if DEBUG_UART_USE_INTERRUPT                        // When debug UART uses interrupt mode
    debug_interrupr_handler();                      // Process debug receive (data stored in the debug receive buffer)
#endif
}

// UART1 defaults to camera configuration
IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
}

IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    camera_uart_handler();                          // Camera UART unified handler
}

// UART2 defaults to serial plotter / wireless module
IFX_INTERRUPT(uart2_tx_isr, 0, UART2_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
}

IFX_INTERRUPT(uart2_rx_isr, 0, UART2_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    wireless_module_uart_handler();                 // Wireless module unified handler
}

// UART3 defaults to GPS positioning module
IFX_INTERRUPT(uart3_tx_isr, 0, UART3_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
}

IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    gnss_uart_callback();                           // GNSS UART callback
}

// UART Error Interrupts
IFX_INTERRUPT(uart0_er_isr, 0, UART0_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    IfxAsclin_Asc_isrError(&uart0_handle);
}
IFX_INTERRUPT(uart1_er_isr, 0, UART1_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    IfxAsclin_Asc_isrError(&uart1_handle);
}
IFX_INTERRUPT(uart2_er_isr, 0, UART2_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    IfxAsclin_Asc_isrError(&uart2_handle);
}
IFX_INTERRUPT(uart3_er_isr, 0, UART3_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // Enable nested interrupts
    IfxAsclin_Asc_isrError(&uart3_handle);
}