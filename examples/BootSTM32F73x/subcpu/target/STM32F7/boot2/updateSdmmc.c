/*
 * Copyright (c) 2020 zhtlab
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

#define _UPDATE_SDMMC_C_


#include        <stdint.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>

#include        "stm32f7.h"
#include        "stm32Sdmmc.h"
#include        "devSdmmc.h"

#include        "config.h"
#include        "system.h"

#include        "main.h"
#include        "sdmmc.h"
#include        "devGpio.h"
#include        "gpio.h"


#include        "fatfsdio.h"
#include        "ff.h"
#include        "updateSdmmc.h"


#define UPDATE_PROGRAM_DOUBLE_WORD_EN   CONFIG_UPDATE_PROGRAM_DOUBLE_WORD_EN
#define UPDATE_PROGRAM_SINGLE_WORD_EN   CONFIG_UPDATE_PROGRAM_SINGLE_WORD_EN
#define UPDATE_PROGRAM_HALF_WORD_EN     CONFIG_UPDATE_PROGRAM_HALF_WORD_EN
#define UPDATE_PROGRAM_BYTE_EN          CONFIG_UPDATE_PROGRAM_BYTE_EN
#define UPDATE_PROGRAM_BURST_EN         CONFIG_UPDATE_PROGRAM_BURST_EN


#define ChAscToLong(a,b,c,d)    ( \
  (((a) & 0xff) << 24) |          \
  (((b) & 0xff) << 16) |          \
  (((c) & 0xff) <<  8) |          \
  (((d) & 0xff) <<  0) )


static const fatfsdioFunc_t updateSdmmcFunc = {
  .init  = SdmmcInit,
  .read  = SdmmcReadBlock,
  .write = SdmmcWriteBlock,
  .ioctl = SdmmcIoctl,
};


void
UpdateSdmmcInit(void)
{
  int           result = -1;

  // init, if the card is inserted
  if(GpioIsSdmmc1CardInserted()) {
    // the sdmmc entry is registered to fatfs diskio
    UpdateSdmmcProgram();
  }

  return;
}


int
UpdateSdmmcProgram(void)
{
  int           result = -1;
  FATFS         fatfs;
  FIL           fp;
  FRESULT       re;
  firmInfo_t    mcuInfo;

  UpdateSdmmcGetMcuInfo(&mcuInfo);              // get the MCU version, product, etc

  UpdateSdmmcInitDevice();

  // the sdmmc entry is registered to fatfs diskio
  FatfsdioRegsiterFunc(0, &updateSdmmcFunc);

  // read init.txt file
  re = f_mount(&fatfs, "", 0);               // 0: delay mount, 1: mount now


  // program the bootloader
  UpdateSdmmcReadAndBurn("bootloader", mcuInfo.product, mcuInfo.verBoot, CONFIG_BOOTLOADER_TOP);
  UpdateSdmmcReadAndBurn("firmware", mcuInfo.product, mcuInfo.verFirm, CONFIG_FIRMWARE_TOP);

  return result;
}


static int
UpdateSdmmcInitDevice(void)
{
  devSdmmcParam_t       param;

  DevSdmmcInit(SDMMC_NUM_INIT, NULL);

  memset(&param, 0, sizeof(param));

  param.hwFlow = 1;
  param.clkPolNeg = 0;
  param.clkPowerEn = 1;
  param.busWidth = 1;
  param.dma = 0;
  param.clk = 400000;
  param.cb = (void *)SdmmcCallback;

#if 1
  param.intr = 1;
  param.nonblock = 0;
  NVIC_SetPriority(SDMMC1_IRQn, 0);
  NVIC_EnableIRQ(SDMMC1_IRQn);
#endif

  DevSdmmcInit(SDMMC1_NUM, &param);

  return 0;
}


static int
UpdateSdmmcSearchLatestFile(uint8_t *fname, int len, uint8_t *name)
{
  int           result = -1;
  FRESULT       re;
  DIR           dir;
  FILINFO       finfo;
  char          strLatest[LEN_FNAME];
  char *        p;
  int           lenName;

  strLatest[0] = '\0';
  lenName = strlen(name);

  re = f_opendir(&dir, "0:");
  if(re != FR_OK) goto fail;

  while(1) {
    re = f_readdir(&dir, &finfo);

    // exit, if end of list
    if(re != FR_OK || finfo.fname[0] == '\0') break;
    p = finfo.fname;

    //           11111111112222222222
    // 012345678901234567890123456789
    // firmware.x.xx.xx.yyyymmdds.bin
    if(!(finfo.fattrib & AM_DIR) &&
       strcmp(p, name) > 0 &&
       strcmp(&p[lenName+18], ".bin") == 0 &&
       strlen(p) == (lenName+22) &&
       p[lenName] == '.' && p[lenName+2] == '.' && p[lenName+5] == '.' && p[lenName+8] == '.' ) {

      if(strcmp(finfo.fname, &strLatest[0]) > 0) {
        strncpy(strLatest, finfo.fname, sizeof(strLatest));
      }
    }
  }

  if(strLatest[0] != '\0') {
    strncpy(fname, strLatest, len);
    result = 0;
  }

fail:
  return result;
}


extern const char     strProductText[];
extern const char     strVersionText[];
static int
UpdateSdmmcGetMcuInfo(firmInfo_t *p)   // bootloader, firmware version
{
  int           result = -1;

  char          **pBoot = (char **) CONFIG_BOOTLOADER_INFO_VERSION_POS;
  char          **pFirm = (char **) CONFIG_FIRMWARE_INFO_VERSION_POS;


  if(p) {
    strncpy(p->product, strProductText, sizeof(p->product));
    //strncpy(p->verBoot, strVersionText, sizeof(p->verBoot));
    strncpy(p->verBoot, *pBoot, sizeof(p->verBoot));
    strncpy(p->verFirm, *pFirm, sizeof(p->verFirm));
    p->addr = CONFIG_FIRMWARE_TOP;

    result = 0;
  }

  return result;
}


#define UPDATESDMMC_DELIMIT_ENT         4
int
UpdateSdmmcGetFileInfo(firmInfo_t *p, FIL *fp)                // product, version, address
{
  int           result = -1;
  char          *re;
  char          str[64];
  int           ac;
  char          *av[UPDATESDMMC_DELIMIT_ENT];
  int           size;

  if(p) {
    while(1) {
      re = f_gets(str, sizeof(str), fp);
      if(re == NULL || f_error(fp)) goto fail;

      size = strlen(str);
      size = UpdateSdmmcChomp(str, size);

      if(size > 0) {
        ac = UpdateSdmmcDelimit((char **)av, UPDATESDMMC_DELIMIT_ENT, str, size);

        // parse
        if(strcmp(av[0], "Product:") == 0) {
          strncpy(p->productFile, av[1], sizeof(p->productFile));

        } else if(strcmp(av[0], "Version:") == 0) {
          strncpy(p->verFile, av[1], sizeof(p->verFile));

        } else if(strcmp(av[0], "FlashStart:") == 0) {
          p->addr = strtoul(av[1], NULL, 16);

        }

      }

      if(f_eof(fp)) break;
    }

    result = 0;
  }

fail:
  return result;
}


static int
UpdateSdmmcChomp(char *str, int size)
{
  int           i;

  /* delete '\r' and '\n' */
  for(int i = size-1; i >= 0; i--) {
    if(str[i] == '\r' || str[i] == '\n') {
      str[i] = '\0';
      size--;
      continue;
    } else {
      break;
    }
  }

  return size;
}


