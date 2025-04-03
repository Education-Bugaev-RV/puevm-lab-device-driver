#include "../incl/push_button.h"

void pushbutton_isr(void)
{
	volatile int *pushbutton_ptr = (int *)PUSHBUTTON_BASE;
	volatile int *interval_timer_ptr = (int *)INTERVAL_TIMER_BASE;
	volatile int *ps2_port_ptr = (int *)PS_2_BASE;

	int press; // Хранит битовое поле нажатых кнопок

	// Сбрасываем флаг факта вызова прерывания от кнопок
	press = *(pushbutton_ptr + 3);
	*(pushbutton_ptr + 3) = press;

	uint32_t sw_io = 0;
	ldio_switch(&sw_io);

	if (press & 0x2)
	{

		sw_io &= 0x1;
		if (sw_io == 0)
		{
			while (ps2_mouse_disable_driver() != OK)
				;
			printf("Driver was disabled\n");
		}
		else if (sw_io == 1)
		{
			while (ps2_mouse_init_driver() != OK)
				;
			printf("Driver was initialized\n");
		}
	}
	if (press & 0x4)
	{
		// Обработка KEY2
		sw_io &= 0x1;
		ps2_mouse_set_visible((bool)sw_io);
		printf("Mouse is %s\n", (bool)sw_io ? "visible" : "invisible");
	}
	if (press & 0x8)
	{
		// Обработка KEY3
		sw_io &= 0x3;
		invert_mouse_y = (bool)(sw_io & 0x1);
		invert_mouse_x = (bool)((sw_io >> 1) & 0x1);
		printf("Invert mouse x: %s, y: %s\n", invert_mouse_x ? "true" : "false", invert_mouse_y ? "true" : "false");
	}
	return;
}