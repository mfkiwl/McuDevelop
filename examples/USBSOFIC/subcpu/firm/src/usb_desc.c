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

#define _USB_DESC_C_

#include        "config.h"

#include        "usb_def.h"
#include        "usb_desc.h"
#include        "usbdif.h"

#if USBDESC_ENABLE_CDCACM
#include        "usb_desc_cdc.h"
#endif
#if USBDESC_ENABLE_CDCRNDIS || USBDESC_ENABLE_CDCECM
#include        "usb_desc_cdcrndis.h"
#endif
#if USBDESC_ENABLE_AUDIO
#include        "usb_def_audio20.h"
#include        "usb_desc_uac200.h"
#endif
#if USBDESC_ENABLE_MSC
#include        "usb_desc_msc.h"
#endif


#define USBD_VID                        0x0483
#define USBD_PID_FS                     0x5745
#define USBD_LANGID_STRING              0x0409
#define USBD_TARGET_RELEASE_NUM         0x0001  /* xx.yy */




#ifndef CONFIG_AUDIO_MIC_CH_COUNT
#define CONFIG_AUDIO_MIC_CH_COUNT       2
#endif
#define CONFIG_AUDIO_MIC_CH_ASSIGN      0x03, 0x00, 0x00, 0x00
#define CONFIG_AUDIO_SP_CH_COUNT        2
#define CONFIG_AUDIO_SP_FIFOFILL_COUNT  (96)

#define CONFIG_AUDIO_I2S_CALLBACK_EN    0
#define CONFIG_AUDIO_I2S_CREATE_DESTROY_EVEYOPENCLOSE   0
#define CONFIG_AUDIO_I2S_USE_PAUSE_RESUME               1
#define CONFIG_AUDIO_I2S_FS_EXTERNAL    0
#define CONFIG_AUDIO_I2S_FS_96K000      96000
#define CONFIG_AUDIO_I2S_FS_48K000      48000
#define CONFIG_AUDIO_I2S_FS_44K100      44100
#define CONFIG_AUDIO_I2S_FS_32K000      32000
#define CONFIG_AUDIO_I2S_FS_16K000      16000

#define CONFIG_AUDIO_VOLUME_CURRENT     (-0x4000)
#define CONFIG_AUDIO_VOLUME_MAX         (0)
#define CONFIG_AUDIO_VOLUME_MIN         (-0x8000)
#define CONFIG_AUDIO_VOLUME_RESOLUTION  (0x1)






#define  USB_LEN_DEV_DESC                               0x12
#define  USB_LEN_DEV_QUALIFIER_DESC                     0x0A
#define  USB_LEN_CFG_DESC                               0x09
#define  USB_LEN_IF_DESC                                0x09
#define  USB_LEN_EP_DESC                                0x07
#define  USB_LEN_OTG_DESC                               0x03
#define  USB_LEN_LANGID_STR_DESC                        0x04
#define  USB_LEN_OTHER_SPEED_DESC_SIZ                   0x09



#define AUDIO_SAMPLE_FREQ(frq)  (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))

#define AUDIO_PACKET_SZE(frq)   (uint8_t)(((frq * 2 * 2)/1000) & 0xFF), \
    (uint8_t)((((frq * 2 * 2)/1000) >> 8) & 0xFF)



uint8_t         usbDescStringManufacturer[]     = "STMicroelectronics";
uint8_t         usbDescStringProduct[]          = "RIG100";
uint8_t         usbDescStringSerialNumber[]     = "00000000001C";
uint8_t         usbDescStringConfigration[]     = "AUDIO Config.200";
uint8_t         usbDescStringInterface[]        = "AUDIO Interface.100";
uint8_t         usbDescStringMacAddress[]       = "aa1122334455";


/*************************************************************
 * USB device descriptor
 */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t usbDescDevice[] __ALIGN_END = {
  USB_LEN_DEV_DESC,                   /* bLength                       */
  USB_DESC_TYPE_DEVICE,               /* bDescriptorType               */
  0x10, 0x02,                         /* bcdUSB Spec release num(2.10) */
  0xef,                               /* bDeviceClass     Misc device  */
  0x02,                               /* bDeviceSubClass  Common class */
  0x01,                               /* bDeviceProtocol  IAD          */
  USB_EP0_MAXSIZE,                    /* bMaxPacketSize                */
  LOBYTE(USBD_VID),  HIBYTE(USBD_VID),        /* idVendor              */
  LOBYTE(USBD_PID_FS), HIBYTE(USBD_PID_FS),   /* idProduct             */
  LOBYTE(USBD_TARGET_RELEASE_NUM),    /* bcdDevice  x.yy               */
  HIBYTE(USBD_TARGET_RELEASE_NUM),
  USB_STRING_INDEX_MFC,               /* Index of manufacturer  string */
  USB_STRING_INDEX_PRODUCT,           /* Index of product string       */
  USB_STRING_INDEX_SERIAL,            /* Index of serial number string */
  CONFIG_USB_MAXNUM_CONFIGURATIONS    /* bNumConfigurations            */
};
int usbDescDeviceLen = sizeof(usbDescDevice);


