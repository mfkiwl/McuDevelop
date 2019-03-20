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
#include        "devGpio.h"
#include        "devUsart.h"
#include        "devSpi16.h"
#include        "devDma4.h"
#include        "devCounter.h"
#include        "gpio.h"
#include        "devCrs.h"
#include        "fifo.h"
#include        "command.h"
#include        "imu.h"

#include        "main.h"

const uint8_t           strVersionText[] = CONFIG_VERSION_FIRM_TEXT;


mainSetting_t           setting;
static volatile uint32_t system1ms = 0;

void            MainEntry(void);
int             _putchar(char c);
int             _getc(void);

static void     MainInitUsart(void);
static void     MainInitTim(void);
static void     MainInitSpi(void);



volatile uint32_t        tim2msb = 0;
typedef struct {
  volatile uint64_t      t;
  volatile uint32_t      cnt;
  volatile uint16_t      flag;
#define       MAIN_TIM2_IC_VALID        (1<<0)
#define       MAIN_TIM2_IC_OVERWRITE    (1<<1)
#define       MAIN_SPIRX_DONE           (1<<2)
} mainTim2Ic_t;
static mainTim2Ic_t     mainTim2Ic[4];


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



static uint8_t  mainCommandBuf[128];
static int      mainCommandPos = 0;
void
MainUartLoop(void)
{
  int           len = 0;
  int           c;
  static int    cnt = 0;

  while((c = _getc()) >= 0) {

    if(c >= 0) {
      if(c == '\n') {
      } else if(c == '\r') {
        int             ac;

        c = '\n';
        puts("\n");

        mainCommandBuf[mainCommandPos] = '\0';
        CommandProcess(mainCommandBuf);

        mainCommandPos = 0;
      } else if(c == '\b') {
        cnt = 0;
        if(mainCommandPos > 0) {
          mainCommandPos--;
          puts("\b \b");
        }
      } else {
        if(mainCommandPos < sizeof(mainCommandBuf)) {
          uint8_t       str[2];
          mainCommandBuf[mainCommandPos] = c;
          mainCommandPos++;
          str[0] = c;
          str[1] = '\0';
          puts(str);
        }
      }
    }
  }

  return;
}


static void
MainLedBlinking(void)
{
  static uint32_t      tLedWait;
  static int           fLed = 0;
  if((tLedWait - SystemGetCounter()) >= 1000) {
    tLedWait = SystemGetCounter();
    if(fLed) {
      SystemGpioSetPowerLedOff();
    } else {
      SystemGpioSetPowerLedOn();
    }
    fLed = !fLed;
  }

  return;
}


volatile static int mainImuNo = 0;
#define MAIN_IMU_SEQ_IDLE       0
#define MAIN_IMU_SEQ_SPIRX      1
#define MAIN_IMU_SEQ_CONVERT    2
#define MAIN_IMU_SEQ_DONE       3
volatile static uint8_t  mainImuSeq = MAIN_IMU_SEQ_IDLE;
volatile static uint8_t  mainImuSpiBusy = 0;
imuValue_t    imu[4];


static uint8_t       str[4][80];
static int           lenStr[4];
void
MainImuLoop(void)
{
  int           j, i;
  int           len;

  /* the sensor data will be received, if the TIM IC interrupt is come */
  __disable_irq();
  if(!mainImuSpiBusy) {
    MainSearchImuIntr();
  }
  __enable_irq();

  /* the received data is converted output data, then send*/
  for(i = 0; i < CONFIG_NUM_OF_IMUS; i++) {
    if(mainTim2Ic[i].flag & MAIN_SPIRX_DONE) {
      __disable_irq();
      mainTim2Ic[i].flag &= ~MAIN_SPIRX_DONE;
      __enable_irq();

      ImuReadValue(i, &imu[i]);

      switch(setting.format) {
      case    MAIN_OUTPUT_FORMAT_TEXT:
        imu[i].t = mainTim2Ic[i].t;
        lenStr[i] = ImuBuildText(i+setting.imuno, &imu[i], &str[i][0], setting.format);
        MainQueueImu(i);
        break;
      case    MAIN_OUTPUT_FORMAT_HAMMING_CRC32:
      case    MAIN_OUTPUT_FORMAT_HAMMING_CRC32_8:
      case    MAIN_OUTPUT_FORMAT_HAMMING_CRC8:
      case    MAIN_OUTPUT_FORMAT_HAMMING_CODE:
        imu[i].t = mainTim2Ic[i].t;
        lenStr[i] = ImuBuildHamming(i+setting.imuno, &imu[i], &str[i][0], setting.format);
        MainQueueImu(i);
        break;
      }

      if(!DevUsartIsSendingDma(1)) {
        __disable_irq();
        MainSendImu();
        __enable_irq();
      }

      __disable_irq();
    }
  }

  return;
}


