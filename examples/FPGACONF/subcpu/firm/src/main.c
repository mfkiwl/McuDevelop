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

#define _MAIN_C_

#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>

#include        "config.h"
#include        "system.h"

#include        "rtos.h"

#include        "main.h"
#include        "intr.h"
#include        "fifo.h"
#include        "fpgaconf.h"
#include        "sdmmc.h"

char            strVersionText[] = CONFIG_VERSION_FIRM_TEXT;
int             __errno;

void            MainUartLoop(void);



void
MainIncSystemTimer(void)
{
  return;
}




const static rtosTaskInfo_t     mainTaskList[] = {
  /*** main task */
  {
    .pFunc = (rtosTaskFunc)MainTask,
    .pName = "main",
    .priority = RTOS_PRI_LOW,
    .szStack = 0x400,
  },
  /*** fpga config task */
  {
    .pFunc = (rtosTaskFunc)MainFpgaConfigTask,
    .pName = "fpga",
    .priority = RTOS_PRI_HIGH,
    .szStack = 0x400,
  },
  /* end of list */
  {
    .pFunc = NULL,
    .pName = NULL,
    .priority = 0,
    .szStack = 0,
  },
};



void
MainUartLoop(void)
{

#if 1
  {
    uint8_t           str[128];
    int               size;

    DevUsartLoop(CONFIG_SYSTEM_USART_PORT);

    if(DevUsartGetDataLen(CONFIG_SYSTEM_USART_PORT) > 0) {
      size = DevUsartRecv(CONFIG_SYSTEM_USART_PORT, str, sizeof(str)-1);
      if(size > 0) {
        str[size] = '\0';
        puts(str);
      }
    }
  }
#endif
}

static void
MainIdleLoop(void)
{
  static uint32_t       tCnt, i;

  if(tCnt >= 1000) {
    systemClockFreq_t         p;
    tCnt = 0;

#if CONFIG_RTOS_DEBUG_SHOW_STACK
    {
      uint8_t   strTaskList[512];
      vTaskList(strTaskList);
      puts(strTaskList);
    }
#endif

    //printf("idle %d\r\n", i);
    i++;
  }
  tCnt++;
}


static void
MainFpgaConfigTask(void const * argument)
{
  FpgaconfMain();
}


static void
MainShowClockValue(void)
{
  systemClockFreq_t         p;

  SystemGetClockValue(&p);
  SystemDebugShowClockValue(&p);

  return;
}


static void
MainTask(void const * argument)
{
  DevDmaInit(-1, 0, NULL);

  MainInitUsart();
  MainInitSpi();
  //MainInitSdmmc();
  MainInitFmc();

  //MainInitI2c();
  //MainInitCounter();

  puts("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n#-----\r\n");
  puts("# " CONFIG_PRODUCT_NAME_TEXT " was started\r\n");

  /* start all tasks */
  const static rtosTaskInfo_t   *pRtos;
  rtosStatus_t                  re;

  pRtos = &mainTaskList[1];   /* skip index 0 which is main task, already executed */
  while(pRtos->pFunc) {
    re = RtosTaskCreate(pRtos, RTOS_NULL, RTOS_NULL);
    if(re != RTOS_SUCCESS) {
      printf("MainTask() rtos entry \"%s\" exec fail(%d)\n", pRtos->pName, re);
    }
    pRtos++;
  }

  //MainShowClockValue();

  /* Infinite loop */
  while(1) {
    MainUartLoop();
    MainIdleLoop();

    RtosTaskSleep(1);
  }
}


