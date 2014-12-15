# Project Name
PROJECT=uMouse

# Tool PATH
TOOLROOT=./arm-none-eabi/bin

# Library PATH
LIBROOT=./STM32F10x_StdPeriph_Lib_V3.5.0

# Tools
CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AS=$(TOOLROOT)/arm-none-eabi-as
CP=$(TOOLROOT)/arm-none-eabi-objcopy

# Main folder
CORE  =$(LIBROOT)/Libraries/CMSIS/CM3/CoreSupport
DEVICE=$(LIBROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
PERIPH=$(LIBROOT)/Libraries/STM32F10x_StdPeriph_Driver

# Code Path
SRC:= $(CURDIR) $(CORE) $(DEVICE) $(PERIPH)/src
# Include Path
INC:= $(CURDIR) $(CORE) $(DEVICE) $(PERIPH)/inc

# C source files
CFILES  := $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
# Assembly source files
ASMFILES:= $(foreach dir, $(SRC), $(wildcard $(dir)/*.s))

# Object filse
COBJ=$(patsubst %.c, %.o, $(CFILES))
SOBJ=$(patsubst %.s, %.o, $(ASMFILES))
OBJ=$(COBJ) $(SOBJ)

#STARTUP file
STARTUP=$(DEVICE)/startup/TrueSTUDIO/startup_stm32f10x_xl.s

#Linker script
LDSCRIPT=$(LIBROOT)/Project/STM32F10x_StdPeriph_Examples/FLASH/Dual_Boot/TrueSTUDIO/STM32F10X_XL_BANK1/stm32f10x_flash_xl_bank1.ld

#FLAGS
LDFLAGS = -O2 -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3
INCFLAGS= $(addprefix -I, $(INC))
CFLAGS  = -O2 -g -mthumb -mcpu=cortex-m3
ASFLAGS = $(CFLAGS)

FEATURES = -DUSE_STDPERIPH_DRIVER \

# Create a raw binary file from the ELF version
$(PROJECT).bin: $(PROJECT).elf
	$(CP) -O binary $< $@

# Build executable 
$(PROJECT).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

# compile and generate dependency info
$(COBJ): %.o: %.c
	$(CC) -c $(FEATURES) $(INCFLAGS) $(CFLAGS) $< -o $@

$(SOBJ): %.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

all:
	$(PROJECT).bin

clean:
	rm -f $(OBJ) *.elf *.bin
