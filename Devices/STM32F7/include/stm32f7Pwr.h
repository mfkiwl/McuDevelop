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

#ifndef _STM32F4PWR_H_
#define _STM32F4PWR_H_

typedef struct {
  __IO uint32_t         CR1;            /* 0x00 */
#define PWR_CR1_ODSWEN_SHIFT            17
#define PWR_CR1_ODSWEN_MASK             (1 << (PWR_CR1_ODSWEN_SHIFT))
#define PWR_CR1_ODSWEN_NO               (0 << (PWR_CR1_ODSWEN_SHIFT))
#define PWR_CR1_ODSWEN_YES              (1 << (PWR_CR1_ODSWEN_SHIFT))
#define PWR_CR1_ODEN_SHIFT              16
#define PWR_CR1_ODEN_MASK               (1 << (PWR_CR1_ODEN_SHIFT))
#define PWR_CR1_ODEN_NO                 (0 << (PWR_CR1_ODEN_SHIFT))
#define PWR_CR1_ODEN_YES                (1 << (PWR_CR1_ODEN_SHIFT))
#define PWR_CR1_VOS_SHIFT               14
#define PWR_CR1_VOS_MASK                (3 << (PWR_CR1_VOS_SHIFT))
#define PWR_CR1_VOS_RANGE_FORBIDDEN     (0 << (PWR_CR1_VOS_SHIFT))
#define PWR_CR1_VOS_RANGE3_144MHZ       (1 << (PWR_CR1_VOS_SHIFT))
#define PWR_CR1_VOS_RANGE2_168MHZ       (2 << (PWR_CR1_VOS_SHIFT))
#define PWR_CR1_VOS_RANGE1_180MHZ       (3 << (PWR_CR1_VOS_SHIFT))

  __IO uint32_t         CSR1;           /* 0x04 */
#define PWR_CSR1_ODSWRDY_SHIFT          17
#define PWR_CSR1_ODSWRDY_MASK           (1 << (PWR_CSR1_ODSWRDY_SHIFT))
#define PWR_CSR1_ODRDY_SHIFT            16
#define PWR_CSR1_ODRDY_MASK             (1 << (PWR_CSR1_ODRDY_SHIFT))
#define PWR_CSR1_VOSRDY_SHIFT           14
#define PWR_CSR1_VOSRDY_MASK            (1 << (PWR_CSR1_VOSRDY_SHIFT))

  __IO uint32_t         CR2;            /* 0x08 */

  __IO uint32_t         CSR2;           /* 0x0c */

} stm32Dev_PWR;

#endif