/*************************************************************
 * USB binary object store descriptor
 */
#define USBDESC_BOS_SIZE           (5 + 7 + 10)
__ALIGN_BEGIN uint8_t usbDescBos[] __ALIGN_END = {
  0x05,                               /* bLength                       */
  USB_DESC_TYPE_BOS,                  /* bDescriptorType               */
  LOBYTE(USBDESC_BOS_SIZE),           /* wTotalLength                  */
  HIBYTE(USBDESC_BOS_SIZE),
  0x02,                               /* wNumDEviceCaps                */

  /*  */
  0x07,                               /* bLength                       */
  USB_DESC_TYPE_DEVICE_CAPABILITY,    /* bDescriptorType               */
  USB_DEVCAP_TYPE_USB20_EXTENSION,    /* bDevCapabilityType            */
  0x06, 0x00, 0x00, 0x00,             /* bmAttributes                  */
                                      /*   [0] reserved                */
                                      /*   [1] LPM                     */
                                      /*   [2] xxx                     */

  /* SuperSpeed device capability desc */
  0x0a,                               /* bLength                       */
  USB_DESC_TYPE_DEVICE_CAPABILITY,    /* bDescriptorType               */
  USB_DEVCAP_TYPE_SUPERSPEED_USB,     /* bDevCapabilityType            */
  0x00,                               /* bmAttributes                  */
  0x06, 0x00,                         /* wSpeedsSupport                */
                                      /* b0:LS, b1:FS, b2:HS, b3:SS gen1   */
  0x02,                               /* bFunctionalitySupport         */
  0x0a,                               /* wU1DevExitLat   10us (unit 1us) */
  0xff, 0x01                          /* wU2DevExitLat  511us (unit 1us) */
};
int     usbDescBosLen = sizeof(usbDescBos);


/*************************************************************
 * USB configuration descriptor
 */
#if !USBDESC_ENABLE_CDCACM
#undef  USB_DESCRIPTOR_CDC_FS_SIZE
#define USB_DESCRIPTOR_CDC_FS_SIZE      0
#endif
#if !USBDESC_ENABLE_CDCRNDIS
#undef  USB_DESCRIPTOR_CDCRNDIS_FS_SIZE
#define USB_DESCRIPTOR_CDCRNDIS_FS_SIZE 0
#endif
#if !USBDESC_ENABLE_CDCECM
#undef  USB_DESCRIPTOR_CDCECM_FS_SIZE
#define USB_DESCRIPTOR_CDCECM_FS_SIZE   0
#endif
#if !USBDESC_ENABLE_MSC
#undef  USB_DESCRIPTOR_MSC_FS_SIZE
#define USB_DESCRIPTOR_MSC_FS_SIZE      0
#endif
#if !USBDESC_ENABLE_AUDIO
#undef  USB_DESCRIPTOR_UAC_200_FS_SIZE
#define USB_DESCRIPTOR_UAC_200_FS_SIZE  0
#endif
#define USBDESC_CONFIG_SIZE     (9 \
                                 + (USB_DESCRIPTOR_CDC_FS_SIZE)  \
                                 +(USB_DESCRIPTOR_UAC_200_FS_SIZE) \
                                 +(USB_DESCRIPTOR_MSC_FS_SIZE) \
                                 +(USB_DESCRIPTOR_CDCRNDIS_FS_SIZE) \
                                 +(USB_DESCRIPTOR_CDCECM_FS_SIZE) \
                                 )


