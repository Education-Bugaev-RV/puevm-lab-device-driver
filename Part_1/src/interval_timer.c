#include "../incl/interval_timer.h"

void interval_timer_isr()
{
	volatile int * interval_timer_ptr = (int *) INTERVAL_TIMER_BASE;
	volatile int * ps2_port_ptr = (int *) PS_2_BASE;

	*(interval_timer_ptr) = 0;	// Сбрасываем флаг факта вызова прерывания от таймера
	
	return;
}

void init_interval_timer(uint32_t start_val){
	volatile int * interval_timer_ptr = (int *) INTERVAL_TIMER_BASE;
	*(interval_timer_ptr + 0x2) = (start_val & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (start_val >> 16) & 0xFFFF;
	return;
}