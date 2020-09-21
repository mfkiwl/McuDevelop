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

#ifndef _GPIO_USBSOFIC_H_
#define _GPIO_USBSOFIC_H_

#define GPIO_POWER_SW           GPIO_PORTNUM(CONFIG_POWER_SW_PORTNUM,   \
                                             CONFIG_POWER_SW_BITNUM)
#define GPIO_UPDATE_LED         GPIO_PORTNUM(CONFIG_UPDATE_LED_PORTNUM, \
                                             CONFIG_UPDATE_LED_BITNUM)
#define GPIO_POWER_LED          GPIO_PORTNUM(CONFIG_POWER_LED_PORTNUM, \
                                             CONFIG_POWER_LED_BITNUM)
#define GPIO_MCO_A8             GPIO_PORTNUM(0, 8)
#define GPIO_MCO_A9             GPIO_PORTNUM(0, 9)

#define GPIO_USART1_TXD         GPIO_PORTNUM(GPIO_PORT_A,  9)
#define GPIO_USART1_RXD         GPIO_PORTNUM(GPIO_PORT_A, 10)
#define GPIO_USART2_TXD         GPIO_PORTNUM(GPIO_PORT_A,  2)
#define GPIO_USART2_RXD         GPIO_PORTNUM(GPIO_PORT_A,  3)

#define GPIO_I2C1_SCL           GPIO_PORTNUM(GPIO_PORT_B,  6)
#define GPIO_I2C1_SDA           GPIO_PORTNUM(GPIO_PORT_B,  7)
#define GPIO_I2C2_SCL           GPIO_PORTNUM(GPIO_PORT_B, 10)
#define GPIO_I2C2_SDA           GPIO_PORTNUM(GPIO_PORT_B, 11)


#define GPIO_USB_FS_DM          GPIO_PORTNUM(GPIO_PORT_A, 11)
#define GPIO_USB_FS_DP          GPIO_PORTNUM(GPIO_PORT_A, 12)
#define GPIO_USB_FS_VBUS        GPIO_PORTNUM(GPIO_PORT_A, 13)

#define GpioGetPowerSw()        GPIO_GET(GPIO_POWER_SW)
#define GpioSetPowerLedOff()    (GPIO_PTR[CONFIG_POWER_LED_PORTNUM].BRR = (1UL<<(CONFIG_POWER_LED_BITNUM)))
#define GpioSetPowerLedOn()     (GPIO_PTR[CONFIG_POWER_LED_PORTNUM].BSRR = (1UL<<(CONFIG_POWER_LED_BITNUM)))
#define GpioSetUpdateLedOff()   (GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].BRR = (1UL<<(CONFIG_UPDATE_LED_BITNUM)))
#define GpioSetUpdateLedOn()    (GPIO_PTR[CONFIG_UPDATE_LED_PORTNUM].BSRR = (1UL<<(CONFIG_UPDATE_LED_BITNUM)))


#endif
