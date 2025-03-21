#include "../incl/ps2_port.h"

void ps2_port_isr(){

	uint8_t ps2_data;

	uint8_t keys = 0;		// Переменная для битового поля нажатых нопок в пакете мышки ps/2
	int8_t delta_x = 0;		// Переменная для знакового значения счетчика перемещения по оси X в пакете мышки ps/2
	int8_t	delta_y = 0;	// Переменная для знакового значения счетчика перемещения по оси Y в пакете мышки ps/2

	while (get_char_ps2(&ps2_data))  // Считываем данные из буфера пока он не станет пустым
	{
		int index = count_of_byte;
		if (!mode_of_print_log)
			index %= byte_in_pocket;
		
		arr_data_from_ps2[index] = ps2_data;
		count_of_byte++;
		
		if(count_of_byte % byte_in_pocket == 0){
			// пакет закончился можно его разбирать на поля
			uint8_t* ptr_to_begin = arr_data_from_ps2+index+1-byte_in_pocket;
			parse_mouse_pocket(ptr_to_begin, &delta_x, &delta_y, &keys, NULL);
			stio_led_g(keys);
			abs_x += delta_x;
			abs_y += delta_y;
			load_bufer_to_hex_display((abs_x << 16) | (abs_y & 0xffff));
		}
	}
	return;
}