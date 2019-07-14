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

#define _USB_CDCRNDIS_C_

#include "system.h"
#include "fifo.h"

#include "usbdif.h"
#include "usbdcdc.h"
#include "usbdcdcrndis.h"
#include "usb_cdcrndis.h"

extern int dUsbCdcRndis;


#define USB_CDCRNDIS_RXDATA_SIZE  2048

usbdcdcRndisCb_t usbcdcRndisCb = {
  UsbCdcRndisCbInit,
  UsbCdcRndisCbDeinit,
  UsbCdcRndisCbCtrl,
  UsbCdcRndisCbRecv,
  NULL,
};

/*usbCdcRndis_t     rndisParam[2];*/

static int
UsbCdcRndisCbInit(usbdifClassDef_t *prc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;
  usbCdcRndis_t         *pRndis;
  int                   size;

  /*pRndis = &rndisParam[prc->numClass];*/
  size = sizeof(usbCdcRndis_t) + USB_CDCRNDIS_RXDATA_SIZE;
  if((pRndis = UsbdifMalloc(size)) == NULL) {
    result = USBDIF_STATUS_MEMORY;
    goto fail;
  }
  pRndis->pRxBuf = (uint8_t *)(pRndis + sizeof(usbCdcRndis_t));

  ((usbdcdcRndisCb_t *)prc->pUserData)->pRndisParam = pRndis;


  UsbdCdcReceiving(dUsbCdcRndis, pRndis->pRxBuf, USB_CDCRNDIS_RXDATA_SIZE);

  result = USBDIF_STATUS_SUCCESS;
fail:
  return result;
}
static int
UsbCdcRndisCbDeinit(usbdifClassDef_t *prc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;
  usbCdcRndis_t         *pRndis;

  pRndis = ((usbdcdcRndisCb_t *)prc->pUserData)->pRndisParam;
  UsbdifFree(pRndis);
  ((usbdcdcRndisCb_t *)prc->pUserData)->pRndisParam = NULL;

  result = USBDIF_STATUS_SUCCESS;

  return result;
}

static int
UsbCdcRndisCbCtrl(usbdifClassDef_t *prc, uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  usbdifStatus_t        result = USBDIF_STATUS_SUCCESS;

  void          *pRndisParam;
  usbCdcRndis_t *pRndis;

  pRndis = ((usbdcdcRndisCb_t *)prc->pUserData)->pRndisParam;


  return result;
}


static int
UsbCdcRndisCbRecv(usbdifClassDef_t *prc, uint8_t* ptr, int len)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;

  usbCdcRndis_t   *pRndis;

  if(!prc || !ptr || len <= 0) goto fail;

  pRndis = ((usbdcdcRndisCb_t *)prc->pUserData)->pRndisParam;


  result = USBDIF_STATUS_SUCCESS;

fail:
  UsbdCdcReceiving(dUsbCdcRndis, ptr, USB_CDCRNDIS_RXDATA_SIZE);

  return result;
}


char               *ptrTxRndis = NULL;
int                sizeTxRndis = 0;
int
UsbCdcRndisLoop(int drc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;

  usbdifClassDef_t      *prc;
  usbCdcRndis_t         *pRndis;

  prc = UsbifGetClassData(drc);
  pRndis = ((usbdcdcRndisCb_t *)prc->pUserData)->pRndisParam;


  result = USBDIF_STATUS_SUCCESS;

  return result;
}


int
UsbCdcRndisSend(int drc, const uint8_t *ptr, int len)
{
  usbdifClassDef_t      *prc;
  usbCdcRndis_t           *pRndis;
  int                   size;

  prc = UsbifGetClassData(drc);
  pRndis = ((usbdcdcRndisCb_t *)prc->pUserData)->pRndisParam;


  return size;
}
