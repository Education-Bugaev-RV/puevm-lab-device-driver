#include "../incl/exceptions.h"

// функция interrupt_handler вызывается из 
// первичного обработчика прерываний, 
// который реализован в файле exeption_handler.s на ассемблере. 
// Поэтому exeption_handler.s нужно также включить в проект

/*****************************************************************************
 * Основной обработчик прерываний Си                                                 
 *  Определяет какое прерывание произошло          
 *  и вызывает соответствующи обработчик.                                                              
 *                                                                           
 * ipending - регистр управления №4, который содержит битовое поле случившихся внешних прерываний
 *****************************************************************************/
void interrupt_handler(void)
{
	int ipending;
	NIOS2_READ_IPENDING(ipending);
	// Проверка случившихся прерываний согласно установленным приоритетам в перечислении priority_irqs
	if ( ipending & IRQ_LVL_1 )				// interval timer is interrupt level 1
	{
	#ifdef INTERVAL_TIMER
		interval_timer_isr();
	#endif
	}
	if ( ipending & IRQ_LVL_2 )				// pushbuttons are interrupt level 2
	{
	#ifdef PUSH_BUTTON
		pushbutton_isr();
	#else
		printf("PushButton ISR is not included\n");
	#endif
	
	}
	if ( ipending & IRQ_LVL_3 )			
	{
	#ifdef PS2_PORT
		ps2_port_isr();
	#endif
	}
	if ( ipending & IRQ_LVL_4 )			
	{
		;
	}
	return;
}