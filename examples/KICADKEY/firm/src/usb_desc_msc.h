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

#ifndef _USB_DESC_MSC_H_
#define _USB_DESC_MSC_H_

#define USB_DESCRIPTOR_MSC_FS_SIZE  (0x8 + 0x17)

#define USB_DESCRIPTOR_MSC(ifMsc, epOut, epIn) \
  /*  */                                                                      \
  /* ########################### MSC   0xbc (0x8 + 0x17)                   */ \
  /* IAD */                                                                   \
  0x08,                           /* bLength                               */ \
  USB_DESC_TYPE_INTERFACE_ASSOCIATION, /* bDescriptorType: IAD             */ \
  (ifMsc),                        /* bFirstInterface:     x                */ \
  0x01,                           /* bInterfaceCount:     1                */ \
  USB_CLASS_TYPE_MASS_STORAGE,    /* bFunctionClass:                       */ \
  USB_SUBCLASS_TYPE_MSC_SCSI,     /* bFunctionSubclass: Vendor(Ethernet)   */ \
  0x50,                           /* bFunctionProtocol:                    */ \
  0x00,                           /* iFunction:                            */ \
\
  /* --------------------------------------  MSC */ \
  /* Standard Audio Control Interface Descriptor */ \
  0x09,                           /* bLength:                              */ \
  USB_DESC_TYPE_INTERFACE,        /* bDescriptorType:                      */ \
  (ifMsc),                        /* bInterfaceNumber:                     */ \
  0x00,                           /* bAlternateSetting:                    */ \
  0x02,                           /* bNumberEndpoints:  IN,OUT             */ \
  USB_CLASS_TYPE_MASS_STORAGE,    /* bFunctionClass:                       */ \
  USB_SUBCLASS_TYPE_MSC_SCSI,     /* bFunctionSubclass: Vendor(Ethernet)   */ \
  0x50,                           /* bFunctionProtocol:                    */ \
  0x00,                           /* iFunction:                            */ \
\
  /* Endpoint Descriptor(OUT packet) */ \
  0x07,                           /* bLength:                              */ \
  USB_DESC_TYPE_ENDPOINT,         /* bDescriptorType:                      */ \
  (epOut),                        /* bEndpointAddress:                     */ \
  USB_EP_TYPE_BULK,               /* bmAttributes:                         */ \
  0x40,0x00,                      /* wMaxPacketSize:  64 bytes             */ \
  0x00,                           /* bInterval:       n/a                  */ \
\
  /* Endpoint Descriptor(IN packet) */ \
  0x07,                           /* bLength:                              */ \
  USB_DESC_TYPE_ENDPOINT,         /* bDescriptorType:                      */ \
  (epIn),                         /* bEndpointAddress:                     */ \
  USB_EP_TYPE_BULK,               /* bmAttributes:                         */ \
  0x40,0x00,                      /* wMaxPacketSize:  64 bytes             */ \
  0x00,                           /* bInterval:       n/a                  */ \
  /* end of MSC descriptor definition */


#endif
