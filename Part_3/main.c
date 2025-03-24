#include <stdbool.h>
#include <stdio.h>

#include "incl/address_map.h"
#include "src/io_ports.c"
#include "src/ps2_port.c"


int main(void)
{
	// Если увидели в терминале эту строку значит функция main запустилась
	printf("Hello World\n");
	clear_hex_display();
	stio_led_g(0);
	
	uint8_t packet_ps2[4] = {0,0,0,0}; 	// Массив для хранения полученных байтов от ps/2 порта  (размером 4 т.к можно будет потом преобразовать к слову uint32_t )
	uint8_t ps2_data = 0;  				// Переменная для считывания данных из ps/2 порта
	int		count_bytes_from_ps2 = 0;	// Счетчик считанных байтов
	bool	get_0xaa_form_mouse = false;	// Флаг для проверки на переподключение мыши 

	int16_t 	x_val 	= 0;	// Переменная для хранения координаты по оси X
	int16_t 	y_val	= 0;	// Переменная для хранения координаты по оси Y

	// Инициализация компьютерной мыши ps/2 порта (процедура описана в части 2)
	while (ps2_mouse_init() != OK);
	
	while (1)
	{
		if (get_char_ps2(&ps2_data) == OK){
			
			// Обработка пакета
	
			packet_ps2[count_bytes_from_ps2] = ps2_data; // Сохраняем считанный байт в массив
			count_bytes_from_ps2++;						 // Увеличиваем счетчик считанных байтов
			
			// Если считаны все 3 байта пакета можно обрабатывать пакеты (первый байт - флаги, второй и третий - модули перемещения по осям X и Y)
			if (count_bytes_from_ps2 % 3 == 0){
				
				int8_t 	package_x_val;
				int8_t 	package_y_val;
				uint8_t	package_keys;

				// TODO: считываем значения кнопок из первого байта пакета (младшие 3 бита)

				// считываем второй байт пакета (как модуль перемещения по оси X) 
				package_x_val  = packet_ps2[1];
				
				// TODO: преобразуем модуль перемещения с учетом знака направления ()

				// считываем третий байт пакета (как модуль перемещения по оси Y) 
				package_y_val  = packet_ps2[2];

				// TODO: преобразуем модуль перемещения с учетом знака направления

				// TODO: изменяем значения координаты по осям X и Y в соответствии с полученными значениями
				
				// зажигаем зеленый светодиод в зависимости от нажатых кнопок
				stio_led_g(package_keys);

				// подготавливаем данные для вывода на 7-сегментный индикатор
				uint32_t	hex_val   =  x_val;
							hex_val	<<= 16;
							hex_val  |=  y_val & 0xffff;

				// выводим данные на 7-сегментный индикатор			
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

