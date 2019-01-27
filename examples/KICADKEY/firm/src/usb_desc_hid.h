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

#ifndef _USB_DESC_HID_H_
#define _USB_DESC_HID_H_

#define USB_DESCRIPTOR_HID_FS_SIZE      (0x8 + 0x19)


#define USB_DESC_HID(ifHid, epHid) \
  /* ###################################### HID   0x18 (0x8 + 0x9 + 0x9 + 0x7) */ \
  /* IAD */ \
  0x08,                           /* bLength:                              */ \
  USB_DESC_TYPE_INTERFACE_ASSOCIATION, /* bDescriptorType: IAD             */ \
  (ifHid),                        /* bFirstInterface:    x                 */ \
  0x01,                           /* bInterfaceCount:    1                 */ \
  USB_CLASS_TYPE_HID,             /* bFunctionClass:                       */ \
  USB_SUBCLASS_TYPE_HID_BOOT,     /* bFunctionSubClass:                    */ \
  USB_PROTCOL_ID_HID_KEYBOARD,    /* bFunctionProtocol:                    */ \
  0x00,                           /* iFunction:                            */ \
\
  /* -------------------------------------- HID control                    */ \
  /* Communication Interface descriptor */ \
  0x09,                           /* bLength:                              */ \
  USB_DESC_TYPE_INTERFACE,        /* bDescriptorType:                      */ \
  (ifHid),                        /* bInterfaceNumber:                     */ \
  0x00,                           /* bAlternateSetting:                    */ \
  0x01,                           /* bNumberEndpoints:   IN                */ \
  USB_CLASS_TYPE_HID,             /* bFunctionClass:                       */ \
  USB_SUBCLASS_TYPE_HID_BOOT,     /* bFunctionSubClass:                    */ \
  USB_PROTCOL_ID_HID_KEYBOARD,    /* bFunctionProtocol:                    */ \
  0x00,                           /* iFunction:                            */ \
\
/* HID device descriptor */                                             \
  0x09,                           /* bLength:                              */ \
  USB_DESC_TYPE_HID,              /* bDescriptorType:    HID               */ \
  0x01, 0x10,                     /* bcdHID:             1.10              */ \
  0,                              /* bContryCode:                          */ \
  1,                              /* bNumDescriptors:    1                 */ \
  USB_DESC_HID_REPORT,            /* bDescriptorType:    Report desc       */ \
  65, 0x00,                       /* wDescriptorLength:  65bytes           */ \
\
  /* Endpoint Descriptor(Interrupt) */ \
  0x07,                           /* bLength:                              */ \
  USB_DESC_TYPE_ENDPOINT,         /* bDescriptorType:                      */ \
  (epHid),                        /* bEndpointAddress:                     */ \
  USB_EP_TYPE_INTR,               /* bmAttributes:                         */ \
  0x08,0x00,                      /* wMaxPacketSize:  8 bytes              */ \
  0x18,                           /* bInterval:       24 ms                */


const uint8_t   usbDescHidReport[] = {        /* adhoc */
  0x05, 0x01, 0x09, 0x06,   0xa1, 0x01, 0x05, 0x07,
  0x19, 0xe0, 0x29, 0xe7,   0x15, 0x00, 0x25, 0x01,
  0x95, 0x08, 0x75, 0x01,   0x81, 0x02, 0x95, 0x01,
  0x75, 0x08, 0x81, 0x01,   0x05, 0x08, 0x19, 0x01,

  0x29, 0x03, 0x95, 0x03,   0x75, 0x01, 0x91, 0x02,
  0x95, 0x05, 0x75, 0x01,   0x91, 0x01, 0x05, 0x07,
  0x19, 0x00, 0x2a, 0xff,   0x00, 0x15, 0x00, 0x26,
  0xff, 0x00, 0x95, 0x06,   0x75, 0x08, 0x81, 0x00,

  0xc0
};

#endif
