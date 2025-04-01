#ifndef VGA_H
#define VGA_H

#include "../incl/address_map.h"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

extern volatile int *reg_buffer;        
extern volatile int *reg_back_buffer;   
extern volatile int *reg_resolution;    
extern volatile int *reg_status;        
extern volatile short int *FRONT_FRAME; 
extern volatile short int *BACK_FRAME;

#define WRITE_VGA_BUFFER(x, y, color, ptr_vga_buff) ptr_vga_buff[((y) << 9) + (x)] = color
#define READ_VGA_BUFFER(x, y, ptr_vga_buff)         ptr_vga_buff[((y) << 9) + (x)]

void draw_rectangle(int x_start, int y_start, int width, int height, short int color, volatile short int *start_address);

#endif