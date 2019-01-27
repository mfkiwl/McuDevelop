/*
 * Copyright (c) 2018 zhtlab
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#define	_SYSTEM_C_

#include	<stdint.h>

#include	"config.h"
#include	"gpio.h"
#include	"system.h"
//#include	"stm32h7xx_hal.h"


uint32_t SystemCoreClock = CONFIG_CLOCK_FREQ_SYSCLK_BOOT2;

uint16_t systemGpioDefaultTbl[] = {
  PORT_CTRL(GPIO_POWER_SW,	GPIO_MODE_INPUT, GPIO_PULL_DOWN, 0),
  PORT_CTRL(GPIO_UPDATE_LED,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_POWER_LED,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL_END,
};

void	SystemChangeClockHigher(void);

void
SystemInit(void)
{
  extern uint32_t	__vector_table;

  /**************************************************
   * set the new interrupt vector table
   */
  SCB->VTOR  = (uint32_t)&__vector_table;

  /**************************************************
   * clock change and gate control
   */

#if 0
  SystemChangeClockHigher();
#endif

  /* HSI on, HSI48 on */
  RCC_PTR->CR = RCC_CR_HSI48ON_YES | RCC_CR_HSION_YES;	/* only HSI on */
  while((RCC_PTR->CR & ((RCC_CR_HSI48RDY_MASK) | (RCC_CR_HSIRDY_MASK))) !=
	((RCC_CR_HSI48RDY_YES) | (RCC_CR_HSIRDY_YES)));

  /* USBSEL=HSI48, USART234578SEL = PCLK1 */
  RCC_PTR->D2CCIP2R = (RCC_D2CCIP2R_USBSEL_HSI48) | (RCC_D2CCIP2R_USART234578SEL_PCLK1);
  RCC_PTR->CFGR = 0;
  RCC_PTR->D1CFGR = 0;
  RCC_PTR->D2CFGR = 0;
  RCC_PTR->D3CFGR = 0;

  SysTick_Config(CONFIG_CLOCK_FREQ_SYSCLK_BOOT2/1000);

  /*RCC_PTR->ahb4enr = DMA1EN_YES | FLASHEN_YES;*/
  RCC_PTR->AHB4ENR = (RCC_AHB4ENR_GPIOAEN_YES | RCC_AHB4ENR_GPIOBEN_YES |
                      RCC_AHB4ENR_GPIOCEN_YES |RCC_AHB4ENR_GPIOHEN_YES);

#if 0
  /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
  *((__IO uint32_t*)0x51008108) = 1;
#endif

  SystemGpioInit();
  
  return;
}


void
SystemLoop(void)
{
  return;
}


/*
 *  PULL UP/DOWN, PUSH/PULL/OPENDRAIN settings table
 *
 *         |  NONE  |  UP     | DOWN    |  OD
 *  -------+--------+---------+---------+----------
 *  input  |  FLOAT |  UP     | DOWN    |  --
 *  output |   PP   |  UP+OD  | DOWN+OD |  OD
 *  func   |   PP   |  UP+OD  | DOWN+OD |  OD
 *  analog | ANALOG | reserved| reserved| reserved
 *
 *  pp/od = (mode == {output || func}) && pull != none
 *
 */
