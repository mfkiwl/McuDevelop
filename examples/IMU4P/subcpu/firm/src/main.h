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

#ifndef _MAIN_H_
#define _MAIN_H_

void                    MainUsbdifIntr(void);
void                    MainEnableTim(void);
void                    MainDisableTim(void);
void                    MainResetTimCounter(void);


void                    SystemIncrement1ms(void);
void                    SystemSysTickIntr(void);
uint32_t                SystemGetCounter(void);
void                    SystemWaitCounter(int ms);


#ifdef _MAIN_C_
static void             MainTask(void const * argument);
static void             MainCommandTask(void const * argument);
static void             MainUsbdifTask(void const * argument);
static void             MainSwitchInit(void);
static void             MainSwitchLoop(void);
static void             MainIdleLoop(void);
static void             MainInitUsart(void);
static void             MainInitCounter(void);
static void             MainInitI2c(void);
static void             MainInitSpi(void);
static void             MainInitAdc(void);
static void             MainInitDac(void);
static void             MainInitUsb(void);
#endif

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)      /* adhoc */

#endif
