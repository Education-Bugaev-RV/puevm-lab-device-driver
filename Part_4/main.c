#include <stdbool.h>
#include <stdio.h>

#include "incl/address_map.h"
#include "src/io_ports.c"
#include "src/ps2_port.c"

#include "src/exceptions.c"


int main(void)
{
	// Если увидели в терминале эту строку значит функция main запустилась
	printf("Hello World\n");
	clear_hex_display();
	stio_led_g(0);

	while (ps2_mouse_init_driver() != OK);
	printf("Driver was initialized\n");


	NIOS2_WRITE_IENABLE( 0b10000000 );	// Устанавливаем значение регистра ienable (определяет обработку отдельных внешних прерываний )	
	NIOS2_WRITE_STATUS( 1 );			// Устанавливаем значение в регистр status (0-бит если равен 1 разрешает принимать внешние прерывания процессору )

	struct change_mouse_t package_change_mouse;

	uint32_t hex_ind_value = 0;

	while (1)
	{
		get_mouse_change(&package_change_mouse);
		stio_led_g(package_change_mouse.keys);
							
		hex_ind_value   = package_change_mouse.x_val;
		hex_ind_value <<= 16;
		hex_ind_value  |= (package_change_mouse.y_val & 0xffff);
		load_bufer_to_hex_display(hex_ind_value);
	}
}

