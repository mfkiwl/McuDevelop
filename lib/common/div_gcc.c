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

#define	_DIV_GCC_C_

#include	<stdint.h>
#include	"div_gcc.h"

uint32_t
___uidiv32(uint32_t *val, uint32_t div)
{
  register uint32_t	re = 0;
  register int		cnt = 0;

  if(div == 0) {
    while(1);
#if 0
    re = *val;
    goto fail;
#endif
  }
  while(!(div & (1UL<<31))) {
    div <<= 1;
    cnt++;
  }

  while(cnt >= 0) {
    re <<= 1;
    if(*val >= div) {
      re |= 1;
      *val -= div;
    }

    div >>= 1;
    cnt--;
  }

  return re;
}
uint32_t
__aeabi_uidiv(uint32_t val, register uint32_t div)
{
  return ___uidiv32(&val, div);
}
uint32_t
__aeabi_uidivmod(uint32_t val, register uint32_t div)
{
  ___uidiv32(&val, div);
  return val;
}