int
MainEntry(void)
{
  extern const uint16_t gpioDefaultTbl[];
  systemClockFreq_t   clk;
  uint32_t            systick;

  SystemInit();

  SystemChangeClockHigher();
  SystemUpdateClockValue();

#if 0
  /* enable, if the FreeRTOS is not used */
  SystemGetClockValue(&clk);
  systick = clk.systick;
  SysTick_Config(systick/1000);
#endif

  DevGpioInit();
  DevGpioSets(gpioDefaultTbl);

  IntrInit(0);

  NVIC_SetPriority(PendSV_IRQn, 0);
  NVIC_SetPriority(SysTick_IRQn, 1);

  /* System interrupt init*/
  NVIC_SetPriority(MemoryManagement_IRQn, 0);
  NVIC_SetPriority(BusFault_IRQn, 0);
  NVIC_SetPriority(UsageFault_IRQn, 0);
  NVIC_SetPriority(SVCall_IRQn, 0);
  NVIC_SetPriority(DebugMonitor_IRQn, 0);

  /* Start the first task */
  RtosTaskCreate(&mainTaskList[0], RTOS_NULL, RTOS_NULL);

  RtosKernelStart();

  while (1);
}


static void
MainInitUsart(void)
{
  /* uart initialize */
  devUsartParam_t       param;

  DevUsartInit(-1, NULL);

  param.baud = CONFIG_SYSTEM_USART_BAUD;
  param.bit = DEVUSART_BIT_8;
  param.stop = DEVUSART_STOP_1;
  param.mode = DEVUSART_MODE_PIO_FIFO;
  param.parity = DEVUSART_PARITY_NONE;
  param.szFifoTx = 10;
  param.szFifoRx = 10;
  DevUsartInit(CONFIG_SYSTEM_USART_PORT, &param);

  return;
}


#if 0
void
MainInitI2c(void)
{
  /* uart initialize */
  devI2cParam_t       param;

  memset(&param, 0, sizeof(param));
  param.speed = DEVI2C_SPEED_400KHZ;
  param.mode = DEVI2C_MODE_PIO;
  param.dma  = 0;
  param.intr = 0;

  NVIC_SetPriority(I2C2_EV_IRQn, 2);
  NVIC_EnableIRQ(I2C2_EV_IRQn);

  return;
}
#endif


static void
MainInitCounter(void)
{
  devCounterParam_t     param;
  systemClockFreq_t     clk;
  int                   div;

  SystemGetClockValue(&clk);
  div = clk.hclk / 48000 / 2;

  DevCounterInit(-1, NULL);

  /********************************************************
   * TIM4:  ADC converter sampling timing
   * PWM, AD timing generator,  98,304kHz / 2048 = 48kHz duty 50% clock
   */
  memset(&param, 0, sizeof(param));
  param.chnum = (DEVCOUNTER_SETCH(DEVCOUNTER_CH_CLKTRG) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_4));
  param.clktrg.mode = (DEVTIME_CLKTRG_MODE_FREERUN |
                       DEVTIME_CLKTRG_CTG_INTERNAL | DEVTIME_CLKTRG_SEL(0));
  param.clktrg.reload = div;
  param.ch.val  = div>>1;
  param.ch.mode = DEVCOUNTER_MODE_PWM;

  DevCounterInit(4, &param);
  TIM4_PTR->CR2 = 0x70;         /* extra command: OC4REF(TRGO) out for dma */

#if 0
  TIM4_PTR->DIER |= 0x02;
  __NVIC_SetPriority(TIM4_IRQn, 0);
  __NVIC_EnableIRQ(TIM4_IRQn);
#endif


  /********************************************************
   *  TIM2: 24.576MHz (49.152?, 98.304?)
   *        CNT  freerun  decrement counter
   *        CH1  GPS PPS counter
   *        CH2  xxx
   *        CH3
   *        CH4  SOF input (from FS SOF OUT)
   */
