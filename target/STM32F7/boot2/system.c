#define	_SYSTEM_C_
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

#include	<stdint.h>

#include	"config.h"
#include	"stm32f7.h"
#include	"gpio.h"
#include	"system.h"



uint32_t SystemCoreClock = 4000000;

#if 0
const uint8_t  AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t  APBPrescTable[8] =  {0, 0, 0, 0, 1, 2, 3, 4};
const uint32_t MSIRangeTable[12] = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                      4000000, 8000000, 16000000, 24000000, 32000000, 48000000};
#endif

uint16_t systemGpioDefaultTbl[] = {
  PORT_CTRL(GPIO_POWER_SW, GPIO_MODE_INPUT, GPIO_PULL_DOWN, 0),
  PORT_CTRL(GPIO_UPDATE_LED, GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_POWER_LED, GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL_END,
};

void	SystemChangeClockHigher(void);

void
SystemInit(void)
{

  /**************************************************
   * clock change and gate control
   */

#if 0
  SystemChangeClockHigher();
  SysTick_Config(CONFIG_CLOCK_FREQ_SYSCLK_BOOT2/1000);
#endif

  /*RCC_PTR->ahb1enr = DMA1EN_YES | FLASHEN_YES;*/
  RCC->AHB1ENR = (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |
		  RCC_AHB1ENR_GPIOCEN |RCC_AHB1ENR_GPIOHEN);

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
    val = GPIO_PTR[port].odr;
    val &= ~ODR_MASK(num);
    val |=  ODR_X(num, d);
    GPIO_PTR[port].odr = val;
  } else if(mode == (GPIO_MODE_FUNC>>GPIO_MODE_SHIFT)) {
    func = (v & GPIO_FUNC_MASK) >> GPIO_FUNC_SHIFT;
    if(num <= AFL_7) {
      val = GPIO_PTR[port].afrl;
      val &= ~AFRL_MASK(num);
      val |=  AFRL_X(num, func);
      GPIO_PTR[port].afrl = val;
    } else {
      val = GPIO_PTR[port].afrh;
      val &= ~AFRH_MASK(num-8);
      val |=  AFRH_X(num-8, func);
      GPIO_PTR[port].afrh = val;
    }
  }

  val  = GPIO_PTR[port].otyper;
  val &= ~OTYPE_MASK(num);
  if(mode == (GPIO_MODE_FUNC>>GPIO_MODE_SHIFT) ||
     mode == (GPIO_MODE_OUTPUT>>GPIO_MODE_SHIFT)) {
    if(pull == (GPIO_PULL_NONE>>GPIO_PULL_SHIFT)) {
      val |=  OTYPE_PUSHPULL(num);
    } else {
      val |=  OTYPE_OD(num);
    }
  } 
  GPIO_PTR[port].otyper = val;

  val = GPIO_PTR[port].pupdr;
  val &= ~PUPD_MASK(num);
  val |=  PUPD_X(num, pull);
  GPIO_PTR[port].pupdr = val;

  val = GPIO_PTR[port].ospeedr;
  val &= ~OSPEED_MASK(num);
  val |=  OSPEED_HIGH(num);
  GPIO_PTR[port].ospeedr = val;

  val = GPIO_PTR[port].moder;
  val &= ~MODE_MASK(num);
  val |=  MODE_SET(num, mode);
  GPIO_PTR[port].moder = val;

  return;
}
void
SystemGpioSets(uint16_t *pGpio)
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
  GPIO_PTR[CONFIG_POWER_LED_PORTNUM].bsrr = (1UL<<(CONFIG_POWER_LED_BITNUM+16));
  return;
}
void
SystemGpioSetPowerLedOn(void)
{
  GPIO_PTR[CONFIG_POWER_LED_PORTNUM].bsrr = (1UL<<(CONFIG_POWER_LED_BITNUM));
  return;
}
void
SystemGpioSetUpdateLedOff(void)
{
  GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].bsrr = (1UL<<(CONFIG_UPDATE_LED_BITNUM+16));
  return;
}
void
SystemGpioSetUpdateLedOn(void)
{
  GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].bsrr = (1UL<<(CONFIG_UPDATE_LED_BITNUM));
  return;
}
int
SystemGpioGetPowerSw(void)
{
  return (GPIO_PTR[CONFIG_POWER_SW_PORTNUM].idr >> (CONFIG_POWER_SW_BITNUM)) & 1;
}


void
SystemChangeClockHigher(void)
{
  /* clock wait for to access the flash rom */
  FLASH_PTR->acr = FLASH_LATENCY_3CLK;

  /* pll select */
  RCC->PLLCFGR = PLLR_DIV2 | PLLQ_DIV3 | PLLP_DIV2 | PLLN_MULX(40) | PLLM_DIV3;
  RCC->CR = MSION_YES;
  while(!(RCC->CR & MSIRDY_MASK));
  RCC->CR |= PLLON_YES;
  while(!(RCC->CR & PLLRDY_MASK));

  /* clock divider select */
  RCC->CFGR = PPRE2_DIV1 | PPRE1_DIV1 | HPRE_DIV1 | SYSCLK_SW_PLL;
  /*while((RCC->CFGR & SYSCLK_SWS_MASK) != SYSCLK_SWS_PLL);*/
  
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
