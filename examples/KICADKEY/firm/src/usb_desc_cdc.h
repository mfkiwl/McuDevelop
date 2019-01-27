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

#ifndef _USB_DESC_CDC_H_
#define _USB_DESC_CDC_H_

#define USB_DESCRIPTOR_CDC_FS_SIZE      (0x8 + 0x23 + 0x17)


#define USB_DESC_CDC_FS(ifCtrl, ifData, epCtrl, epOut, epIn) \
  /* ###################################### CDC   0x42 (0x8 + 0x23 + 0x17) */ \
  /* IAD */ \
  0x08,                           /* bLength:                              */ \
  USB_DESC_TYPE_INTERFACE_ASSOCIATION, /* bDescriptorType: IAD             */ \
  (ifCtrl),                       /* bFirstInterface:    x                 */ \
  0x02,                           /* bInterfaceCount:    2                 */ \
  USB_CLASS_TYPE_CDC,             /* bFunctionClass:                       */ \
  USB_SUBCLASS_TYPE_CDC_ACM,      /* bFunctionSubClass:                    */ \
  USB_PROTCOL_ID_CDC_V25TER,      /* bFunctionProtocol:                    */ \
  0x00,                           /* iFunction:                            */ \
\
  /* -------------------------------------- CDC control                    */ \
  /* Communication Interface descriptor */ \
  0x09,                           /* bLength:                              */ \
  USB_DESC_TYPE_INTERFACE,        /* bDescriptorType:                      */ \
  (ifCtrl),                       /* bInterfaceNumber:                     */ \
  0x00,                           /* bAlternateSetting:                    */ \
  0x01,                           /* bNumberEndpoints:   IN                */ \
  USB_CLASS_TYPE_CDC,             /* bFunctionClass:                       */ \
  USB_SUBCLASS_TYPE_CDC_ACM,      /* bFunctionSubClass:                    */ \
  USB_PROTCOL_ID_CDC_V25TER,      /* bFunctionProtocol:                    */ \
  0x00,                           /* iFunction:                            */ \
\
  /* CDC Class-specific Descriptors */                                        \
  /* Header functional Descriptor */                                          \
  0x05,                           /* bLength:                              */ \
  USB_DESC_TYPE_CS_INTERFACE,     /* bDescriptorType:                      */ \
  USB_CS_CDC_HEADER,              /* bDescriptorSubType: Header Funcl Desc */ \
  0x10, 0x01,                     /* bcdCDC:  CDC Release Number 1.01      */ \
\
  /* Abstract Control Management Functional Descriptor */                     \
  0x04,                           /* bLength:                              */ \
  USB_DESC_TYPE_CS_INTERFACE,     /* bDescriptorType: CS_INTERFACE         */ \
  USB_CS_CDC_ABSTRACT_CONTROL,    /* bDescriptorSubType: Abstract Control     \
                                               Model Functional Descriptor */ \
  0x02,                           /* bmCapabilities: Set_Line_Coding,         \
                                                 Set_Control_Line_State,      \
                                                 Get_Line_Coding and          \
                                             the notification Serial_State */ \
\
  /* Union Functional Descriptor */ \
  0x05,                           /* bLength:                              */ \
  USB_DESC_TYPE_CS_INTERFACE,     /* bDescriptorType: CS_INTERFACE         */ \
  USB_CS_CDC_UNION,               /* bDescriptorSubType: Union Functional     \
                                                                  Descrior */ \
  (ifCtrl),                       /* bMasterInterface:                     */ \
  (ifData),                       /* bSlaveInterface:                      */ \
\
  /* Call Management Functional Descriptor */ \
  0x05,                           /* bLength:                              */ \
  USB_DESC_TYPE_CS_INTERFACE,     /* bDescriptorType:                      */ \
  USB_CS_CDC_CALL_MANGEMENT,      /* bDescriptorSubType:                   */ \
                                  /*        Functional Descriptor          */ \
  0x00,                           /* bmCapabilities: Device sends/receives */ \
                                  /*        call management information    */ \
                                  /*        only over the Communication    */ \
                                  /*        Class Interface.               */ \
  0x01,                           /* bDataInterface: num of inteterfaces   */ \
\
  /* Endpoint Descriptor(Interrupt) */ \
  0x07,                           /* bLength:                              */ \
  USB_DESC_TYPE_ENDPOINT,         /* bDescriptorType:                      */ \
  (epCtrl),                       /* bEndpointAddress:                     */ \
  USB_EP_TYPE_INTR,               /* bmAttributes:                         */ \
  0x08,0x00,                      /* wMaxPacketSize:  8 bytes              */ \
  0x10,                           /* bInterval:       16 ms                */ \
\
\
  /* -------------------------------------- CDC data                       */ \
  /* Data Interface Descriptor */                                             \
  0x09,                           /* bLength:                              */ \
  USB_DESC_TYPE_INTERFACE,        /* bDescriptorType:                      */ \
  (ifData),                       /* bInterfaceNumber:                     */ \
  0x00,                           /* bAlternateSetting:                    */ \
  0x02,                           /* bNumberEndpoints:  IN,OUT             */ \
  USB_CLASS_TYPE_CDC_DATA,        /* bFunctionClass:                       */ \
  0x00,                           /* bFunctionSubClass:                    */ \
  0x00,                           /* bFunctionProtocol:                    */ \
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

#endif
