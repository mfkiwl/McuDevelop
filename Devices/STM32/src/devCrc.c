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

#define _DEVCRC_C_

#include        <stdint.h>
#include        "system.h"
#include        "devErrno.h"
#include        "devCrc.h"

#include        "stm32Crc.h"
int
DevCrcInit(devCrcParam_t *param)
{
  int           result = DEV_ERRNO_UNKNOWN;
  uint32_t      cr;

  /* reset, auto clear */
  CRC_PTR->CR = CRC_CR_RESET_YES;

  cr = 0;

  /* polynominal size */
  switch(param->cr & DEVCRC_CR_POLYSIZE_MASK) {
  case  DEVCRC_CR_POLYSIZE_7BIT:
    cr |= CRC_CR_POLYSIZE_7;
    break;
  case  DEVCRC_CR_POLYSIZE_8BIT:
    cr |= CRC_CR_POLYSIZE_8;
    break;
  case  DEVCRC_CR_POLYSIZE_16BIT:
    cr |= CRC_CR_POLYSIZE_16;
    break;
  case  DEVCRC_CR_POLYSIZE_32BIT:
    cr |= CRC_CR_POLYSIZE_32;
    break;
  }

  /* bit reverse */
  switch(param->cr & DEVCRC_CR_REVIN_MASK) {
  case  DEVCRC_CR_REVIN_NO:
    break;
  case  DEVCRC_CR_REVIN_8BIT:
    cr |= CRC_CR_REV_IN_BYTE;
    break;
  case  DEVCRC_CR_REVIN_16BIT:
    cr |= CRC_CR_REV_IN_HALFWORD;
    break;
  case  DEVCRC_CR_REVIN_32BIT:
    cr |= CRC_CR_REV_IN_WORD;
    break;
  }

  if(param->cr & DEVCRC_CR_REVOUT_32BIT) {
    cr |=  CRC_CR_REV_OUT_YES;
  }

  CRC_PTR->CR = cr;

  CRC_PTR->INIT = param->init;  /* set the initial value */
  CRC_PTR->POL = param->poly;   /* set the polynominal */

  return DEV_ERRNO_SUCCESS;
}
int
DevCrcCalcBufWord(uint32_t *ptr, int count)
{
  for(int i = 0; i < count; i++) {
    DevCrcCalc32(*ptr++);
  }
}
int
DevCrcCalcBufHalfWord(uint16_t *ptr, int count)
{
  for(int i = 0; i < count; i++) {
    DevCrcCalc16(*ptr++);
  }
}
int
DevCrcCalcBufByte(uint8_t *ptr, int count)
{
  for(int i = 0; i < count; i++) {
    DevCrcCalc8(*ptr++);
  }
}
