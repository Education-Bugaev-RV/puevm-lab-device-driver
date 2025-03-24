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
	
	uint8_t packet_ps2[4]; 			// Массив для хранения полученных байтов от ps/2 порта  (размером 4 т.к можно будет потом преобразовать к слову uint32_t )
	uint8_t ps2_data = 0;  			// Переменная для считывания данных из ps/2 порта
	int count_bytes_from_ps2 = 0;	// Счетчик считанных байтов

	// TODO: включить отправку пакетов и убедится что мы получили 0xFA (я бы использовал цикл while)

	//-----------------------------------------------------------------------------------------------
	
	// Отладочное сообщение
	printf("The Mouse message is Enabled\n");
	
	// Непрерывное считывание содержимого регистра 
	while ( 1 )
	{
		if (/*Условие проверяющее валидность данных*/)
		{
			// Сохраняем полученный байт в массив
			
			// Увеличиваем счетчик считанных байтов

			// Если получили 3 байта, то выводим их на экран
			
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