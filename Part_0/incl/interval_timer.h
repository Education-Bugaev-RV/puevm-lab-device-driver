#ifndef INTERVAL_TIMER
#define INTERVAL_TIMER

#include <stdint.h>
#include <stdlib.h>

#include "../incl/nios2_ctrl_reg_macros.h"
#include "../incl/address_map.h"

void interval_timer_isr();
void init_interval_timer(uint32_t start_val);

#endif