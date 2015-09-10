#include "mcu_adc.h"

#include "mcu_delay.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

uint16_t sensor_buffers[4];

static void ADC_RCC_init(void)
{
    //ADC CLK MAX = 14 MHz
    //current CLK = 72 MHz / 6 = 12 MHz
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);

    //Enable ADC & GPIO
    rcc_periph_clock_enable(REC_RCC_ADC | REC_RCC_PORT);
}

static void ADC_DMA_init(void)
{
    rcc_periph_clock_enable(REC_RCC_DMA);

    // Reset and disable DMA1 Channel1
    dma_channel_reset(REC_DMA, REC_DMA_CHANNEL);

    // Set mode: High priority, Circular mode, read from peripheral
    dma_enable_circular_mode(REC_DMA, REC_DMA_CHANNEL);
    dma_set_priority(REC_DMA, REC_DMA_CHANNEL, DMA_CCR_PL_HIGH);
    dma_set_read_from_peripheral(REC_DMA, REC_DMA_CHANNEL);

    // Set memory:
    dma_set_peripheral_address(REC_DMA, REC_DMA_CHANNEL, (uint32_t)(&ADC_DR(REC_ADC)));
    dma_set_peripheral_size(REC_DMA, REC_DMA_CHANNEL, DMA_CCR_PSIZE_16BIT);

    dma_set_number_of_data(REC_DMA, REC_DMA_CHANNEL, 4);

    dma_set_memory_address(REC_DMA, REC_DMA_CHANNEL, (uint32_t)sensor_buffers);
    dma_set_memory_size(REC_DMA, REC_DMA_CHANNEL, DMA_CCR_MSIZE_16BIT);

    // Set increment mode
    dma_disable_peripheral_increment_mode(REC_DMA, REC_DMA_CHANNEL);
    dma_enable_memory_increment_mode(REC_DMA, REC_DMA_CHANNEL);

    // Interrupt when transfer complete
    dma_enable_transfer_complete_interrupt(REC_DMA, REC_DMA_CHANNEL);

    // Enable DMA channel
    dma_enable_channel(REC_DMA, REC_DMA_CHANNEL);
}


void ADC_init(void)
{
    // init adc clk & gpio clk
    ADC_RCC_init();

    /* Make sure the ADC doesn't run during config. */
    adc_off(REC_ADC);

    // set up gpio
    gpio_set_mode(REC_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG,
        L_REC_PIN | LF_REC_PIN | RF_REC_PIN | R_REC_PIN);

    // init DMA for adc
    ADC_DMA_init();

    /* We configure everything for one single conversion. */
    adc_disable_scan_mode(REC_ADC);
    adc_set_single_conversion_mode(REC_ADC);
    adc_disable_external_trigger_regular(REC_ADC);
    adc_set_right_aligned(REC_ADC);
    adc_set_sample_time_on_all_channels(REC_ADC, ADC_SMPR_SMP_28DOT5CYC);

    adc_power_on(REC_ADC);

    delay_ms(10);

    adc_reset_calibration(REC_ADC);
    adc_calibration(REC_ADC);
}
