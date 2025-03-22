#include <stdbool.h>
#include <stdio.h>

#include "incl/address_map.h"
#include "src/io_ports.c"
#include "src/ps2_port.c"


enum{
	START,	// Состояние при первом подключении миши (требуется перезагрузить и разрешить отправку пакетов)
	READY,	// Рабочее состояние (Считываются байты и выводятся на hex-индикаторы)
	GET_AA	// Обнаружен байт AA если следующий байт будет 00, то значит произошло переподключение мыши
}mouse_state = START;


int main(void)
{
	// Если увидели в терминале эту строку значит функция main запустилась
	printf("Hello World\n");
	clear_hex_display();
	stio_led_g(0);
	
	uint8_t packet_ps2[4] = {0,0,0,0}; 	// Массив для хранения полученных байтов от ps/2 порта  (размером 4 т.к можно будет потом преобразовать к слову uint32_t )
	uint8_t ps2_data = 0;  				// Переменная для считывания данных из ps/2 порта
	int		count_bytes_from_ps2 = 0;
	bool	get_0xaa_form_mouse = false; 

	int16_t 	x_val 	= 0;
	int16_t 	y_val	= 0;

	while (ps2_mouse_init() != OK);
	
	while (1)
	{
		if (get_char_ps2(&ps2_data) == OK){
			
			// Обработка пакета
	
			packet_ps2[count_bytes_from_ps2] = ps2_data;
			count_bytes_from_ps2++;
			if (count_bytes_from_ps2 % 3 == 0){
				
				int8_t 	package_x_val;
				int8_t 	package_y_val;
				uint8_t	package_keys;

				package_keys = packet_ps2[0] & 0x7;

				// считываем второй байт пакета (как модуль перемещения по оси X) 
				package_x_val  = packet_ps2[1];
				// преобразуем модуль перемещения с учетом знака направления
				package_x_val  -= (packet_ps2[0] & 0x10) << 0x4;

				// считываем третий байт пакета (как модуль перемещения по оси Y) 
				package_y_val  = packet_ps2[2];
				// преобразуем модуль перемещения с учетом знака направления
				package_y_val  -= (packet_ps2[0] & 0x20) << 0x3;

				x_val 	+= package_x_val;
				y_val	+= package_y_val; 
				
				stio_led_g(package_keys);
				uint32_t	hex_val   =  x_val;
							hex_val	<<= 16;
							hex_val  |=  y_val & 0xffff;

				load_bufer_to_hex_display(hex_val);

				count_bytes_from_ps2 = 0;
			}

			// Поверка на переподключение
			if(get_0xaa_form_mouse)
			{
				if (ps2_data == 0x00)
				{
					count_bytes_from_ps2 = 0;
					while (ps2_mouse_init() != OK);
				}
				if (ps2_data != 0xaa){
					get_0xaa_form_mouse = false;
				}
			}
			if (ps2_data == 0xaa)
			{
				printf("The 0xaa was got\n");
				get_0xaa_form_mouse = true;
			}
		}
	}
}

