#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h>
#include <stdlib.h>

#include "../incl/nios2_ctrl_reg_macros.h"
#include "../incl/address_map.h"

/* Прототипы функций */
void interrupt_handler(void);


void ps2_port_isr();

// Перечисление которое содержит идентификаторы случившихся прерываний
enum id_irqs
{
    IRQ_INTERVAL_TIMER	= 0x1,		// идентификатор прерывания от таймера
    IRQ_PUSH_BUTTON		= 0x2,		// идентификатор прерывания от кнопок
    IRQ_PS_2 			= 0x80,		// идентификатор прерывания от PS/2
    IRQ_JTAG			= 0x100		// идентификатор прерывания от JTAG
};

// Перечисление которое содержит распределение приоритетов обрабатываемых прерываний
// Приоритеты можно настроить именно в этом перечислении
enum priority_irqs 
{
    IRQ_LVL_1	= IRQ_INTERVAL_TIMER,	// Идентификатор прерывания с наивысшим приоритетом
    IRQ_LVL_2	= IRQ_PUSH_BUTTON,
    IRQ_LVL_3	= IRQ_PS_2,
    IRQ_LVL_4	= IRQ_JTAG
};

#endif