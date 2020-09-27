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

#define _SYSTEM_STM32L0_C_

#include        <stdint.h>
#include        <string.h>

#include        "config.h"
#include        "gpio.h"
#include        "intr.h"
#include        "system.h"


#define SYSTEM_TIMER_REG        (TIM5_PTR->CNT)     /* decrement counter */
static systemClockFreq_t        systemClk;

uint32_t SystemCoreClock = CONFIG_CLOCK_FREQ_SYSCLK_BOOT2;

const uint8_t  AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t  APBPrescTable[8] =  {0, 0, 0, 0, 1, 2, 3, 4};
const uint32_t MSIRangeTable[12] = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                      4000000, 8000000, 16000000, 24000000, 32000000, 48000000};


uint16_t systemGpioDefaultTbl[] = {
  PORT_CTRL(GPIO_POWER_SW,   GPIO_MODE_INPUT,  GPIO_PULL_DOWN, 0),
  PORT_CTRL(GPIO_UPDATE_LED, GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_POWER_LED,  GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
#if CONFIG_MCO_ENABLE
  PORT_CTRL(GPIO_MCO_A8, GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC(0)),
#endif
  PORT_CTRL(GPIO_USART1_RXD,  GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),

  PORT_CTRL_END,
};

void    SystemChangeClockHigher(void);

void
SystemInit(void)
{

  extern uint32_t       __VECTOR_TABLE[];
  SCB->VTOR = (uint32_t)__VECTOR_TABLE;

  /**************************************************
   * clock change and gate control
   */
  SystemChangeClockHigher();

  RCC_PTR->AHB1ENR |= RCC_AHB1ENR_DMA1EN_YES;
  RCC_PTR->AHB1ENR |= (RCC_GPIOAEN_YES |
                       RCC_GPIOBEN_YES |
                       RCC_GPIOCEN_YES |
                       RCC_GPIODEN_YES |
                       RCC_GPIOHEN_YES);


  RCC_PTR->APB2ENR |= RCC_APB2ENR_USART1EN_YES;

#if 1
  RCC_PTR->APB1ENR |= RCC_APB1ENR_PWREN_YES;
  RCC_PTR->APB2ENR |= RCC_APB2ENR_SYSCFGEN_YES;

  PWR_PTR->CR1 |= PWR_CR1_VOS_RANGE3_144MHZ;
  while(PWR_PTR->CSR1 & PWR_CSR1_VOSRDY_MASK);
#endif

  // SDMMC
  RCC_PTR->APB2ENR |= RCC_APB2ENR_SDMMC1EN_YES;
  RCC_PTR->DCKCFGR2 |= RCC_DCKCFGR2_SDMMC1SEL_SYS;


  SystemGpioInit();

#if 1
  IntrInit(0);
  NVIC_SetPriority(SysTick_IRQn, 1);

  {
    systemClockFreq_t     clk;
    uint32_t              systick;
    SystemGetClockValue(&clk);
    systick = clk.systick;
    SysTick_Config(systick/1000);
  }
#endif

#if 0
  SysTick_Config(CONFIG_CLOCK_FREQ_SYSCLK_BOOT2/1000);
  NVIC_SetPriority(SysTick_IRQn, 5);
  NVIC_EnableIRQ(SysTick_IRQn);
#endif

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
  GpioSetPowerLedOff();
  return;
}
void
SystemGpioSetPowerLedOn(void)
{
  GpioSetPowerLedOn();
  return;
}
void
SystemGpioSetUpdateLedOff(void)
{
  GpioSetUpdateLedOff();
  return;
}
void
SystemGpioSetUpdateLedOn(void)
{
  GpioSetUpdateLedOn();
  return;
}
int
SystemGpioGetPowerSw(void)
{
  //return (GPIO_PTR[CONFIG_POWER_SW_PORTNUM].IDR >> (CONFIG_POWER_SW_BITNUM)) & 1;
  return (~GPIO_PTR[CONFIG_POWER_SW_PORTNUM].IDR >> (CONFIG_POWER_SW_BITNUM)) & 1;
}


void
SystemChangeClockHigher(void)
{
#if ((CONFIG_CLOCK_FREQ_SYSCLK_BOOT2) == (CONFIG_CLOCK_FREQ_32MHZ))
  /* clock wait for to access the flash rom
   * set 1WS, if HREF is over 16MHz
   */
  FLASH_PTR->ACR = FLASH_ACR_LATENCY_YES;
#endif

  FLASH_PTR->ACR = FLASH_LATENCY_CLK(2);


#if 0
  PWR_PTR->CR1 |= PWR_CR1_VOS_RANGE3_144MHZ;
  while(PWR_PTR->CSR1 & PWR_CSR1_VOSRDY_MASK);
#endif

  /* pll select  16MHz(HSI16)/4 * 8 = 32MHz (max 32MHz)
   */

  /* HSI16 enable */
  RCC_PTR->CR |= RCC_CR_HSION_YES;
  while(!(RCC_PTR->CR & RCC_CR_HSIRDY_MASK));
  RCC_PTR->CFGR |= (RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1 |
                    RCC_CFGR_HPRE_DIV1);

#if ((CONFIG_CLOCK_FREQ_SYSCLK_BOOT2) == (CONFIG_CLOCK_FREQ_32MHZ))
  /* set pll factor and start */
  RCC_PTR->CFGR |= (RCC_CFGR_PLLDIV_DIV2 | RCC_CFGR_PLLMUL_X4 |
                    RCC_CFGR_PLLSRC_HSI16);
  RCC_PTR->CR |= RCC_CR_PLLON_YES;
  while(!(RCC_PTR->CR & RCC_CR_PLLRDY_MASK));

  /* clock divider select */
  RCC_PTR->CFGR |= RCC_CFGR_SW_PLL;
  while((RCC_PTR->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL);
#endif

#if CONFIG_MCO_ENABLE
  RCC_PTR->CFGR |= (RCC_CFGR_MCO1PRE_DIV1 | RCC_CFGR_MCO1SEL_PLL);
#endif

  return;
}


const static uint8_t   systemAHBPrescalerTable[]   = RCC_CLOCK_HPRETABLE;
const static uint8_t   systemPeri1PrescalerTable[] = RCC_CLOCK_PPRETABLE;
const static uint8_t   systemPeri2PrescalerTable[] = RCC_CLOCK_PPRETABLE;
void
SystemUpdateClockValue(void)
{
  int           div, shift;
  uint32_t      freqPllIn;
  uint32_t      m, n;
  uint32_t      vco;

  /*** PLL clock source selector */
  switch(RCC_PTR->PLLCFGR & RCC_PLLCFGR_PLLSRC_MASK) {
  case RCC_PLLCFGR_PLLSRC_HSI16: freqPllIn = RCC_CLOCK_HSI;    break;
  case RCC_PLLCFGR_PLLSRC_HSE:   freqPllIn = CONFIG_CLOCK_HSE; break;
    /* case PLLCKSELR_PLLSRC_CSI: */
  default:                   freqPllIn = 0;    break;
  }
  systemClk.pllin = freqPllIn;

  /*** freq_VCO = (freq_PLLIN / M) * N,   freq_SYSCLK = freq_VCO / DIVP */
  /*** calc PLL1 frequency value */
  CALCPLL(RCC_PTR->PLLCFGR, 1, freqPllIn);

  /* calc core,HCLK,SysTick clock */
  SystemCoreClockUpdate();

  /* APB1, 2 */
  div   = (RCC_PTR->CFGR &  RCC_CFGR_PPRE1_MASK) >>  RCC_CFGR_PPRE1_SHIFT;
  shift = systemPeri1PrescalerTable[div];
  systemClk.pclk1 = systemClk.hclk >> shift;
  div   = (RCC_PTR->CFGR &  RCC_CFGR_PPRE2_MASK) >>  RCC_CFGR_PPRE2_SHIFT;
  shift = systemPeri2PrescalerTable[div];
  systemClk.pclk2 = systemClk.hclk >> shift;

  return;
}
void
SystemCoreClockUpdate(void)
{
  int           div, shift;
  uint32_t      clk;

  /* SYSCLK selector */
  switch (RCC_PTR->CFGR & RCC_CFGR_SWS_MASK) {
  case RCC_CFGR_SWS_HSI:
    clk = RCC_CLOCK_HSI;
    break;

  case RCC_CFGR_SWS_HSE:
    clk = CONFIG_CLOCK_HSE;
    break;

  case RCC_CFGR_SWS_PLL:
    clk = systemClk.pll1.P;
    break;
  }
  systemClk.sysclk  = clk;

  /* AHB1: HCLK frequency */
  div   = (RCC_PTR->CFGR & RCC_CFGR_HPRE_MASK);
  div >>= RCC_CFGR_HPRE_SHIFT;
  shift = systemAHBPrescalerTable[div];
  systemClk.hclk  = clk >> shift;
  systemClk.core = systemClk.hclk;
  systemClk.systick = systemClk.hclk;

  /* for FreeRTOS */
  SystemCoreClock = systemClk.hclk;

  return;
}
int
SystemGetClockValue(systemClockFreq_t *p)
{
  int           result = -1;
  if(p) {
    memcpy(p, &systemClk, sizeof(systemClockFreq_t));
    result = 0;
  }

  return result;
}


uint32_t
SystemGetSystemTimer(void)
{
  return SYSTEM_TIMER_REG;
}
void
SystemWaitSystemTimer(uint32_t tout)
{
  uint32_t      t;
  t = SYSTEM_TIMER_REG;
  while((t - (SYSTEM_TIMER_REG)) < tout);
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

int
puts(const char *str)
{
}
