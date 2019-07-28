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

#ifndef _USB_DESC_H_
#define _USB_DESC_H_


/*** enalbe / disable */
#define USBDESC_ENABLE_CDCACM   1
#define USBDESC_ENABLE_AUDIO    0
#define USBDESC_ENABLE_MSC      0
#define USBDESC_ENABLE_CDCRNDIS 0
#define USBDESC_ENABLE_CDCECM   0

/*** interface number */
#define USBDESC_IF_CDC_CTRL     0
#define USBDESC_IF_CDC_DATA     1
#define USBDESC_IF_MAX          2


#define USBDESC_EP_NUM_MASK     0x0f
#define USBDESC_EP_DIR_MASK     0x80

#define USBDESC_EP_CDC_OUT      0x01
#define USBDESC_EP_CDC_IN       0x81
#define USBDESC_EP_CDC_CTRL     0x82

#endif
