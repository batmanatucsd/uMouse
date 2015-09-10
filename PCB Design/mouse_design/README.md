Basic setting
===
VCC = 3.3v  
VDDA = 3.3V  
V_BATT = battery voltage  

AVOID
===
there is a temperature sensor at ADC1_IN16

PC13,14,15 less output current

review the direction of motor connector in board design

CIN for NCP1117 -- 10uf (typ)  
Cout for NCP1117 > 4.7uf (ESR) that is within the limits of 33 m (typ) to 2.2  is required.

boot
===
Default boot from bank 1(512K)

Bootloader: activate by apply Pattern3  
BOOT0--GND  
BOOT1--GND  
BFB2--in the user option memory  

USART
===
USART1 be used in simple debug situation  
PA9--USART1_TX  
PA10--USART1_RX  

PA8--USART1_CK  
PA11--USART1_CTS  
PA12--USART1_RTS  

JTAG
===
CONNECT to JTAG(for advance debug)  
NRST--RST  
PA13--JT_TMS  
PA14--JT_TCK  
PA15--JT_TDI  
PB3--JT_TDO  
PB4--JT_TRST  

Gyro
===
PB5--I2C1_SMBA  
PC12--INT  
PB8--I2C1_SCL--REMAP  
PB9--I2C1_SDA--REMAP  

Motor Driver
===
    PA4--STBY

    RIGHT
    PA1--M1PWM (TIM5_CH2 or TIM2_CH2)
    PA3--M1I1
    PA2--M1I2

    LEFT
    PA7--M2PWM (TIM3_CH2 or TIM8_CH1N or TIM14_CH1)
    PA5--M2I1
    PA6--M2I2

/*****************************************************************************/
// Encoder
/*****************************************************************************/
// RIGHT
PB6--M1CA (TIM4_CH1)
PB7--M1CB (TIM4_CH2)

// LEFT
PC7--M2CA (TIM3_CH2) // need to remap
PC6--M2CB (TIM3_CH1)

might be able to just use TIM8 instead of TIM3

/*****************************************************************************/
// IR Sensor
/*****************************************************************************/
// From left to right
PC0--IR_IN1 (ADC123_IN10)
PC1--IR_IN2 (ADC123_IN11)
PC2--IR_IN3 (ADC123_IN12)
PC3--IR_IN4 (ADC123_IN13)

PB15--IR_OUT1
PB14--IR_OUT2
PB13--IR_OUT3
PB12--IR_OUT4

/*****************************************************************************/
// Indicator
/*****************************************************************************/
PB10--BUTTON1
PC4--LED1 - green
PC5--LED2 - yellow
PB0--LED3 - red
