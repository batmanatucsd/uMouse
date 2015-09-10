##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

BINARY = uMouse

OBJS += mcu_usart.o

OBJS += mcu_soft_iic.o
OBJS += mcu_delay.o
OBJS += eMPL/inv_mpu.o
OBJS += eMPL/inv_mpu_dmp_motion_driver.o
OBJS += eMPL/mpu_basic.o

CPPFLAGS += -DMPU6050 -DEMPL_TARGET_UMOUSE

LDLIBS += -lm

OPENCM3_DIR=../libopencm3
LDSCRIPT = $(OPENCM3_DIR)/lib/stm32/f1/stm32f100xb.ld
#LDSCRIPT = stm32_flash.ld

include ../libopencm3.target.mk

debug:
	$(GDB) $(BINARY).elf