static void
MainSearchImuIntr(void)
{
  int           i;
  int           no;

  for(i = 0; i < (CONFIG_NUM_OF_IMUS-1); i++) {
    mainImuNo++;
    if(mainImuNo >= CONFIG_NUM_OF_IMUS) mainImuNo = 0;
    no = mainImuNo;

    if(mainTim2Ic[no].flag & MAIN_TIM2_IC_VALID) {
      if(mainTim2Ic[no].flag & MAIN_TIM2_IC_OVERWRITE) {
        imu[no].flag |= IMU_FLAG_OVERWRITE;
      }
      mainTim2Ic[no].flag &= ~(MAIN_TIM2_IC_OVERWRITE | MAIN_TIM2_IC_VALID);

      mainImuSpiBusy = 1;
      ImuRecvValue(no, &imu[no]);
      imu[no].cnt = mainTim2Ic[no].cnt;

      break;
    }
  }

  return;
}


#define IMUBUF_SIZE     8
static uint8_t          mainQueueImuBuf[IMUBUF_SIZE] = {-1};
volatile static uint8_t          mainQueueImuStart = 0;
volatile static uint8_t          mainQueueImuEnd = 0;

static void
MainQueueImu(int no)
{

  if(((mainQueueImuStart+1) & (IMUBUF_SIZE-1)) == mainQueueImuEnd) {
    /* flush */
    __disable_irq();
    mainQueueImuEnd++;
    mainQueueImuEnd &= (IMUBUF_SIZE-1);
    __enable_irq();
  }

  mainQueueImuBuf[mainQueueImuStart] = no;
  mainQueueImuStart++;
  mainQueueImuStart &= (IMUBUF_SIZE-1);

  return;
}
static void
MainSendImu(void)
{
  int           n;

  if(mainQueueImuStart != mainQueueImuEnd) {
    n = mainQueueImuBuf[mainQueueImuEnd];

    //puts(&str[n][0]);
    DevUsartSend(CONFIG_CONSOLE_NUMBER, &str[n][0], lenStr[n]);

    mainQueueImuEnd++;
    mainQueueImuEnd &= (IMUBUF_SIZE-1);
  }

  return;
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

  DevGpioInit();
  DevGpioSets(gpioDefaultTbl);
  IntrInit(0);

  {
    systemClockFreq_t   clk;
    SystemGetClockValue(&clk);
    SysTick_Config((clk.systick<<2)/1000);      /* adhoc */
  }

  /* other interrupt */
  NVIC_SetPriority(MemoryManagement_IRQn, 0);
  NVIC_SetPriority(BusFault_IRQn, 0);
  NVIC_SetPriority(UsageFault_IRQn, 0);
  NVIC_SetPriority(DebugMonitor_IRQn, 0);

  FifoInit();

  DevDmaInit(-1, 0, NULL);
  MainInitUsart();
  MainInitTim();
  MainInitSpi();

#if 0
  puts("\n\n\n\n\n\n\n\n\n\n#-----\n");
  puts("# IMU4P was started\n");

  systemClockFreq_t   clk;
  SystemGetClockValue(&clk);
  SystemDebugShowClockValue(&clk);
#endif

  NVIC_SetPriority(DMA1_CH1_IRQn, 5);
  NVIC_EnableIRQ(DMA1_CH1_IRQn);
  NVIC_SetPriority(DMA1_CH2TO3_IRQn, 5);
  NVIC_EnableIRQ(DMA1_CH2TO3_IRQn);
  NVIC_SetPriority(DMA1_CH4TO7_IRQn, 5);
  NVIC_EnableIRQ(DMA1_CH4TO7_IRQn);

  __enable_irq();

  /*******************************************
   * imu probe and initialize
   */
  /* initialize the internal variables */
  ImuInit(-1);

  /* CS pin is generated low and high */
  for(int j = 0; j < 1000; j++) {
    for(int i = 0; i < CONFIG_NUM_OF_IMUS; i++) {
      ImuGenCsPulse(i, 0);
    }
  }

  {
    setting.format = MAIN_OUTPUT_FORMAT_HAMMING_CRC32_8;
    uint8_t       val;
    val = *(uint8_t *)(EEPROM_BASE + MAIN_SETTING_ID_POS);
    if(val >= 0x10) val = 0;
    setting.imuno = val;    /* adhoc */
  }

  for(int i = 0; i < CONFIG_NUM_OF_IMUS; i++) {
    mainImuNo = i;
    ImuProbe(i);
  }
  for(int i = 0; i < CONFIG_NUM_OF_IMUS; i++) {
    mainImuNo = i;
    ImuInit(i);
  }
  mainImuNo = 0;

  int i;
  int isPushSw = 0, isPushSwPrev = 0;

  /* Infinite loop */
  while(1) {
    MainUartLoop();
    MainLedBlinking();
    MainImuLoop();
  }
  /************************************************
   * end
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
  param.mode = DEVUSART_MODE_DMA;
  param.parity = DEVUSART_PARITY_NONE;
  param.szFifoTx = 7;
  param.szFifoRx = 4;
  param.intrDma = 1;
  DevUsartInit(CONFIG_SYSTEM_USART_PORT, &param);

  return;
}


/*** TIM  ***********************************************************/
/* PWM */
static void
MainInitTim(void)
{
  devCounterParam_t     param;

  DevCounterInit(-1, NULL);

  /********************************************************
   * 32.768kHz --> [ETR TIM2]
   */

  /********************************************************
   * TIM2:  freerun count, input capture
   */
  memset(&param, 0, sizeof(param));
  param.chnum = (DEVCOUNTER_SETCH(DEVCOUNTER_CH_CLKTRG) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_1) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_2) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_3) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_4));

  param.clktrg.mode = (DEVTIME_CLKTRG_MODE_FREERUN |
                       DEVTIME_CLKTRG_CTG_EXTERNAL2 | DEVTIME_CLKTRG_SEL(0));
  param.clktrg.intr = 1;
  //param.clktrg.filter = 0;
  param.clktrg.reload = 0xffff;
  //param.clktrg.prescaler = 0;

  param.ch.mode = DEVCOUNTER_MODE_IC;
  param.ch.intr = 1;
  DevCounterInit(TIM2_NUM, &param);

  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);

  return;
}


