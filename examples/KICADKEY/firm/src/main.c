/*
 * Copyright (c) 2018,2019 zhtlab
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

#include        <stdint.h>
#include        <stdio.h>

#include        "config.h"

#include        "system.h"
#include        "intr.h"
#include        "rtos.h"
#include        "devGpio.h"
#include        "devUsart.h"
#include        "devCounter.h"
#include        "gpio.h"
#include        "devCrs.h"
#include        "fifo.h"
#include        "command.h"

#include        "usb_desc.h"
#include        "usbdif.h"
#if USBDESC_ENABLE_CDCACM
#include        "usb_cdcacm.h"
int             dUsbCdc;
#endif
#include        "usb_hid.h"
#include        "kicad.h"
int             dUsbHid;
#include        "devUsb16.h"

#include        "main.h"

const uint8_t           strVersionText[] = CONFIG_VERSION_FIRM_TEXT;


static uint32_t         system1ms = 0;

void            MainEntry(void);
int             _putchar(char c);
int             _getc(void);

static void     MainTask(void const * argument);
rtosTaskId      idMain;

static void     MainInitUsart(void);
static void     MainInitTim(void);


void
SystemIncrement1ms(void)
{
  system1ms--;

  return;
}

 /*__ATTR_INTR("SystemSysTickIntr")*/ void
SystemSysTickIntr(void)
{
  system1ms--;
  return;
}
uint32_t
SystemGetCounter(void)
{
  return system1ms;
}
void
SystemWaitCounter(int ms)
{
  uint32_t      t;

  t = system1ms;
  while((t - system1ms) < ms);

  return;
}


const static rtosTaskInfo_t     mainTaskList[] = {
  /*** main task */
  {
    .pFunc = (rtosTaskFunc)MainTask,
    .pName = NULL,
    .priority = RTOS_PRI_IDLE,
    .szStack = 0x200,
  },
  /*** command task */
  {
    .pFunc = (rtosTaskFunc)MainCommandTask,
    .pName = NULL,
    .priority = RTOS_PRI_IDLE,
    .szStack = 0x200,
  },
  /*** UsbTask */
  {
    .pFunc = (rtosTaskFunc)MainUsbdifTask,
    .pName = NULL,
    .priority = RTOS_PRI_NORMAL,
    .szStack = 0x200,
  },
  /* end of list */
  {
    .pFunc = NULL,
    .pName = NULL,
    .priority = 0,
    .szStack = 0,
  },
};

static uint8_t  mainCommandBuf[128];
static int      mainCommandPos = 0;
void
MainUartLoop(void)
{
  int           len = 0;

  do {
    int                 cnt = 0;
    uint8_t             c;

    len = UsbCdcAcmRecv(dUsbCdc, &c, sizeof(c));
    if(len > 0) {
      if(c == '\n') {
      } else if(c == '\r') {
        int             ac;

        c = '\n';
        UsbCdcAcmSend(dUsbCdc, &c, 1);

        mainCommandBuf[mainCommandPos] = '\0';
        CommandProcess(mainCommandBuf);

        mainCommandPos = 0;
      } else if(c == '\b') {
        cnt = 0;
        if(mainCommandPos > 0) {
          mainCommandPos--;
          UsbCdcAcmSend(dUsbCdc, "\b \b", 3);
        }
      } else {
        if(mainCommandPos < sizeof(mainCommandBuf)) {
          mainCommandBuf[mainCommandPos] = c;
          mainCommandPos++;
          UsbCdcAcmSend(dUsbCdc, &c, 1);
        }
      }
    }

    UsbCdcAcmLoop(dUsbCdc);
  } while (len > 0);

  return;
}


static void
MainCommandTask(void const * argument)
{
  RtosTaskSleep(5000);
  while(1) {
    //    puts("# command\n");
    //UsbCdcAcmSend(dUsbCdc, "asdf\n", 5);
    RtosTaskSleep(1000);
  }
}


