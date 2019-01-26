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

#define _STM32L4_C_

#include        <stdint.h>

#include        "config.h"
#include        "stm32l4.h"
#include        "system.h"

void
SystemInit(void)
{

  /**************************************************
   * clock change and gate control
   */
  RCC_PTR->CR = RCC_CR_HSION_YES;
  while(!(RCC_PTR->CR & RCC_CR_HSIRDY_MASK));
  RCC_PTR->CFGR = (RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1 |
                   RCC_CFGR_HPRE_DIV1 | RCC_CFGR_SW_HSI);

  SysTick_Config(CONFIG_CLOCK_FREQ_SYSCLK_BOOT1/1000);

  RCC_PTR->AHB2ENR = (RCC_AHB2ENR_GPIOAEN_YES |
                      RCC_AHB2ENR_GPIOBEN_YES |
                      RCC_AHB2ENR_GPIOCEN_YES |
                      RCC_AHB2ENR_GPIOHEN_YES);

  return;
}


void
SystemGpioInit(void)
{
  uint32_t              val;
  /* power sw */
  val = GPIO_PTR[CONFIG_POWER_SW_PORTNUM].MODER;
  val &= ~MODE_MASK(CONFIG_POWER_SW_BITNUM);
  val |=  MODE_INPUT(CONFIG_POWER_SW_BITNUM);
  GPIO_PTR[CONFIG_POWER_SW_PORTNUM].MODER = val;

  /* update led */
  val  = GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].MODER;
  val &= ~MODE_MASK(CONFIG_UPDATE_LED_BITNUM);
  val |=  MODE_OUTPUT(CONFIG_UPDATE_LED_BITNUM);
  GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].MODER = val;

  /* power led */
  val  = GPIO_PTR[CONFIG_POWER_LED_PORTNUM].MODER;
  val &= ~MODE_MASK(CONFIG_POWER_LED_BITNUM);
  val |=  MODE_OUTPUT(CONFIG_POWER_LED_BITNUM);
  GPIO_PTR[CONFIG_POWER_LED_PORTNUM].MODER = val;

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
  return ( GPIO_PTR[CONFIG_POWER_SW_PORTNUM].IDR >> (CONFIG_POWER_SW_BITNUM)) & 1;
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
