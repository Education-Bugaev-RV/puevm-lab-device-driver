#include "../incl/ps2_port.h"


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

// Функция инициализации компьютерной мыши ps/2 порта из части 2
uint8_t ps2_mouse_init(){
    uint8_t ps2_data = 0;  // Переменная для считывания данных из ps/2 порта
    
    clear_ps2(); 				// отчистка чтобы предыдущие данные не повлияли на интерпретацию ответа на следующую команду
    put_char_ps2(0xff);

    // Ожидаем первый байт ответа 
    while (get_char_ps2(&ps2_data) != OK);
    // Если не равен 0xfa прекращаем проверку
    if (ps2_data != 0xfa)
    {
        return ERR;
    }
    
    // Ожидаем второй байт ответа 
    while (get_char_ps2(&ps2_data) != OK);
    // Если не равен 0xaa прекращаем проверку
    if (ps2_data != 0xaa)
    {
        return ERR;
    }

    // Сюда дошли только если получили корректный ответ на сброс
    printf("Mouse has been reseting\n");
    put_char_ps2(0xf4);

    // Ожидаем байт ответа 
    while (get_char_ps2(&ps2_data) != OK);
    // Если не равен 0xfa прекращаем проверку
    if (ps2_data != 0xfa)
    {
        return ERR;
    }

    // Сюда дошли только если получили корректный ответ на сброс и успешно разрешили отправку сообщений от мыши
    printf("The messages of mouse is Enabled\n");
    return OK;
}