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

#ifndef _USB_HID_H_
#define _USB_HID_H_


#include "usbdhid.h"


typedef struct {
  int           dFifoTx;
  int           dFifoRx;
  uint32_t      baud;
  uint8_t       stop;
  uint8_t       parity;
  uint8_t       bits;

  uint8_t       *pRxBuf;
} usbHid_t;



extern usbdhidCb_t              usbHidCb;
int             UsbHidSend(int drc, const uint8_t *ptr, int len);
int             UsbHidRecv(int drc, uint8_t *ptr, int len);
int             UsbHidLoop(int drc);

int             UsbHidGetReport(int drc, int num, uint8_t **pptr, uint32_t *plen);


#ifdef _USB_HID_C_
static int      UsbHidCbInit(usbdifClassDef_t *prc);
static int      UsbHidCbDeinit(usbdifClassDef_t *prc);
static int      UsbHidCbCtrl(usbdifClassDef_t *prc, uint8_t cmd, uint8_t* pbuf, int length);
static int      UsbHidCbRecv(usbdifClassDef_t *prc, uint8_t* ptr, int *len);
#endif

#endif
