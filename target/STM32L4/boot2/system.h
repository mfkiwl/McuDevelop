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

#ifndef	_SYSTEM_H_
#define	_SYSTEM_H_

//#include	"stm32l4xx.h"
#include	"stm32l4.h"

#if (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M0)
#include	"core_cm0.h"
#elif (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M0PLUS)
#include	"core_cm0plus.h"
#elif (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M3)
#include	"core_cm3.h"
#elif (DEVICE_ARCHTYPE == CONFIG_ARCHTYPE_CORTEX_M4)
#include	"core_cm4.h"
#else
#error "DEVICE_ARCHTYPE is not defined"
#endif

void		SystemInit(void);

void		SystemGpioInit(void);
void		SystemGpioSet(uint16_t v);
void		SystemGpioSets(uint16_t *pGpio);
void		SystemGpioFuncInit(int group, int bit, int func);

void		SystemGpioSetPowerLedOff(void);
void		SystemGpioSetPowerLedOn(void);
void		SystemGpioSetUpdateLedOff(void);
void		SystemGpioSetUpdateLedOn(void);
int		SystemGpioGetPowerSw(void);

void		SystemWdtInit(void);
void		SystemWdtClear(void);

#if 0
void *		memset(void *s, int c, int size);
void *		memcpy(void *d, void *s, int size);
#endif

#endif
