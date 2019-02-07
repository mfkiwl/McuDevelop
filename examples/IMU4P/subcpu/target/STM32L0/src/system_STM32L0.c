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
#define _SYSTEM_C_

#include        <stdint.h>

#include        "config.h"
#include        "system.h"

#include        "gpio.h"

static systemClockFreq_t        systemClk;
uint32_t                        systemCoreClock; /* used in the CMSIS */



void    SystemChangeClockHigher(void);

void
SystemInit(void)
{
  uint32_t      val;
  uint32_t      *p, *pS, *pE;
  int           i;

  extern uint32_t       __vector_table[];
  SCB->VTOR = (uint32_t)__vector_table;

  /**************************************************
   * clock change and gate control
   */
  /* pwr, crs */
  RCC_PTR->APB1ENR |=  RCC_APB1ENR_PWREN_YES;
  RCC_PTR->APB1ENR |=  RCC_APB1ENR_CRSEN_YES;

  val  = PWR_PTR->CR;
  val &= ~PWR_CR_VOS_MASK;
  val |=  PWR_CR_VOS_RANGE1_1800MV;
  PWR_PTR->CR = val;
  while(PWR_PTR->CSR & PWR_CSR_VOSF_MASK);


  /* change the system clock */
  SystemChangeClockHigher();
  SysTick_Config(CONFIG_CLOCK_FREQ_CPU/1000);

  /* usb clock */
  RCC_PTR->APB2ENR |= RCC_APB2ENR_SYSCFGEN_YES;
  SYSCFG_PTR->CFGR3 |= (SYSCFG_CFGR3_ENREF_HSI48_YES |
                        SYSCFG_CFGR3_EN_VREFINT_YES);
  RCC_PTR->CRRCR |= RCC_CRRCR_HSI48ON_YES;
  while(!(RCC_PTR->CRRCR & RCC_CRRCR_HSI48RDY_MASK));

  /* calc the clock value */
  SystemUpdateClockValue();

  RCC_PTR->IOPENR = (RCC_IOPENR_IOPAEN_YES |
                     RCC_IOPENR_IOPBEN_YES |
                     RCC_IOPENR_IOPCEN_YES |
                     RCC_IOPENR_IOPDEN_YES |
                     RCC_IOPENR_IOPHEN_YES);

  /* peripheral clock select */
  RCC_PTR->CCIPR &= ~RCC_CCIPR_USART1SEL_MASK;
  RCC_PTR->CCIPR |=  RCC_CCIPR_USART1SEL_PCLK;
  RCC_PTR->CCIPR &= ~RCC_CCIPR_USART2SEL_MASK;
  RCC_PTR->CCIPR |=  RCC_CCIPR_USART2SEL_PCLK;
  /* peripheral enable */
  RCC_PTR->APB2ENR |= RCC_APB2ENR_USART1EN_YES;
  RCC_PTR->APB2ENR |= RCC_APB2ENR_SPI1EN_YES;
  RCC_PTR->APB1ENR |= RCC_APB1ENR_TIM2EN_YES;

  /* usb clock select, crs, power select */
  RCC_PTR->CCIPR   &= ~RCC_CCIPR_CLK48SEL_MASK;
  RCC_PTR->CCIPR   |=  RCC_CCIPR_CLK48SEL_HSI48;
  RCC_PTR->APB1ENR |= RCC_APB1ENR_USBFSEN_YES;
#ifdef _STM32L4_H_
  PWR_PTR->CR2 |= PWR_CR2_USV_YES;
#endif

  /* flash control */
  RCC_PTR->AHB1ENR = RCC_AHB1ENR_FLASHEN_YES;

  /* dma */
  RCC_PTR->AHB1ENR |= (RCC_AHB1ENR_DMA1EN_YES);

  /* reset once */
  RCC_PTR->APB1RSTR |=  RCC_APB1RSTR_CRSRST_YES;
  RCC_PTR->APB1RSTR &= ~RCC_APB1RSTR_CRSRST_MASK;

  return;
}


