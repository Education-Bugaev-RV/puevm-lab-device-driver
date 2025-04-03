#ifndef PUSH_BUTTON
#define PUSH_BUTTON

#include <stdint.h>
#include <stdlib.h>

#include "nios2_ctrl_reg_macros.h"
#include "address_map.h"
#include "io_ports.h"
#include "ps2_port.h"

// сопоставление кодов возврата функций
#define ERR (0)
#define OK (1)

void pushbutton_isr();

#endif