static void
MainTask(void const * argument)
{
  FifoInit();

  //DevDmaInit(-1, 0, NULL);
  MainInitUsart();
  //MainInitTim();

  /* start all tasks */
  const static rtosTaskInfo_t     *p;
  p = &mainTaskList[1];   /* skip index 0 which is this main task, already executed */
  while(p->pFunc) {
    RtosTaskCreate(p, NULL);
    p++;
  }

  puts("\n\n\n\n\n\n\n\n\n\n#-----\n");
  puts("# KICADKEY was started\n");

  systemClockFreq_t   clk;
  SystemGetClockValue(&clk);
  SystemDebugShowClockValue(&clk);

  RtosTaskSleep(1000);

  KicadSwitchInit();

  int i;
  int isPushSw = 0, isPushSwPrev = 0;
  /* Infinite loop */
  while(1) {
    MainUartLoop();
    RtosTaskSleep(10);
    KicadSwitchLoop();

    isPushSw = !GpioGetPowerSw();
    if(isPushSw && !isPushSwPrev) {
      uint8_t           rep[8];
      memset(rep, 0, sizeof(rep));
      rep[2] = 4;
      while(UsbdHidIsTxBusy(dUsbHid)) {
        RtosTaskSleep(1);
      }
      UsbdHidTransmit(dUsbHid, rep, 8);

      rep[2] = 0;
      while(UsbdHidIsTxBusy(dUsbHid)) {
        RtosTaskSleep(1);
      }
      UsbdHidTransmit(dUsbHid, rep, 8);
    }
    isPushSwPrev = isPushSw;

  }
}


void main(void) {
  MainEntry();
}
void
MainEntry(void)
{
  uint32_t              *pSrc, *pSrcEnd, *pDest;
  register uint32_t     sp, pc;
  int                   isPowerSwPushed;
  uint32_t              tout;
  uint32_t              val, valPrev;

  extern uint32_t  sectVectorStart;

  extern const uint16_t gpioDefaultTbl[];

  //SystemGpioSetUpdateLedOn();

  DevGpioInit();
  DevGpioSets(gpioDefaultTbl);
  IntrInit(0);

  {
    systemClockFreq_t   clk;
    SystemGetClockValue(&clk);
    SysTick_Config(clk.systick/1000);
  }

  /* init the systick counter */
  IntrSetEntry(0, SysTick_IRQn, (void *)RtosSysTickEntry);
  NVIC_SetPriority(SysTick_IRQn, 15);
  NVIC_EnableIRQ(SysTick_IRQn);

  /* init the SVCall/PendSV */
  IntrSetEntry(0, SVCall_IRQn,  (void *)RtosSvcEntry);
  NVIC_SetPriority(SVCall_IRQn, 0);
  NVIC_EnableIRQ(SVCall_IRQn);
  IntrSetEntry(0, PendSV_IRQn,  (void *)RtosPendSvEntry);
  NVIC_SetPriority(PendSV_IRQn, 0);
  NVIC_EnableIRQ(PendSV_IRQn);

  /* other interrupt */
  NVIC_SetPriority(MemoryManagement_IRQn, 0);
  NVIC_SetPriority(BusFault_IRQn, 0);
  NVIC_SetPriority(UsageFault_IRQn, 0);
  NVIC_SetPriority(DebugMonitor_IRQn, 0);

  /* Start the first task */
  idMain = RtosTaskCreate(&mainTaskList[0], NULL);
  RtosKernelStart();

  /************************************************
   *  Updater is not exist,  halt
   */
  while(1);

}


/*** USART **********************************************************/
static void
MainInitUsart(void)
{
  devUsartParam_t       param;

  DevUsartInit(-1, NULL);

  memset(&param, 0, sizeof(param));
  param.baud = CONFIG_SYSTEM_USART_BAUD;
  param.bit = DEVUSART_BIT_8;
  param.stop = DEVUSART_STOP_1;
  param.mode = DEVUSART_MODE_FIFO;
  param.parity = DEVUSART_PARITY_NONE;
  param.szFifoTx = 6;
  param.szFifoRx = 6;
  DevUsartInit(CONFIG_SYSTEM_USART_PORT, &param);

#if 0
  __NVIC_SetPriority(DMA1_Stream0_IRQn, 5);
  __NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  __NVIC_SetPriority(DMA1_Stream1_IRQn, 5);
  __NVIC_EnableIRQ(DMA1_Stream1_IRQn);
#endif

  return;
}


