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

#define _MAIN_C_

#include        <stdint.h>
#include        <stdlib.h>
#include        <string.h>

#include        "config.h"
#include        "system.h"

#include        "update.h"
#include        "peri.h"

#include        "main.h"


#ifdef  COMPILER_GCC
#define __ATTR_INTR(x)  __attribute__((interrupt(x)))
#endif
#ifdef  COMPILER_IAR
#define __ATTR_INTR(x)  __irq
#endif


const char strProductText[] = CONFIG_PRODUCT_NAME_TEXT;
const char strVersionText[] = CONFIG_VERSION_BOOT_TEXT;

static volatile uint32_t        system1ms = 0;
static int                      ledSpeed = 0;
static uint32_t                 activeTimer = 0;


int
_putchar(char c)
{
  return c;
}


void
MainSetUpdateLedSpeed(int val)
{
  activeTimer = SystemGetCounter() | 1;
  return;
}

void
MainIdle(void)
{
  ledSpeed = CONFIG_FIRMUPDATE_POWERLED_BLINK_SLOW;;
  if(activeTimer) {
    ledSpeed = CONFIG_FIRMUPDATE_POWERLED_BLINK_FAST;
    if((activeTimer - SystemGetCounter()) > CONFIG_FIRMUPDATE_POWERLED_BLINK_TIME) {
      activeTimer = 0;
    }
  }

  if(!(system1ms & ledSpeed)) {
    SystemGpioSetUpdateLedOff();
  } else {
    SystemGpioSetUpdateLedOn();
  }

  return;
}

extern int      seq;


void
MainEntry(void)
{
  int                   re;
  int                   i;

  while(SystemGpioGetPowerSw());

  SystemInit();

  __enable_irq();

  SystemGpioSetPowerLedOff();
  PeriInit();

  /* search active peripheral */
  while(1) {
    MainIdle();
    SystemLoop();
    SystemWdtClear();
    re = PeriSearchResponsedUnit();
    if(re == PERI_SUCCESS) {
      PeriUninitExceptSelected();
      break;
    }
  }

  SystemGpioSetUpdateLedOn();

  /* update routine */
  UpdateInit();

  while(1) {

    SystemLoop();
    MainIdle();
    SystemWdtClear();
    UpdateLoop();
  }
}


static void
SystemIncrement1ms(void)
{
  if(SysTick->CTRL & (1<<SysTick_CTRL_COUNTFLAG_Pos)) {
    system1ms--;
  }
  return;
}
__ATTR_INTR("SystemSysTickIntr") void
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
void
Boot1Exception(void)
{
  while(1) {
    SystemGpioSetUpdateLedOn();
    SystemGpioSetUpdateLedOff();
  }
}


extern uint8_t  __HeapBase;
extern uint8_t  __HeapLimit;
uint8_t         *pHeapBase = &__HeapBase;
int             szHeap;
uint8_t         *pStartNextHeap = &__HeapBase;
void *
_sbrk(int size)
{
  uint8_t               *ptr = NULL;

  if(size > 0) {
    szHeap = (&__HeapLimit - &__HeapBase);
    if(pHeapBase + szHeap > pStartNextHeap + size) {
      ptr = pStartNextHeap + size;
      memset(ptr, 0, size);
    }
  }

  return ptr;
}

void *
malloc(size_t size)
{
  return _sbrk(size);
}

void
free(void *p)
{
  return;
}
