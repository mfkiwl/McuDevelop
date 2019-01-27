#ifndef	_PERIUSB_H_
#define	_PERIUSB_H_
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


#define	PERIUSB_SYNC_CHAR	0x7f

int		PeriUsbInit(int unit);
int		PeriUsbUninit(int unit);
int		PeriUsbProbe(int unit);
int		PeriUsbSend(int unit, const uint8_t *ptr, int len);
int		PeriUsbRecv(int unit, uint8_t *ptr, int *len);
int		PeriUsbIoctl(int unit, int req, void *ptr, int len);

#ifdef  _PERIUSB_C_
static void     PeriUsbGpioSetFsEnableOff(void);
static void     PeriUsbGpioSetFsEnableOn(void);
#endif

#endif
