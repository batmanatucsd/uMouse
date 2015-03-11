# Project Name
PROJECT=uMouse

# DEVICE(MOUSE,DISCOVERY)
BOARD  =DISCOVERY

# Serial Debug(DEBUG)
SERIAL =DEBUG

# Tool PATH
TOOLDIR=./arm-none-eabi/bin

# Library PATH
LIBDIR =./STM32F10x_StdPeriph_Lib_V3.5.0

# Object PATH
OBJDIR =./build

# Code PATH
SRCDIR =./src

# Tools
CC =$(TOOLDIR)/arm-none-eabi-gcc
LD =$(TOOLDIR)/arm-none-eabi-gcc
AS =$(TOOLDIR)/arm-none-eabi-as
CP =$(TOOLDIR)/arm-none-eabi-objcopy

# Main folder
CORE   =$(LIBDIR)/Libraries/CMSIS/CM3/CoreSupport
DEVICE =$(LIBDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
PERIPH =$(LIBDIR)/Libraries/STM32F10x_StdPeriph_Driver

# define STARTUP file & Linker script for specific board
ifeq ($(BOARD), MOUSE)
STARTUP =$(DEVICE)/startup/gcc_ride7/startup_stm32f10x_xl.s
LDSCRIPT=$(SRCDIR)/stm32_flash.ld
FEATURES=-DSTM32F10X_XL
else ifeq ($(BOARD), DISCOVERY)
STARTUP =$(DEVICE)/startup/gcc_ride7/startup_stm32f10x_md_vl.s
LDSCRIPT=$(LIBDIR)/Project/STM32F10x_StdPeriph_Template/TrueSTUDIO/STM32100B-EVAL/stm32_flash.ld
FEATURES=-DSTM32F10X_MD_VL
endif

# Code search path
vpath %.c $(SRCDIR)
vpath %.c $(CORE)
vpath %.c $(DEVICE)
vpath %.c $(PERIPH)/src

vpath %.o $(OBJDIR)

# Include Path
INC := $(SRCDIR) $(CORE) $(DEVICE) $(PERIPH)/inc

# The project file
SRC	 = $(notdir $(wildcard $(SRCDIR)/*.c))
# used parts of the STM-Library
SRC	+= core_cm3.c
SRC += system_stm32f10x.c
SRC += misc.c
SRC	+= stm32f10x_adc.c
SRC += stm32f10x_dma.c
SRC += stm32f10x_exti.c
SRC	+= stm32f10x_gpio.c
SRC += stm32f10x_i2c.c
SRC	+= stm32f10x_rcc.c
SRC += stm32f10x_tim.c

ifeq ($(SERIAL), DEBUG)
SRC += stm32f10x_usart.c
FEATURES+=-DSERIAL_DEBUG
endif

# the object file to be compiled
OBJ := $(notdir $(STARTUP:.s=.o)) $(SRC:.c=.o)

#FLAGS
GFLAGS  = -mthumb -mcpu=cortex-m3
OPT		= -Os

LDFLAGS = $(OPT) $(GFLAGS) -T$(LDSCRIPT) -nostartfiles
CFLAGS  = $(OPT) $(GFLAGS) -g -Wall
ASFLAGS = $(GFLAGS) -g

INCFLAGS  = $(addprefix -I, $(INC))
FEATURES += -DUSE_STDPERIPH_DRIVER -Wall

# Create a raw binary file from the ELF version
$(PROJECT).bin: $(PROJECT).elf
	$(CP) -O binary $< $@

# Linking object file
$(PROJECT).elf: $(addprefix $(OBJDIR)/,$(OBJ))
	$(LD) $(LDFLAGS) $^ -o $@

# compile and generate dependency info
$(OBJDIR)/%.o: %.c
	$(CC) -c $(FEATURES) $(INCFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(STARTUP)
	$(AS) -c $(ASFLAGS) $< -o $@

$(OBJDIR)/%.d: %.c | $(OBJDIR)
	@set -e; rm -f $@; echo $@; \
	$(CC) -MM $(FEATURES) $(INCFLAGS) $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJDIR):
	mkdir $(OBJDIR)

all:
	$(PROJECT).bin

clean:
	rm -rf $(OBJDIR)
	rm -f $(PROJECT).bin $(PROJECT).elf

debug:
	$(GDB) $(PROJECT).elf

-include $(addprefix $(OBJDIR)/, $(SRC:.c=.d))
