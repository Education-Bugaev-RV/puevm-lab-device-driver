echo off

set GCC=%QUARTUS_ROOTDIR%\..\nios2eds\bin\gnu\H-x86_64-mingw32\bin\nios2-elf-gcc.exe
set FLAGS=-std=c11 -g -O1 -ffunction-sections -fverbose-asm -fno-inline -mhw-mul -mhw-div
set HAL=..\compile\HAL\inc
SET LD=%QUARTUS_ROOTDIR%/../University_Program/Monitor_Program/build/nios_cpp_build.ld

SET OUTPUT_FILE_NAME=main.elf
SET JTAG_LIB=%QUARTUS_ROOTDIR%\..\University_Program\Monitor_Program\amp\lib\src\niosII_jtag_uart.c
SET SRC_FILES=%CD%\main.c %JTAG_LIB%

echo on

%GCC% %FLAGS% -I%HAL% -DSYSTEM_BUS_WIDTH=32 -DALT_SINGLE_THREADED -D_JTAG_UART_BASE=268439552 -Wl,--defsym -Wl,nasys_stack_top=0x8000000 -Wl,--defsym -Wl,nasys_program_mem=0x1000 -Wl,--defsym -Wl,nasys_data_mem=0x1000 -Wl,--section-start -Wl,.exceptions=0x20 -Wl,--section-start -Wl,.reset=0x0 -Wl,--script=%LD% -o main.elf %SRC_FILES%