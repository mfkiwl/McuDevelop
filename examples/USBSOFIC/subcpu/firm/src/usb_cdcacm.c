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

#define _USB_CDCACM_C_

#include "system.h"
#include "fifo.h"

#include "usbdif.h"
#include "usbdcdc.h"
#include "usb_cdcacm.h"

extern int dUsbCdc;


#define USB_CDCACM_RXDATA_SIZE  2048

usbdcdcCb_t usbcdcCb = {
  UsbCdcAcmCbInit,
  UsbCdcAcmCbDeinit,
  UsbCdcAcmCbCtrl,
  UsbCdcAcmCbRecv,
  NULL,
};

/*usbCdcAcm_t     acmParam[2];*/

static int
UsbCdcAcmCbInit(usbdifClassDef_t *prc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;
  usbCdcAcm_t           *pAcm;
  int                   size;

  /*pAcm = &acmParam[prc->numClass];*/
  size = sizeof(usbCdcAcm_t) + USB_CDCACM_RXDATA_SIZE;
  if((pAcm = UsbdifMalloc(size)) == NULL) {
    result = USBDIF_STATUS_MEMORY;
    goto fail;
  }
  pAcm->pRxBuf = (uint8_t *)(pAcm + sizeof(usbCdcAcm_t));

  ((usbdcdcCb_t *)prc->pUserData)->pCdcParam = pAcm;

  pAcm->dFifoTx = FifoCreate(8);
  pAcm->dFifoRx = FifoCreate(8);

  UsbdCdcReceiving(dUsbCdc, pAcm->pRxBuf, USB_CDCACM_RXDATA_SIZE);

  result = USBDIF_STATUS_SUCCESS;
fail:
  return result;
}
static int
UsbCdcAcmCbDeinit(usbdifClassDef_t *prc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;
  usbCdcAcm_t   *pAcm;

  pAcm = ((usbdcdcCb_t *)prc->pUserData)->pCdcParam;
  UsbdifFree(pAcm);
  ((usbdcdcCb_t *)prc->pUserData)->pCdcParam = NULL;

  result = USBDIF_STATUS_SUCCESS;

  return result;
}

static int
UsbCdcAcmCbCtrl(usbdifClassDef_t *prc, uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  usbdifStatus_t        result = USBDIF_STATUS_SUCCESS;

  void          *pCdcParam;
  usbCdcAcm_t   *pAcm;

  pAcm = ((usbdcdcCb_t *)prc->pUserData)->pCdcParam;

  /* USER CODE BEGIN 5 */
  switch (cmd) {
  case CDC_SEND_ENCAPSULATED_COMMAND:
    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:
    break;

  case CDC_SET_COMM_FEATURE:
    break;

  case CDC_GET_COMM_FEATURE:
    break;

  case CDC_CLEAR_COMM_FEATURE:
    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */ 
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
  case CDC_SET_LINE_CODING:
    pAcm->baud   =  pbuf[0]        &       0xff;
    pAcm->baud  |= (pbuf[1] <<  8) &     0xff00;
    pAcm->baud  |= (pbuf[2] << 16) &   0xff0000;
    pAcm->baud  |= (pbuf[3] << 24) & 0xff000000;
    pAcm->stop   =  pbuf[4];
    pAcm->parity =  pbuf[5];
    pAcm->bits   =  pbuf[6];
    break;

  case CDC_GET_LINE_CODING:
    break;

  case CDC_SET_CONTROL_LINE_STATE:      /* 0x22 */
    break;

  case CDC_SEND_BREAK:
    break;

  default:
    result = USBDIF_STATUS_UNKNOWN;
    break;
  }

  return result;
}


static int
UsbCdcAcmCbRecv(usbdifClassDef_t *prc, uint8_t* ptr, int len)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;

  usbCdcAcm_t   *pAcm;

  if(!prc || !ptr || len <= 0) goto fail;

  pAcm = ((usbdcdcCb_t *)prc->pUserData)->pCdcParam;
  FifoWriteIn(pAcm->dFifoRx, ptr, len);

  result = USBDIF_STATUS_SUCCESS;

fail:
  UsbdCdcReceiving(dUsbCdc, ptr, USB_CDCACM_RXDATA_SIZE);

  return result;
}


char               *ptrTx = NULL;
int                sizeTx = 0;
int
UsbCdcAcmLoop(int drc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;

  usbdifClassDef_t      *prc;
  usbCdcAcm_t           *pAcm;

  prc = UsbifGetClassData(drc);
  pAcm = ((usbdcdcCb_t *)prc->pUserData)->pCdcParam;
  if(!pAcm) goto fail;

  if(!UsbdCdcIsTxBusy(dUsbCdc)) {
    if(sizeTx > 0) {
      FifoAddReadPointer(pAcm->dFifoTx, sizeTx);
      sizeTx = 0;
    }
    if(FifoGetDirtyLen(pAcm->dFifoTx) > 0) {
      FifoGetReadPointer(pAcm->dFifoTx, &ptrTx, &sizeTx);
      if(sizeTx > 64) sizeTx = 64;
      UsbdCdcTransmit(dUsbCdc, ptrTx, sizeTx);
    }
  }

  result = USBDIF_STATUS_SUCCESS;

fail:
  return result;
}


int
UsbCdcAcmSend(int drc, const uint8_t *ptr, int len)
{
  usbdifClassDef_t      *prc;
  usbCdcAcm_t           *pAcm;
  int                   size;

  prc = UsbifGetClassData(drc);
  if(!prc->pUserData) goto end;
  pAcm = ((usbdcdcCb_t *)prc->pUserData)->pCdcParam;
  if(!pAcm) goto end;

  size = FifoWriteIn(pAcm->dFifoTx, (char *)ptr, len);
  UsbCdcAcmLoop(drc);

end:
  return size;
}


int
UsbCdcAcmRecv(int drc, uint8_t *ptr, int len)
{
  usbdifClassDef_t      *prc;
  usbCdcAcm_t           *pAcm;
  int                   size = -1;

  prc = UsbifGetClassData(drc);
  if(!prc->pUserData) goto end;
  pAcm = ((usbdcdcCb_t *)prc->pUserData)->pCdcParam;
  if(!pAcm) goto end;

  size = FifoReadOut(pAcm->dFifoRx, ptr, len);

end:
  return size;
}
