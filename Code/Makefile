# Project Name
PROJECT=uMouse

# DEVICE(RG,DISCOVERY)
BOARD=RG

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

#define STARTUP file & Linker script for specific board
ifeq ($(BOARD), RG)
STARTUP=$(DEVICE)/startup/TrueSTUDIO/startup_stm32f10x_xl.s
LDSCRIPT=stm32_flash.ld
FEATURES = -DSTM32F10X_XL
else ifeq ($(BOARD), DISCOVERY)
STARTUP=$(DEVICE)/startup/TrueSTUDIO/startup_stm32f10x_md_vl.s
LDSCRIPT=$(LIBROOT)/Project/STM32F10x_StdPeriph_Template/TrueSTUDIO/STM32100B-EVAL/stm32_flash.ld
FEATURES = -DSTM32F10X_MD_VL
endif


# Code Path
SRC:= $(CURDIR) $(CORE) $(DEVICE) $(PERIPH)/src
# Include Path
INC:= $(CURDIR) $(CORE) $(DEVICE) $(PERIPH)/inc

# C source files
CFILES  := $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
# Assembly source files
ASMFILES:= $(foreach dir, $(SRC), $(wildcard $(dir)/*.s)) $(STARTUP)

# Object filse
COBJ=$(patsubst %.c, %.o, $(CFILES))
SOBJ=$(patsubst %.s, %.o, $(ASMFILES))
OBJ=$(COBJ) $(SOBJ)

#FLAGS
GFLAGS  = -mthumb -mcpu=cortex-m3
LDFLAGS = -O2 -T$(LDSCRIPT) $(GFLAGS)
CFLAGS  = -O2 -g $(GFLAGS)
ASFLAGS = -g $(GFLAGS)
INCFLAGS= $(addprefix -I, $(INC))
FEATURES += -DUSE_STDPERIPH_DRIVER \

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

cleanall:
	rm -f $(OBJ) *.elf *.bin

clean:
	rm -f *o *.elf *.bin
