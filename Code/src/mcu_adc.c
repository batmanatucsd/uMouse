#include "mcu_adc.h"

#include "mcu_delay.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>

#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>

#include <stdint.h>

static uint8_t data_ready = 0;
uint16_t sensor_buffers[4];

static void ADC_RCC_init(void)
{
    //ADC CLK MAX = 14 MHz
    //current CLK = 72 MHz / 6 = 12 MHz
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);

    //Enable ADC & GPIO
    rcc_periph_clock_enable(REC_RCC_PORT | EMIT_RCC_PORT |
        REC_RCC_DMA | REC_RCC_ADC);
}

static void ADC_GPIO_init(void)
{
    // set up receiver gpio
    gpio_set_mode(REC_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG,
        L_REC_PIN | LF_REC_PIN | RF_REC_PIN | R_REC_PIN);

    // Set up emitter gpio
    gpio_set_mode(EMIT_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
        L_EMIT_PIN | LF_EMIT_PIN | RF_EMIT_PIN | R_EMIT_PIN);
}

static void ADC_NVIC_init(void)
{
    //divide Priority into 2 Preemption & 2 Sub
    scb_set_priority_grouping(SCB_AIRCR_PRIGROUP_GROUP4_SUB4);

    // Priority[0~255] = [7:4][3:0]
    // [3:0] is ignored
    // [7:4] is seperated into Preemption and sub according to scb_aircr
    nvic_enable_irq(REC_DMA_ISR);
    nvic_set_priority(REC_DMA_ISR, 0);
}

static void ADC_DMA_init(void)
{
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

    ADC_GPIO_init();

    ADC_NVIC_init();

    // init DMA for adc
    ADC_DMA_init();

    // setup independent mode
    adc_set_dual_mode(ADC_CR1_DUALMOD_IND);

    // scan regular channels only when requested
    adc_enable_scan_mode(REC_ADC);
    adc_set_single_conversion_mode(REC_ADC);

    // disable all external trigger
    adc_disable_external_trigger_regular(REC_ADC);
    adc_disable_external_trigger_injected(REC_ADC);

    // set internal trigger mode : software start
    ADC_CR2(REC_ADC) |= ADC_CR2_EXTSEL_SWSTART;

    // align to LSB
    adc_set_right_aligned(REC_ADC);

    //set sample time
    adc_set_sample_time(REC_ADC, L_REC_ADC, REC_ADC_TSAMPLE);
    adc_set_sample_time(REC_ADC, LF_REC_ADC,REC_ADC_TSAMPLE);
    adc_set_sample_time(REC_ADC, RF_REC_ADC,REC_ADC_TSAMPLE);
    adc_set_sample_time(REC_ADC, R_REC_ADC, REC_ADC_TSAMPLE);

    // set up regular group sequence
    uint8_t channel_group[] = {L_REC_ADC, LF_REC_ADC,
        RF_REC_ADC, R_REC_ADC};

    // initial the sequence
    adc_set_regular_sequence(REC_ADC, 4, channel_group);

    // power on adc
    adc_power_on(REC_ADC);

    // delay to wait for stablize
    delay_ms(10);

    // enable dma channel
    adc_enable_dma(REC_ADC);

    // self calibration
    adc_reset_calibration(REC_ADC);
    adc_calibration(REC_ADC);
}

void ADC_request(void)
{
    // start receive
    gpio_set(EMIT_PORT, L_EMIT_PIN);
    gpio_set(EMIT_PORT, LF_EMIT_PIN);
    gpio_set(EMIT_PORT, RF_EMIT_PIN);
    gpio_set(EMIT_PORT, R_EMIT_PIN);

    // wait for stablize
    delay_us(80);

    // start conversion
    adc_start_conversion_regular(REC_ADC);
}

void ADC_read(void)
{
    while(!data_ready);

    data_ready = 0;
}

void dma1_channel1_isr(void)
{
    if(dma_get_interrupt_flag(REC_DMA, REC_DMA_CHANNEL, DMA_TCIF))
    {
        // clear intertupt flag
        dma_clear_interrupt_flags(REC_DMA, REC_DMA_CHANNEL, DMA_TCIF);

        // data is ready;
        data_ready = 1;

        // close emitter
        gpio_clear(EMIT_PORT, L_EMIT_PIN);
        gpio_clear(EMIT_PORT, LF_EMIT_PIN);
        gpio_clear(EMIT_PORT, RF_EMIT_PIN);
        gpio_clear(EMIT_PORT, R_EMIT_PIN);
    }
    else
    {
        // stop if other interrupt took place
        while(1);
    }
}
