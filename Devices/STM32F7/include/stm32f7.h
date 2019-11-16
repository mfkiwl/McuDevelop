/*
 * Copyright (c) 2019 zhtlab
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

#ifndef _STM32F7_H_
#define _STM32F7_H_

#define __CM7_REV               0
#define __MPU_PRESENT           1
#define __FPU_PRESENT           1
#define __NVIC_PRIO_BITS        4
#define __Vendor_SysTickConfig  0

#define __FPU_PRESENT           1

#ifndef __IO
#define __IO            volatile
#endif


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

  DMA1_CH0_IRQn         =  11,
  DMA1_CH1_IRQn,
  DMA1_CH2_IRQn,
  DMA1_CH3_IRQn,
  DMA1_CH4_IRQn,
  DMA1_CH5_IRQn,
  DMA1_CH6_IRQn,
  DMA1_CH7_IRQn,
  ADC1_IRQn,

  TIM1_BRK_TIM9_IRQn    =  24,
  TIM1_UP_TIM10_IRQn,
  TIM1_TRG_COM_TIM11_IRQn,
  TIM1_CC_IRQn,
  TIM2_IRQn,
  TIM3_IRQn,
  TIM4_IRQn,
  I2C1_EV_IRQn          =  31,
  I2C1_ER_IRQn,
  I2C2_EV_IRQn,
  I2C2_ER_IRQn,
  SPI1_IRQn             =  35,
  SPI2_IRQn,
  USART1_IRQn           =  37,
  USART2_IRQn,
  USART3_IRQn,
  EXTI15_10_IRQn,

  OTG_FS_WAKE_IRQn      =  42,
  SDMMC1_IRQn           =  49,

  DMA2_CH0_IRQn         =  56,
  DMA2_CH1_IRQn,
  DMA2_CH2_IRQn,
  DMA2_CH3_IRQn,
  DMA2_CH4_IRQn,

  OTG_FS_IRQn           =  67,
  DMA2_CH5_IRQn,
  DMA2_CH6_IRQn,
  DMA2_CH7_IRQn,
  USART6_IRQn,
  I2C3_EV_IRQn,
  I2C3_ER_IRQn,
  OTG_HS_EP1_OUT_IRQn   =  74,
  OTG_HS_EP1_IN_IRQn,
  OTG_HS_WAKE_IRQn,
  OTG_HS_IRQn,

  SDMMC2_IRQn           = 103,

};

typedef int     IRQn_Type;


/*************************************************************
 * base address definitions
 */
#define AHB1_BASE               (0x40020000)
#define AHB2_BASE               (0x50000000)
#define AHB3_BASE               (0xa0000000)
#define APB1_BASE               (0x40000000)
#define APB2_BASE               (0x40010000)

#define POR_PC_ENTRY_BASE       (0x08000000)


/*************************************************************
 * 03 FLASH
 */
#define FLASH_NO_PRGKEYR        1
#define FLASH_NO_FWWERR         1

#include        "stm32f7Flash.h"

#define FLASH_PTR	((stm32Dev_FLASH *) ((AHB1_BASE) + 0x3c00))


/*************************************************************
 * 05 PWR
 */
#include        "stm32f7Pwr.h"

#define PWR_PTR      ((stm32Dev_PWR *) ((APB1_BASE) + 0x7000))


/*************************************************************
 * 06 RCC
 */
#define RCC_CLOCK_LSI              32768
#define RCC_CLOCK_HSI           16000000

#include        "stm32f7Rcc.h"

#define RCC_PTR ((stm32Dev_RCC *) ((AHB1_BASE) + 0x3800))


//#define CONFIG_STM32F4_POWERSW_FUNCBASE	(0)


/*************************************************************
 * 07 SYSCFG
 */
//#include        "stm32f4Syscfg.h"

//#define SYSCFG_PTR      ((stm32Dev_SYSCFG *) ((APB2_BASE) + 0x3800))

/*************************************************************
 * 06 GPIO
 */
#undef  STM32GPIO_BRR_28H_EXIST
#include        "stm32Gpio.h"

#define GPIO_PTR	((stm32Dev_GPIO *) ((AHB1_BASE) + 0x0000))


/*************************************************************
 * 09 DMA
 */
#define DMA_NO_CSELR            1

#include        "stm32Dma7.h"

typedef enum  {
  DMA_NUM_INIT = -1,
  DMA1_NUM = 0,
  DMA2_NUM,
  DMA_NUM_MAX
} dmaNo_t;

