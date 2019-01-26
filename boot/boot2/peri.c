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

#define	_PERI_C_

#include	<stdlib.h>
#include	<stdint.h>
#include	<string.h>

#include	"config.h"

#include	"periUart.h"
#include	"periI2c.h"
#if (defined(CONFIG_PERIPHERAL_USB1) || defined(CONFIG_PERIPHERAL_USB2) || defined(CONFIG_PERIPHERAL_USB_ALL))
#include	"periUsb.h"
#endif
#include	"peri.h"


struct _stPeri	peri;

const struct _stPeriEntry periEntry[] = {
#if (defined(CONFIG_PERIPHERAL_UART1) || defined(CONFIG_PERIPHERAL_UART_ALL))
  {0, 0, PeriUartInit, PeriUartUninit, PeriUartProbe,
   PeriUartSend, PeriUartRecv, PeriUartIoctl},		/* USART1 */
#endif
#if (defined(CONFIG_PERIPHERAL_UART2) || defined(CONFIG_PERIPHERAL_UART_ALL))
 {1, 1, PeriUartInit, PeriUartUninit, PeriUartProbe,
   PeriUartSend, PeriUartRecv, PeriUartIoctl},		/* USART2 */
#endif
#if (defined(CONFIG_PERIPHERAL_UART3) || defined(CONFIG_PERIPHERAL_UART_ALL))
  {2, 2, PeriUartInit, PeriUartUninit, PeriUartProbe,
   PeriUartSend, PeriUartRecv, PeriUartIoctl},		/* USART3 */
#endif

#if (defined(CONFIG_PERIPHERAL_I2C1) || defined(CONFIG_PERIPHERAL_I2C_ALL))
  {3, 0, PeriI2cInit, PeriI2cUninit, PeriI2cProbe,
   PeriI2cSend, PeriI2cRecv, PeriI2cIoctl},		/* I2C1 */
#endif
#if (defined(CONFIG_PERIPHERAL_I2C2) || defined(CONFIG_PERIPHERAL_I2C_ALL))
  {4, 1, PeriI2cInit, PeriI2cUninit, PeriI2cProbe,
   PeriI2cSend, PeriI2cRecv, PeriI2cIoctl},		/* I2C2 */
#endif
#if (defined(CONFIG_PERIPHERAL_I2C3) || defined(CONFIG_PERIPHERAL_I2C_ALL))
  {5, 2, PeriI2cInit, PeriI2cUninit, PeriI2cProbe,
   PeriI2cSend, PeriI2cRecv, PeriI2cIoctl},		/* I2C3 */
#endif

#if (defined(CONFIG_PERIPHERAL_SPI1) || defined(CONFIG_PERIPHERAL_SPI_ALL))
  {6, 0, PeriSpiInit, PeriSpiUninit, PeriSpiProbe,
   PeriSpiSend, PeriSpiRecv, PeriSpiIoctl},		/* SPI1 */
#endif
#if (defined(CONFIG_PERIPHERAL_SPI2) || defined(CONFIG_PERIPHERAL_SPI_ALL))
  {7, 1, PeriSpiInit, PeriSpiUninit, PeriSpiProbe,
   PeriSpiSend, PeriSpiRecv, PeriSpiIoctl},		/* SPI2 */
#endif
#if (defined(CONFIG_PERIPHERAL_SPI3) || defined(CONFIG_PERIPHERAL_SPI_ALL))
  {8, 2, PeriSpiInit, PeriSpiUninit, PeriSpiProbe,
   PeriSpiSend, PeriSpiRecv, PeriSpiIoctl},		/* SPI3 */
#endif
  
#if (defined(CONFIG_PERIPHERAL_USB1) || defined(CONFIG_PERIPHERAL_USB_ALL))
  {9, 0, PeriUsbInit, PeriUsbUninit, PeriUsbProbe,
   PeriUsbSend, PeriUsbRecv, PeriUsbIoctl},		/* USB */
#endif

  /*{PERI_LIST_TERMINATOR, 0, NULL, NULL, NULL, NULL, NULL, NULL},*/
};
#define	PERI_LIST_COUNT_MAX	((sizeof(periEntry))/(sizeof(struct _stPeriEntry)))


void
PeriInit(void)
{
  const struct _stPeriEntry 	*p;
  int			i;

  memset(&peri, 0, sizeof(peri));
  peri.selectedEntry = -1;

  p = periEntry;
  for(i = 0; i < PERI_LIST_COUNT_MAX; i++) {
    if(p->init) p->init(p->unit);
    p++;
  }

  return;
}


