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

#ifndef _USB_CDCRNDIS_H_
#define _USB_CDCRNDIS_H_


#include "usbdcdcrndis.h"


typedef struct {
  uint8_t       *pRxBuf;
} usbCdcRndis_t;



extern usbdcdcRndisCb_t         usbcdcRndisCb;
int             UsbCdcRndisSend(int drc, const uint8_t *ptr, int len);
int             UsbCdcRndisRecv(int drc, uint8_t *ptr, int len);
int             UsbCdcRndisLoop(int drc);


#ifdef _USB_CDCRNDIS_C_
static int      UsbCdcRndisCbInit(usbdifClassDef_t *prc);
static int      UsbCdcRndisCbDeinit(usbdifClassDef_t *prc);
static int      UsbCdcRndisCbCtrl(usbdifClassDef_t *prc, uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int      UsbCdcRndisCbRecv(usbdifClassDef_t *prc, uint8_t* ptr, int len);
#endif

#endif