/*** TIM  ***********************************************************/
/* PWM */
static void
MainInitTim(void)
{

#if 0
  /* TIM2 pps counter */
  {
    TIM2_PTR->SMCR = TIM_SMCR_ETP_RISING | TIM_SMCR_ECE_MODE2DIS | \
      TIM_SMCR_ETPS_DIV1 | TIM_SMCR_SMS_INTCLK | TIM_SMCR_TS_TI2FP2;
#if 0
    TIM2_PTR->CCMR1 = TIM_CCMR1_IC2F_CKINT_N4 | TIM_CCMR1_CC2S_TI2;
    TIM2_PTR->ccmr2 = 0;
    TIM2_PTR->dier = TIM_DIER_CC2IE_YES;
    TIM2_PTR->ccer = TIM_CCER_CC2E_YES;
#endif
    TIM2_PTR->CCMR1 = (TIM_CCMR1_IC2F_CKINT_N4 | TIM_CCMR1_CC2S_TI2 |
                       TIM_CCMR1_IC1F_CKINT_N4 | TIM_CCMR1_CC1S_TI1);
    TIM2_PTR->CCMR2 = 0;
    TIM2_PTR->DIER = TIM_DIER_CC2IE_YES | TIM_DIER_CC1IE_YES;
    TIM2_PTR->CCER = TIM_CCER_CC2E_YES | TIM_CCER_CC1E_YES;

    TIM2_PTR->CR1 = TIM_CR1_CKD_DIV1 | TIM_CR1_DIR_DOWN |
      TIM_CR1_OPM_NO | TIM_CR1_CEN_YES;

    IntrSetEntry(0, TIM2_IRQn, MainTim2Intr);
    NVIC_SetPriority(TIM2_IRQn, 15);
    NVIC_EnableIRQ(TIM2_IRQn);
  }
#endif

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


#if USBDESC_ENABLE_HID
  /* audio initialize */
  memset(&class, 0, sizeof(class));
  class.type = USBDIF_CLASSTYPE_HID;
  class.cb = NULL;
  class.numClass = num++;       /* adhoc */
  class.ifnum = 1<<(USBDESC_IF_HID);
  class.epnum = USBDIF_EPBIT(USBDESC_EP_HID_IN);
  class.pUserData = &usbHidCb;
  dUsbHid = UsbifRegisterClass(unit, &class);
#endif
#if USBDESC_ENABLE_AUDIO
  /* audio initialize */
  memset(&class, 0, sizeof(class));
  class.type = USBDIF_CLASSTYPE_AUDIO;
  class.cb = NULL;
  class.numClass = num++;       /* adhoc */
  class.ifnum = ((1<<(USBDESC_IF_AUDIO_CTRL))
                 |(1<<(USBDESC_IF_AUDIO_OUT))
                 |(1<<(USBDESC_IF_AUDIO_IN)));
  class.epnum = USBDIF_EPBIT(USBDESC_EP_AUDIO_OUT)
    | USBDIF_EPBIT(USBDESC_EP_AUDIO_IN)
    | USBDIF_EPBIT(USBDESC_EP_AUDIO_FB);
  class.pUserData = &usbAudioCb;
  dUsbAudio = UsbifRegisterClass(unit, &class);
#endif

#if USBDESC_ENABLE_MSC
  /* msc initialize */
  memset(&class, 0, sizeof(class));
  class.type = USBDIF_CLASSTYPE_MASS_STORAGE;
  class.cb = NULL;
  class.numClass = num++;       /* adhoc */
  class.ifnum = (1<<(USBDESC_IF_MSC));
  class.epnum = (USBDIF_EPBIT(USBDESC_EP_MSC_OUT)
                 | USBDIF_EPBIT(USBDESC_EP_MSC_IN));
  class.pUserData = &usbmscCb;
  dUsbMsc = UsbifRegisterClass(unit, &class);
#endif

#if 0
  /* cdc rndis initialize */
  memset(&class, 0, sizeof(class));
  class.type = USBDIF_CLASSTYPE_CDC_DATA;
  class.cb = NULL;
  class.numClass = num++;       /* adhoc */
  class.ifnum = ((1<<(USBDESC_IF_CDCRNDIS_DATA))
                 | (1<<(USBDESC_IF_CDCRNDIS_CTRL)));
  class.epnum = (USBDIF_EPBIT(USBDESC_EP_CDCRNDIS_OUT)
                 | USBDIF_EPBIT(USBDESC_EP_CDCRNDIS_IN)
                 | USBDIF_EPBIT(USBDESC_EP_CDCRNDIS_CTRL));
  class.pUserData = &usbcdcRndisCb;
  class.pUserData = NULL;
  dUsbCdcRndis = UsbifRegisterClass(unit, &class);
#endif

  /* start usb middleware */
  UsbifStart(unit);

  /* start usb module */
  {
    devUsbParam_t    param;
    const static usbdifDevFifo_t  fifo = {
      /* OUT    0  1... */
      {        64,  64,  64,  64,   64,  64,   4,   4},
      /* IN     0    1    2    3     4    5    6    7 */
      {        64,  64,  64,  64,   64,  64,   4,   4}, };

    DevUsbInit(-1, NULL, NULL);

    param.dma = 0;
    param.sof = 1;
    param.dbuf  = (1<<(USBDESC_EP_MSC_OUT & 0xf));      /* double buffer */
    param.dbuf |= (1<<(USBDESC_EP_MSC_IN  & 0xf));      /* double buffer */

    //    DevUsbSetTRxFifo(unit, &fifo);
    DevUsbInit(unit, &param, (usbdifDevFifo_t *)&fifo);
    //DevUsbSetTRxFifo(unit, &fifo);

    GpioSetUsbEnable();

    /* interrupt enable */
    NVIC_SetPriority(USB_FS_IRQn, 5);
    NVIC_EnableIRQ(USB_FS_IRQn);
  }

  return;
}
int     mainUsbdifTaskQueueId;
static void
MainUsbdifTask(void const * argument)
{
  uint32_t      val;
  mainUsbdifTaskQueueId = (int)RtosQueueCreate(1, sizeof(uint32_t));

  RtosTaskSleep(100);

  MainInitUsb();
  devCrsParam_t     crs;
  crs.src = CRS_CLOCK_SOURCE_USB1;
  DevCrsInit(&crs);

  NVIC_SetPriority(RCCCRS_IRQn, 15);
  NVIC_EnableIRQ(RCCCRS_IRQn);


  /* Infinite loop */
  while(1) {
    UsbifLoop();
  }
}
#if 0
void
MainUsbdifIntr(void)
{
  uint32_t      val = 0;
  RtosQueueSendIsr(mainUsbdifTaskQueueId, (void *) &val, 10);
  return;
}
#endif

int
_putchar(char c)
{
  DevUsartSend(CONFIG_CONSOLE_NUMBER, &c, 1);
  UsbCdcAcmSend(dUsbCdc, (uint8_t *)&c, sizeof(c));
  return c;
}
int
puts(const char *str)
{
  DevUsartSend(CONFIG_CONSOLE_NUMBER, (uint8_t *)str, strlen(str));
  UsbCdcAcmSend(dUsbCdc, (uint8_t *)str, strlen(str));
  return 0;
}

int
_getc(void)
{
  int           result = -1;
  int           re;
  uint8_t       c;

  re = DevUsartRecv(CONFIG_CONSOLE_NUMBER, &c, 1);
  if(re > 0) {
    result = c & 0xff;
  }

  return result;
}