void
PeriUninit(void)
{
  const struct _stPeriEntry	*p;
  int			i;

  p = periEntry;

  for(i = 0; i < PERI_LIST_COUNT_MAX; i++) {
    if(p->uninit) p->uninit(p->unit);
    p++;
  }

  return;
}


void
PeriUninitExceptSelected(void)
{
  const struct _stPeriEntry	*p;
  int			i;

  p = periEntry;

  for(i = 0; i < PERI_LIST_COUNT_MAX; i++) {
    if(i != peri.selectedEntry) {
      if(p->uninit) p->uninit(p->unit);
    }
    p++;
  }

  return;
}


int
PeriSearchResponsedUnit(void)
{
  int			result = PERI_ERROR_NOTSELECTED;
  int			re;
  int			i;
  const struct _stPeriEntry	*p;

  p = periEntry;

  for(i = 0; i < PERI_LIST_COUNT_MAX; i++) {
    if(p->probe) {
      re = p->probe(p->unit);
      if(re == PERI_SUCCESS) {
	peri.selectedEntry = i;
	result = PERI_SUCCESS;
	break;
      }
    }
    p++;
  }

  return result;
}
int
PeriSelectActiveUnit(int sel)
{
  int		result = PERI_ERROR_NOTSELECTED;

  if(sel >= PERI_LIST_COUNT_MAX) {
    result = PERI_ERROR_EXCEEDED;
    goto fail;
  }

  peri.selectedEntry = sel;

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriSend(const uint8_t *ptr, int len)
{
  int		result = PERI_ERROR_UNKNOWN;
  const struct _stPeriEntry *p;

  if(peri.selectedEntry == -1) {
    result = PERI_ERROR_NOTSELECTED;
    goto fail;
  }

  p = &periEntry[peri.selectedEntry];
  if(p->send) result = p->send(p->unit, ptr, len);

fail:
  return result;
}


int
PeriRecv(uint8_t *ptr, int *len)
{
  int		result = PERI_ERROR_UNKNOWN;
  const struct _stPeriEntry *p;

  if(peri.selectedEntry == -1) {
    result = PERI_ERROR_NOTSELECTED;
    goto fail;
  }

  p = &periEntry[peri.selectedEntry];
  if(p->recv) result = p->recv(p->unit, ptr, len);
  
fail:
  return result;
}


int
PeriGetRecvSize(void)
{
  int		result = PERI_ERROR_UNKNOWN;
  int		size;
  const struct _stPeriEntry *p;

  if(peri.selectedEntry == -1) {
    result = PERI_ERROR_NOTSELECTED;
    goto fail;
  }

  p = &periEntry[peri.selectedEntry];
  if(p->ioctl) result = p->ioctl(p->unit, PERI_GET_RECV_SIZE, &size, sizeof(size));

  if(result < 0) size = -1;

fail:
  return size;
}


/*****************************************************************
 * weak functions
 */
#pragma weak PeriUartInit=PeriDevInit
#pragma weak PeriUartUninit=PeriDevUninit
#pragma weak PeriUartProbe=PeriDevProbe
#pragma weak PeriUartSend=PeriDevSend
#pragma weak PeriUartRecv=PeriDevRecv
#pragma weak PeriUartIoctl=PeriDevIoctl

int
PeriDevInit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int
PeriDevUninit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int
PeriDevProbe(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int
PeriDevSend(int unit, const uint8_t *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


int
PeriDevRecv(int unit, uint8_t *ptr, int *len)
{
  return PERI_ERROR_UNKNOWN;
}


int
PeriDevIoctl(int unit, int req, void *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


#if 0
int __ATTR_WEAK
PeriUartInit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUartUninit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUartProbe(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUartSend(int unit, const uint8_t *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


int  __ATTR_WEAK
PeriUartRecv(int unit, uint8_t *ptr, int *len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUartIoctl(int unit, int req, void *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}
int __ATTR_WEAK
PeriI2cInit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriI2cUninit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriI2cProbe(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriI2cSend(int unit, const uint8_t *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriI2cRecv(int unit, uint8_t *ptr, int *len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriI2cIoctl(int unit, int req, void *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriSpiInit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriSpiUninit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriSpiProbe(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriSpiSend(int unit, const uint8_t *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriSpiRecv(int unit, uint8_t *ptr, int *len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriSpiIoctl(int unit, int req, void *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUsbInit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUsbUninit(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUsbProbe(int unit)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUsbSend(int unit, const uint8_t *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUsbRecv(int unit, uint8_t *ptr, int *len)
{
  return PERI_ERROR_UNKNOWN;
}


int __ATTR_WEAK
PeriUsbIoctl(int unit, int req, void *ptr, int len)
{
  return PERI_ERROR_UNKNOWN;
}
#endif
