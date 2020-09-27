/*
 * Copyright (c) 2019 zhtlab
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

#define _FATFSDIO_C_

#include        <string.h>

#include        "system.h"

#include        "ff.h"
#include        "diskio.h"

#include        "config.h"
#include        "devErrno.h"

#include        "sdmmc.h"
#include        "fatfsdio.h"

#define FATFSDIO_DEBUG_API              0

static fatfsdioFunc_t funcTbl[CONFIG_FATFS_PHYMEDIA_COUNT_MAX];


int
FatfsdioRegsiterFunc(int unit, const fatfsdioFunc_t *ptr)
{
  int   result = 0;

  if(unit >= CONFIG_FATFS_PHYMEDIA_COUNT_MAX) goto fail;

  memcpy(&funcTbl[unit], ptr, sizeof(fatfsdioFunc_t));

fail:
  return result;
}


/********************************************************
 * entry from FATFS
 */
DSTATUS
disk_status(BYTE pdrv)
{
  DSTATUS stat = STA_NOINIT;
  int           re = DEV_ERRNO_UNKNOWN;

  if(pdrv >= CONFIG_FATFS_PHYMEDIA_COUNT_MAX) goto fail;
  if(!funcTbl[pdrv].ioctl) goto fail;

  // check medium ready
  re = funcTbl[pdrv].ioctl(pdrv, SDMMC_IOCTL_REQ_IS_MEDIUM_READY, NULL);
  if(re == DEV_ERRNO_NOTREADY) {
    stat = STA_NODISK;
    goto fail;
  }

  // check the media protect
  re = funcTbl[pdrv].ioctl(pdrv, SDMMC_IOCTL_REQ_IS_WRITABLE, NULL);
  if(re == DEV_ERRNO_PROTECTED) {
    stat = STA_PROTECT;
    goto fail;
  }

  stat = 0;

fail:
  return stat;
}


DSTATUS
disk_initialize(BYTE pdrv)
{
  DSTATUS       stat = STA_NOINIT;
  int           re = SDMMC_ERRNO_UNKNOWN;

  if(pdrv >= CONFIG_FATFS_PHYMEDIA_COUNT_MAX) goto fail;

#if FATFSDIO_DEBUG_API
  printf("fatfsdio initialize %x %x\n", pdrv, funcTbl[pdrv].init);
#endif

  if(funcTbl[pdrv].init) {
    re = funcTbl[pdrv].init(pdrv);
    if(re == SDMMC_SUCCESS) stat = 0;
  }

fail:
  return stat;
}


DRESULT
disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT result = RES_NOTRDY;
  int           re;

  if(funcTbl[pdrv].read) {
    re = funcTbl[pdrv].read(pdrv, sector, count, buff);
    if(re == DEV_ERRNO_SUCCESS) result = RES_OK;
  }

#if FATFSDIO_DEBUG_API
  printf("fatfsdio read %x %x %x  re:%d\n", pdrv, sector, count, re);
#endif

  return result;
}


#if FF_FS_READONLY == 0
DRESULT
disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT result = RES_NOTRDY;
  int           re;

#if FATFSDIO_DEBUG_API
  printf("fatfsdio write %x\n", pdrv);
#endif

  if(funcTbl[pdrv].write) {
    re = funcTbl[pdrv].write(pdrv, sector, count, buff);
    if(re == DEV_ERRNO_SUCCESS) result = RES_OK;
  }

  return result;
}
#endif


DRESULT
disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
  DRESULT result = RES_NOTRDY;
  int           re;

#if FATFSDIO_DEBUG_API
  printf("fatfsdio ioctl unit:%x req:cmd\n", pdrv, cmd);
#endif

  if(funcTbl[pdrv].ioctl) {
    re = funcTbl[pdrv].ioctl(pdrv, cmd, buff);
    if(re == DEV_ERRNO_SUCCESS) result = RES_OK;
  }

  return result;
}
