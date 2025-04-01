#ifndef VGA_C
#define VGA_C

#include "../incl/vga.h"

volatile int *reg_buffer        = (int *)PIXEL_BUF_CTRL_BASE;
volatile int *reg_back_buffer   = (int *)(PIXEL_BUF_CTRL_BASE + 4);
volatile int *reg_resolution    = (int *)(PIXEL_BUF_CTRL_BASE + 8);
volatile int *reg_status        = (int *)(PIXEL_BUF_CTRL_BASE + 12);
volatile short int *FRONT_FRAME = (short int *)FPGA_PIXEL_BUF_BASE;
volatile short int *BACK_FRAME  = (short int *)0x0803BE7E;

void draw_rectangle(int x_start, int y_start, int width, int height, short int color, volatile short int *start_address)
{ // функция, закрашивающая экран заданным цветом
    short int *ptr = start_address;
    for (int y = y_start; y < y_start + height; ++y)
    {
        for (int x = x_start; x < x_start + width; ++x)
        {
            WRITE_VGA_BUFFER(x, y, color, ptr);
            //ptr[(y << 9) + x] = color;
        }
    }
}

#endif