#include <stdint.h>

#define PS_2_BASE 0x10000100
#define ERR (0)
#define OK (1)

uint8_t get_char_ps2(uint8_t* data);
uint8_t put_char_ps2(uint8_t data);
uint8_t get_cnt_byte_in_pocket(uint8_t id);

uint8_t parse_mouse_pocket( uint8_t* frist_byte, 
                            int8_t* x_val, 
                            int8_t* y_val, 
                            uint8_t* keys, 
                            uint8_t* overflow
                        );

uint8_t get_char_ps2(uint8_t* data) {
    volatile uint16_t *ps_2_base = (uint16_t *)PS_2_BASE ;
    uint16_t data_reg = *ps_2_base; // Считываем данные из JTAG UART

    if (data_reg & 0x8000) { // Проверяем флаг RVALID
        *data = data_reg & 0xff;
        return OK;
    } else {
        return ERR; // Возвращаем EOF, если данных нет
    }
}

uint8_t put_char_ps2(uint8_t data) {
    volatile uint16_t *ps_2_base = (uint16_t *)PS_2_BASE ;
    volatile uint8_t* ptr_data   = (uint8_t*) ps_2_base ;
    *ptr_data = data;
}

void clear_ps2() {
    uint8_t tmp;
    while (get_char_ps2(&tmp)){
        ;
    }   
}

uint8_t get_cnt_byte_in_pocket(uint8_t id){
    switch (id)
	{
	case 0x00: return 3;
		break;
	case 0x03 : return 4;
		break;
	case 0x04 : return 4;
		break;
	default: return 10;
		break;
	}
    return -1;
}

uint8_t parse_mouse_pocket(uint8_t* frist_byte, int8_t* x_val, int8_t* y_val, uint8_t* keys, uint8_t* overflow){
    if (keys != NULL)
    {
        *keys = (*frist_byte) & 0x7;
    }
    if (overflow != NULL)
    {
        *overflow  = ((*frist_byte) >> 6) & 0x3;
    }
    if (x_val != NULL)
    {
        *x_val  = *(frist_byte+1);
        *x_val  -= ((*frist_byte) & 0x10) << 0x4;     
    }
    if (y_val != NULL)
    {
        *y_val  = *(frist_byte+2);
        *y_val  -= ((*frist_byte) & 0x20) << 0x3;  
    }
    return OK;
}