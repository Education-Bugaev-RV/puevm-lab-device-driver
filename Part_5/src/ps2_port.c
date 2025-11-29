#include "../incl/ps2_port.h"

struct change_mouse_t global_change_mouse ={
    .x_val = 0,
    .y_val = 0,
    .keys  = 0,
    .edge_capture = 0
};

struct change_mouse_t package_change_mouse ={
    .x_val = 0,
    .y_val = 0,
    .keys  = 0,
    .edge_capture = 0
};

uint16_t x_val_max = 0;
uint16_t y_val_max = 0;

void check_mouse_position();

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

    // Ожидаем третий байт ответа 
    while (get_char_ps2(&ps2_data) != OK);
    // Если не равен 0x00 прекращаем проверку
    if (ps2_data != 0x00)
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

uint8_t ps2_mouse_init_driver(){
    // Выключаем прерывания от порта ps/2
    volatile int *ps2_port_ptr = (int *)PS_2_BASE;
    *(ps2_port_ptr + 1) = 0;

    uint8_t result = OK;

    // Совершаем 5 попыток инициализации подключенной мыши
    for (size_t i = 0; i < 5; i++)
    {
        result = ps2_mouse_init();
        if (result == OK){
            break;
        }
        
        // задержка для ожидания подключения
        for (size_t i = 0; i < 1000; i++); 
    }
    if (result != OK){
        return result;
    }

    // Разрешаем прерывания от порта ps/2
    *(ps2_port_ptr+1) = 1;
    
    return result;
}

uint8_t ps2_mouse_disable_driver(){
    
    // Выключаем прерывания от порта ps/2
    volatile int * ps2_port_ptr = (int *) PS_2_BASE;
    *(ps2_port_ptr+1) = 0;

    uint8_t ps2_data = 0;  // Переменная для считывания данных из ps/2 порта

    // отправляем команду запрета отправки пакетов сообщений
    put_char_ps2(0xf5);
    
    // Ожидаем байт ответа 
    while (get_char_ps2(&ps2_data) != OK);
    // Если не равен 0xfa прекращаем проверку
    if (ps2_data != 0xfa)
    {
        return ERR;
    }

    // Сюда дошли только если получили подтверждение на команду запрета отправки сообщений
    printf("The messages of mouse is Disabled\n");


    return OK;
}

uint8_t ps2_port_parse_mouse_package(struct change_mouse_t* package_change_mouse_ptr){
    static int		count_bytes_from_ps2    = 0;
	static bool	    get_0xaa_form_mouse     = false;
    static uint8_t  packet_ps2[3]           = {0,0,0}; 	// Массив для хранения полученных байтов от ps/2 порта
    
    uint8_t ps2_data = 0;  // Переменная для считывания данных из ps/2 порта
    uint8_t code_return = OK;

    if (get_char_ps2(&ps2_data) == OK){
            
        // Поверка на переподключение #2-стадия
        if(get_0xaa_form_mouse)
        {
            if (ps2_data == 0x00)
            {
                count_bytes_from_ps2 = 0;
                return RECONNECT_DETECTING;
            }
            if (ps2_data != 0xaa){
                get_0xaa_form_mouse = false;
            }
        }
        
        // Заполняем буфер принятых байтов
        packet_ps2[count_bytes_from_ps2] = ps2_data;
        count_bytes_from_ps2++;

        
        if (count_bytes_from_ps2 % 3 == 0){
            // Получили 3 байта - можно парсить пакет

            int8_t 	package_x_val;
            int8_t 	package_y_val;
            uint8_t	package_keys;
    
            package_keys = packet_ps2[0] & 0x7;
    
            // считываем второй байт пакета (как модуль перемещения по оси X) 
            package_x_val  = packet_ps2[1];
            // преобразуем модуль перемещения с учетом знака направления
            package_x_val  -= (packet_ps2[0] & 0x10) << 0x4;
    
            // считываем третий байт пакета (как модуль перемещения по оси Y) 
            package_y_val  = packet_ps2[2];
            // преобразуем модуль перемещения с учетом знака направления
            package_y_val  -= (packet_ps2[0] & 0x20) << 0x3;
    
            package_change_mouse_ptr->x_val 	        = package_x_val;
            package_change_mouse_ptr->y_val	            = package_y_val; 
            package_change_mouse_ptr->keys              = package_keys;
            package_change_mouse_ptr->edge_capture      = package_keys;
    
            count_bytes_from_ps2 = 0;
            code_return = OK_PACKAGE_COMPLETE;
        }
    
        // Поверка на переподключение #1-стадия
        if (ps2_data == 0xaa)
        {
            printf("The 0xaa was got\n");
            get_0xaa_form_mouse = true;
        }
    }
    return code_return;
}

void ps2_port_isr(){
    volatile int * ps2_port_ptr = (int *) PS_2_BASE;
    volatile uint8_t * IE = ((uint8_t*)ps2_port_ptr) + 5;
    uint8_t return_code_of_parsing;

    while ((*IE) & 1 == 1){
        // Обновляем структуру хранящую состояния мыши в пределах пакета
        return_code_of_parsing = ps2_port_parse_mouse_package(&package_change_mouse);
        
        if (return_code_of_parsing == RECONNECT_DETECTING)
        {
            ps2_mouse_init();
            continue;
        }

        if (return_code_of_parsing ==  OK_PACKAGE_COMPLETE)
        {
            // Обновляем структуру хранящую глобальное состояние мыши
            global_change_mouse.x_val           += package_change_mouse.x_val       ;
            global_change_mouse.y_val           += package_change_mouse.y_val       ;
            global_change_mouse.keys             = package_change_mouse.keys        ;
            global_change_mouse.edge_capture    |= package_change_mouse.edge_capture;

            check_mouse_position();
        }        
    }
}

void check_mouse_position(){
    if (global_change_mouse.x_val < 0)
    {
        global_change_mouse.x_val = 0;
    }
    else if (x_val_max != 0 && global_change_mouse.x_val > x_val_max)
    {
        global_change_mouse.x_val = x_val_max;
    }
    
    if (global_change_mouse.y_val < 0)
    {
        global_change_mouse.y_val = 0;
    }
    else if (y_val_max != 0 && global_change_mouse.y_val > y_val_max){
        global_change_mouse.y_val = y_val_max;
    }
    
}

void set_mouse_bounds(uint16_t x_max, uint16_t y_max){
    x_val_max = x_max;
    y_val_max = y_max;
}

void get_mouse_change(struct change_mouse_t *package_change_mouse_ptr)
{
    package_change_mouse_ptr->x_val          = package_change_mouse.x_val;
    package_change_mouse_ptr->y_val          = package_change_mouse.y_val;
    package_change_mouse_ptr->keys           = package_change_mouse.keys;
    package_change_mouse_ptr->edge_capture   = package_change_mouse.edge_capture;
}

void get_mouse_state(struct change_mouse_t* global_change_mouse_ptr)
{
    global_change_mouse_ptr->x_val          = global_change_mouse.x_val;
    global_change_mouse_ptr->y_val          = global_change_mouse.y_val;
    global_change_mouse_ptr->keys           = global_change_mouse.keys;
    global_change_mouse_ptr->edge_capture   = global_change_mouse.edge_capture;
}