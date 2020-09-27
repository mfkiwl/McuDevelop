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

#ifndef	_PERI_H_
#define	_PERI_H_

#ifdef	COMPILER_GCC
#define	__ATTR_WEAK	__attribute__((weak))
#else
#define	__ATTR_WEAK
#endif

#define	PERI_SUCCESS		(0)
#define	PERI_ERROR_UNKNOWN	(-1)
#define	PERI_ERROR_NOTSELECTED	(-2)
#define	PERI_ERROR_EXCEEDED	(-2)


#define	PERI_IF_NONE		0
#define	PERI_IF_UART		1
#define	PERI_IF_SPI		2
#define	PERI_IF_I2C		3
#define	PERI_IF_USB		4
#define	PERI_IF_SD		5


struct _stPeriEntry {
  int		no;
#define	PERI_LIST_TERMINATOR		(-1)
  int		unit;
  int		(*init)(int unit);
  int		(*uninit)(int unit);
  int		(*probe)(int unit);
  int		(*send)(int unit, const uint8_t *ptr, int len);
  int		(*recv)(int unit, uint8_t *ptr, int *len);
  int		(*ioctl)(int unit, int req, void *ptr, int len);
#define	PERI_GET_RECV_SIZE		(0x0001)
};

struct _stPeri {
#define	PERI_RX_BUF_SIZE	260
  int			selectedEntry;

  uint8_t		bufRx[PERI_RX_BUF_SIZE];
  int			lenRx;
};


void		PeriInit(void);
void		PeriUninit(void);
void		PeriUninitExceptSelected(void);
int		PeriSearchResponsedUnit(void);
int		PeriSelectActiveUnit(int sel);

int		PeriSend(const uint8_t *ptr, int len);
int		PeriRecv(uint8_t *ptr, int *len);

int		PeriGetRecvSize(void);


int		PeriDevInit(int unit);
int		PeriDevUninit(int unit);
int		PeriDevProbe(int unit);
int		PeriDevSend(int unit, const uint8_t *ptr, int len);
int		PeriDevRecv(int unit, uint8_t *ptr, int *len);
int		PeriDevIoctl(int unit, int req, void *ptr, int len);

#if 0
int		PeriUartInit(int unit)	__ATTR_WEAK;
int		PeriUartUninit(int unit)	__ATTR_WEAK;
int		PeriUartProbe(int unit)	__ATTR_WEAK;
int		PeriUartSend(int unit, const uint8_t *ptr, int len) __ATTR_WEAK;
int		PeriUartRecv(int unit, uint8_t *ptr, int *len) __ATTR_WEAK;
int		PeriUartIoctl(int unit, int req, void *ptr, int len) __ATTR_WEAK;
int		PeriI2cInit(int unit)	__ATTR_WEAK;
int		PeriI2cUninit(int unit)	__ATTR_WEAK;
int		PeriI2cProbe(int unit)	__ATTR_WEAK;
int		PeriI2cSend(int unit, const uint8_t *ptr, int len) __ATTR_WEAK;
int		PeriI2cRecv(int unit, uint8_t *ptr, int *len) __ATTR_WEAK;
int		PeriI2cIoctl(int unit, int req, void *ptr, int len) __ATTR_WEAK;
int		PeriSpiInit(int unit)	__ATTR_WEAK;
int		PeriSpiUninit(int unit)	__ATTR_WEAK;
int		PeriSpiProbe(int unit)	__ATTR_WEAK;
int		PeriSpiSend(int unit, const uint8_t *ptr, int len) __ATTR_WEAK;
int		PeriSpiRecv(int unit, uint8_t *ptr, int *len) __ATTR_WEAK;
int		PeriSpiIoctl(int unit, int req, void *ptr, int len) __ATTR_WEAK;
int		PeriUsbInit(int unit)	__ATTR_WEAK;
int		PeriUsbUninit(int unit)	__ATTR_WEAK;
int		PeriUsbProbe(int unit)	__ATTR_WEAK;
int		PeriUsbSend(int unit, const uint8_t *ptr, int len) __ATTR_WEAK;
int		PeriUsbRecv(int unit, uint8_t *ptr, int *len) __ATTR_WEAK;
int		PeriUsbIoctl(int unit, int req, void *ptr, int len) __ATTR_WEAK;
#endif

#endif