void
SystemDeinit(void)
{
  RCC_PTR->APB1ENR = 0;
  RCC_PTR->APB2ENR = 0;
  RCC_PTR->AHB1ENR = RCC_AHB1ENR_MIFEN_MASK;

  RCC_PTR->CR = 0x0c00;
  while(!(RCC_PTR->CR & RCC_CR_MSIRDY_MASK));

  /* clock divider select */
  RCC_PTR->CFGR |= RCC_CFGR_SW_MSI;
  while((RCC_PTR->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_MSI);

  RCC_PTR->CSR = 0x0c000000;

  SCB->VTOR = 0x0;

  return;
}


void
SystemLoop(void)
{

  return;
}




void
SystemGpioAnalogInit(int group, int bit)
{
  uint32_t              val;

  val = GPIO_PTR[group].MODER;
  val &= ~MODE_MASK(bit);
  val |=  MODE_ANALOG(bit);
  GPIO_PTR[group].MODER = val;

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
  /* clock wait for to access the flash rom
   * set 1WS, if HREF is over 16MHz
   */
  FLASH_PTR->ACR = FLASH_ACR_LATENCY_YES;

  /* HSI16 enable */
  RCC_PTR->CR |= RCC_CR_HSI16ON_YES /*| RCC_CR_HSI16DIVEN_YES*/;
  RCC_PTR->CFGR &= ~(RCC_CFGR_PPRE2_MASK | RCC_CFGR_PPRE1_MASK |
                     RCC_CFGR_HPRE_MASK);
  RCC_PTR->CFGR |=  (RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1 |
                     RCC_CFGR_HPRE_DIV1);
  while(!(RCC_PTR->CR & RCC_CR_HSI16RDYF_MASK));

  /*** adhoc  PLL is work, but code is not work, it occured exception */

  /* set pll factor and start */
  /* pll select  16MHz(HSI16) * 4 / 2 = 32MHz (max 32MHz) */
  RCC_PTR->CFGR |= (RCC_CFGR_PLLDIV_DIV2 | RCC_CFGR_PLLMUL_X4 |
                    RCC_CFGR_PLLSRC_HSI16);
  RCC_PTR->CR |= RCC_CR_PLLON_YES;
  while(!(RCC_PTR->CR & RCC_CR_PLLRDY_MASK));

  /* clock divider select */
  RCC_PTR->CFGR |= RCC_CFGR_SW_PLL;
  while((RCC_PTR->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL);

  /* LSI clock  for 32.768KHz timestamp test */
  RCC_PTR->CSR |= RCC_CSR_LSION_YES;
  while(!(RCC_PTR->CSR & RCC_CSR_LSIRDY_MASK));

#if CONFIG_MCO_ENABLE
  RCC_PTR->CFGR &= ~(RCC_CFGR_MCOPRE_MASK | RCC_CFGR_MCOSEL_MASK);
  RCC_PTR->CFGR |= (RCC_CFGR_MCOPRE_DIV1 | RCC_CFGR_MCOSEL_LSI);
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


#define CALCPLL(reg, no, freqPllIn)      {                              \
  int div;                                                              \
  m = ((((reg) & RCC_PLLCFGR_PLLM_MASK) >> RCC_PLLCFGR_PLLM_SHIFT) + 1);        \
  n = ( ((reg) & RCC_PLLCFGR_PLLN_MASK) >> RCC_PLLCFGR_PLLN_SHIFT);             \
  vco = (freqPllIn) / m * n;                                            \
  systemClk.pll ##no.vco = vco;                                         \
\
}


const uint8_t   systemAHBPrescalerTable[]  = RCC_CLOCK_HPRETABLE;
const uint8_t   systemPeriPrescalerTable[] = RCC_CLOCK_PPRETABLE;

static void
SystemUpdateClockValue(void)
{
  int           div, shift;
  int           freqPllIn;
  int           m, n;
  int           vco;
  const uint8_t pllmul[] = RCC_PLL_N_TBL;

  /*** PLL clock source selector */
  switch(RCC_PTR->CFGR & RCC_CFGR_PLLSRC_MASK) {
  case RCC_CFGR_PLLSRC_HSI16: freqPllIn = RCC_CLOCK_HSI;  break;
  case RCC_CFGR_PLLSRC_HSE:   freqPllIn = CONFIG_CLOCK_HSE; break;
  }
  systemClk.pllin = freqPllIn;


  /*** freq_VCO = (freq_PLLIN / M) * N,   freq_SYSCLK = freq_VCO / DIVP */
  /*** calc PLL1 vco frequency value */
  systemClk.pll1.vco  = systemClk.pllin;
  systemClk.pll1.vco *= pllmul[(RCC_PTR->CFGR & RCC_CFGR_PLLMUL_MASK) >> RCC_CFGR_PLLMUL_SHIFT];
  if((RCC_PTR->CFGR & RCC_CFGR_PLLDIV_MASK) == RCC_CFGR_PLLDIV_INVALID0) {
    while(1);
  }
  systemClk.pll1.out  = systemClk.pll1.vco;
  systemClk.pll1.out /= (((RCC_PTR->CFGR & RCC_CFGR_PLLDIV_MASK) >> RCC_CFGR_PLLDIV_SHIFT) + 1);

  /* calc core clock */
  SystemCoreClockUpdate();

  /* AHB1, and SYSTICK */
  div   = (RCC_PTR->CFGR &  RCC_CFGR_HPRE_MASK) >> RCC_CFGR_HPRE_SHIFT;
  shift = systemAHBPrescalerTable[div];
  systemClk.hclk      = systemClk.sysclk >> shift;
  systemClk.systick   = systemClk.sysclk;
  systemClk.systick >>= SYSTEM_CLOCK_FREQ_DIV_SYSTICK_SHIFT;

  /*APB1, APB2 */
  div   = (RCC_PTR->CFGR &  RCC_CFGR_PPRE1_MASK) >>  RCC_CFGR_PPRE1_SHIFT;
  shift = systemPeriPrescalerTable[div];
  systemClk.pclk1 = systemClk.hclk >> shift;

  div   = (RCC_PTR->CFGR &  RCC_CFGR_PPRE2_MASK) >>  RCC_CFGR_PPRE2_SHIFT;
  shift = systemPeriPrescalerTable[div];
  systemClk.pclk2 = systemClk.hclk >> shift;

  return;
}
static void
SystemCoreClockUpdate(void)
{
  int           div, shift;
  uint32_t      clk;

  switch (RCC_PTR->CFGR & RCC_CFGR_SWS_MASK) {
  case RCC_CFGR_SWS_HSI:
    clk = RCC_CLOCK_HSI;        /* HSI16 */
    break;

  case RCC_CFGR_SWS_HSE:
    clk = CONFIG_CLOCK_HSE;
    break;

  case RCC_CFGR_SWS_PLL:
    clk = systemClk.pll1.out;
    break;
  default:
  /*case CFGR_SWS_MSI:*/
    clk = RCC_CLOCK_MSI_4MHZ;
  }

  /* HCLK frequency */
  systemClk.sysclk = clk;
  systemCoreClock = clk;

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
void
SystemDebugShowClockValue(systemClockFreq_t *p)
{
  printf("# clock info\n");
  printf("  pll in %d\n", p->pllin);
  printf("  pll1 vco: %d, out: %d\n",
         p->pll1.vco, p->pll1.out);

  printf("  sysclk:  %d, systick: %d, hclk: %d\n",
         p->sysclk, p->systick, p->hclk);

  printf("  pclk1: %d, pclk2: %d\n",
         p->pclk1, p->pclk2);

  return;
}



void *
malloc(int size)
{
  return NULL;
}

void
free(void *ptr)
{
  return;
}

