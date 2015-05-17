/**
  ******************************************************************************
  * @file    Examples/GPIOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "mcu_lib.h"
#include "stm32f10x_it.h"

/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint16_t readADC_status;
extern uint16_t sensor_buffers[4];
extern uint16_t sensor_readings[4];
uint16_t emitter = 0x1000;
uint16_t i = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
// void NMI_Handler(void)
// {
// }

// /**
//   * @brief  This function handles Hard Fault exception.
//   * @param  None
//   * @retval None
//   */
// void HardFault_Handler(void)
// {
//   /* Go to infinite loop when Hard Fault exception occurs */
//   while (1)
//   {
//   }
// }

// /**
//   * @brief  This function handles Memory Manage exception.
//   * @param  None
//   * @retval None
//   */
// void MemManage_Handler(void)
// {
//   /* Go to infinite loop when Memory Manage exception occurs */
//   while (1)
//   {
//   }
// }

// /**
//   * @brief  This function handles Bus Fault exception.
//   * @param  None
//   * @retval None
//   */
// void BusFault_Handler(void)
// {
//   /* Go to infinite loop when Bus Fault exception occurs */
//   while (1)
//   {
//   }
// }

// /**
//   * @brief  This function handles Usage Fault exception.
//   * @param  None
//   * @retval None
//   */
// void UsageFault_Handler(void)
// {
//   /* Go to infinite loop when Usage Fault exception occurs */
//   while (1)
//   {
//   }
// }

// /**
//   * @brief  This function handles SVCall exception.
//   * @param  None
//   * @retval None
//   */
// void SVC_Handler(void)
// {
// }

// /**
//   * @brief  This function handles Debug Monitor exception.
//   * @param  None
//   * @retval None
//   */
// void DebugMon_Handler(void)
// {
// }

// *
//   * @brief  This function handles PendSV_Handler exception.
//   * @param  None
//   * @retval None
  
// void PendSV_Handler(void)
// {
// }

// /**
//   * @brief  This function handles SysTick Handler.
//   * @param  None
//   * @retval None
//   */
// void SysTick_Handler(void)
// {

// }

// /******************************************************************************/
// /*                 STM32F10x Peripherals Interrupt Handlers                   */
// /*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
// /*  available peripheral interrupt handler's name please refer to the startup */
// /*  file (startup_stm32f10x_xx.s).                                            */
// /******************************************************************************/

// /**
//   * @brief  This function handles EXTI0 interrupt request.
//   * @param  None
//   * @retval None
//   */
// void EXTI0_IRQHandler(void)
// {
// 	//Check if EXTI_Line0 is asserted
// }

// void USART1_IRQHandler(void)
// {


// }

// /**
//   * @}
//   */
/*void DMA1_Channel1_IRQHandler(void)*/
/*{*/
  //Test on DMA1 Channel1 Transfer Complete interrupt
  /*if(DMA_GetITStatus(DMA1_IT_TC1))*/
  /*{*/
    /*readADC_status=0;*/
   /*Clear DMA1 interrupt pending bits*/
    /*DMA_ClearITPendingBit(DMA1_IT_GL1);*/
  /*}*/
/*}*/


/**
  * @}
  */
void DMA2_Channel4_5_IRQHandler(void)/*{{{*/
{
  //Test on DMA1 Channel1 Transfer Complete interrupt
  if(DMA_GetITStatus(DMA2_IT_TC5))
  {
    readADC_status = 0;
   /*Clear DMA1 interrupt pending bits*/
    DMA_ClearITPendingBit(DMA2_IT_GL5);
  }
}/*}}}*/

/**
  * @}
  */
void ADC1_IRQHandler(void)/*{{{*/
{
  // /* Set PC.06 pin */
  // GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
  // /* Get injected channel11 converted value */
  // ADC_InjectedConvertedValueTab[Index++] = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
  // /* Clear ADC1 JEOC pending interrupt bit */
  
  // /* Reset PC.06 pin */
  // GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
  // if(ADC1->JDR1 & 0xF000 == 0xF000) ADC1->JDR1 = 0;
  // if(ADC1->JDR2 & 0xF000 == 0xF000) ADC1->JDR2 = 0;
  // if(ADC1->JDR3 & 0xF000 == 0xF000) ADC1->JDR3 = 0;
  // if(ADC1->JDR4 & 0xF000 == 0xF000) ADC1->JDR4 = 0;

  /*if((uint16_t)ADC1->JDR1 > 65000) ADC1->JDR1 = 0;*/
  /*if((uint16_t)ADC1->JDR2 > 65000) ADC1->JDR2 = 0;*/
  /*if((uint16_t)ADC1->JDR3 > 65000) ADC1->JDR3 = 0;*/
  /*if((uint16_t)ADC1->JDR4 > 65000) ADC1->JDR4 = 0;*/
  
  /*if(readADC_status) {*/
    /*GPIO_SetBits(GPIOC, GPIO_Pin_4); */
    /*sensor_readings[i] = sensor_buffers[i];*/

    /*if(i == 3) {*/
    
      /*i = 0;*/
      /*readADC_status = 0;*/
    /*}*/
    /*else*/
      /*i++;*/

    /*GPIO_ResetBits(GPIOB, emitter);*/
    /*if(emitter == 0x8000)*/
      /*emitter = 0x1000;*/
    /*else {*/
      /*emitter = emitter << 1;*/
      /*GPIO_SetBits(GPIOB, emitter); */

    /*}*/
  /*} else {*/
    /*GPIO_ResetBits(GPIOC, GPIO_Pin_4); */
  /*}*/

  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}/*}}}*/

/**
  * @}
  */
void SysTick_Handler(void)
{
    /*printf("%d\r\n", wait_time);*/
    /*GPIO_ResetBits(GPIOC, GPIO_Pin_4);*/
  /*if(waiting) {*/
    /*GPIO_SetBits(GPIOC, GPIO_Pin_4);*/
    /*printf(" while waiting %d\r\n", wait_time);*/
  /*}*/
  Decrement_WaitTime();  
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
