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

#ifndef _DEVCRC_H_
#define _DEVCRC_H_

typedef struct {
  int           cr;
#define DEVCRC_CR_REVIN_SHIFT   (10)
#define DEVCRC_CR_REVIN_MASK    (3<<(DEVCRC_CR_REVIN_SHIFT))
#define DEVCRC_CR_REVIN_NO      (0<<(DEVCRC_CR_REVIN_SHIFT))
#define DEVCRC_CR_REVIN_8BIT    (1<<(DEVCRC_CR_REVIN_SHIFT))
#define DEVCRC_CR_REVIN_16BIT   (2<<(DEVCRC_CR_REVIN_SHIFT))
#define DEVCRC_CR_REVIN_32BIT   (3<<(DEVCRC_CR_REVIN_SHIFT))
  /* bit reverse */
#define DEVCRC_CR_REVOUT_SHIFT  (8)
#define DEVCRC_CR_REVOUT_MASK   (1<<(DEVCRC_CR_REVOUT_SHIFT))
#define DEVCRC_CR_REVOUT_NO     (0<<(DEVCRC_CR_REVOUT_SHIFT))
#define DEVCRC_CR_REVOUT_32BIT  (3<<(DEVCRC_CR_REVOUT_SHIFT))
#define DEVCRC_CR_POLYSIZE_SHIFT (0)
#define DEVCRC_CR_POLYSIZE_MASK (0xff<<(DEVCRC_CR_POLYSIZE_SHIFT))
#define DEVCRC_CR_POLYSIZE_VAL(x) (((x)<<(DEVCRC_CR_POLYSIZE_SHIFT)) & (DEVCRC_CR_POLYSIZE_MASK))
#define DEVCRC_CR_POLYSIZE_7BIT  (7<<(DEVCRC_CR_POLYSIZE_SHIFT))
#define DEVCRC_CR_POLYSIZE_8BIT  (8<<(DEVCRC_CR_POLYSIZE_SHIFT))
#define DEVCRC_CR_POLYSIZE_16BIT (16<<(DEVCRC_CR_POLYSIZE_SHIFT))
#define DEVCRC_CR_POLYSIZE_32BIT (32<<(DEVCRC_CR_POLYSIZE_SHIFT))

  uint32_t      poly;
#define DEVCRC_POLY_CRC7        0x09
#define DEVCRC_POLY_CRC8        0xd5
#define DEVCRC_POLY_CRC8ATM     0x07
#define DEVCRC_POLY_CRC8CCITT   0x31
#define DEVCRC_POLY_CRC16CCITT  0x1021
#define DEVCRC_POLY_CRC16IBM    0x1005
#define DEVCRC_POLY_CRC32       0x04c11db7

  uint32_t      init;   /* initial value */
#define DEVCRC_INIT_0XFFFFFFFF  0xffffffff

} devCrcParam_t;

int             DevCrcInit(devCrcParam_t *param);
#define DevCrcCalc8(x)          {*(volatile uint8_t  *)&CRC_PTR->DR = (x);}
#define DevCrcCalc16(x)         {*(volatile uint16_t *)&CRC_PTR->DR = (x);}
#define DevCrcCalc32(x)         {*(volatile uint32_t *)&CRC_PTR->DR = (x);}
#define DevCrcGetValue()        (CRC_PTR->DR)

#define DevCrcSetInit(x)        {CRC_PTR->INIT = (uint32_t)(x);}

#endif
