#ifndef IO_PORTS
#define IO_PORTS

#include <stdint.h>
#include <stdlib.h>

#include "../incl/nios2_ctrl_reg_macros.h"
#include "../incl/address_map.h"


void load_bufer_to_hex_display( int buffer );
void clear_hex_display();
void update_hex_display();

void stio_led_r(uint32_t data);
void stio_led_g(uint8_t data);
void ldio_switch(uint32_t *data);

//  массив байтов, которые загрузятся по адресу 7-ых индикаторов при вызове функции update_hex_display()
extern char	hex_segments[8];

// Пока что декодирует только 16-ые цифры 0-F (можно добавить требуемые символы в конец массива)
extern char	seven_seg_decode_table[16];

#endif