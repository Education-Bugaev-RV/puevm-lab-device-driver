#include <stdint.h>

uint8_t get_char(uint8_t* data);
uint8_t put_char(uint8_t data);
void clear_jtag_uart();
uint8_t get_string(uint8_t* string, uint8_t size, uint8_t echo);
void put_string(uint8_t* string, uint8_t new_line);

#define JTAG_UART_BASE 0x10001000 /* базовый адрес JTAG UART */
#define ERR (0)
#define OK (1)

uint8_t get_char(uint8_t* data) {
    volatile uint16_t *jtag_uart = (uint16_t *)JTAG_UART_BASE ;
    uint16_t data_reg = *jtag_uart; // Считываем данные из JTAG UART

    if (data_reg & 0x8000) { // Проверяем флаг RVALID
        *data = data_reg & 0xff;
        return OK;
    } else {
        return ERR; // Возвращаем EOF, если данных нет
    }
}


uint8_t put_char(uint8_t data) {
    volatile uint16_t *jtag_uart = (uint16_t *)JTAG_UART_BASE ;
    volatile uint16_t *wspase = (uint16_t *)(JTAG_UART_BASE + 0x6) ;
    volatile uint8_t* ptr_data   =  (uint8_t*) jtag_uart ;

    // Получаем количесво свободных байт для отправки и выводим на led_g загруженность 
    //uint16_t wspase     = *(jtag_uart + 3);

    // Отправляем данные если в буфере есть место
    if (*wspase > 0 ) {
        *ptr_data = data;
        return OK; // Возвращаем считанный байт
    } else {
        int i;
        return ERR;
    }
}


void clear_jtag_uart() {
    uint8_t tmp;
    while (get_char(&tmp)){
        ;
    }   
}


uint8_t get_string(uint8_t* string, uint8_t size, uint8_t echo) {
    
    uint8_t data = 0;
    uint8_t count_byte = 0;
    uint8_t ret = OK;

    while (data != '\n' && count_byte < size)
    {
        if(get_char(&data)){
            
            string[count_byte] = data;
            if(data>31){
                count_byte++;
            }
            // Не сохраняем управляющие символы в массив данных
            else{
                count_byte--;
            }
            
            // Отправляем сивол обратно если включен эхо режим
            if (echo)       
            {
                put_char(data);
            }
            
        }
    }
    if (data != '\n')
    {
        // Выделенное кол-во байт под строку закончилось но ввод не завершился !!!
        // можно вывести строку о том что последний симол не был сохранен
        ret = ERR;
    }
    string[count_byte+1] = '\0';
    return ret;       
}


void put_string(uint8_t* string, uint8_t new_line) {
    while (*string != '\0')
    {
        if (put_char(*string)){
            string += 1;
        }
    }
    if (new_line)
    {
        while (!put_char('\n')){
            ;
        }   
    }
}