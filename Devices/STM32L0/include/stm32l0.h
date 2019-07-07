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
#define AHB2_BASE               (IOPORT_BASE)
#define AHB1_BASE               (0x40020000)
#define APB1_BASE               (0x40000000)
#define APB2_BASE               (0x40010000)

#define SRAM_BASE               (0x20000000)

#define POR_PC_ENTRY_BASE       (0x08000000)
#define EEPROM_BASE             (0x08080000)


/*************************************************************
 * interrupt request definitions
 */
enum irqNumbers {
  Reset_IRQn            = (-15),
  NMI_IRQn              = (-14),
  HardFault_IRQn        = (-13),
  MemoryManagement_IRQn = (-12),
  BusFault_IRQn         = (-11),
  UsageFault_IRQn       = (-10),
  SVCall_IRQn           = ( -5),
  DebugMonitor_IRQn     = ( -4),
  PendSV_IRQn           = ( -2),
  SysTick_IRQn          = ( -1),
  WWDG_IRQn             =    0, //  0
  PVD_IRQn,                     //  1
  RTC_IRQn,                     //  2
  FLASH_IRQn,                   //  3
  RCCCRS_IRQn,                  //  4
  EXTI1_0_IRQn,                 //  5
  EXTI3_2_IRQn,                 //  6
  EXTI15_4_IRQn,                //  7
  TSC_IRQn,                     //  8
  DMA1_CH1_IRQn,                //  9
  DMA1_CH2TO3_IRQn,             // 10
  DMA1_CH4TO7_IRQn,             // 11
  ADC1_IRQn,                    // 12
  LPTIM1_IRQn,                  // 13
  USART45_IRQn,                 // 14
  TIM2_IRQn,                    // 15
  TIM3_IRQn,                    // 16
  TIM6_DAC_IRQn,                // 17
  TIM7_IRQn,                    // 18
  Reserved19_IRQn,              // 19
  TIM21_IRQn,                   // 20
  I2C3_IRQn,                    // 21
  TIM22_IRQn,                   // 22
  I2C1_IRQn,                    // 23
  I2C2_IRQn,                    // 24
  SPI1_IRQn,                    // 25
  SPI2_IRQn,                    // 26
  USART1_IRQn,                  // 27
  USART2_IRQn,                  // 28
  LPUART1_AES_RNG_IRQn,         // 29
  Reserved30_IRQn,              // 30
  USB_FS_IRQn,                  // 31

};

typedef int     IRQn_Type;

/*************************************************************
 * 02 SYSCFG
 */
#include        "stm32l0Syscfg.h"

#define SYSCFG_PTR      ((stm32Dev_SYSCFG *) ((APB2_BASE) + 0x0000))


/*************************************************************
 * 03 FLASH
 */

#include        "stm32l0Flash.h"

#define FLASH_PTR ((stm32Dev_FLASH *) ((AHB1_BASE) + 0x2000))


/*************************************************************
 * 04 CRC
 */

#include        "stm32Crc.h"

#define CRC_PTR         ((stm32Dev_CRC *) ((AHB1_BASE) + 0x3000))


/*************************************************************
 * 06 PWR
 */

#include        "stm32l0Pwr.h"

#define PWR_PTR ((stm32Dev_PWR *) ((APB1_BASE) + 0x7000))


/*************************************************************
 * 07 RCC
 */
#define RCC_CLOCK_LSI              37000
#define RCC_CLOCK_LSE              32768
#define RCC_CLOCK_MSI_4MHZ       4000000
#define RCC_CLOCK_HSI           16000000

#include        "stm32l0Rcc.h"

#define RCC_PTR ((stm32Dev_RCC *) ((AHB1_BASE) + 0x1000))

#define RCC_PLL_N_TBL   {3, 4, 6, 8, 16, 24, 32, 48}

/*******************************************
 * 08 CRS
 */

#include        "stm32Crs.h"

#define CRS_PTR        ((stm32Dev_CRS *) (APB1_BASE + 0x6c00))


/*************************************************************
 * 09 GPIO
 */

#define STM32GPIO_BRR_28H_EXIST         1

#include        "stm32Gpio.h"