void
MainEnableTim(void)
{
  devCounterParam_t     param;

  memset(&param, 0, sizeof(param));
  param.chnum = (DEVCOUNTER_SETCH(DEVCOUNTER_CH_1) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_2) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_3) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_4));

  param.ch.mode = DEVCOUNTER_MODE_IC;
  param.ch.intr = 1;

  DevCounterInit(TIM2_NUM, &param);

  return;
}


void
MainDisableTim(void)
{
  devCounterParam_t     param;

  memset(&param, 0, sizeof(param));
  param.chnum = (DEVCOUNTER_SETCH(DEVCOUNTER_CH_1) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_2) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_3) |
                 DEVCOUNTER_SETCH(DEVCOUNTER_CH_4));

  param.ch.mode = DEVCOUNTER_MODE_DISABLE;
  DevCounterInit(TIM2_NUM, &param);

  return;
}


void
MainInterruptTim(void)
{
  uint32_t      val, t;
  uint32_t      sr, mask, dier;

  sr = TIM2_PTR->SR;
  dier = TIM2_PTR->DIER;

  mask = TIM_SR_CC1IF_MASK;
  sr &= dier;

  for(int i = 0; i < 4; i++) {
    if(sr & mask) {
      DevCounterGetIcValue(TIM2_NUM, i+1, &val);
      if(mainTim2Ic[i].flag & MAIN_TIM2_IC_VALID) {
        mainTim2Ic[i].flag |= MAIN_TIM2_IC_OVERWRITE;
      }
      mainTim2Ic[i].flag |= MAIN_TIM2_IC_VALID;

      /* carry adjustment */
      t = tim2msb;
      if(sr & TIM_SR_UIF_MASK && !(val & 0xf000)) {
        t++;
      }
      mainTim2Ic[i].t    = t << 16;
      mainTim2Ic[i].t   |= val;

      mainTim2Ic[i].cnt++;
    }
    mask <<= 1;
  }

  if(sr & TIM_SR_UIF_MASK) {
    TIM2_PTR->SR = ~TIM_SR_UIF_MASK;
    tim2msb++;
  }

  return;
}


