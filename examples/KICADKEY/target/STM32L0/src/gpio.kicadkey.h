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

#ifndef _GPIO_KICADKEY_H_
#define _GPIO_KICADKEY_H_

#define GPIO_POWER_SW           GPIO_PORTNUM(GPIO_PORT_D,  2)
#define GPIO_UPDATE_LED         GPIO_PORTNUM(GPIO_PORT_B, 15)
#define GPIO_POWER_LED          GPIO_PORTNUM(GPIO_PORT_B, 14)

#define GPIO_USART1_TXD         GPIO_PORTNUM(GPIO_PORT_A,  9)
#define GPIO_USART1_RXD         GPIO_PORTNUM(GPIO_PORT_A, 10)

#define GPIO_USB_FS_DM          GPIO_PORTNUM(GPIO_PORT_A, 11)
#define GPIO_USB_FS_DP          GPIO_PORTNUM(GPIO_PORT_A, 12)
#define GPIO_USB_FS_EN          GPIO_PORTNUM(GPIO_PORT_A, 13)

#define GPIO_ROW0               GPIO_PORTNUM(GPIO_PORT_A,  0)   /* row pin 1 */
#define GPIO_ROW1               GPIO_PORTNUM(GPIO_PORT_A,  1)   /* row pin 8 */
#define GPIO_ROW2               GPIO_PORTNUM(GPIO_PORT_A,  2)   /* row pin 9 */
#define GPIO_ROW3               GPIO_PORTNUM(GPIO_PORT_A,  3)   /* row pin10 */
#define GPIO_ROW4               GPIO_PORTNUM(GPIO_PORT_A,  4)   /* row pin11 */
#define GPIO_ROW5               GPIO_PORTNUM(GPIO_PORT_A,  5)   /* row pin12 */
#define GPIO_ROW6               GPIO_PORTNUM(GPIO_PORT_A,  6)   /* row pin13 */

#define GPIO_COL0               GPIO_PORTNUM(GPIO_PORT_B,  3)   /* col pin3 */
#define GPIO_COL1               GPIO_PORTNUM(GPIO_PORT_B,  4)   /* col pin4 */
#define GPIO_COL2               GPIO_PORTNUM(GPIO_PORT_B,  5)   /* col pin5 */
#define GPIO_COL3               GPIO_PORTNUM(GPIO_PORT_B,  6)   /* col pin6 */
#define GPIO_COL4               GPIO_PORTNUM(GPIO_PORT_B,  7)   /* col pin7 */
#define GPIO_COL_MASK           0xf8

#define GPIO_TIM3_3             GPIO_PORTNUM(GPIO_PORT_B,  0)   /* illumi led */
#define GPIO_TIM3_4             GPIO_PORTNUM(GPIO_PORT_B,  1)   /* numlock led */

#define GPIO_MCO                GPIO_PORTNUM(GPIO_PORT_A,  8)


#ifdef _SYSTEM_C_
const uint16_t gpioDefaultTbl[] = {
  PORT_CTRL(GPIO_POWER_SW,   GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),
  PORT_CTRL(GPIO_UPDATE_LED, GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_POWER_LED,  GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),

  PORT_CTRL(GPIO_ROW0,        GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),
  PORT_CTRL(GPIO_ROW1,        GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),
  PORT_CTRL(GPIO_ROW2,        GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),
  PORT_CTRL(GPIO_ROW3,        GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),
  PORT_CTRL(GPIO_ROW4,        GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),
  PORT_CTRL(GPIO_ROW5,        GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),
  PORT_CTRL(GPIO_ROW6,        GPIO_MODE_INPUT,  GPIO_PULL_UP,   0),

  PORT_CTRL(GPIO_COL0,        GPIO_MODE_OUTPUT, GPIO_PULL_OD,   1),
  PORT_CTRL(GPIO_COL1,        GPIO_MODE_OUTPUT, GPIO_PULL_OD,   1),
  PORT_CTRL(GPIO_COL2,        GPIO_MODE_OUTPUT, GPIO_PULL_OD,   1),
  PORT_CTRL(GPIO_COL3,        GPIO_MODE_OUTPUT, GPIO_PULL_OD,   1),
  PORT_CTRL(GPIO_COL4,        GPIO_MODE_OUTPUT, GPIO_PULL_OD,   1),

  PORT_CTRL(GPIO_USART1_TXD, GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC(4)),
  PORT_CTRL(GPIO_USART1_RXD, GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC(4)),

  PORT_CTRL(GPIO_TIM3_3, GPIO_MODE_FUNC, GPIO_PULL_DOWN, GPIO_FUNC(2)),
  PORT_CTRL(GPIO_TIM3_4, GPIO_MODE_FUNC, GPIO_PULL_DOWN, GPIO_FUNC(2)),

#if 1
  PORT_CTRL(GPIO_USB_FS_DP, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_USB_FS_DM, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
#endif

#if CONFIG_MCO_ENABLE
  PORT_CTRL(GPIO_MCO, GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC(0)),
#endif

  PORT_CTRL_END,
};
#endif

#define GpioSetUsbEnable()              
#define GpioSetUsbDisable()             

#define GpioGetPowerSw()        GPIO_GET(GPIO_POWER_SW)
#define GpioSetPowerLedOff()    //(GPIO_PTR[CONFIG_POWER_LED_PORTNUM].BRR = (1UL<<(CONFIG_POWER_LED_BITNUM)))
#define GpioSetPowerLedOn()     //(GPIO_PTR[CONFIG_POWER_LED_PORTNUM].BSRR = (1UL<<(CONFIG_POWER_LED_BITNUM)))
#define GpioSetUpdateLedOff()   //(GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].BRR = (1UL<<(CONFIG_UPDATE_LED_BITNUM)))
#define GpioSetUpdateLedOn()    //(GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].BSRR = (1UL<<(CONFIG_UPDATE_LED_BITNUM)))

#define GPIO_DIS_ALLCOLS                (GPIO_COL_MASK)
#define GPIO_SET_COL0                   (0x08)
#define GpioDisCols(portnum)  \
  GPIO_PTR[(((portnum) & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT)].BSRR = (GPIO_COL_MASK)
#define GpioSelectCol(portnum, x)  \
  GPIO_PTR[(((portnum) & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT)].BRR  = (x)
#define GpioGetRows(portnum)  \
  GPIO_PTR[(((portnum) & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT)].IDR

#endif
