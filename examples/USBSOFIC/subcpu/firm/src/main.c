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

#include        "config.h"
#include        "system.h"

#include        "rtos.h"


#include        "usb_desc.h"

#include        "main.h"
#include        "intr.h"
#include        "fifo.h"
#include        "devGpio.h"
#include        "devCounter.h"
#include        "devUsart.h"
#include        "devI2c.h"
#include        "devSpi32.h"
#include        "si5351.h"

#include        "usbdif.h"
#include        "usb_cdcacm.h"
#include        "devUsb320a.h"



char            strVersionText[] = CONFIG_VERSION_FIRM_TEXT;


void            MainUartLoop(void);



/*rtosThreadId defaultTaskHandle;*/
rtosTaskId      idMain;

/*extern UART_HandleTypeDef huart3;*/

void
MainIncSystemTimer(void)
{
  return;
}



int             dUsbCdc;

const static rtosTaskInfo_t     mainTaskList[] = {
  /*** main task */
  {
    .pFunc = (rtosTaskFunc)MainTask,
    .pName = NULL,
    .priority = RTOS_PRI_IDLE,
    .szStack = 0x400,
  },
  /*** usb task */
  {
    .pFunc = (rtosTaskFunc)MainUsbdifTask,
    .pName = NULL,
    .priority = RTOS_PRI_NORMAL,
    .szStack = 0x100,
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


#define PRINTF_CDC_FIFO  0

#if PRINTF_CDC_FIFO
  UsbCdcAcmLoop(dUsbCdc);
#endif

#if 0
  {
    uint8_t           str[256];
    int               len;
    static int        cnt = 0;

    len = UsbCdcAcmRecv(dUsbCdc, str, 200);
    if(len > 0) {
      str[len] = '\0';
      printf(str);
      UsbCdcAcmSend(dUsbCdc, str, len);
    }
    UsbCdcAcmLoop(dUsbCdc);
  }
#endif

  return;
}

static void
MainIdleLoop(void)
{
  static uint32_t       tCnt, i;

  static uint32_t       pp;

  if(tCnt >= 1000) {
    systemClockFreq_t         p;
    tCnt = 0;
    i++;
    printf("idle %d\r\n", i);
#if 0
    SystemGetClockValue(&p);
    SystemDebugShowClockValue(&p);
#endif
  }
  tCnt++;
}


/*extern USBD_DescriptorsTypeDef  usbDescTbl;*/
static void
MainTask(void const * argument)
{
  //DevDmaInit(-1, 0, NULL);
  //MainInitSpi();

  MainInitUsart();
  //MainInitI2c();

  {
    systemClockFreq_t         p;
    SystemGetClockValue(&p);
    SystemDebugShowClockValue(&p);
  }
  //MainInitCounter();

  /* start all tasks */
  const static rtosTaskInfo_t     *p;
  p = mainTaskList+1;   /* skip index 0 which is main task, already executed */
  while(1) {
    if(!p->pFunc) break;
    RtosTaskCreate(p, NULL);
    p++;
  }

  puts("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n#-----\r\n");
  puts("# " CONFIG_PRODUCT_NAME_TEXT " was started\r\n");



  /* Infinite loop */
  while(1) {
    MainUartLoop();
    RtosTaskSleep(1);
    MainIdleLoop();
  }
}


int
MainEntry(void)
{
  extern const uint16_t gpioDefaultTbl[];
  systemClockFreq_t   clk;
  uint32_t            systick;

  //MainInitSi5351();
  //SystemChangeClockHigher();
  //SystemUpdateClockValue();

  //DevGpioSets(gpioDefaultTbl);

  IntrInit(0);

  /* enable, if the FreeRTOS is not used */
#if 0
  SystemGetClockValue(&clk);
  systick = clk.systick;
  SysTick_Config(systick/1000);
#endif

  //  NVIC_SetPriority(SysTick_IRQn, 15);
  NVIC_SetPriority(PendSV_IRQn, 0);
  NVIC_SetPriority(SysTick_IRQn, 0);

  /* System interrupt init*/
  NVIC_SetPriority(MemoryManagement_IRQn, 0);
  NVIC_SetPriority(BusFault_IRQn, 0);
  NVIC_SetPriority(UsageFault_IRQn, 0);
  NVIC_SetPriority(SVCall_IRQn, 0);
  NVIC_SetPriority(DebugMonitor_IRQn, 0);

#if 0
  /* adhoc */
  extern uint32_t       sectVectorStart[];
  SCB->VTOR = (uint32_t)&sectVectorStart;
#endif

  /* Start the first task */
  idMain = RtosTaskCreate(&mainTaskList[0], NULL);

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


void
MainInitI2c(void)
{
  /* uart initialize */
  devI2cParam_t       param;

  DevI2cInit(-1, NULL);

  memset(&param, 0, sizeof(param));
  param.speed = DEVI2C_SPEED_400KHZ;
  param.mode = DEVI2C_MODE_PIO;
  param.dma  = 0;
  param.intr = 0;

  DevI2cInit(I2C2_NUM, &param);

  NVIC_SetPriority(I2C2_EV_IRQn, 2);
  NVIC_EnableIRQ(I2C2_EV_IRQn);

  return;
}


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

  DevSpiInit(-1, NULL);

  param.clkmode = 0;
  param.bit = 8;

  /* USB storage */
  param.speed = 50000000;
  param.prescaler = 0;
  param.dmaTx = 0;      /* STM32H743 SPI has errata */
  param.dmaRx = 1;      /* do not use dma with both tx and rx */
  DevSpiInit(5, &param);

  return;
}




  /*************************************
   * usb
   */
static void
MainInitUsb(void)
{
  /* variables */
  usbdifInitParam_t     usbInitParam;
  extern uint8_t        usbDescDevice[];
  extern int            usbDescDeviceLen;
  extern uint8_t        usbDescDeviceQualifier[];
  extern int            usbDescDeviceQualifierLen;
  extern uint8_t        usbDescConfig[];
  extern int            usbDescConfigLen;
  extern usbdifStringDesc_t usbDescString;
  usbdifClassDef_t      class;
  int                   unit;
  int                   num = 0;       /* adhoc */

  /* global initialize */
  UsbifInit(-1, NULL);

  unit = 1;
  /* device initialize */
  usbInitParam.pDeviceDesc   = usbDescDevice;
  usbInitParam.lenDeviceDesc = usbDescDeviceLen;
  usbInitParam.pDeviceQualifierDesc   = usbDescDeviceQualifier;
  usbInitParam.lenDeviceQualifierDesc = usbDescDeviceQualifierLen;
  usbInitParam.pConfigDesc   = usbDescConfig;
  usbInitParam.lenConfigDesc = usbDescConfigLen;
  usbInitParam.numConfigDesc = 1;
  usbInitParam.pStringDescTbl[0] = usbDescString;
  UsbifInit(unit, &usbInitParam);

  /* cdc initialize */
#if USBDESC_ENABLE_CDCACM
  memset(&class, 0, sizeof(class));
  class.type = USBDIF_CLASSTYPE_CDC;
  class.cb = NULL;
  class.numClass = num++;       /* adhoc */
  class.ifnum = ((1<<(USBDESC_IF_CDC_DATA))
                 | (1<<(USBDESC_IF_CDC_CTRL)));
  class.epnum = (USBDIF_EPBIT(USBDESC_EP_CDC_OUT)
                 | USBDIF_EPBIT(USBDESC_EP_CDC_IN)
                 | USBDIF_EPBIT(USBDESC_EP_CDC_CTRL));
  class.pUserData = &usbcdcCb;
  dUsbCdc = UsbifRegisterClass(unit, &class);
#endif


  /* start usb middleware */
  UsbifStart(unit);

  /* start usb module */
  {
    devUsbParam_t    param;
    const static usbdifDevFifo_t  fifo = {
      /* OUT    0  1... */
      {      1024, 0},
      /* IN     0    1    2     3    4    5    6    7 */
      {       192, 128,  64, 1024, 256,  64, 256,  64}, };

    DevUsbInit(-1, NULL);

    param.dma = 0;

    DevUsbInit(unit, &param);
    DevUsbSetTRxFifo(unit, (usbdifDevFifo_t *)&fifo);

    GpioSetUsbEnable();

#if 0
    /* power control usb phy 3.3V power enable
     * see document section 6 PWR, PWR_CR3 */
    PWR_PTR->CR3 |= PWR_CR3_USB33DEN_YES;
#endif

    /* interrupt enable */
    NVIC_SetPriority(OTG_FS_IRQn, 5);
    NVIC_EnableIRQ(OTG_FS_IRQn);
  }

  return;
}
static void
MainUsbdifTask(void const * argument)
{

  MainInitUsb();

  /* Infinite loop */
  while(1) {
    RtosTaskSleep(4000);
  }
}



/*
 *   XO -- PLLA -- MULTISYNTH0 -- DIV_R0 -- CLK0
 *   XO -- PLLB -- MULTISYNTH2 -- DIV_R2 -- CLK2
 *
 *  Limitations PLL
 *    fCLKIN      =  10 --  40MHz after input divider (divCLKIN=1,2,4,8)
 *    fVCO        = 600 -- 900MHz
 *    N/M         = (15+0/1048575), and up to (90*0/1048575)
 *
 *  Limitations MULTITYNTH
 *    fMultisynth = 0.5 -- 200MHz
 *    Multisyth   = 4, 6, 8, (8+1/1048575), and up to (900+0/1)
 *
 *  Limitations OUTPUT
 *    output div  = 1, 2, 4, 8, 16, 32, 64, 128
 *
 *  fOUT = fVCO / (Multisynth * R)
 *
 *  fVCO = fXTAL * (a + b/c)
 *
 *   8+1/1048575 -- 900*0/1
 *
 *    VCO(a, b, c) = (30, 402, 1625)
 * 24.576MHz*(32+0/1)    = 786.432MHz = 26MHz * 786432/26000 = 26 * (30 + 402/1625)
 *    DIV(a, b, c) = (32, 0, 1)
 * 24.000MHz*(32+96/125) = 786.432MHz = 26MHz * 786432/26000 = 26 * (30 + 402/1625)
 *    DIV(a, b, c) = (32, 96, 125)
 *
 */
void
MainInitSi5351(void)
{

  Si5351Init(0,
             26000000,                  // IN
             786432000, 0,              // VCO0, VCO1
             24576000, 0, 24000000,     // OUT0, 1, 2
             0);

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

#if PRINTF_CDC_FIFO
  UsbCdcAcmSend(dUsbCdc, &c, 1);
#endif



  return;
}
int
puts(const char *str)
{
  int           len;

  len = strlen(str);
  DevUsartSend(CONFIG_SYSTEM_USART_PORT, (uint8_t *)str, len);

#if PRINTF_CDC_FIFO
  len = strlen(str);
  UsbCdcAcmSend(dUsbCdc, str, len);
#endif

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
  /* SOF pulse */
  if(sr & TIM_SR_CC4IF_MASK) {
    UsbdevSofEntry(0);
  }

  return;
}


