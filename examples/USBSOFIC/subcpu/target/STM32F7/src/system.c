/*
 * Copyright (c) 2019 zhtlab
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

#define _SYSTEM_STM32H7_C_
#define _SYSTEM_C_

#include        <stdio.h>
#include        <stdint.h>
#include        <string.h>

#include        "config.h"
#include        "system.h"
#include        "devGpio.h"
#include        "devCounter.h"
#include        "stm32f7Rcc.h"
#include        "devCrs.h"

void            MainInitI2c(void);
void            MainInitSi5351(void);


#define SYSTEM_TIMER_REG        (TIM2_PTR->CNT)     /* decrement counter */

static systemClockFreq_t        systemClk;
uint32_t                        SystemCoreClock; /* used in the CMSIS */


void
SystemInit(void)
{
  //SystemChangeClockDefault();

  /*********************************
   * cache control
   */
  //SystemMpuConfig();
  SCB_EnableICache();
  //SCB_EnableDCache();


  /*********************************
   * fpu control
   */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  __DSB();
  __ISB();      /* reset the instruction pipe line and enable FPU */
#endif


  /*********************************
   * clock control
   */
  //systemClockFreq_t     clk;
  RCC_PTR->APB2ENR |= RCC_APB2ENR_SYSCFGEN_YES;
  SystemUpdateClockValue();

#if 0
  devCrsParam_t     crs;
  /* Clock recovery base clock is set enable */
  RCC_PTR->APB1HENR |= (RCC_APB1HENR_CRSEN_YES);

  /* reset once */
  (RCC_PTR->APB1HRSTR) |=  RCC_APB1HRSTR_CRSRST_YES;
  (RCC_PTR->APB1HRSTR) &= ~RCC_APB1HRSTR_CRSRST_MASK;

  crs.src = CRS_CLOCK_SOURCE_USB2;
  DevCrsInit(&crs);
#endif


  /*********************************
   * clock gate control
   */

  /* pwr */
  RCC_PTR->APB1ENR |= RCC_APB1ENR_PWREN_YES;

  /* gpio */
  RCC_PTR->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN_YES | RCC_AHB1ENR_GPIOBEN_YES |
                       RCC_AHB1ENR_GPIOCEN_YES | RCC_AHB1ENR_GPIODEN_YES |
                       RCC_AHB1ENR_GPIOEEN_YES | RCC_AHB1ENR_GPIOFEN_YES |
                       RCC_AHB1ENR_GPIOGEN_YES | RCC_AHB1ENR_GPIOHEN_YES |
                       RCC_AHB1ENR_GPIOIEN_YES);
  /* i2c */
  RCC_PTR->APB1ENR |= RCC_APB1ENR_I2C2EN_YES;

  /* usart */
  RCC_PTR->APB2ENR |= RCC_APB2ENR_USART1EN_YES;


  /*********************************
   * gpio initialize
   */
  DevGpioInit();
  DevGpioSets(gpioDefaultTbl);

  /* timer */
  RCC_PTR->APB1ENR |= RCC_APB1ENR_TIM2EN_YES;
  RCC_PTR->APB1ENR |= RCC_APB1ENR_TIM3EN_YES;
  RCC_PTR->APB1ENR |= RCC_APB1ENR_TIM5EN_YES;
  RCC_PTR->APB2ENR |= RCC_APB2ENR_TIM8EN_YES;
  MainInitCounter();


  /*********************************
   * external pll control
   */
  MainInitI2c();
  MainInitSi5351();

  /* wait about 20ms @16MHz */
  for(int i = 0; i < 20000; i++) RCC_PTR->CR;

  /* switch to higher freq  24MHz -> 192MHz */
  SystemChangeClockHigher();
  SystemUpdateClockValue();

  /* usb, otg */
  RCC_PTR->APB2ENR |= RCC_APB2ENR_OTGPHYCEN_YES;
  RCC_PTR->AHB1ENR |= RCC_AHB1ENR_OTGHSEN_YES;

  return;
}


void
SystemLoop(void)
{
  return;
}



