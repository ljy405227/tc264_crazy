#include "zf_common_headfile.h"
#include "stdio.h"

/****** VOFA+ Oscilloscope Protocol ******
 *
 * Usage: VOFA_Send(data);     // Call once per data point, supports unlimited channels
 *        VOFA_draw();         // Trigger waveform display on the host PC
 *
 * Place both calls inside the main loop.
 ******/

uint8 t_test = 0;
uint8 send_date[4] = {0};
uint8 byte[4] = {0};

// Convert float to 4 bytes (little-endian)
void Float_to_Byte(float f, unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata = f;
    byte[0] = (unsigned char)fl.ldata;
    byte[1] = (unsigned char)(fl.ldata >> 8);
    byte[2] = (unsigned char)(fl.ldata >> 16);
    byte[3] = (unsigned char)(fl.ldata >> 24);
}

// Convert 4 bytes back to float
void Byte_to_Float(float *f, unsigned char byte[])
{
    FloatLongType fl;
    fl.ldata = 0;
    fl.ldata = byte[3];
    fl.ldata = (fl.ldata << 8) | byte[2];
    fl.ldata = (fl.ldata << 8) | byte[1];
    fl.ldata = (fl.ldata << 8) | byte[0];
    *f = fl.fdata;
}

// VOFA+ send data
void VOFA_Send(float t)
{
    Float_to_Byte(t * 1.0, byte);

    for (t_test = 0; t_test < 4; t_test++)
    {
        uart_write_byte(UART_2, byte[t_test]);
    }
}

// VOFA+ trigger draw
void VOFA_draw(void)
{
    // Curve drawing protocol suffix
    send_date[0] = 0x00;
    send_date[1] = 0x00;
    send_date[2] = 0x80;
    send_date[3] = 0x7f;

    for (t_test = 0; t_test < 4; t_test++)
    {
        uart_write_byte(UART_2, send_date[t_test]);
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