static int
UpdateSdmmcDelimit(char *av[], int ac, char *p, int len)
{
  char          *tp;
  int           cnt = 0;

  tp = strtok(p, " \t");
  while(tp != NULL) {
    av[cnt] = tp;
    tp = strtok(NULL, " \t");
    cnt++;
    if(cnt >= ac) break;
  }

  return cnt;
}


/*
 * file: file name ("firmware" or "bootloader")
 * strVersion: version text ("x.xx.xx.yyyymmdds")
 * addr: address of entry
 */
static int
UpdateSdmmcReadAndBurn(char *strFile, char *strProduct, char *strVersion, uint32_t addr)
{
  int           result = -1;
  FIL           fp;
  FRESULT       re;
  uint8_t       fname[LEN_FNAME];
  char          buf[256];
  int           size;
  firmInfo_t    fileInfo;

  if(UpdateSdmmcSearchLatestFile(&fname[2], sizeof(fname)-2, strFile) < 0) goto fail;
  fname[0] = '0';
  fname[1] = ':';

  re = f_open(&fp, fname, FA_READ);
  if(re != FR_OK) goto fail;

  // get the file information
  UpdateSdmmcGetFileInfo(&fileInfo, &fp);
  // close
  f_close(&fp);

  // check the file info and the MCU info
  //   Is the product name same
  //   the file version is newer than the MCU version
  if(strcmp(fileInfo.productFile, strProduct) != 0) goto fail;
  if(strcmp(fileInfo.verFile, strVersion) <= 0) goto fail;


  MainSetUpdateLedSpeed(1);

  // re-open
  re = f_open(&fp, fname, FA_READ);
  if(re != FR_OK) goto fail;

  // skip header
  while(1) {
    re = f_read(&fp, (void *)buf, 1, &size);
    if(f_error(&fp)) goto fail_close;

    if(buf[0] == '\0') break;

    if(f_eof(&fp)) goto fail_close;
  }

  // erase
  if(addr >= CONFIG_FIRMWARE_TOP) {
    for(int i = 2; i < 8; i++) {
      UpdateEraseSect(0, i);
    }
  } else {
    UpdateEraseSect(0, 0);
    UpdateEraseSect(0, 1);
  }

  // blank check  (skip)


  // burn and verify
  while(1) {
    re = f_read(&fp, (void *)buf, sizeof(buf), &size);
    if(f_error(&fp)) goto fail_close;

#define     UPDATESDMMC_RETRY_PROGRAM       10
    for(int i = 0; i < UPDATESDMMC_RETRY_PROGRAM; i++) {
      // burn
      UpdateProgram(0, addr, size, buf);

      // verify
      if(memcmp((char *)addr, buf, size) == 0) break;
      if(i == UPDATESDMMC_RETRY_PROGRAM) {
        __disable_irq();
        SystemGpioSetUpdateLedOn();
        while(1);
      }
    }

    addr += size;

    if(f_eof(&fp)) break;

    MainIdle();
  }

  result = 0;

fail_close:
  f_close(&fp);
  MainSetUpdateLedSpeed(0);

fail:

  return result;
}