void
MainResetTimCounter(void)
{
  __disable_irq();

  //MainInitTim();
  DevCounterClearCounterValue(TIM2_NUM);
  tim2msb = 0;

  __enable_irq();
  return;
}

/*** SPI  ***********************************************************/
static void
MainInitSpi(void)
{
  devSpiParam_t       param;

  DevSpiInit(-1, NULL);

  memset(&param, 0, sizeof(param));
  param.clkmode = SPI_CR1_CPOL_YES | SPI_CR1_CPHA_YES;
  param.bit = 8;

  /* IMU control */
  //param.speed = 40000000;     /* speed: don't care */
  param.prescaler = 0;          /* 32MHz/(2^(x+1)) x={1:8Mbps, 2:4Mbps} */
  param.dmaTx = 0;
  param.dmaRx = 1;
  param.dmaIntr = 1;
  param.dmaNonblock = CONFIG_IMU_SPI_NONBLOCK;
  DevSpiInit(1, &param);

  return;
}


/*** DMA  ***********************************************************/
void
MainInterruptDmaCh1(void)
{
  return;
}
void
MainInterruptDmaCh2to3(void)
{
  uint32_t      sr;

  sr = DMA1_PTR->ISR;
  if(sr & DMA_ISR_GIF_MASK(DMA_CH2)) {
    DevDmaClearIntr(1, DMA_CH2);

#if CONFIG_IMU_SPI_NONBLOCK
    ImuReadValueNonblockEnd(mainImuNo);
    mainImuSpiBusy = 0;
#endif

    mainTim2Ic[mainImuNo].flag |= MAIN_SPIRX_DONE;

    //MainSearchImuIntr();      /* imu2,3 can not be received, if enable */
  }
#if 0
  if(sr & DMA_ISR_GIF_MASK(DMA_CH3)) {
    DevDmaClearIntr(1, DMA_CH3);
  }
#endif
  return;
}
void
MainInterruptDmaCh4to7(void)
{
  uint32_t       sr;
  sr = DMA1_PTR->ISR;
  if(sr & DMA_ISR_GIF_MASK(DMA_CH4)) {
    DevDmaClearIntr(1, DMA_CH4);

    DevUsartSendStopDma(1);
    MainSendImu();
  }

  return;
}


int
_putchar(char c)
{
  DevUsartSend(CONFIG_CONSOLE_NUMBER, &c, 1);
  return c;
}
int
puts(const char *str)
{
  DevUsartSend(CONFIG_CONSOLE_NUMBER, (uint8_t *)str, strlen(str));
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
