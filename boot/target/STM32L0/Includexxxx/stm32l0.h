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

#ifndef _STM32L0_H_
#define _STM32L0_H_

#define __CM0PLUS_REV           1
#define __MPU_PRESENT           1
#define __FPU_PRESENT           1
#define __NVIC_PRIO_BITS        4
#define __Vendor_SysTickConfig  0

#ifndef __IO
#define __IO            volatile
#endif


#define __VTOR_PRESENT          1



/*************************************************************
 * base address definitions
 */
#define IOPORT_BASE             (0x50000000)
#define AHB1_BASE               (0x40020000)
#define APB1_BASE               (0x40000000)
#define APB2_BASE               (0x40010000)

#define SRAM_BASE               (0x20000000)


/*************************************************************
 * interrupt request definitions
 */
enum irqNumbers {
  Reset_IRQn            =  (-15),
  NMI_IRQn              =  (-14),
  HardFault_IRQn        =  (-13),
  MemoryManagement_IRQn =  (-12),
  BusFault_IRQn         =  (-11),
  UsageFault_IRQn       =  (-10),
  SVCall_IRQn           =  ( -5),
  DebugMonitor_IRQn     =  ( -4),
  PendSV_IRQn           =  ( -2),
  SysTick_IRQn          =  ( -1),
  ADC1_IRQn             =    12,
  TIM2_IRQn             =    15,
  TIM3_IRQn,
  TIM6_DAC_IRQn,
  TIM7_IRQn,
  TIM21_IRQn            =    20,
  I2C3_IRQn,
  TIM22_IRQn,
  I2C1_IRQn,
  I2C2_IRQn,
  SPI1_IRQn,
  SPI2_IRQn,
  USART1_IRQn,
  USART2_IRQn,

  USB_IRQn              =    31,

};

typedef int     IRQn_Type;


/*************************************************************
 * 03 FLASH
 */

#include        "stm32l0Flash.h"

#define FLASH_PTR ((stm32Dev_FLASH *) ((AHB1_BASE) + 0x2000))


/*************************************************************
 * 07 RCC
 */
#define RCC_CLOCK_LSI              37000
#define RCC_CLOCK_LSE              32768
#define RCC_CLOCK_HSI           16000000

#include        "stm32l0Rcc.h"

#define RCC_PTR ((stm32Dev_RCC *) ((AHB1_BASE) + 0x1000))


/*************************************************************
 * 09 GPIO
 */

#include        "stm32Gpio.h"

#define GPIO_PTR        ((stm32Dev_GPIO *) ((IOPORT_BASE) + 0x0000))


/*************************************************************
 * 20,21,22 TIM2,3, TIM21,22, TIM6,7
 */

#include        "stm32Tim.h"

#define TIM2_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x0000))
#define TIM3_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x0400))
#define TIM6_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x1000))
#define TIM7_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x1400))
#define TIM21_PTR       ((stm32Dev_TIM *) ((APB2_BASE) + 0x0800))
#define TIM22_PTR       ((stm32Dev_TIM *) ((APB2_BASE) + 0x1400))


/*************************************************************
 * 27 I2C
 */

#include        "stm32I2c.h"

#define I2C1_PTR        ((stm32Dev_I2C *) ((APB1_BASE) + 0x5400))
#define I2C2_PTR        ((stm32Dev_I2C *) ((APB1_BASE) + 0x5800))
#define I2C3_PTR        ((stm32Dev_I2C *) ((APB1_BASE) + 0x7800))


/*************************************************************
 * 28 USART
 */

#define PERIUART_UNIT_MAX               2

#include        "stm32Usart.h"

#define USART1_PTR      ((stm32Dev_USART *) ((APB2_BASE) + 0x3800))
#define USART2_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x4400))
#define USART4_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x4c00))
#define USART5_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x5000))

#define LPUART1_PTR     ((stm32Dev_USART *) ((APB1_BASE) + 0x4800))


/*************************************************************
 * 30 SPI
 */

#if 0
#include        "stm32Spi.h"

#define SPI1_PTR        ((stm32Dev_SPI *) ((APB2_BASE) + 0x3000))
#endif


/*************************************************************
 * 31 USB
 */

#include        "stm32Usb16.h"

#define USB1_PTR        ((stm32Dev_SPI *) ((APB1_BASE) + 0x5c00))
#define USBSRAM_PTR     ((uint32_t *) ((APB1_BASE) + 0x6000))


#endif
