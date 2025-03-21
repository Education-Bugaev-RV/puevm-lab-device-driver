#ifndef PS2_PORT
#define PS2_PORT

#include <stdint.h>
#include <stdlib.h>

#include "../incl/nios2_ctrl_reg_macros.h"
#include "../incl/address_map.h"

#define ERR (0)
#define OK (1)


uint8_t get_char_ps2(uint8_t* data);
void put_char_ps2(uint8_t data);
void clear_ps2();

uint8_t ps2_mouse_init();


#endif