#if 1
  TIM2_PTR->SMCR  = 0x0;
  TIM2_PTR->ARR   = 0xffffffff;
  TIM2_PTR->PSC   = 0;
  TIM2_PTR->CCMR1 = 0x2121; /* ch1,2 input capture */
  TIM2_PTR->CCMR2 = 0x0121; /* ch3,4 input capture */
  TIM2_PTR->CCER  = 0xb101;
  TIM2_PTR->DIER = 0x1a;
  TIM2_PTR->TISEL = 0;
  TIM2_PTR->CR1 = 0x11;

  NVIC_SetPriority(TIM2_IRQn, 2);
  NVIC_EnableIRQ(TIM2_IRQn);
#endif


  /********************************************************
   * TIM5   freerun counter
   */
  memset(&param, 0, sizeof(param));
  param.chnum = DEVCOUNTER_SETCH(DEVCOUNTER_CH_CLKTRG);
  param.clktrg.mode = (DEVTIME_CLKTRG_MODE_FREERUN |
                       DEVTIME_CLKTRG_CTG_INTERNAL | DEVTIME_CLKTRG_SEL(0));
  param.clktrg.prescaler = 197;
  param.clktrg.reload = 0xffffffff;
  param.clktrg.down = 1;
  DevCounterInit(5, &param);


  /********************************************************
   *  TIM8, TIM3  write counter
   *  NWE -- [TIM8_ETR, CH2OUT(PWM:1/2)] -- [TIM3_CH3(IC), INTR] -- DMA
   *  [TIM3CH1,2,4, PWM]
   */
#define DIV     2
  memset(&param, 0, sizeof(param));
  param.chnum = (DEVCOUNTER_SETCH(DEVCOUNTER_CH_CLKTRG) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_2));
  param.clktrg.mode = (DEVTIME_CLKTRG_MODE_FREERUN |
                       DEVTIME_CLKTRG_CTG_EXTERNAL2 | DEVTIME_CLKTRG_SEL(0));
  param.clktrg.filter = 2;
  param.clktrg.reload = DIV-1;
  param.ch.val  = DIV/2;
  param.ch.mode = DEVCOUNTER_MODE_PWM;
  DevCounterInit(8, &param);


  /********************************************************
   * TIM3
   */
  memset(&param, 0, sizeof(param));
  /* TIM3 CH3 */
  param.chnum = (DEVCOUNTER_SETCH(DEVCOUNTER_CH_CLKTRG) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_3));
  param.clktrg.mode = (DEVTIME_CLKTRG_MODE_FREERUN |
                       DEVTIME_CLKTRG_CTG_INTERNAL | DEVTIME_CLKTRG_SEL(0));
  param.clktrg.filter = 2;
  param.clktrg.polneg = 1;
  param.clktrg.reload = 0xffff;
  param.ch.val  = DIV/2;
  param.ch.mode = DEVCOUNTER_MODE_IC;
  param.ch.intr = 1;
  param.ch.filter = 1;
  param.ch.dma = 1;
  DevCounterInit(3, &param);

  /* TIM3CH1,2,4  are PWM */
  param.chnum = (DEVCOUNTER_SETCH(DEVCOUNTER_CH_1) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_2) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_4));
  param.ch.mode = DEVCOUNTER_MODE_PWM;
  param.ch.intr = 0;
  param.ch.filter = 0;
  param.ch.dma = 0;
  param.ch.val  = 0xffff;
  DevCounterInit(3, &param);

  DevCounterSetPwmDutyValue(3, 1, 0xffff);
  DevCounterSetPwmDutyValue(3, 2, 0x7fff);
  DevCounterSetPwmDutyValue(3, 4, 0x0fff);

#if 0
  __NVIC_SetPriority(TIM3_IRQn, 0);
  __NVIC_EnableIRQ(TIM3_IRQn);
#endif

  return;
}


