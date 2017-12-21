# MPLAB IDE generated this makefile for use with GNU make.
# Project: Mesotreat.mcp
# Date: Mon Nov 27 15:48:43 2017

AS = pic32-as.exe
CC = pic32-gcc.exe
LD = pic32-ld.exe
AR = pic32-ar.exe
HX = pic32-bin2hex.exe
RM = rm

./Objects/Mesotreat.hex : ./Objects/Mesotreat.elf
	$(HX) ".\Objects\Mesotreat.elf"

./Objects/Mesotreat.elf : Objects/main.o Objects/Control_TLC5620.o Objects/Delay.o Objects/Tratamientos.o Objects/usb_descriptors.o Objects/usb_device.o Objects/usb_function_hid.o Objects/Comunicacion.o Objects/AD9834.o Objects/Control_TPIC.o Objects/Lectura_ADC.o Objects/Control_Volumen.o
	$(CC) -mdebugger -mprocessor=32MX564F128H "Objects\main.o" "Objects\Control_TLC5620.o" "Objects\Delay.o" "Objects\Tratamientos.o" "Objects\usb_descriptors.o" "Objects\usb_device.o" "Objects\usb_function_hid.o" "Objects\Comunicacion.o" "Objects\AD9834.o" "Objects\Control_TPIC.o" "Objects\Lectura_ADC.o" "Objects\Control_Volumen.o" -o".\Objects\Mesotreat.elf" -Wl,-L"C:\Program Files (x86)\Microchip\mplabc32\v2.02\pic32mx\lib",--script="app_32MX564F128H.ld",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=__ICD2RAM=1,-Map=".\Objects\Mesotreat.map"

Objects/main.o : main.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "main.c" -o".\Objects\main.o" -MMD -MF".\Objects\main.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/Control_TLC5620.o : Control_TLC5620.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "Control_TLC5620.c" -o".\Objects\Control_TLC5620.o" -MMD -MF".\Objects\Control_TLC5620.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/Delay.o : Delay.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "Delay.c" -o".\Objects\Delay.o" -MMD -MF".\Objects\Delay.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/Tratamientos.o : Tratamientos.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "Tratamientos.c" -o".\Objects\Tratamientos.o" -MMD -MF".\Objects\Tratamientos.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/usb_descriptors.o : usb_descriptors.c usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h
	$(CC) -mprocessor=32MX564F128H -x c -c "usb_descriptors.c" -o".\Objects\usb_descriptors.o" -MMD -MF".\Objects\usb_descriptors.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/usb_device.o : usb_device.c usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_device_local.h
	$(CC) -mprocessor=32MX564F128H -x c -c "usb_device.c" -o".\Objects\usb_device.o" -MMD -MF".\Objects\usb_device.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/usb_function_hid.o : usb_function_hid.c GenericTypeDefs.h Compiler.h usb.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h
	$(CC) -mprocessor=32MX564F128H -x c -c "usb_function_hid.c" -o".\Objects\usb_function_hid.o" -MMD -MF".\Objects\usb_function_hid.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/Comunicacion.o : Comunicacion.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "Comunicacion.c" -o".\Objects\Comunicacion.o" -MMD -MF".\Objects\Comunicacion.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/AD9834.o : AD9834.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "AD9834.c" -o".\Objects\AD9834.o" -MMD -MF".\Objects\AD9834.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/Control_TPIC.o : Control_TPIC.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "Control_TPIC.c" -o".\Objects\Control_TPIC.o" -MMD -MF".\Objects\Control_TPIC.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/Lectura_ADC.o : Lectura_ADC.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "Lectura_ADC.c" -o".\Objects\Lectura_ADC.o" -MMD -MF".\Objects\Lectura_ADC.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

Objects/Control_Volumen.o : Control_Volumen.c main.h iomap.h Control_TLC5620.h Comunicacion.h Delay.h Tratamientos.h usb.h GenericTypeDefs.h Compiler.h usb_config.h usb_common.h usb_ch9.h usb_device.h usb_hal.h usb_hal_pic32.h usb_function_hid.h Control_TPIC.h AD9834.h Lectura_ADC.h Control_Volumen.h
	$(CC) -mprocessor=32MX564F128H -x c -c "Control_Volumen.c" -o".\Objects\Control_Volumen.o" -MMD -MF".\Objects\Control_Volumen.d" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -mlong-calls -mno-float -Os

clean : 
	$(RM) "Objects\main.o" "Objects\Control_TLC5620.o" "Objects\Delay.o" "Objects\Tratamientos.o" "Objects\usb_descriptors.o" "Objects\usb_device.o" "Objects\usb_function_hid.o" "Objects\Comunicacion.o" "Objects\AD9834.o" "Objects\Control_TPIC.o" "Objects\Lectura_ADC.o" "Objects\Control_Volumen.o" ".\Objects\Mesotreat.elf" ".\Objects\Mesotreat.hex"

