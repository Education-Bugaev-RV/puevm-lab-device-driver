#include "../incl/push_button.h"

void pushbutton_isr( void )
{
	volatile int * pushbutton_ptr       = (int *) PUSHBUTTON_BASE;
	volatile int * interval_timer_ptr   = (int *) INTERVAL_TIMER_BASE;
	volatile int * ps2_port_ptr         = (int *) PS_2_BASE;


	int press;	// Хранит битовое поле нажатых кнопок
		
	// Сбрасываем флаг факта вызова прерывания от кнопок
	press = *(pushbutton_ptr + 3);
	*(pushbutton_ptr + 3) = press; 		

	if (press & 0x2)	
	{
		// Обработка KEY1
		
		
	}
	if (press & 0x4)	
	{
		// Обработка KEY2 
		
	}
	if (press & 0x8)
	{
		// Обработка KEY3
		
	}
	return;
}