static void
MainInitSpi(void)
/* spi initialize */
{
  devSpiParam_t       param;

  DevSpiInit(SPI_NUM_INIT, NULL);

  memset(&param, 0, sizeof(param));

  param.clkmode = 3;
  param.bit = 8;
  //param.mode = DEVSPI_MODE_DMA;
  param.speed = 33000000;
  param.prescaler = 1;
  param.dmaTx = 0;
  param.dmaRx = 0;
  DevSpiInit(SPI1_NUM, &param);

#if 0
  // dummy send
  DevSpiSend(CONFIG_FPGA_SPI_DEVNUM, "", 1);
  DevSpiSend(CONFIG_FPGA_SPI_DEVNUM, "", 1);
#endif

  return;
}


static void
MainInitSdmmc(void)
{
  devSdmmcParam_t       param;

  DevSdmmcInit(SDMMC_NUM_INIT, NULL);

  memset(&param, 0, sizeof(param));

  param.hwFlow = 1;
  param.clkPolNeg = 0;
  param.clkPowerEn = 1;
  param.busWidth = 1;
  param.clk = 400000;

  DevSdmmcInit(SDMMC1_NUM, &param);

#if 0
  NVIC_SetPriority(SDMMC1_IRQn, 0);
  NVIC_EnableIRQ(SDMMC1_IRQn);
#endif

  return;
}


static void
MainInitFmc(void)
{
  stm32Dev_FMC_bank     *pBank;

  pBank = &FMC_PTR->B[0];

  for(int i = 0; i < STM32FMC_NUMBEROF_BANK; i++) {
    /* set mode-1 */
    pBank->CR = (FMC_BCR_FMCEN_YES |
                 FMC_BCR_ASYNCWAIT_NO |
                 FMC_BCR_WREN_YES |
                 FMC_BCR_MWID_8BIT |
                 FMC_BCR_MTYP_SRAM |
                 FMC_BCR_MBKEN_YES);
    // low term:  DATAST
    // high term: ADDSET+1
    // ex  DATAST=3,ADDST=2: 33MHz@HCLK=200MHz
    pBank->TR = FMC_BTR_DATAST(3) | FMC_BTR_ADDSET(2);
    pBank++;
  }

  return;
}
void
MainSetFmcWait(int unit, int act, int inact)
{
  stm32Dev_FMC_bank     *pBank;

  pBank = &FMC_PTR->B[unit];

  if(inact > 0) {
    if(inact < 2)       inact = 2;
    else if(inact > 15) inact = 15;
    inact--;
    pBank->TR &= ~FMC_BTR_ADDSET_MASK;
    pBank->TR |=  FMC_BTR_ADDSET(inact);
  }

  if(act > 0) {
    if(act > 15) act = 15;
    pBank->TR &= ~FMC_BTR_DATAST_MASK;
    pBank->TR |=  FMC_BTR_DATAST(act);
  }

  return;
}







/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  while(1);
}



char            c1;
void
_putchar(char c)
{
  DevUsartSend(CONFIG_SYSTEM_USART_PORT, &c, 1);

  return;
}
int
puts(const char *str)
{
  int           len;

  len = strlen(str);
  DevUsartSend(CONFIG_SYSTEM_USART_PORT, (uint8_t *)str, len);

  return len;
}


void
TIM2_IRQHandler(void)
{
  uint32_t              sr;
  static uint32_t       ch1, ch3, prevCh1, prevCh3;

  sr = TIM2_PTR->SR;
  TIM2_PTR->SR = sr;

  /* GPS pps pulse */
  if(sr & TIM_SR_CC1IF_MASK) {
    ch1 = TIM2_PTR->CCR1;
    printf("TIM2->CCR1 %x %d\r\n", ch1, prevCh1 - ch1);
    prevCh1 = ch1;
  }
  if(sr & TIM_SR_CC3IF_MASK) {
    ch3 = TIM2_PTR->CCR3;
    printf("TIM2->CCR3 %x %d\r\n", ch3, prevCh3 - ch3);
    prevCh3 = ch3;
  }

  return;
}


void
MainIntrruptSpi1Tx(void)
{
  return;
}

void
MainIntrruptSpi1Rx(void)
{
  return;
}
