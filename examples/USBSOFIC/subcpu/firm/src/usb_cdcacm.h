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

#ifndef _USB_CDCACM_H_
#define _USB_CDCACM_H_


#include "usbdcdc.h"


typedef struct {
  int           dFifoTx;
  int           dFifoRx;
  uint32_t      baud;
  uint8_t       stop;
  uint8_t       parity;
  uint8_t       bits;

  uint8_t       *pRxBuf;
} usbCdcAcm_t;



extern usbdcdcCb_t              usbcdcCb;
int             UsbCdcAcmSend(int drc, const uint8_t *ptr, int len);
int             UsbCdcAcmRecv(int drc, uint8_t *ptr, int len);
int             UsbCdcAcmLoop(int drc);


#ifdef _USB_CDCACM_C_
static int      UsbCdcAcmCbInit(usbdifClassDef_t *prc);
static int      UsbCdcAcmCbDeinit(usbdifClassDef_t *prc);
static int      UsbCdcAcmCbCtrl(usbdifClassDef_t *prc, uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int      UsbCdcAcmCbRecv(usbdifClassDef_t *prc, uint8_t* ptr, int len);
#endif

#endif
