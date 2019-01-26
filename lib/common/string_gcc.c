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

#define	_STRING_GCC_C_

#include	<stdint.h>

void *
memset(void *s, int c, int size)
{
  register uint8_t	*p;
  p = (uint8_t *)s;
  while(size > 0) {
    *p++ = c;
    size--;
  }
  return s;
}
void *
memcpy(void *d, void *s, int size)
{
  register uint8_t	*pD, *pS;

  pD = (uint8_t *)d;
  pS = (uint8_t *)s;
  while(size > 0) {
    *pD++ = *pS++;
    size--;
  }
  return s;
}

char *
strcpy(char *dst, const char *src)
{
  char	*dst_bak;
  while(*dst++ = *src++);
  return dst_bak;
}
int
strlen(const char *str)
{
  register int		i = 0;
  while(*str++ != '\0') i++;
  return i;
}
