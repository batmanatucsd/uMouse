>arm-none-eabi toolchain: https://launchpad.net/gcc-arm-embedded  
>STM flash loader: http://www.st.com/web/en/catalog/tools/PF257525  
>ST-LINK utility: http://www.st.com/web/en/catalog/tools/PF258168


prepare
===
1. make sure you have
 		make
 For windows install MINGW.  
 For MAC install XCODE.  
 Linux usually comes with it

2. download the arm-none-eabi toolchain for your system

3. extract them anywhere you want

	note: some 64 bit system might need ia32-libs to run 32 bit program  
	(or NO such file or directory error appear)

	For me(ubuntu 14.04 64 bit):
		sudo apt-get install libc6-i386

4. add the toolchain into PATH(use export for temporary path)  
	you may need to add path for python to compile libopencm3
		export PATH=$ARM_GCC_HOME/bin:$PYTHON_HOME:$PATH

5. first make the submodule(libopencm3)
		cd $PROJECT_HOME/libopencm3
		make

6. make the project

notes
===

	uMouse Pin out(HEAD UP)
		ST|VCC
		TMS|TX			TMS:SWDIO
		TDO|RX
		TCK|GND			TCK:SWCLK

	Discovery(USB UP)
		VDD(use 3.3V on board)
		SWCLK
		GND
		SWDIO


1. stm32f10x_conf.h comes from /STM32F10x_StdPeriph_Lib_V3.5.0/Project/STM32F10x_StdPeriph_Template/
	reason: USE_STDPERIPH_DRIVER

2. stm32_flash.ld comes from /STM32F10x_StdPeriph_Lib_V3.5.0/Project/STM32F10x_StdPeriph_Template/TrueSTUDIO/STM3210E-EVAL_XL
	reason: need to modify linker script for different MCU

debug
===
First approach:
1. use texane/stlink to start the gdb server:
		st-util --stlinkv1
2. load the compiled code:
		arm-none-eabi-gdb uMouse.elf
	Which can be awoke by:
		make debug
3. connect to gdb server:
		(gdb) target extended localhost:4242
	Or simply
		tar ext :4242
4. load the code:
		load

The second debug approach:
1. use openocd to start gdb server:
		openocd -f interface/stlink-v1.cfg -f target/stm32f1x_stlink.cfg
2. Follow the same step as before.
