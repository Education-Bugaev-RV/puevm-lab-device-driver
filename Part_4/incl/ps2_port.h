#ifndef PS2_PORT
#define PS2_PORT

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../incl/nios2_ctrl_reg_macros.h"
#include "../incl/address_map.h"

// сопоставление кодов возврата функций
#define ERR (0)
#define OK (1)
#define RECONNECT_DETECTING (3)
#define OK_PACKAGE_COMPLETE (5)

typedef struct change_mouse_t
{
    int16_t 	x_val;
	int16_t 	y_val;
    uint8_t     keys;
    uint8_t     edge_capture;
};


uint8_t get_char_ps2(uint8_t* data);
void put_char_ps2(uint8_t data);
void clear_ps2();

// Отправляет в ps/2 порт команды инициализирующие состояние компьютерной мыши
uint8_t ps2_mouse_init();

// Инициализирует подключенную компьютерную мышь и разрешает прерывания от ps/2 порта
uint8_t ps2_mouse_init_driver();

// Функция обработки прерываний от ps/2 порта
void ps2_port_isr();

// функция обработки полученных данных в пакет
uint8_t ps2_port_parse_mouse_package();

// получите информацию о ПОСЛЕДНИХ изменениях положения и кнопок мыши
void get_mouse_change(struct change_mouse_t* package_change_mouse_ptr);

#endif