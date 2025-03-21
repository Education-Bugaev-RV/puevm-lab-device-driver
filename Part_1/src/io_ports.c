#include "../incl/io_ports.h"


		//  массив байтов, которые загрузятся по адресу 7-ых индикаторов при вызове функции update_hex_display()
char	hex_segments[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		// Пока что декодирует только 16-ые цифры 0-F (можно добавить треубемые символы в конец массива)
char	seven_seg_decode_table[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};


/*******************************************************************************
 * преобразует значение из буфера (buffer) и отображает 
 * его на 7-ых индикаторах как 16-ое число
********************************************************************************/
void load_bufer_to_hex_display( int buffer )
{
	
	int shift_buffer, nibble;
	char code;
	int i;
	
	shift_buffer = buffer;
	for (i = 0; i < 8; ++i )
	{
		nibble = shift_buffer & 0xF;		// выделяем цифру из отображаемого числа
		code = seven_seg_decode_table[nibble];
		hex_segments[i] = code;
		shift_buffer = shift_buffer >> 4;
	}
	update_hex_display();
	return;
}

void clear_hex_display()
{
	int i;
	for (i = 0; i < 8; ++i )
	{
		hex_segments[i] = 0x00;
	}
	update_hex_display();
	return;
}

/*******************************************************************************
 * зажигает 7-ые индикаторы согласно 
 * значениям в массиве hex_segments
********************************************************************************/
void update_hex_display(){
	volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
	volatile int * HEX7_HEX4_ptr = (int *) HEX7_HEX4_BASE;
	*(HEX3_HEX0_ptr) = *(int *) hex_segments; 		// drive the hex displays
	*(HEX7_HEX4_ptr) = *(int *) (hex_segments+4);	// drive the hex displays
	return;
}


void stio_led_r(uint32_t data){
	volatile int * led_r = (uint32_t *) RED_LED_BASE;
	*led_r = data;
	return;
}

void stio_led_g(uint8_t data){
	volatile int * led_g = (uint8_t *) RED_LED_BASE + 0x10;
	*led_g = data;
	 return;
}

void ldio_swith(uint32_t *data){
	volatile int * sw = (uint8_t *) SLIDER_SWITCH_BASE;
	*data = *sw ;
	 return;
}