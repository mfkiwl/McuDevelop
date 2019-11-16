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

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include        <stdint.h>
//#include        <stdlib.h>

#include        "config.h"

#include        "stm32f7.h"
#include        "devGpio.h"
#include        "devDma7.h"
#include        "gpio.usbsofic.h"

#if (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M0)
#include        "core_cm0.h"
#elif (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M0PLUS)
#include        "core_cm0plus.h"
#elif (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M4)
#include        "core_cm4.h"
#elif (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M7)
#include        "core_cm7.h"
#else
#error "DEVICE_ARCHTYPE is not definited"
#endif


#define SYSTEM_TIMER_1M000S     (CONFIG_CLOCK_FREQ_PERI/1000)
#define SYSTEM_TIMER_10M00S     (CONFIG_CLOCK_FREQ_PERI/100)
#define SYSTEM_TIMER_100M0S     (CONFIG_CLOCK_FREQ_PERI/10)
#define SYSTEM_TIMER_1S000      (CONFIG_CLOCK_FREQ_PERI)


typedef struct {
  uint32_t      vco;
  uint32_t      P;
  uint32_t      Q;
  uint32_t      R;
} systemClockPllFreq_t;
typedef struct {
  uint32_t              pllin;
  systemClockPllFreq_t  pll1;
  systemClockPllFreq_t  pll2;
  systemClockPllFreq_t  pll3;
  uint32_t              core;
#define       SYSTEM_CLOCK_FREQ_DIV_SYSTICK_SHIFT   3
  uint32_t              systick;
  uint32_t              hclk;   /* AHB1,2,3,4 clock */
  uint32_t              pclk1;  /* D2 APB1 clock */
  uint32_t              pclk1tim;
  uint32_t              pclk2;  /* D2 APB2 clock */
  uint32_t              pclk2tim;
  uint32_t              pclk3;  /* D1 APB3 clock */
  uint32_t              pclk4;  /* D3 APB4 clock */
} systemClockFreq_t;

void            SystemMpuConfig(void);
int             SystemGetClockValue(systemClockFreq_t *p);
void            SystemDebugShowClockValue(systemClockFreq_t *p);
void            SystemGpioInit(void);

void            *SystemMallocStreamBuf(int type, int size, void *vp);
void            SystemSysTickIntr(void);

void            SystemChangeClockHigher(void);
void            SystemChangeClockDefault(void);
void            SystemUpdateClockValue(void);

void            SystemInitSystemTimer(void);
uint32_t        SystemGetSystemTimer(void);
void            SystemWaitSystemTimer(uint32_t tout);

#ifdef  _SYSTEM_C_
static void             SystemCoreClockUpdate(void);
static int              SystemFmcInit(void);
#endif

#endif