#define DMA1_PTR ((stm32Dev_DMA *)  ((AHB1_BASE) + 0x6000))
#define DMA2_PTR ((stm32Dev_DMA *)  ((AHB1_BASE) + 0x6400))


/* F7xx   [7:4]:req-ch, [3:0]:stream(ch reg number) */
/*                               SPI1, SPI2, SPI3, SPI4, SPI5 */
#define DMAMUX_REQ_SPITX_TBL       {0x32, 0x04, 0x05, 0x40, 0x24}
#define DMAMUX_REQ_SPIRX_TBL       {0x33, 0x03, 0x00, 0x41, 0x23}
/*                               USART1, USART2, USART6 */
#define DMAMUX_REQ_USARTTX_TBL     {0x47,   0x46,   0x56}
#define DMAMUX_REQ_USARTRX_TBL     {0x45,   0x45,   0x51}
/*                               I2C1,   I2C2,   I2C3 */
#define DMAMUX_REQ_I2CTX_TBL       {0x16,   0x77,   0x34}
#define DMAMUX_REQ_I2CRX_TBL       {0x17,   0x73,   0x32}
#define DMAMUX_MODULE_I2CTX_TBL    {1,   1,   1}
#define DMAMUX_MODULE_I2CRX_TBL    {1,   1,   1}

/*                               SDMMC1, SDMMC2 */
#define DMAMUX_REQ_SDMMCTX_TBL     {0x46, 0x30}
#define DMAMUX_REQ_SDMMCRX_TBL     {0x43, 0x35}

#define DevDmaGetReq(x)         (((x) >> 4) & 0xf)
#define DevDmaGetCh(x)          ( (x)       & 0xf)


/*************************************************************
 * 12 FMC
 */
#include        "stm32Fmc.h"

#define FMC_PTR         ((stm32Dev_FMC *) (AHB3_BASE + 0x0000))

#define FMC_BANK1_PTR   ((void *) (0x60000000))
#define FMC_BANK1SRAM0_PTR   ((void *) (0x60000000))
#define FMC_BANK1SRAM1_PTR   ((void *) (0x64000000))
#define FMC_BANK1SRAM2_PTR   ((void *) (0x68000000))
#define FMC_BANK1SRAM3_PTR   ((void *) (0x6c000000))
#define FMC_BANK2_PTR   ((void *) (0x70000000))
#define FMC_BANK3_PTR   ((void *) (0x80000000))
#define FMC_BANK4_PTR   ((void *) (0x90000000))
#define FMC_BANK1SDRAM_PTR      ((void *) (0xc0000000))
#define FMC_BANK2SDRAM_PTR      ((void *) (0xd0000000))


/*************************************************************
 * 18 TIM1/8
 * 19 TIM2 -- TIM5
 * 20 TIM10 -- TIM14
 */
#include        "stm32Tim.h"

typedef enum  {
  TIM_NUM_INIT = -1,
  TIM1_NUM = 0,
  TIM2_NUM,
  TIM3_NUM,
  TIM4_NUM,
  TIM5_NUM,
  TIM6_NUM,
  TIM7_NUM,
  TIM8_NUM,
  TIM9_NUM,
  TIM10_NUM,
  TIM11_NUM,
  TIM12_NUM,
  TIM13_NUM,
  TIM14_NUM,
  TIM_NUM_MAX
} timNo_t;


#define TIM1_PTR        ((stm32Dev_TIM *) ((APB2_BASE) + 0x0000))
#define TIM2_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x0000))
#define TIM3_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x0400))
#define TIM4_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x0800))
#define TIM5_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x0c00))
#define TIM6_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x1000))
#define TIM7_PTR        ((stm32Dev_TIM *) ((APB1_BASE) + 0x1400))
#define TIM8_PTR        ((stm32Dev_TIM *) ((APB2_BASE) + 0x0400))
#define TIM9_PTR        ((stm32Dev_TIM *) ((APB2_BASE) + 0x4000))
#define TIM10_PTR       ((stm32Dev_TIM *) ((APB2_BASE) + 0x4400))
#define TIM11_PTR       ((stm32Dev_TIM *) ((APB2_BASE) + 0x4c00))
#define TIM12_PTR       ((stm32Dev_TIM *) ((APB1_BASE) + 0x1800))
#define TIM13_PTR       ((stm32Dev_TIM *) ((APB1_BASE) + 0x1c00))
#define TIM14_PTR       ((stm32Dev_TIM *) ((APB1_BASE) + 0x2000))


/*************************************************************
 * 27 USART
 */
#include        "stm32Usart.h"

#undef USART_MODULE_FIFO_YES
#define USART_NO_PRESC          1
#undef  USART_NO_ICR

