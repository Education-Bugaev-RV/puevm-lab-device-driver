#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "incl/address_map.h"
#include "src/io_ports.c"
#include "src/ps2_port.c"

#include "src/exceptions.c"

enum
{
	MASSAGE_ENABLE,
	MASSAGE_DISABLE
} mouse_state = MASSAGE_ENABLE;

int main(void)
{
	// Если увидели в терминале эту строку значит функция main запустилась
	printf("Hello World\n");
	clear_hex_display();
	stio_led_g(0);
	stio_led_r(0);

	while (ps2_mouse_init_driver() != OK);
	printf("Driver was initialized\n");

	set_mouse_bounds(319, 239);

	NIOS2_WRITE_IENABLE( 0b10000000 );	// Устанавливаем значение регистра ienable (определяет обработку отдельных внешних прерываний )	
	NIOS2_WRITE_STATUS( 1 );			// Устанавливаем значение в регистр status (0-бит если равен 1 разрешает принимать внешние прерывания процессору )

	struct change_mouse_t package_change_mouse;
	struct change_mouse_t global_change_mouse;

	uint32_t hex_ind_value = 0;			// Переменная для хранения значения для вывода на индикаторы
	uint32_t sw_io = 0;					// Переменная для получения состояния переключателей

	while (1)
	{
		// Получаем состояние переключателей
		ldio_switch(&sw_io);

		
		switch (sw_io)
		{
		// Основное состояние программы (получение данных и отображение перемещения мыши)
		case 0b0:
			// Вызов функции для получения глобального перемещения мыши
			get_mouse_state(&global_change_mouse);
			stio_led_g(global_change_mouse.keys);  			// Отображение состояния кнопок мыши
			stio_led_r(global_change_mouse.edge_capture);	// Отображение захваченное состояния кнопок мыши
			
			hex_ind_value   = global_change_mouse.x_val;
			hex_ind_value <<= 16;
			hex_ind_value  |= (global_change_mouse.y_val & 0xffff);
			load_bufer_to_hex_display(hex_ind_value);
			break;
		// TODO: Состояние включения драйвера
		case 0b1:
			
			break;
		// TODO: Состояние выключения драйвера
		case 0b10:
			
			break;
			
		default:
			printf("Unknown State!!!\n");
			break;
		}
	}
}