void
SystemChangeClockDefault(void)
{
  /* enable HSION bit */
  RCC_PTR->CR |= RCC_CR_HSION_YES;

  RCC_PTR->CFGR = 0;
  RCC_PTR->CR &= ~(RCC_CR_HSEON_MASK | RCC_CR_CSSON_MASK |
               RCC_CR_PLLON_MASK);

  /* reset the following registers */
  RCC_PTR->PLLCFGR = 0x24003010;
  RCC_PTR->CR &= ~RCC_CR_HSEBYP_MASK;
  RCC_PTR->CIR = 0;
  while(!(RCC_PTR->CR & (RCC_CR_HSIRDY_MASK)));

  return;
}
void
SystemChangeClockHigher(void)
{
  uint32_t              val;

  /* change flash access time */
  /* 2.7V -- 3.6V
   *  (  0 -  30]: 0WS
   *  ( 30 -  60]: 1WS
   *  ( 60 -  90]: 2WS
   *  ( 90 - 120]: 3WS
   *  (120 - 150]: 4WS
   *  (150 - 180]: 5WS
   *  (180 - 210]: 6WS
   */
  FLASH_PTR->ACR = FLASH_LATENCY_CLK(CONFIG_CLOCK_FREQ_CPU/30000001);

  /** change core voltage */
  //PWR_PTR->CR1 |= PWR_CR1_VOS_RANGE3_144MHZ;
  //while(!(PWR_PTR->D3CR & PWR_D3CR_VOSRDY_MASK));

  /*** select clock sources */
  RCC_PTR->CR |= RCC_CR_HSEON_YES| RCC_CR_HSEBYP_YES;
  while(!(RCC_PTR->CR & (RCC_CR_HSERDY_MASK)));

  /* pll1 settings  vco range (100-432MHz)
   * HSE=24MHz
   * M=12(2MHz), N=96(192MHz)
   * P=2(96MHz), Q=8(48MHz)
   */
  RCC_PTR->PLLCFGR  = (RCC_PLLCFGR_PLLSRC_HSE |
                       RCC_PLLCFGR_PLLM_DIV((CONFIG_CLOCK_HSE)/2000000) |
                       RCC_PLLCFGR_PLLN_MULX((CONFIG_CLOCK_FREQ_CPU)/1000000) |
                       RCC_PLLCFGR_PLLP_DIV2 | RCC_PLLCFGR_PLLQ_DIV(8) );
  RccPll1Enable();

  /* clock divider */
  /* APB1 is up to  54MHz
   * APB2 is up to 108MHz
   */
  RCC_PTR->CFGR = (RCC_CFGR_HPRE_DIV1 |
                   RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2);

  /* the system clock is selected to PLL1 */
  RCC_PTR->CFGR |= RCC_CFGR_SW_PLL;
  while((RCC_PTR->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL);

  /*** MCO setting */
#if CONFIG_MCO_ENABLE
  val = RCC_PTR->CFGR;
  val &= ~(RCC_CFGR_MCO2SEL_MASK | RCC_CFGR_MCO2PRE_MASK |
           RCC_CFGR_MCO1SEL_MASK | RCC_CFGR_MCO1PRE_MASK);
  val |= RCC_CFGR_MCO1SEL_PLL | RCC_CFGR_MCO1PRE_DIV2;
  val |= RCC_CFGR_MCO2SEL_SYSCLK | RCC_CFGR_MCO2PRE_DIV2;
  RCC_PTR->CFGR = val;
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
static void
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
void
SystemDebugShowClockValue(systemClockFreq_t *p)
{
  printf("# clock info\r\n");
  printf("  clkin: %d\r\n", p->pllin);
  printf("  vco: %d\r\n", p->pll1.vco);
  printf("  pll1p: %d, q: %d\r\n",
         p->pll1.P, p->pll1.Q);
#if 0
  printf("  pll2p: %d, q: %d, r: %d\r\n",
         p->pll2.P, p->pll2.Q, p->pll2.R);
  printf("  pll3p: %d, q: %d, r: %d\r\n",
         p->pll3.P, p->pll3.Q, p->pll3.R);
#endif
  printf("  core:  %d, systick(x8): %d, hclk: %d\r\n",
         p->core, p->systick, p->hclk);
  printf("  pclk1: %d, pclk2: %d, pclk3: %d, pclk4: %d\r\n",
         p->pclk1, p->pclk2, p->pclk3, p->pclk4);

  return;
}

/*
 * see the AN4838 document
 */
void
SystemMpuConfig(void)
{
  /* disable memory fault  and  disable mpu */
  SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;
  MPU->CTRL  &= ~MPU_CTRL_ENABLE_Msk;

  /* 0x60000000 (512bytes) cache disable */
  MPU->RNR  = 0;         /* region number 0 */
  MPU->RBAR = 0x60000000;
  MPU->RASR = ((1<<MPU_RASR_XN_Pos) |   /* instruction access disable */
               (3<<MPU_RASR_AP_Pos) |   /* full access */
               (0<<MPU_RASR_TEX_Pos) |  /* level 0 */
               (0<<MPU_RASR_C_Pos) |    /* cachable: no */
               (1<<MPU_RASR_S_Pos) |    /* shared: yes */
               (1<<MPU_RASR_B_Pos) |    /* buffer: yes */
               (0<<MPU_RASR_SRD_Pos) |  /* sub rigion */
               (8<<MPU_RASR_SIZE_Pos) | /* 2^(n+1), min 4 (32bytes) */
               (1<<MPU_RASR_ENABLE_Pos) /* enable */
               );

  /* enable mpu  and  enable memory fault */
  MPU->CTRL   = MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_ENABLE_Msk;
  SCB->SHCSR  = SCB_SHCSR_MEMFAULTENA_Msk;

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


struct _stSystemMallocInfo {
  int                           size;
  void                          *vp;
  struct _stSystemMallocInfo    *next;
  uint32_t                      pad[1];
  /*** the following data is malloc buffer
       uint32_t                    buf[1];*/
};
static struct _stSystemMallocInfo     *pinfo[2] = {
  (struct _stSystemMallocInfo *)0x30000000,
  (struct _stSystemMallocInfo *)0x30020000,
};
static int      szRemain[2] = {
  0x20000,
  0x20000,
};
void *
SystemMallocStreamBuf(int type, int size, void *vp)
{
  void          *p = NULL;
  uint32_t      next;

  if(type >= 2) goto fail;

  if(szRemain[type] < (size + sizeof(struct _stSystemMallocInfo))) goto fail;

  pinfo[type]->size = size;
  pinfo[type]->vp = vp;
  p = (void *)(pinfo[type]+1);
  next  = (size + 15) & ~15;
  next += (uint32_t) p;
  pinfo[type]->next = (struct _stSystemMallocInfo *) next;

  pinfo[type] = pinfo[type]->next;

fail:
  return p;
}


void
SystemInitSystemTimer(void)
{
  devCounterParam_t     param;

#if 0
  memset(&param, 0, sizeof(param));
  param.chnum = DEVCOUNTER_SETCH(DEVCOUNTER_CH_CLKTRG);
  param.clktrg.mode = (DEVTIME_CLKTRG_MODE_FREERUN |
                       DEVTIME_CLKTRG_CTG_INTERNAL | DEVTIME_CLKTRG_SEL(0));
  param.clktrg.prescaler = 0;
  param.clktrg.reload = 0xffffffff;
  param.clktrg.down = 1;
  DevCounterInit(TIM5_NUM, &param);
#endif

  return;
}

uint32_t
SystemGetSysCounter(void)
{
  return SYSTEM_TIMER_REG;
}
void
SystemWaitSysCounter(uint32_t tout)
{
  uint32_t      t;
  t = SYSTEM_TIMER_REG;
  while((t - (SYSTEM_TIMER_REG)) < tout);
  return;
}


void
SystemSysTickIntr(void)
{
  GpioSetUpdateLedOn();
  GpioSetUpdateLedOff();
  SysTick->VAL;

  return;
}

extern void *sectHeapStart;
static void *ptrMalloc = &sectHeapStart;
void *
malloc(int size)
{
  void *p;
  p = ptrMalloc;
  ptrMalloc += (size + 0xf) & ~0xf;
  return p;
}

void
free(void *ptr)
{
  return;
}
