#include <stdio.h>


#include "src/push_button.c"
#include "src/exceptions.c"

int main(void)
{

	volatile int * pushbutton_ptr = (int *) PUSHBUTTON_BASE;

	*(pushbutton_ptr + 2) = 0b1110; 	// Устанавливаем маску кнопок от которых будут обрабатываться прерывания	
	
	NIOS2_WRITE_IENABLE( 0b10000011 );	// Устанавливаем значение регистра ienable (определяет обработку отдельных внешних прерываний )	
	NIOS2_WRITE_STATUS( 1 );			// Устанавливаем значение в регистр status (0-бит если равет 1 разрешает принимать внешние прерывания процесоору )

	printf("Hello World\n");

	while ( 1 )
	{
		;
	}
	return 0;
}