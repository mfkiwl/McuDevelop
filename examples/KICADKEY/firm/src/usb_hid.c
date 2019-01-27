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

#define _USB_HID_C_

#include "system.h"
#include "fifo.h"

#include "usbdif.h"
#include "usbdhid.h"
#include "usb_hid.h"

extern int dUsbHid;

usbdhidCb_t usbHidCb = {
  UsbHidCbInit,
  UsbHidCbDeinit,
  UsbHidCbCtrl,
  UsbHidCbRecv,
  NULL,
};



static int
UsbHidCbInit(usbdifClassDef_t *prc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;
  usbHid_t              *pHid;
  int                   size;

  pHid = ((usbdhidCb_t *)prc->pUserData)->pHidParam;
  if(!pHid) {
    size = sizeof(usbHid_t) + 8;                /* adhoc 8 */
    pHid = UsbdifMalloc(size);
    if(pHid == NULL) {
      result = USBDIF_STATUS_MEMORY;
      goto fail;
    }
    ((usbdhidCb_t *)prc->pUserData)->pHidParam = pHid;

  }
  pHid->pRxBuf = (uint8_t *)(pHid + sizeof(usbHid_t));

  //UsbdHidReceiving(dUsbHid, pHid->pRxBuf, 8);           /* adhoc 8 */

  result = USBDIF_STATUS_SUCCESS;
fail:
  return result;
}
static int
UsbHidCbDeinit(usbdifClassDef_t *prc)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;
  usbHid_t              *pHid;

#if 0
  pHid = ((usbdhidCb_t *)prc->pUserData)->pHidParam;
  UsbdifFree(pHid);
  ((usbdhidCb_t *)prc->pUserData)->pHidParam = NULL;
#endif

  result = USBDIF_STATUS_SUCCESS;

  return result;
}

static int
UsbHidCbCtrl(usbdifClassDef_t *prc, uint8_t cmd, uint8_t *pbuf, int length)
{
  usbdifStatus_t        result = USBDIF_STATUS_SUCCESS;

  usbHid_t              *pHid;
  uint8_t               **pptr = (uint8_t **)pbuf;

  //pHid = ((usbdhidCb_t *)prc->pUserData)->pHidParam;

  if(cmd == USBDHID_CB_REQ_GET_REPORT0) {
    extern const uint8_t   usbDescHidReport[];
    *pptr = (uint8_t *)&usbDescHidReport[0];
  }

  return result;
}


static int
UsbHidCbRecv(usbdifClassDef_t *prc, uint8_t* ptr, int *len)
{
  usbdifStatus_t        result = USBDIF_STATUS_UNKNOWN;

  usbHid_t   *pHid;

#if 0
  if(!prc || !ptr || *len <= 0) goto fail;

  pHid = ((usbdhidCb_t *)prc->pUserData)->pHidParam;
  result = USBDIF_STATUS_SUCCESS;

fail:
  UsbdHidReceiving(dUsbHid, ptr, USB_HID_RXDATA_SIZE);
#endif

  return result;
}


int
UsbHidRecv(int drc, uint8_t *ptr, int len)
{
  usbdifClassDef_t      *prc;
  usbHid_t               *pHid;
  int                   size = 0;

  prc = UsbifGetClassData(drc);
#if 0
  pHid = ((usbdhidCb_t *)prc->pUserData)->pHidParam;
  if(!pHid) goto end;

#endif

end:
  return size;
}