void
SystemGpioSet(uint16_t v)
{
  uint32_t	val;
  int		port, num;
  int		mode, pull, d, func;

  port = (v & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;
  num  = (v & GPIO_NUM_MASK)  >> GPIO_NUM_SHIFT;
  mode = (v & GPIO_MODE_MASK) >> GPIO_MODE_SHIFT;
  pull = (v & GPIO_PULL_MASK) >> GPIO_PULL_SHIFT;

  if(mode == (GPIO_MODE_OUTPUT>>GPIO_MODE_SHIFT)) {
    d    = (v & GPIO_VALFUNC_MASK)  >> GPIO_VAL_SHIFT;
    val = GPIO_PTR[port].ODR;
    val &= ~ODR_MASK(num);
    val |=  ODR_X(num, d);
    GPIO_PTR[port].ODR = val;
  } else if(mode == (GPIO_MODE_FUNC>>GPIO_MODE_SHIFT)) {
    func = (v & GPIO_FUNC_MASK) >> GPIO_FUNC_SHIFT;
    if(num <= AFL_7) {
      val = GPIO_PTR[port].AFRL;
      val &= ~AFRL_MASK(num);
      val |=  AFRL_X(num, func);
      GPIO_PTR[port].AFRL = val;
    } else {
      val = GPIO_PTR[port].AFRH;
      val &= ~AFRH_MASK(num-8);
      val |=  AFRH_X(num-8, func);
      GPIO_PTR[port].AFRH = val;
    }
  }

  val  = GPIO_PTR[port].OTYPER;
  val &= ~OTYPE_MASK(num);
  if(mode == (GPIO_MODE_FUNC>>GPIO_MODE_SHIFT) ||
     mode == (GPIO_MODE_OUTPUT>>GPIO_MODE_SHIFT)) {
    if(pull == (GPIO_PULL_NONE>>GPIO_PULL_SHIFT)) {
      val |=  OTYPE_PUSHPULL(num);
    } else {
      val |=  OTYPE_OD(num);
    }
  } 
  GPIO_PTR[port].OTYPER = val;

  val = GPIO_PTR[port].PUPDR;
  val &= ~PUPD_MASK(num);
  val |=  PUPD_X(num, pull);
  GPIO_PTR[port].PUPDR = val;

  val = GPIO_PTR[port].OSPEEDR;
  val &= ~OSPEED_MASK(num);
  val |=  OSPEED_HIGH(num);
  GPIO_PTR[port].OSPEEDR = val;

  val = GPIO_PTR[port].MODER;
  val &= ~MODE_MASK(num);
  val |=  MODE_SET(num, mode);
  GPIO_PTR[port].MODER = val;

  return;
}
void
SystemGpioSets(const uint16_t *pGpio)
{

  while(*pGpio != PORT_CTRL_END) {
    SystemGpioSet(*pGpio++);
  }

  return;
}


void
SystemGpioInit(void)
{
  uint32_t		val;
  SystemGpioSets(systemGpioDefaultTbl);

  return;
}


void
SystemGpioSetPowerLedOff(void)
{
  GPIO_PTR[CONFIG_POWER_LED_PORTNUM].BSRR = (1UL<<(CONFIG_POWER_LED_BITNUM+16));
  return;
}
void
SystemGpioSetPowerLedOn(void)
{
  GPIO_PTR[CONFIG_POWER_LED_PORTNUM].BSRR = (1UL<<(CONFIG_POWER_LED_BITNUM));
  return;
}
void
SystemGpioSetUpdateLedOff(void)
{
  GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].BSRR = (1UL<<(CONFIG_UPDATE_LED_BITNUM+16));
  return;
}
void
SystemGpioSetUpdateLedOn(void)
{
  GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].BSRR = (1UL<<(CONFIG_UPDATE_LED_BITNUM));
  return;
}
int
SystemGpioGetPowerSw(void)
{
  return (GPIO_PTR[CONFIG_POWER_SW_PORTNUM].IDR >> (CONFIG_POWER_SW_BITNUM)) & 1;
}


void
SystemChangeClockHigher(void)
{
#if 0
  /* clock wait for to access the flash rom */
  FLASH_PTR->acr = FLASH_LATENCY_3CLK;

  /* pll select */
  RCC_PTR->PLLCFGR = PLLR_DIV2 | PLLQ_DIV3 | PLLP_DIV2 | PLLN_MULX(40) | PLLM_DIV3;
  RCC_PTR->CR1 = MSION_YES;
  while(!(RCC_PTR->CR1 & MSIRDY_MASK));
  RCC_PTR->CR1 |= PLLON_YES;
  while(!(RCC_PTR->CR1 & PLLRDY_MASK));

  /* clock divider select */
  RCC_PTR->CFGR = PPRE2_DIV1 | PPRE1_DIV1 | HPRE_DIV1 | SYSCLK_SW_PLL;
  /*while((RCC_PTR->CFGR & SYSCLK_SWS_MASK) != SYSCLK_SWS_PLL);*/
#endif

  return;
}


void
SystemWdtInit(void)
{
  return;
}
void
SystemWdtClear(void)
{
  return;
}


void
_Error_Handler(char * file, int line)
{
}


void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}
#if 0
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}
#endif
#if 0
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /*HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);*/

  /* System interrupt init*/
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

  return;
}
#endif