#define GPIO_PTR        ((stm32Dev_GPIO *) ((IOPORT_BASE) + 0x0000))


/*************************************************************
 * 11 DMACH
 */
#define DMA_MODULE_COUNT                (2)

#include "stm32Dma4.h"

#define DMA1_PTR ((stm32Dev_DMA *)  ((AHB1_BASE) + 0x0000))


#define DMA_MODULE1     (1)
#define DMA_MODULE2     (2)

#define DMA_CH1         (1)
#define DMA_CH2         (2)
#define DMA_CH3         (3)
#define DMA_CH4         (4)
#define DMA_CH5         (5)
#define DMA_CH6         (6)
#define DMA_CH7         (7)

/* [7:4]:ch [3:0]:req */
#define DMA_REQ_SPITX_TBL       {0, 0x31, 0x52}
#define DMA_REQ_SPIRX_TBL       {0, 0x21, 0x42}
#define DMA_REQ_USARTTX_TBL     {0, 0x43, 0x74}
#define DMA_REQ_USARTRX_TBL     {0, 0x53, 0x64}


/*************************************************************
 * 20,21,22 TIM2,3, TIM21,22, TIM6,7
 */

#include        "stm32Tim.h"

typedef enum  {
  TIM_NUM_INIT = -1,
  TIM2_NUM = 0,
  TIM3_NUM,
  TIM6_NUM,
  TIM7_NUM,
  TIM21_NUM,
  TIM22_NUM,
  TIM_NUM_MAX
} timNo_t;
typedef enum  {
  TIM_UNIT_INIT = -1,
  TIM2_UNIT = 0,
  TIM3_UNIT,
  TIM6_UNIT,
  TIM7_UNIT,
  TIM21_UNIT,
  TIM22_UNIT,
  TIM_UNIT_MAX
} timUnit_t;

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

typedef enum  {
  I2C_UNIT_INIT = -1,
  I2C1_UNIT = 0,
  I2C2_UNIT,
  I2C3_UNIT,
  I2C_UNIT_MAX
} i2cUnit_t;

#define I2C1_PTR        ((stm32Dev_I2C *) ((APB1_BASE) + 0x5400))
#define I2C2_PTR        ((stm32Dev_I2C *) ((APB1_BASE) + 0x5800))
#define I2C3_PTR        ((stm32Dev_I2C *) ((APB1_BASE) + 0x7800))


/*************************************************************
 * 28 USART
 */

#define PERIUART_UNIT_MAX               3
#define USART_MODULE_COUNT              3

#include        "stm32Usart.h"

typedef enum  {
  USART_UNIT_INIT = -1,
  USART1_UNIT = 0,
  USART2_UNIT,
  USART4_UNIT,
  USART5_UNIT,
  USART_UNIT_MAX
} usartUnit_t;

#define USART1_PTR      ((stm32Dev_USART *) ((APB2_BASE) + 0x3800))
#define USART2_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x4400))
#define USART4_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x4c00))
#define USART5_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x5000))

//#define LPUART1_PTR     ((stm32Dev_USART *) ((APB1_BASE) + 0x4800))


/*************************************************************
 * 30 SPI
 */

#define SPI_MODULE_COUNT        1
#define SPI_MODULE_FIFO_NO      1

#include        "stm32Spi16.h"

typedef enum  {
  SPI_UNIT_INIT = -1,
  SPI1_UNIT = 0,
  SPI2_UNIT,
  SPI_UNIT_MAX
} spiUnit_t;

#define SPI1_PTR        ((stm32Dev_SPI *) ((APB2_BASE) + 0x3000))


/*************************************************************
 * 31 USB
 */

#define USB_MODULE_COUNT                (1)

/* EP0 - EP7 */
#define USB_MAX_EPIN            7
#define USB_MAX_EPOUT           7

#include        "stm32Usb16.h"

#define USB_FS_PTR      ((stm32Dev_USB *) ((APB1_BASE) + 0x5c00))
#define USBSRAM_PTR     ((stm32Dev_USB_SRAM_Header *) ((APB1_BASE) + 0x6000))
#define USBSRAM_SIZE    (1024)


#endif
