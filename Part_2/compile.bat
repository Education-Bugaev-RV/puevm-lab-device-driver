@echo off
chcp 1251 > nul

REM Установка путей и переменных
if exist "%SOPC_KIT_NIOS2%\gnu\H-x86_64-mingw32\bin" (
    set GCC=%SOPC_KIT_NIOS2%\gnu\H-x86_64-mingw32\bin\nios2-elf-gcc.exe
) else if exist "%SOPC_KIT_NIOS2%\bin\gnu\H-i686-mingw32\bin" ( 
    set GCC=%SOPC_KIT_NIOS2%\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc.exe
) else (
    echo Ошибка: Не удалось найти компилятор Nios II. Проверьте установку Nios II EDS.
    exit /b 1
)
echo Компилятор Nios II найден: %GCC%

set FLAGS=-std=c11 -g -O1 -ffunction-sections -fverbose-asm -fno-inline -mhw-mul -mhw-div
set HAL=..\compile\HAL\inc
set LD=%QUARTUS_ROOTDIR%\..\University_Program\Monitor_Program\build\nios_cpp_build.ld

REM Настройка выходного файла и исходников
set OUTPUT_FILE_NAME=main.elf
set JTAG_LIB=..\compile\amp\lib\src\niosII_jtag_uart.c

set SRC_FILES=%CD%\main.c %JTAG_LIB%

REM Конфигурация памяти и секций
set SYSTEM_BUS_WIDTH=32
set ALT_SINGLE_THREADED=1
set JTAG_UART_BASE=268439552
set STACK_TOP=0x8000000
set PROGRAM_MEM=0x1000
set DATA_MEM=0x1000
set EXCEPTIONS_ADDR=0x20
set RESET_ADDR=0x0

REM Выполнение сборки
%GCC% %FLAGS% -I%HAL% ^
    -DSYSTEM_BUS_WIDTH=%SYSTEM_BUS_WIDTH% ^
    -DALT_SINGLE_THREADED=%ALT_SINGLE_THREADED% ^
    -D_JTAG_UART_BASE=%JTAG_UART_BASE% ^
    -Wl,--defsym -Wl,nasys_stack_top=%STACK_TOP% ^
    -Wl,--defsym -Wl,nasys_program_mem=%PROGRAM_MEM% ^
    -Wl,--defsym -Wl,nasys_data_mem=%DATA_MEM% ^
    -Wl,--section-start -Wl,.exceptions=%EXCEPTIONS_ADDR% ^
    -Wl,--section-start -Wl,.reset=%RESET_ADDR% ^
    -Wl,--script=%LD% ^
    -o %OUTPUT_FILE_NAME% %SRC_FILES%

REM Проверка результата компиляции
if %ERRORLEVEL% equ 0 (
    echo Компиляция завершена успешно.
) else (
    echo Ошибка компиляции. Код ошибки: %ERRORLEVEL%.
)

pause
exit /b %ERRORLEVEL%