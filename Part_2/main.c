#include <stdbool.h>
#include <stdio.h>

#include "incl/address_map.h"
#include "src/io_ports.c"

#define ERR (0)
#define OK (1)


uint8_t get_char_ps2(uint8_t* data);
void put_char_ps2(uint8_t data);
void clear_ps2();

enum{
	START,	// Состояние при первом подключении миши (требуется перезагрузить и разрешить отправку пакетов)
	READY,	// Рабочее состояние (Считываются байты и выводятся на hex-индикаторы)
	GET_AA	// Обнаружен байт AA если следующий байт будет 00, то значит произошло переподключение мыши
}mouse_state = START;


int main(void)
{
	// Если увидели в терминале эту строку значит функция main запустилась
	printf("Hello World\n");
	clear_hex_display();
	
	uint8_t packet_ps2[4] = {0,0,0,0}; 	// Массив для хранения полученных байтов от ps/2 порта  (размером 4 т.к можно будет потом преобразовать к слову uint32_t )
	uint8_t ps2_data = 0;  				// Переменная для считывания данных из ps/2 порта
	int count_bytes_from_ps2 = 0;		// Счетчик считанных байтов

	while (1)
	{
		switch (mouse_state)
		{
		case START:{
			count_bytes_from_ps2 = 0;
			
			// Чистим буфер от предыдущих данных
				
			// Отправляем команду сброса


			// Ожидаем первый байт ответа 

			// Если не равен 0xfa прекращаем проверку

			
			// Ожидаем второй байт ответа 

			// Если не равен 0xaa прекращаем проверку
			


			// Сюда дошли только если получили корректный ответ на сброс
			printf("Mouse has been reseting\n"); // Отладочное сообщение
			
			// Разрешаем отправку пакетов от мыши
			put_char_ps2(0xf4);

			// Ожидаем байт ответа 

			// Если не равен 0xfa прекращаем проверку


			// Сюда дошли только если получили корректный ответ на сброс и успешно разрешили отправку сообщений от мыши
			printf("The messages of mouse is Enabled\n"); // Отладочное сообщение
			
			// Меняем состояние на соответствующее

			break;
		}
		case READY:{
			
			// Условие проверяющее валидность данных
			if (get_char_ps2(&ps2_data) == OK){
				packet_ps2[count_bytes_from_ps2] = ps2_data;	// Сохраняем полученный байт в массив
				count_bytes_from_ps2++; 					    // Увеличиваем счетчик считанных байтов
				
				// Если получили 3 байта, то выводим их на экран
				if (count_bytes_from_ps2 % 3 == 0){
					load_bufer_to_hex_display( *((uint32_t*)packet_ps2));
					count_bytes_from_ps2 = 0;
				}

				// Проверка на переподключение мыши (первый байт последовательности 0xaa00)

				// Меняем состояние на соответствующее
			}
			break;
		}	
		case GET_AA:{
			// Условие проверяющее валидность данных
			if (get_char_ps2(&ps2_data) == OK){
				packet_ps2[count_bytes_from_ps2] = ps2_data; // Сохраняем полученный байт в массив
				count_bytes_from_ps2++;						 // Увеличиваем счетчик считанных байтов

				// Если получили 3 байта, то выводим их на экран
				if (count_bytes_from_ps2 % 3 == 0){
					load_bufer_to_hex_display( *((uint32_t*)packet_ps2));
					count_bytes_from_ps2 = 0;
				}

				// Меняем состояние на соответствующее
				
			}
			break;
		}	
		
		default:{
			printf("Unknown state!\n");
			mouse_state = START;
			break;
		}
		}
	}
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

void clear_ps2() {
    uint8_t tmp;
    while (get_char_ps2(&tmp) == OK){
        ;
    }   
}