# Project Name
PROJECT=uMouse

# DEVICE(RG,DISCOVERY)
BOARD=DISCOVERY

# Tool PATH
TOOLROOT=./arm-none-eabi/bin

# Library PATH
LIBROOT =./STM32F10x_StdPeriph_Lib_V3.5.0

# Object PATH
OBJROOT =./build

# Code PATH
SRCROOT =./src

# Tools
CC =$(TOOLROOT)/arm-none-eabi-gcc
LD =$(TOOLROOT)/arm-none-eabi-gcc
AS =$(TOOLROOT)/arm-none-eabi-as
CP =$(TOOLROOT)/arm-none-eabi-objcopy
OD =$(TOOLROOT)/arm-none-eabi-objdump
GDB=$(TOOLROOT)/arm-none-eabi-gdb

# Main folder
CORE   =$(LIBROOT)/Libraries/CMSIS/CM3/CoreSupport
DEVICE =$(LIBROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
PERIPH =$(LIBROOT)/Libraries/STM32F10x_StdPeriph_Driver
STARTUP=$(DEVICE)/startup/gcc_ride7

# Code search path
vpath %.c $(SRCROOT)
vpath %.c $(CORE)
vpath %.c $(DEVICE)
vpath %.c $(PERIPH)/src

vpath %.s $(STARTUP)

vpath %.o $(OBJROOT)

# Include Path
INC := $(SRCROOT) $(CORE) $(DEVICE) $(PERIPH)/inc

# The project file
SRC	 = main.c mcu_lib.c
# used parts of the STM-Library
SRC	+= core_cm3.c
SRC += system_stm32f10x.c
SRC	+= stm32f10x_adc.c
SRC	+= stm32f10x_gpio.c
SRC += stm32f10x_i2c.c
SRC	+= stm32f10x_rcc.c
SRC += stm32f10x_usart.c

# define STARTUP file & Linker script for specific board
ifeq ($(BOARD), RG)
SRC += startup_stm32f10x_xl.s
LDSCRIPT=stm32_flash.ld
FEATURES = -DSTM32F10X_XL
else ifeq ($(BOARD), DISCOVERY)
SRC += startup_stm32f10x_md_vl.s
LDSCRIPT=$(LIBROOT)/Project/STM32F10x_StdPeriph_Template/TrueSTUDIO/STM32100B-EVAL/stm32_flash.ld
FEATURES = -DSTM32F10X_MD_VL
endif

# the object file to be compiled
OBJ  = $(notdir $(SRC:.s=.o) $(SRC:.c=.o))

#FLAGS
GFLAGS  = -mthumb -mcpu=cortex-m3
OPT		= -Os

LDFLAGS = $(OPT) $(GFLAGS) -T$(LDSCRIPT) -nostartfiles
CFLAGS  = $(OPT) $(GFLAGS) -g -Wall
ASFLAGS = $(GFLAGS) -g

INCFLAGS  = $(addprefix -I, $(INC))
FEATURES += -DUSE_STDPERIPH_DRIVER

# Create a raw binary file from the ELF version
$(PROJECT).bin: $(PROJECT).elf
	$(CP) -O binary $< $@

# Build executable 
$(PROJECT).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

# compile and generate dependency info
$(OBJROOT)%.o: %.c
	$(CC) -c $(FEATURES) $(INCFLAGS) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%.d: %.c
	@set -e; rm -f $@; echo $@; \
	$(CC) -MM $(FEATURES) $(INCFLAGS) $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

all:
	$(PROJECT).bin

clean:
	rm -f *.o *.d *.elf *.bin

debug:
	$(GDB) $(PROJECT).elf

-include $(addprefix $(OBJROOT)/, $(SRC:.c=.d))
