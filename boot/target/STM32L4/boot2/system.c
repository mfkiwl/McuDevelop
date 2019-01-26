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

#define _SYSTEM_STM32L4_C_

#include        <stdint.h>

#include        "config.h"
#include        "stm32l4.h"
#include        "gpio.h"
#include        "system.h"


uint32_t SystemCoreClock = 4000000;

const uint8_t  AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t  APBPrescTable[8] =  {0, 0, 0, 0, 1, 2, 3, 4};
const uint32_t MSIRangeTable[12] = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                      4000000, 8000000, 16000000, 24000000, 32000000, 48000000};


uint16_t systemGpioDefaultTbl[] = {
  PORT_CTRL(GPIO_POWER_SW, GPIO_MODE_INPUT, GPIO_PULL_DOWN, 0),
  PORT_CTRL(GPIO_UPDATE_LED, GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_POWER_LED, GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL_END,
};

void    SystemChangeClockHigher(void);

void
SystemInit(void)
{

  extern uint32_t       __vector_table[];
  SCB->VTOR = (uint32_t)__vector_table;

  /**************************************************
   * clock change and gate control
   */
  SystemChangeClockHigher();
  SysTick_Config(CONFIG_CLOCK_FREQ_SYSCLK_BOOT2/1000);
  NVIC_SetPriority(SysTick_IRQn, 5);
  NVIC_EnableIRQ(SysTick_IRQn);


  RCC_PTR->AHB1ENR = RCC_AHB1ENR_DMA1EN_YES | RCC_AHB1ENR_FLASHEN_YES;
  RCC_PTR->AHB2ENR = (RCC_AHB2ENR_GPIOAEN_YES |
                      RCC_AHB2ENR_GPIOBEN_YES |
                      RCC_AHB2ENR_GPIOCEN_YES |
                      RCC_AHB2ENR_GPIOHEN_YES);

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
  uint32_t      val;
  int           port, num;
  int           mode, pull, d, func;

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
  uint32_t              val;
  SystemGpioSets(systemGpioDefaultTbl);

  return;
}


void
SystemGpioSetPowerLedOff(void)
{
  GPIO_PTR[CONFIG_POWER_LED_PORTNUM].BRR = (1UL<<(CONFIG_POWER_LED_BITNUM));
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
  GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].BRR = (1UL<<(CONFIG_UPDATE_LED_BITNUM));
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
  /* clock wait for to access the flash rom */
  FLASH_PTR->ACR = FLASH_LATENCY_3CLK;

  /* pll select  16MHz(HSI16) / 4 * 40 = 160MHz
   * PLLR = 80MHz = 160MHz / 2
   */
  RCC_PTR->PLLCFGR = (RCC_PLLCFGR_PLLREN_YES |
                      RCC_PLLCFGR_PLLR_DIV2 | RCC_PLLCFGR_PLLQ_DIV4 | RCC_PLLCFGR_PLLP_DIV7 |
                      RCC_PLLCFGR_PLLN_MULX(40) | RCC_PLLCFGR_PLLM_DIV4 |
                      RCC_PLLCFGR_PLLSRC_HSI16);
  RCC_PTR->CR = RCC_CR_HSION_YES;
  while(!(RCC_PTR->CR & RCC_CR_HSIRDY_MASK));
  RCC_PTR->CR |= RCC_CR_PLLON_YES;
  while(!(RCC_PTR->CR & RCC_CR_PLLRDY_MASK));

  /* clock divider select */
  RCC_PTR->CFGR = RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_SW_PLL;
  while((RCC_PTR->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL);

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
