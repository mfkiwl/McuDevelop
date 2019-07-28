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


#define GPIO_POWER_SW           GPIO_PORTNUM(CONFIG_POWER_SW_PORTNUM, CONFIG_POWER_SW_BITNUM)
#define GPIO_UPDATE_LED         GPIO_PORTNUM(CONFIG_UPDATE_LED_PORTNUM, CONFIG_UPDATE_LED_BITNUM)
#define GPIO_POWER_LED          GPIO_PORTNUM(CONFIG_POWER_LED_PORTNUM, CONFIG_POWER_LED_BITNUM)





//#define GPIO_FSSOF              GPIO_PORTNUM(GPIO_PORT_A,  8)
#define GPIO_MCO1               GPIO_PORTNUM(GPIO_PORT_A,  8)
#define GPIO_MCO2               GPIO_PORTNUM(GPIO_PORT_C,  9)


#define GPIO_USB_FS_DM          GPIO_PORTNUM(GPIO_PORT_A, 11)
#define GPIO_USB_FS_DP          GPIO_PORTNUM(GPIO_PORT_A, 12)
#define GPIO_USB_FS_EN          GPIO_PORTNUM(GPIO_PORT_G, 6)



#define GPIO_USART1_TXD         GPIO_PORTNUM(GPIO_PORT_A,  9)
#define GPIO_USART1_RXD         GPIO_PORTNUM(GPIO_PORT_A, 10)


#define GPIO_TIM2CH1            GPIO_PORTNUM(GPIO_PORT_A,  0)
#define GPIO_TIM2CH2            GPIO_PORTNUM(GPIO_PORT_A,  1)
#define GPIO_TIM2CH3            GPIO_PORTNUM(GPIO_PORT_A,  2)
#define GPIO_TIM2CH4            GPIO_PORTNUM(GPIO_PORT_A,  3)


#define GPIO_I2C2_SCL           GPIO_PORTNUM(GPIO_PORT_B, 10)
#define GPIO_I2C2_SDA           GPIO_PORTNUM(GPIO_PORT_B, 11)


#ifdef _SYSTEM_C_
const uint16_t gpioDefaultTbl[] = {

  PORT_CTRL(GPIO_POWER_SW,      GPIO_MODE_INPUT,  GPIO_PULL_UP,   GPIO_VAL_0),
  PORT_CTRL(GPIO_UPDATE_LED,    GPIO_MODE_OUTPUT, GPIO_PULL_NONE, GPIO_VAL_0),
  PORT_CTRL(GPIO_POWER_LED,     GPIO_MODE_OUTPUT, GPIO_PULL_NONE, GPIO_VAL_0),


  PORT_CTRL(GPIO_MCO1,          GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC0_SYS),
  //PORT_CTRL(GPIO_FSSOF,         GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC10_USB),
  PORT_CTRL(GPIO_MCO2,          GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC0_SYS),

  PORT_CTRL(GPIO_I2C2_SDA,      GPIO_MODE_FUNC, GPIO_PULL_UP,   GPIO_FUNC4_I2C),
  PORT_CTRL(GPIO_I2C2_SCL,      GPIO_MODE_FUNC, GPIO_PULL_UP,   GPIO_FUNC4_I2C),

  PORT_CTRL(GPIO_USART1_TXD,    GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC7_USART),
  PORT_CTRL(GPIO_USART1_RXD,    GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC7_USART),

  PORT_CTRL(GPIO_USB_FS_DM,     GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC10_USB),
  PORT_CTRL(GPIO_USB_FS_DP,     GPIO_MODE_FUNC, GPIO_PULL_NONE, GPIO_FUNC10_USB),
  PORT_CTRL(GPIO_USB_FS_EN,     GPIO_MODE_OUTPUT, GPIO_PULL_NONE, GPIO_VAL_0),

  PORT_CTRL_END,
};
#endif


#define GpioSetUpdateLedOff()           GPIO_CLEAR(GPIO_UPDATE_LED)
#define GpioSetUpdateLedOn()            GPIO_SET(GPIO_UPDATE_LED)
#define GpioSetPowerLedOff()            GPIO_CLEAR(GPIO_POWER_LED)
#define GpioSetPowerLedOn()             GPIO_SET(GPIO_POWER_LED)

#define GpioSetSpi4CsxDDS_Act()         GPIO_CLEAR(GPIO_SPI4_CSX_DDS)
#define GpioSetSpi4CsxDDS_Inact()       GPIO_SET(GPIO_SPI4_CSX_DDS)

#define GpioSetSpi5CsxFlash_Act()       GPIO_CLEAR(GPIO_SPI5_CSX_FLASH)
#define GpioSetSpi5CsxFlash_Inact()     GPIO_SET(GPIO_SPI5_CSX_FLASH)

#define GpioSetUsbEnable()              GPIO_SET(GPIO_USB_FS_EN)
#define GpioSetUsbDisable()             GPIO_CLEAR(GPIO_USB_FS_EN)


#endif
