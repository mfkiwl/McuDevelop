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

#ifndef _STM32USBPHYC_H_
#define _STM32USBPHYC_H_

typedef struct {
  __IO uint32_t         PLL1;
#define USBPHYC_PLL1_EN_SHIFT           (0)
#define USBPHYC_PLL1_EN_MASK            (1 << (USBPHYC_PLL1_EN_SHIFT))
#define USBPHYC_PLL1_EN_NO              (0 << (USBPHYC_PLL1_EN_SHIFT))
#define USBPHYC_PLL1_EN_YES             (1 << (USBPHYC_PLL1_EN_SHIFT))
#define USBPHYC_PLL1_SEL_SHIFT          (1)
#define USBPHYC_PLL1_SEL_MASK           (7 << (USBPHYC_PLL1_SEL_SHIFT))
#define USBPHYC_PLL1_SEL_12MHZ          (0 << (USBPHYC_PLL1_SEL_SHIFT))
#define USBPHYC_PLL1_SEL_12_5MHZ        (1 << (USBPHYC_PLL1_SEL_SHIFT))
#define USBPHYC_PLL1_SEL_12_5MHZ        (2 << (USBPHYC_PLL1_SEL_SHIFT))
#define USBPHYC_PLL1_SEL_16MHZ          (3 << (USBPHYC_PLL1_SEL_SHIFT))
#define USBPHYC_PLL1_SEL_24MHZ          (4 << (USBPHYC_PLL1_SEL_SHIFT))
#define USBPHYC_PLL1_SEL_25MHZ          (5 << (USBPHYC_PLL1_SEL_SHIFT))
#define USBPHYC_PLL1_SEL_25MHZ          (6 << (USBPHYC_PLL1_SEL_SHIFT))

  uint32_t              reserved04;
  uint32_t              reserved08;

  __IO uint32_t         TUNE;

  uint32_t              reserved10;
  uint32_t              reserved14;

  __IO uint32_t         LDO;
#define USBPHYC_LDO_DISABLE_SHIFT       (2)
#define USBPHYC_LDO_DISABLE_MASK        (1 << (USBPHYC_LDO_DISABLE_SHIFT))
#define USBPHYC_LDO_DISABLE_NO          (0 << (USBPHYC_LDO_DISABLE_SHIFT))
#define USBPHYC_LDO_DISABLE_YES         (1 << (USBPHYC_LDO_DISABLE_SHIFT))
#define USBPHYC_LDO_STATUS_SHIFT        (1)
#define USBPHYC_LDO_STATUS_MASK         (1 << (USBPHYC_LDO_STATUS_SHIFT))
#define USBPHYC_LDO_USED_SHIFT          (0)
#define USBPHYC_LDO_USED_MASK           (1 << (USBPHYC_LDO_USED_SHIFT))
} stm32Dev_USBPHYC;

#endif
