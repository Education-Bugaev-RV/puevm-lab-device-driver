#include <stdio.h>

#include "incl/address_map.h"
#include "src/io_ports.c"

#define ERR (0)
#define OK (1)


uint8_t get_char_ps2(uint8_t* data);
void put_char_ps2(uint8_t data);

int main(void)
{
	// Если увидели в терминале эту строку значит функция main запустилась
	printf("Hello World\n");
	clear_hex_display();

	volatile int * ps2_base_ptr = (int *) PUSHBUTTON_BASE;
	
	uint8_t packet_ps2[4]; // Массив для хранения полученных байтов от ps/2 порта  (размером 4 т.к можно будет потом преобразовать к слову uint32_t )
	uint8_t ps2_data = 0;  // Переменная для считывания данных из ps/2 порта

	put_char_ps2(0xf4);
	int count_bytes_from_ps2 = 0;

	// Нужно получить подтверждение
	while (ps2_data != 0xfa)
	{
		get_char_ps2(&ps2_data);
	}
	printf("The Mouse message is Enabled\n");
	
	while ( 1 )
	{
		if (get_char_ps2(&ps2_data) == OK){
			packet_ps2[count_bytes_from_ps2] = ps2_data;
			
			count_bytes_from_ps2++;

			if (count_bytes_from_ps2 % 3 == 0){
				load_bufer_to_hex_display( *((uint32_t*)packet_ps2));
				count_bytes_from_ps2 = 0;
			}
		}
	}
	return 0;
}

uint8_t get_char_ps2(uint8_t* data) {
    volatile uint16_t *ps_2_base = (uint16_t *)PS_2_BASE ;
    uint16_t data_reg = *ps_2_base;

    if (data_reg & 0x8000) { // Проверяем флаг RVALID
        *data = data_reg & 0xff;
        return OK;
    } else {
        return ERR; // Возвращаем EOF, если данных нет
    }
}

void put_char_ps2(uint8_t data) {
    volatile uint16_t *ps_2_base = (uint16_t *)PS_2_BASE ;
	// выделяем 8ми битное поле date  
    volatile uint8_t* ptr_data   = (uint8_t*) ps_2_base ;
	// отправляем переданное значение в ps/2 порт 
    *ptr_data = data;
}