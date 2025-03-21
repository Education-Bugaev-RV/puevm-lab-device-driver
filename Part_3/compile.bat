set GCC=%QUARTUS_ROOTDIR%/../nios2eds/bin/gnu/H-i686-mingw32/bin/nios2-elf-gcc.exe
set OBJC=%QUARTUS_ROOTDIR%/../nios2eds/bin/gnu/H-i686-mingw32/bin/nios2-elf-objcopy.exe


%GCC% -g -O1 -ffunction-sections -fverbose-asm -fno-inline -mhw-mul -mhw-div -IC:/intelFPGA_lite/13.1.0/quartus/../nios2eds/components/altera_nios2/HAL/inc -DSYSTEM_BUS_WIDTH=32 -DALT_SINGLE_THREADED -D_JTAG_UART_BASE=268439552 -Wl,--defsym -Wl,nasys_stack_top=0x8000000 -Wl,--defsym -Wl,nasys_program_mem=0x1000 -Wl,--defsym -Wl,nasys_data_mem=0x1000 -Wl,--section-start -Wl,.exceptions=0x20 -Wl,--section-start -Wl,.reset=0x0 -Wl,--script=C:/intelFPGA_lite/13.1.0/University_Program/Monitor_Program/build/nios_cpp_build.ld -o C:/intelFPGA_lite/Projecrts/Mouse_deiver/Part_1/exeption_handler.elf C:/intelFPGA_lite/Projecrts/Mouse_deiver/Part_1/asm/exeption_handler.s C:/intelFPGA_lite/Projecrts/Mouse_deiver/Part_1/main.c C:/intelFPGA_lite/13.1.0/University_Program/Monitor_Program/lib/jtag_uart.c 


%OBJC% -O srec C:/intelFPGA_lite/Projecrts/Mouse_deiver/Part_1/exeption_handler.elf C:/intelFPGA_lite/Projecrts/Mouse_deiver/Part_1/exeption_handler.srec 

