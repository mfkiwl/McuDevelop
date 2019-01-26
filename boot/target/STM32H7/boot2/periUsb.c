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

#define _PERIUSB_C_

#include        <stdint.h>
#include        <string.h>

#include        "stm32h743xx.h"

#include        "config.h"
#include        "system.h"
#include        "gpio.h"

#include        "usbd_core.h"
#include        "usbd_desc.h"
#include        "usbd_cdc.h"
#include        "usbd_cdc_if.h"

#include        "peri.h"
#include        "periUsb.h"

#define PERIUSB_UNIT_MAX                2

extern struct _stPeri   peri;

#if 0
static struct _stStm32_USB      *const pu[] = {
  USB1_PTR, USB2_PTR,
};
#endif
const static uint16_t   gpioInitUsb[][5] = {
  {PORT_CTRL(GPIO_USB_FS_VBUS, GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC10_USB),
   PORT_CTRL(GPIO_USB_FS_DM,   GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC10_USB),
   PORT_CTRL(GPIO_USB_FS_DP,   GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC10_USB),
   PORT_CTRL(GPIO_USB_FS_EN,   GPIO_MODE_OUTPUT, GPIO_PULL_NONE, 0),
   PORT_CTRL_END},
};
const static uint16_t   gpioUninitUsb[][5] = {
  {PORT_CTRL(GPIO_USB_FS_VBUS, GPIO_MODE_INPUT,  GPIO_PULL_NONE, 0),
   PORT_CTRL(GPIO_USB_FS_DM,   GPIO_MODE_INPUT,  GPIO_PULL_NONE, 0),
   PORT_CTRL(GPIO_USB_FS_DP,   GPIO_MODE_INPUT,  GPIO_PULL_NONE, 0),
   PORT_CTRL(GPIO_USB_FS_EN,   GPIO_MODE_INPUT,  GPIO_PULL_NONE, 0),
   PORT_CTRL_END},
};



void OTG_FS_IRQHandler(void);
USBD_HandleTypeDef hUsbDeviceFS;
int
PeriUsbInit(int unit)
{
  int                   result = PERI_ERROR_UNKNOWN;
  uint32_t              *pVectorTbl;

  if(unit >= PERIUSB_UNIT_MAX) goto fail;

  /* gpio function setting */
  SystemGpioSets(gpioInitUsb[unit]);

  /* usb setting */
  switch(unit) {
  case  0:
    pVectorTbl = (uint32_t*)SCB->VTOR;
    pVectorTbl[OTG_FS_IRQn+16] = (uint32_t) OTG_FS_IRQHandler;  /*** adhoc adhoc */
    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
    USBD_Start(&hUsbDeviceFS);
    PWR->CR3 |= PWR_CR3_USB33DEN;
    PeriUsbGpioSetFsEnableOn();
    break;
  case  1:
    break;
  }

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriUsbUninit(int unit)
{
  int           result = PERI_ERROR_UNKNOWN;

  if(unit >= PERIUSB_UNIT_MAX) goto fail;

  SystemGpioSets(gpioUninitUsb[unit]);

  switch(unit) {
  case  0:
    break;
  case  1:
    break;
  }

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriUsbProbe(int unit)
{
  int		result = PERI_ERROR_UNKNOWN;
  uint8_t	c;
  int		len;

  if(unit >= PERIUSB_UNIT_MAX) goto fail;

  len = sizeof(c);
  PeriUsbRecv(unit, &c, &len);
  if(len == 1 && c == PERIUSB_SYNC_CHAR) {
    result = PERI_SUCCESS;
  }

fail:
  return result;
}


int
PeriUsbSend(int unit, const uint8_t *ptr, int len)
{
  int		result = PERI_ERROR_UNKNOWN;
  int		i;

  if(unit >= PERIUSB_UNIT_MAX) goto fail;

  while(1) {
    if(CDC_Transmit_FS(NULL, 0) == USBD_OK) break;
  }
  if(CDC_Transmit_FS((char *)ptr, len) == USBD_OK) {
    result = PERI_SUCCESS;
  }

fail:
  return result;
}


int
PeriUsbRecv(int unit, uint8_t *ptr, int *len)
{
  int		result = PERI_ERROR_UNKNOWN;

  int		sz;

  if(unit >= PERIUSB_UNIT_MAX) goto fail;
  if(!ptr || !len || *len <= 0) goto fail;

  sz = UsbdCdcIfRead(0, ptr, *len);
  if(sz != *len) {
    *len = 0;
  }
  
  result = PERI_SUCCESS;
fail:

  return result;
}


int
PeriUsbIoctl(int unit, int req, void *ptr, int len)
{
  int		result = PERI_ERROR_UNKNOWN;

  if(unit >= PERIUSB_UNIT_MAX) goto fail;
  if(!ptr || len <= 0) goto fail;

  switch(req) {
  case	PERI_GET_RECV_SIZE:
    *(uint32_t *) ptr = UsbdCdcIfGetReadBufCount(0);
    break;
  }
    
  result = PERI_SUCCESS;
fail:
  return result;
}

static void
PeriUsbGpioSetFsEnableOff(void)
{
  GPIO_PTR[CONFIG_USB_FS_EN_PORTNUM].bsrr = (1UL<<(CONFIG_USB_FS_EN_BITNUM+16));
  return;
}
static void
PeriUsbGpioSetFsEnableOn(void)
{
  GPIO_PTR[CONFIG_USB_FS_EN_PORTNUM].bsrr = (1UL<<(CONFIG_USB_FS_EN_BITNUM));
  return;
}
