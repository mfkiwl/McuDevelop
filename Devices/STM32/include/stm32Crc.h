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

#ifndef _STM32CRC_H_
#define _STM32CRC_H_

typedef struct {
/*** 0x00  CRC CR */
  __IO uint32_t         DR;
  __IO uint32_t         IDR;
  __IO uint32_t         CR;
#define CRC_CR_REV_OUT_SHIFT    (7)
#define CRC_CR_REV_OUT_MASK     (1 << (CRC_CR_REV_OUT_SHIFT))
#define CRC_CR_REV_OUT_NO       (0 << (CRC_CR_REV_OUT_SHIFT))
#define CRC_CR_REV_OUT_YES      (1 << (CRC_CR_REV_OUT_SHIFT))
#define CRC_CR_REV_IN_SHIFT     (5)
#define CRC_CR_REV_IN_MASK      (3 << (CRC_CR_REV_IN_SHIFT))
#define CRC_CR_REV_IN_NO        (0 << (CRC_CR_REV_IN_SHIFT))
#define CRC_CR_REV_IN_BYTE      (1 << (CRC_CR_REV_IN_SHIFT))
#define CRC_CR_REV_IN_HALFWORD  (2 << (CRC_CR_REV_IN_SHIFT))
#define CRC_CR_REV_IN_WORD      (3 << (CRC_CR_REV_IN_SHIFT))
#define CRC_CR_POLYSIZE_SHIFT   (3)
#define CRC_CR_POLYSIZE_MASK    (3 << (CRC_CR_POLYSIZE_SHIFT))
#define CRC_CR_POLYSIZE_32      (0 << (CRC_CR_POLYSIZE_SHIFT))
#define CRC_CR_POLYSIZE_16      (1 << (CRC_CR_POLYSIZE_SHIFT))
#define CRC_CR_POLYSIZE_8       (2 << (CRC_CR_POLYSIZE_SHIFT))
#define CRC_CR_POLYSIZE_7       (3 << (CRC_CR_POLYSIZE_SHIFT))
#define CRC_CR_RESET_SHIFT      (0)
#define CRC_CR_RESET_MASK       (1 << (CRC_CR_RESET_SHIFT))
#define CRC_CR_RESET_NO         (0 << (CRC_CR_RESET_SHIFT))
#define CRC_CR_RESET_YES        (1 << (CRC_CR_RESET_SHIFT))

  __IO uint32_t         Reserved10;
  __IO uint32_t         INIT;
#define CRC_INIT_VALUE          (0xffffffff)
  __IO uint32_t         POL;
#define CRC_POL_CRC32           (0x04c11bd7)

} stm32Dev_CRC;

#endif
