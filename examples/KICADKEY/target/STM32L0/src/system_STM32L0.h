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

#ifndef	_SYSTEM_STM32L0_H_
#define	_SYSTEM_STM32L0_H_

#include	"stm32l0.h"



typedef struct {
  uint32_t      vco;
  uint32_t      out;
} systemClockPllFreq_t;
typedef struct {
  uint32_t              pllin;
  systemClockPllFreq_t  pll1;
  uint32_t              sysclk;
#define       SYSTEM_CLOCK_FREQ_DIV_SYSTICK_SHIFT   3
  uint32_t              systick;
  uint32_t              hclk;   /* AHB1 clock */
  uint32_t              pclk1;  /* APB1 clock */
  uint32_t              pclk2;  /* APB2 clock */
} systemClockFreq_t;

int                     SystemGetClockValue(systemClockFreq_t *p);
void                    SystemDebugShowClockValue(systemClockFreq_t *p);

void                    *SystemMallocStreamBuf(int type, int size, void *vp);



#ifdef  _SYSTEM_STM32L0_C_
static void             SystemUpdateClockValue(void);
static void             SystemCoreClockUpdate(void);
#endif

#endif