/* USB AUDIO device Configuration Descriptor */
__ALIGN_BEGIN uint8_t usbDescConfig[USBDESC_CONFIG_SIZE] __ALIGN_END = {
  /* Configuration 1 */
  0x09,                                 /* bLength                     */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType             */
  LOBYTE(USBDESC_CONFIG_SIZE),          /* wTotalLength                */
  HIBYTE(USBDESC_CONFIG_SIZE),
  USBDESC_IF_MAX,                       /* bNumInterfaces              */
  0x01,                                 /* bConfigurationValue         */
  0x00,                                 /* iConfiguration              */
  0xc0,                                 /* bmAttributes  BUS Powred    */
  0x32,                                 /* bMaxPower = 100 mA          */
  /* 09 byte*/


#if USBDESC_ENABLE_CDCACM
  USB_DESC_CDC_FS(USBDESC_IF_CDC_CTRL,
                  USBDESC_IF_CDC_DATA,
                  USBDESC_EP_CDC_CTRL,
                  USBDESC_EP_CDC_OUT,
                  USBDESC_EP_CDC_IN)
#endif


#if USBDESC_ENABLE_AUDIO
  USB_DESCRIPTOR_UAC_200(USBDESC_IF_AUDIO_CTRL,
                         USBDESC_IF_AUDIO_OUT,
                         USBDESC_IF_AUDIO_IN,
                         USBDESC_EP_AUDIO_OUT,
                         USBDESC_EP_AUDIO_FB,
                         USBDESC_EP_AUDIO_IN,
                         UNDEF_UAC_200_SS_EP_COMPANION,
                         UNDEF_UAC_200_SS_EP_COMPANION)
#endif


#if USBDESC_ENABLE_MSC
  USB_DESCRIPTOR_MSC(USBDESC_IF_MSC,
                     USBDESC_EP_MSC_OUT,
                     USBDESC_EP_MSC_IN)
#endif


#if USBDESC_ENABLE_CDCECM
#define USB_STRING_INDEX_MACADDRESS     6       /* adhoc  also defiled in usbdstm.c */
  USB_DESC_CDCECM_FS(USBDESC_IF_CDCRNDIS_CTRL,
                       USBDESC_IF_CDCRNDIS_DATA,
                       USBDESC_EP_CDCRNDIS_CTRL,
                       USBDESC_EP_CDCRNDIS_OUT,
                       USBDESC_EP_CDCRNDIS_IN)
#endif


#if USBDESC_ENABLE_CDCRNDIS
  USB_DESC_CDCRNDIS_FS(USBDESC_IF_CDCRNDIS_CTRL,
                       USBDESC_IF_CDCRNDIS_DATA,
                       USBDESC_EP_CDCRNDIS_CTRL,
                       USBDESC_EP_CDCRNDIS_OUT,
                       USBDESC_EP_CDCRNDIS_IN)
#endif


};
int     usbDescConfigLen = sizeof(usbDescConfig);


#if defined (__ICCARM__)        /*!< IAR Compiler */
  #pragma data_alignment=4
#endif

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t usbDescLangID[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
  USB_LEN_LANGID_STR_DESC,      /* bLength                       */
  USB_DESC_TYPE_STRING,         /* bDescriptorType               */
  LOBYTE(USBD_LANGID_STRING),   /* wLANGID                       */
  HIBYTE(USBD_LANGID_STRING),
};


/*************************************************************
 * USB device qualifier descriptor
 */
/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t usbDescDeviceQualifier[] __ALIGN_END = {
  USB_LEN_DEV_QUALIFIER_DESC,         /* bLength                       */
  USB_DESC_TYPE_DEVICE_QUALIFIER,     /* bDescriptorType               */
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};
int     usbDescDeviceQualifierLen = sizeof(usbDescDeviceQualifier);





usbdifStringDesc_t usbDescString = {            /* adhoc */
  .langId        = usbDescLangID,
  .manufacturer  = usbDescStringManufacturer,
  .product       = usbDescStringProduct,
  .serialnumber  = usbDescStringSerialNumber,
  .configuration = usbDescStringConfigration,
  .interface     = usbDescStringInterface,
};


const usbdifDescritprTbl_t usbDescritptorTbl = {
  .device =  {usbDescDevice, sizeof(usbDescDevice)},
  .bos    =  {usbDescBos,    sizeof(usbDescBos)},
  .qualifier = {usbDescDeviceQualifier, sizeof(usbDescDeviceQualifier)},
  .config = {{usbDescConfig, sizeof(usbDescConfig)},
             {usbDescConfig, sizeof(usbDescConfig)},
             {usbDescConfig, sizeof(usbDescConfig)},
             {usbDescConfig, sizeof(usbDescConfig)},
             {usbDescConfig, sizeof(usbDescConfig)},
             {usbDescConfig, sizeof(usbDescConfig)},
             {usbDescConfig, sizeof(usbDescConfig)},
             {usbDescConfig, sizeof(usbDescConfig)} },
  .langId  = {usbDescLangID, sizeof(usbDescLangID)},
  .pString = {usbDescLangID,
              usbDescStringManufacturer,
              usbDescStringProduct,
              usbDescStringSerialNumber,
              usbDescStringConfigration,
              usbDescStringInterface},
};
