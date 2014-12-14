# Tool path

TOOLROOT=./arm-none-eabi/bin

# Library path

LIBROOT=./STM32F10x_StdPeriph_Lib_V3.5.0

# Tools

CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as
OBJCOPY = $(TOOLROOT)/arm-none-eabi-objcopy

# Code Paths

DEVICE=$(LIBROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CORE=$(LIBROOT)/Libraries/CMSIS/CM3/CoreSupport
PERIPH=$(LIBROOT)/Libraries/STM32F10x_StdPeriph_Driver

#STARTUP file
STARTUP=

CFLAGS = -mcpu=cortex-m3 -mthumb

uMouse.bin: 
	

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d


clean:
	rm *.o