typedef enum  {
  USART_NUM_INIT = -1,
  USART1_NUM = 0,
  USART2_NUM,
  USART3_NUM,
  USART4_NUM,
  USART6_NUM,
  USART_NUM_MAX
} usartNo_t;

#define USART_MODULE_COUNT      3
#define PERIUART_UNIT_MAX       3
#define USART1_PTR      ((stm32Dev_USART *) ((APB2_BASE) + 0x1000))
#define USART2_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x4400))
#define USART3_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x4800))
#define USART4_PTR      ((stm32Dev_USART *) ((APB1_BASE) + 0x4000))
#define USART6_PTR      ((stm32Dev_USART *) ((APB2_BASE) + 0x1400))


/*************************************************************
 * 28 SPI
 */

#define SPI_MODULE_COUNT        2
#define SPI_MODULE_FIFO_YES     1

typedef enum  {
  SPI_NUM_INIT = -1,
  SPI1_NUM = 0,
  SPI2_NUM,
  SPI3_NUM,
  SPI4_NUM,
  SPI5_NUM,
  SPI_NUM_MAX
} spiNo_t;

#include        "stm32Spi16.h"

#define SPI1_PTR        ((stm32Dev_SPI *) ((APB2_BASE) + 0x3000))
#define SPI2_PTR        ((stm32Dev_SPI *) ((APB1_BASE) + 0x3800))
#define SPI3_PTR        ((stm32Dev_SPI *) ((APB1_BASE) + 0x3c00))
#define SPI4_PTR        ((stm32Dev_SPI *) ((APB2_BASE) + 0x3400))
#define SPI5_PTR        ((stm32Dev_SPI *) ((APB2_BASE) + 0x5000))


/*************************************************************
 * 26 I2C
 */

#define I2C_MODULE_COUNT        2
//#define SPI_MODULE_FIFO_NO      1

typedef enum  {
  I2C_NUM_INIT = -1,
  I2C1_NUM = 0,
  I2C2_NUM,
  I2C3_NUM,
  I2C_NUM_MAX
} i2cNo_t;

#include        "stm32I2c.h"

#define I2C1_PTR      ((stm32Dev_I2C *) ((APB1_BASE) + 0x5400))
#define I2C2_PTR      ((stm32Dev_I2C *) ((APB1_BASE) + 0x5800))
#define I2C3_PTR      ((stm32Dev_I2C *) ((APB1_BASE) + 0x5c00))


/*************************************************************
 * 30 SDMMC
 */
typedef enum  {
  SDMMC_NUM_INIT = -1,
  SDMMC1_NUM = 0,
  SDMMC2_NUM,
  SDMMC_NUM_MAX
} sdmmcNo_t;

#include        "stm32Sdmmc.h"

#define SDMMC1_PTR      ((stm32Dev_SDMMC *) ((APB2_BASE) + 0x2c00))
#define SDMMC2_PTR      ((stm32Dev_SDMMC *) ((APB2_BASE) + 0x1c00))


/*******************************************
 * 32 OTG
 */
#define USB_EPOUT_COUNTS        9     /* max number of ep  plus 1 */
#define USB_EPIN_COUNTS         9     /* max number of ep  plus 1 */
#define USB_HS_INTPHY           1

#include        "stm32Otg.h"

#define USB1_HS                 ((stm32Dev_USB *) ((AHB1_BASE)+0x20000))
#define USB2_FS                 ((stm32Dev_USB *) ((AHB2_BASE)+0x00000))
#define USB1_HS_PTR             ((stm32Dev_USB *) ((AHB1_BASE)+0x20000))
#define USB2_FS_PTR             ((stm32Dev_USB *) ((AHB2_BASE)+0x00000))

#define USB_FS_MAX_PACKET_SIZE          64
#define USB_HS_MAX_PACKET_SIZE          512
#define USB_SPEED_SUPER                 3
#define USB_SPEED_HIGH                  2
#define USB_SPEED_LOW                   1
#define USB_SPEED_FULL                  0

#define USB_MODULE_TBL                  {USB2_OTG_FS, USB1_OTG_HS, USB2_OTG_FS}
typedef enum  {
  USB_NUM_INIT = -1,
  USBHS_NUM = 1,
  USBFS_NUM,
  USB_NUM_MAX
} usbNo_t;


/*******************************************
 * 33 USBPHY
 */
#include        "stm32Usbphyc.h"
#define USBPHYC_PTR             ((stm32Dev_USBPHYC *)((APB2_BASE)+0x07c00))


#endif
