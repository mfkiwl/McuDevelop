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

#ifndef _STM32OTG_H_
#define _STM32OTG_H_

typedef struct {
  __IO uint32_t         POWER;          // 0x00
  __IO uint32_t         CLKCR;          // 0x04
  __IO uint32_t         ARG;            // 0x08
  __IO uint32_t         CMD;            // 0x0c
  __IO uint32_t         RESPCMD;        // 0x10
  __IO uint32_t         RESP1;          // 0x14
  __IO uint32_t         RESP2;          // 0x18
  __IO uint32_t         RESP3;          // 0x1c
  __IO uint32_t         RESP4;          // 0x20
  __IO uint32_t         DTIMER;         // 0x24
  __IO uint32_t         DLEN;           // 0x28
  __IO uint32_t         DCTRL;          // 0x2c
  __IO uint32_t         DCOUNT;         // 0x30
  __IO uint32_t         STA;            // 0x34
  __IO uint32_t         ICR;            // 0x38
  __IO uint32_t         MASK;           // 0x3c
  uint32_t              reserved40;     // 0x40
  uint32_t              reserved44;     // 0x44
  __IO uint32_t         FIFOCNT;        // 0x48
  uint32_t              reserved44[13]; // 0x4c
  __IO uint32_t         FIFO;           // 0x80

} stm32Dev_SDMMC;

#endif
