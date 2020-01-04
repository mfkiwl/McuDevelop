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

#define _SDMMC_C_

#include        <stdio.h>
#include        <string.h>

#include        "config.h"
#include        "system.h"
#include        "devErrno.h"
#include        "intr.h"
#if CONFIG_SDMMC_MIDDLE_USE_RTOS
#include        "rtos.h"
#endif

#include        "sdmmc.h"
#include        "sdmmc_def.h"

#if CONFIG_SDMMC_MIDDLE_USE_RTOS || 1
#define SDMMC_SLEEP(x)          CONFIG_SDMMC_SLEEP(x)
#endif

#define SDMMC_DEBUG_API         0
#define SDMMC_DEBUG_CMD         0
#define SDMMC_DEBUG_TRANSFER    0
#define SDMMC_DEBUG_INFO        0
#define SDMMC_DEBUG_DETAIL      0
#define SDMMC_DEBUG_DUMP        0


static sdmmc_t  sdmmc;
#if CONFIG_SDMMC_MIDDLE_USE_RTOS
static rtosQueueId idQueue;
#endif

/********************************************************
 *
 *
 */
sdmmcResult
SdmmcInit(int unit)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  int                   re;
  sdmmcResult           reSdmmc;
  devSdmmcParam_t       param;

#if SDMMC_DEBUG_API
  printf("# SdmmcInit() unit:%d\n", unit);
#endif

  memset(&sdmmc, 0, sizeof(sdmmc));

  sdmmc.sc[0].maxclk = 50000000;

#if 0
  DevSdmmcInit(SDMMC_NUM_INIT, NULL);

  memset(&param, 0, sizeof(param));

  param.hwFlow = 1;
  param.clkPolNeg = 0;
  param.clkPowerEn = 1;
  param.busWidth = 1;
  param.clk = 400000;

  re = DevSdmmcInit(unit, &param);
  //if(re != DEV_ERRNO_SUCCESS) result = SDMMC_ERRNO_DEVICE_NOT_READY;

#endif

  result = SdmmcSubGetCapability(unit);
  if(result != SDMMC_SUCCESS) goto fail;

  result = SdmmcSubInitCard(unit);
  if(result != SDMMC_SUCCESS) goto fail;

#if CONFIG_SDMMC_MIDDLE_USE_RTOS
  idQueue = RtosQueueCreate(15, sizeof(uint32_t));
#endif

  result = SDMMC_SUCCESS;  // !!!!!!!!!!!!!!!!!! adhoc

fail:
  return result;
}


sdmmcResult
SdmmcReadBlock(int unit, uint32_t lba, int count, uint8_t *ptr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  sdmmcResult           re;
  uint32_t              val;
  uint32_t              status;

  sdmmcSc_t             *psc;
  devSdmmcIoctlTransferInfo_t   info;

  psc = &sdmmc.sc[unit];

#if SDMMC_DEBUG_API
  printf("# SdmmcReadBlock() unit:%d, lba:%x, cnt:%x\n", unit, lba, count);
#endif

  // get status
  SdmmcCmd13SendStatus(unit, psc->rca, &status);        // cmd 13

  // set the block size
  val = count<<SET_TRANSFER_INFO_BLKSIZE_512B;
  SdmmcCmd16SetBlockLength(unit, val);          // CMD16  the block size is set 512

  // set the recv count and block size to the module
  info.cntBlock = count;
  info.szBlock = SET_TRANSFER_INFO_BLKSIZE_512B;
  info.dir = DEV_SDMMC_IOCTL_TRANSTERINFO_DIR_CARD_TO_CTRL;
  info.ptrDma = ptr;
  DevSdmmcIoctl(unit, DEV_SDMMC_IOCTL_SET_TRANSFER_INFO, &info);

  // send the read command
  if(count == 1) {
    re = SdmmcCmd17ReadSingleBlock(unit, lba);
  } else {
    if(psc->supportCmd23) {
       result = SdmmcCmd23SetBlockCount(unit, count);
      if(result != SDMMC_SUCCESS) {
        psc->supportCmd23 = 0;
      }
    }
    re = SdmmcCmd18ReadMultiBlock(unit, lba);
  }

  // data recv
  result = DevSdmmcWaitRecvData(unit, (uint32_t *)ptr, 400000);
  if(result == DEV_ERRNO_WOULDBLOCK) {
#if CONFIG_SDMMC_MIDDLE_USE_RTOS
    RtosQueueRecv(idQueue, &val, 2);
#else
    psc->fRecvDone = 0;
    while(1) {
      INTR_DISABLE();
      if(psc->fRecvDone) {
        psc->fRecvDone = 0;
        INTR_ENABLE();
        break;
      }
      INTR_ENABLE();
    }
#endif

    result = DEV_ERRNO_SUCCESS;
  }

  // stop sending
  if(!psc->supportCmd23 && count > 1) {
    SdmmcCmd12StopTransmission(unit);
  }

#if SDMMC_DEBUG_DUMP
  SdmmcDump(unit, lba, count, ptr);
#endif

  return result;
}


sdmmcResult
SdmmcWriteBlock(int unit, uint32_t lba, int count, uint8_t *ptr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;

#if SDMMC_DEBUG_API
  printf("# SdmmcWriteBlock() unit:%d, lba:%x, cnt:%x\n", unit, lba, count);
#endif

#if SDMMC_DEBUG_DUMP
  SdmmcDump(unit, lba, count, ptr);
#endif

  return result;
}


sdmmcResult
SdmmcIoctl(int unit, int req, void *ptr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;

#if SDMMC_DEBUG_API
  printf("# SdmmcIoctl() unit:%d, req:%x, ptr:%x\n", unit, req, ptr);
#endif

  switch(req) {
  case SDMMC_IOCTL_REQ_IS_MEDIUM_READY:
    result = (1)? DEV_ERRNO_SUCCESS: DEV_ERRNO_NODISK;
    break;
  case SDMMC_IOCTL_REQ_IS_WRITABLE:
    result = (1)? DEV_ERRNO_SUCCESS: DEV_ERRNO_PROTECTED;
    break;
  }

  return result;
}


void
SdmmcCallback(int unit, int req, void *ptr)
{
#if CONFIG_SDMMC_MIDDLE_USE_RTOS
  uint32_t      val;

  val = 1;
  RtosQueueSendIsr(idQueue, &val, 12);
#else
  sdmmcSc_t             *psc;

  psc = &sdmmc.sc[unit];
  psc->fRecvDone = 1;
#endif

  return;
}


/********************************************************
 * sd command functions
 * access to the lower functions (device driver)
 */

// CMD0
sdmmcResult
SdmmcCmd0GoIdleState(int unit)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;

  SdmmcSubSendCommand(unit, SDMMC_CMD0_GO_IDLE_STATE, NULL);
  result = SdmmcSubWaitSendCmd(unit);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd0GoIdleState() unit:%d, result:%d\n",
         unit, result);
#endif

  return result;
}

// CMD2
sdmmcResult
SdmmcCmd2AllSendCID(int unit, uint32_t *pCid)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;

  SdmmcSubSendCommand(unit, SDMMC_CMD2_ALL_SEND_CID, NULL);
  result = SdmmcSubRecvCmdResp2(unit, pCid);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd2AllSendCID() unit:%d, result:%d\n",
         unit, result);
  printf("        cid %x %x %x %x\n",
         pCid[0], pCid[1], pCid[2], pCid[3]);
#endif

  return result;
}

// CMD3
sdmmcResult
SdmmcCmd3SendRelativeAddr(int unit, uint32_t *pRCA)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              rca;

  SdmmcSubSendCommand(unit, SDMMC_CMD3_SET_RELATIVE_ADDR, NULL);
  result = SdmmcSubRecvCmdResp6(unit, &rca);
  if(pRCA) *pRCA = rca;

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd3SendRelativeAddr() unit:%d, result:%d, rca:%x\n",
         unit, result, rca);
#endif

  return result;
}

// CMD7
sdmmcResult
SdmmcCmd7SelDeselCard(int unit, uint32_t addr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              val;

  result = SdmmcSubSendCommand(unit, SDMMC_CMD7_SEL_DESEL_CARD, &addr);
  SdmmcSubRecvCmdResp1(unit, &val);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd7SelDeselCard() unit:%d, addr:%x, result:%d\n",
         unit, addr, result);
#endif

  return result;
}

// CMD8
sdmmcResult
SdmmcCmd8SendIfCond(int unit, uint32_t arg)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;

  SdmmcSubSendCommand(unit, SDMMC_CMD8_SEND_IF_COND, &arg);
  result = SdmmcSubRecvCmdResp7(unit);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd8SendIfCond() unit:%d, arg:%x, result:%d\n",
         unit, arg, result);
#endif

  return result;
}

// CMD9
sdmmcResult
SdmmcCmd9SendCSD(int unit, uint32_t addr, uint32_t *pCsd)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              val;

  result = SdmmcSubSendCommand(unit, SDMMC_CMD9_SEND_CSD, &addr);
  SdmmcSubRecvCmdResp2(unit, pCsd);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd9SendCSD() unit:%d, addr:%x, result:%d\n",
         unit, addr, result);
  printf("        csd %x %x %x %x\n",
         pCsd[0], pCsd[1], pCsd[2], pCsd[3]);
#endif

  return result;
}

// CMD10
sdmmcResult
SdmmcCmdSendCID(int unit)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;

  SdmmcSubSendCommand(unit, SDMMC_CMD10_SEND_CID, NULL);
  result = SdmmcSubWaitSendCmd(unit);

  return result;
}

// CMD12
sdmmcResult
SdmmcCmd12StopTransmission(int unit)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD12_STOP_TRANSMISSION, NULL);
  result = SdmmcSubRecvCmdResp1(unit, &val);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd12StopTransmission() unit:%d, result:%d\n",
         unit, result);
#endif

  return result;
}

// CMD13
sdmmcResult
SdmmcCmd13SendStatus(int unit, uint32_t arg, uint32_t *status)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;

  SdmmcSubSendCommand(unit, SDMMC_CMD13_SEND_STATUS, &arg);
  result = SdmmcSubRecvCmdResp1(unit, status);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd13SendStatus() unit:%d, arg:%x, status:%x, result:%d\n",
         unit, arg, status, result);
#endif
  return result;
}

// CMD16
sdmmcResult
SdmmcCmd16SetBlockLength(int unit, uint32_t blocksize)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  int                   re;
  uint32_t              val;
  //  devSdmmcIoctlTransferInfo_t   info;

  SdmmcSubSendCommand(unit, SDMMC_CMD16_SET_BLOCKLEN, &blocksize);
  result = SdmmcSubRecvCmdResp1(unit, &val);
  if(result != SDMMC_SUCCESS) goto fail;

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd16SetBlockLength() unit:%d, blocksize:%x, result:%d\n",
         unit, blocksize, result);
#endif

fail:
  return result;
}

// CMD17
sdmmcResult
SdmmcCmd17ReadSingleBlock(int unit, uint32_t addr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD17_READ_SINGLE_BLOCK, &addr);
  result = SdmmcSubRecvCmdResp1(unit, &val);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd17ReadSingleBlock() unit:%d, addr:%x, result:%d\n",
         unit, addr, result);
#endif

  return result;
}

// CMD18
sdmmcResult
SdmmcCmd18ReadMultiBlock(int unit, uint32_t addr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD18_READ_MULTIPLE_BLOCK, &addr);
  result = SdmmcSubRecvCmdResp1(unit, &val);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd18ReadMultiBlock() unit:%d, addr:%x, result:%d\n",
         unit, addr, result);
#endif

  return result;
}

// CMD23
sdmmcResult
SdmmcCmd23SetBlockCount(int unit, uint32_t cnt)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD23_SET_BLOCK_COUNT, &cnt);
  result = SdmmcSubRecvCmdResp1(unit, &val);
  printf("# SdmmcCmd23SetBlockCount() xxxx  val:%x %d\n", val, result);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd23SetBlockCount() unit:%d, count:%d, result:%d\n",
         unit, cnt, result);
#endif

  return result;
}

// CMD24
sdmmcResult
SdmmcCmd24WriteSingleBlock(int unit, uint32_t addr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD24_WRITE_SINGLE_BLOCK, &addr);
  SdmmcSubWaitSendCmd(unit);
  result = SdmmcSubRecvCmdResp1(unit, &val);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd24WriteSingleBlock() unit:%d, addr:%d, result:%d\n",
         unit, addr, result);
#endif

  return result;
}

// CMD25
sdmmcResult
SdmmcCmd25WriteMultiBlock(int unit, uint32_t addr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD25_WRITE_MULTIPLE_BLOCK, &addr);
  SdmmcSubWaitSendCmd(unit);
  SdmmcSubRecvCmdResp1(unit, &val);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd25WriteMultiBlock() unit:%d, addr:%d, result:%d\n",
         unit, addr, result);
#endif

  return result;
}

// CMD32
sdmmcResult
SdmmcCmd32EraseStartAddr(int unit, uint32_t addr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD32_TAG_SECTOR_START, &addr);
  SdmmcSubWaitSendCmd(unit);
  SdmmcSubRecvCmdResp1(unit, &val);

  return result;
}
// CMD33
sdmmcResult
SdmmcCmd33EraseEndAddr(int unit, uint32_t addr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD33_TAG_SECTOR_END, &addr);
  SdmmcSubWaitSendCmd(unit);
  SdmmcSubRecvCmdResp1(unit, &val);

  return result;
}
// CMD35
sdmmcResult
SdmmcCmd35EraseExtStartAddr(int unit, uint32_t StartAdd)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD35_TAG_ERASE_GROUP_START, NULL);
  SdmmcSubWaitSendCmd(unit);
  SdmmcSubRecvCmdResp1(unit, &val);

  return result;
}
// CMD36
sdmmcResult
SdmmcCmd36EraseExtEndAddr(int unit, uint32_t EndAdd)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD36_TAG_ERASE_GROUP_END, NULL);
  SdmmcSubWaitSendCmd(unit);
  SdmmcSubRecvCmdResp1(unit, &val);

  return result;
}
// CMD38
sdmmcResult
SdmmcCmd38Erase(int unit)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD38_ERASE, NULL);
  SdmmcSubWaitSendCmd(unit);
  SdmmcSubRecvCmdResp1(unit, &val);

  return result;
}
#if 0
// CMD41
sdmmcResult
SdmmcCmdStatusRegister(int unit)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              r3;

  SdmmcSubSendCommand(unit, SDMMC_CMD41_STATUS_REGISTER, NULL);
  result = SdmmcSubRecvCmdResp3(unit, &r3);

  return result;
}
#endif


// CMD41
sdmmcResult
SdmmcCmdOperCond(int unit)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;

  SdmmcSubSendCommand(unit, SDMMC_CMD41_STATUS_REGISTER, NULL);
  result = SdmmcSubRecvCmdResp7(unit);

  return result;
}
// CMD42
sdmmcResult
SdmmcCmd42LockUnlock(int unit, uint32_t arg)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              val;
  uint32_t              buf[512/4];

  val = 0;
  SdmmcSubSendCommand(unit, SDMMC_CMD42_LOCK_UNLOCK, &val);
  result = SdmmcSubRecvCmdResp1(unit, &val);

  memset(buf, 0, sizeof(buf));
  DevSdmmcWriteData(unit, buf, sizeof(buf), 100000000);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcCmd42LockUnlock() unit:%d, arg:%x, result %d\n",
         unit, arg, result);
#endif
  return result;
}

// CMD55
sdmmcResult
SdmmcCmd55AppCommand(int unit, uint32_t arg)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD55_APP_CMD, &arg);
  result = SdmmcSubRecvCmdResp1(unit, &val);

  return result;
}


// ACMD6
sdmmcResult
SdmmcAppCmd6SetBusWidth(int unit, uint32_t arg)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  sdmmcSc_t             *psc;
  uint32_t              val;

  psc = &sdmmc.sc[unit];

  result = SdmmcCmd55AppCommand(unit, psc->rca);          // CMD55
  if(result < SDMMC_SUCCESS) goto fail;

  SdmmcSubSendCommand(unit, SDMMC_CMD6_SWTICH, &arg);
  result = SdmmcSubRecvCmdResp1(unit, &val);

  val = 1<<arg;
  DevSdmmcIoctl(unit, DEV_SDMMC_IOCTL_SET_BUSWIDTH, &val);

fail:
#if SDMMC_DEBUG_CMD
  printf("# SdmmcAppCmd6SetBusWidth() unit:%d, arg:%x\n",
         unit, arg);
#endif
  return result;
}
// ACMD41
sdmmcResult
SdmmcAppCmd41SdSendOpCond(int unit, uint32_t SdType, uint32_t *ocr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  sdmmcSc_t     *psc;

  psc = &sdmmc.sc[unit];

  result = SdmmcCmd55AppCommand(unit, psc->rca);  // CMD55
  if(result < SDMMC_SUCCESS) goto fail;

  SdmmcSubSendCommand(unit, SDMMC_ACMD41_APP_SEND_OP_COND, &SdType);
  result = SdmmcSubRecvCmdResp3(unit, ocr);

#if SDMMC_DEBUG_CMD
  printf("# SdmmcAppCmd41SdSendOpCond() unit:%d, SDtype:%x, cnt:%x\n",
         unit, SdType, ocr);
#endif

fail:
  return result;
}
// ACMD51
sdmmcResult
SdmmcAppCmd51SendSCR(int unit, uint32_t arg, uint32_t *pScr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  sdmmcSc_t     *psc;
  uint32_t      val;
  uint32_t      bufScr[2];

  psc = &sdmmc.sc[unit];

  result = SdmmcCmd55AppCommand(unit, psc->rca);  // CMD55
  if(result < SDMMC_SUCCESS) goto fail;

  SdmmcSubSendCommand(unit, SDMMC_ACMD51_SEND_SCR, NULL);
  result = SdmmcSubRecvCmdResp1(unit, &val);
  DevSdmmcReadData(unit, bufScr, 8, 1000000);
#if 1
  pScr[0] = __REV(bufScr[0]);
  pScr[1] = __REV(bufScr[1]);
#else
  pScr[0] = bufScr[0];
  pScr[1] = bufScr[1];
#endif

#if SDMMC_DEBUG_CMD
  printf("# SdmmcAppCmd51SendSCR() unit:%d, addr:%x\n",
         unit, arg);
  printf("        scr %x %x\n", pScr[0], pScr[1]);
#endif

fail:
  return result;
}


/********************************************************
 *
 */
static sdmmcResult
SdmmcSubGetCapability(int unit)
{
  sdmmcResult   result = SDMMC_ERRNO_UNKNOWN;
  sdmmcSc_t     *psc;
  uint32_t      cap = 0;
  uint32_t      ocr;

  psc = &sdmmc.sc[unit];

  result = SdmmcCmd0GoIdleState(unit);                  // CMD0
  if(result < SDMMC_SUCCESS) goto fail;

  result = SdmmcCmd8SendIfCond(unit, 0x1aa);            // CMD8
  if(result < SDMMC_SUCCESS) {
    psc->cardVersion = SDMMC_CARDVERSIONE_1_X;
    cap = SDMMC_ACMD41_CCS_SDSC;
  } else {
    psc->cardVersion = SDMMC_CARDVERSIONE_2_X;
    cap = SDMMC_ACMD41_CCS_SDXC;
  }

  cap |= SDMMC_ACMD41_VOLTAGE_WINDOW_SD;

  do {
    result = SdmmcAppCmd41SdSendOpCond(unit, cap, &ocr);      // ACMD41
    if(result == SDMMC_ERRNO_TOUT) goto fail;
    SDMMC_SLEEP(50);
  } while(!(ocr & SDMMC_ACMD41_READY_MASK));
#if SDMMC_DEBUG_DETAIL
  SdmmcDebugShowOcr(&ocr);
#endif

  /* voltage */
  psc->vccio = SDMMC_VCCIO_3300MV;
  if((ocr & SDMMC_ACMD41_S18_MASK) == SDMMC_ACMD41_S18R_1800MV) {
    psc->vccio = SDMMC_VCCIO_1800MV;
#if 0
    SdmmcCmd11VoltageSwitch();          // CMD11 changed to 1.8V
#endif
  }

  /* card type */
  psc->cardType = SDMMC_CARDTYPE_SDSC;
  if(psc->cardVersion == SDMMC_CARDVERSIONE_2_X &&
     (ocr & SDMMC_ACMD41_CCS_MASK) == SDMMC_ACMD41_CCS_SDXC) {
    psc->cardType = SDMMC_CARDTYPE_SDHC_SDXC;
  }


#if SDMMC_DEBUG_INFO
  /* debug */
  if(psc->cardVersion == SDMMC_CARDVERSIONE_1_X) {
    puts("# SD cap  1.x  SDSC\n");
  } else {
    if(psc->cardType == SDMMC_CARDTYPE_SDHC_SDXC) {
      puts("# SD cap  2.x  SDHC/SDXC\n");
    } else {
      puts("# SD cap  2.x  SDSC\n");
    }
  }
  printf("# SD voltage: %dmV\n", (psc->vccio == SDMMC_VCCIO_1800MV)? 1800: 3300);
#endif

  result == SDMMC_SUCCESS;

fail:
  return result;
}


static sdmmcResult
SdmmcSubInitCard(int unit)
{
  sdmmcResult   result = SDMMC_ERRNO_UNKNOWN;
  int           re;
  sdmmcSc_t     *psc;

  uint32_t      rca = 1;
  uint32_t      val;

  devSdmmcIoctlTransferInfo_t   info;

  psc = &sdmmc.sc[unit];

  // get the CID info
  result = SdmmcCmd2AllSendCID(unit, psc->bufCid);     // CMD2
#if SDMMC_DEBUG_DETAIL
  SdmmcSplitCid(&sdmmc.sc[unit].cid, psc->bufCid);
  SdmmcDebugShowCid(&sdmmc.sc[unit].cid, psc->bufCid);
#endif

  // get the relative card address
  SdmmcCmd3SendRelativeAddr(unit, &rca);                // CMD3
  // set the relative card address
  psc->rca = rca & 0xffff0000;

  SdmmcCmd9SendCSD(unit, psc->rca, psc->bufCsd);        // cmd9
#if SDMMC_DEBUG_DETAIL
  SdmmcSplitCsd2(&psc->csd2, psc->bufCsd);
  SdmmcDebugShowCsd2(&psc->csd2, psc->bufCsd);
#endif

  //psc->class = psc->csd[1] >> 20;
  SdmmcCmd7SelDeselCard(unit, psc->rca);                // cmd7

  // the block size is set 8
  val = 1<<SET_TRANSFER_INFO_BLKSIZE_8B;
  SdmmcCmd16SetBlockLength(unit, val);                  // CMD16  the block size is set 8

  // the block size, count, destination address are set to the sdmodule
  info.cntBlock = 1;
  info.szBlock = SET_TRANSFER_INFO_BLKSIZE_8B;
  info.dir = DEV_SDMMC_IOCTL_TRANSTERINFO_DIR_CARD_TO_CTRL;
  info.ptrDma = psc->bufScr;
  DevSdmmcIoctl(unit, DEV_SDMMC_IOCTL_SET_TRANSFER_INFO, &info);

  SdmmcAppCmd51SendSCR(unit, psc->rca, psc->bufScr); // ACMD51 get scr
  SdmmcSplitScr(&psc->scr, psc->bufScr);
#if SDMMC_DEBUG_DETAIL
  SdmmcDebugShowScr(&psc->scr, psc->bufScr);
#endif

  if(SdmmcIsLocked(unit) != SDMMC_SUCCESS) {
    result = SDMMC_ERRNO_LOCKED;
    goto fail;
  }

#if 0
  // the block size is set 8
  val = 1<<SET_TRANSFER_INFO_BLKSIZE_512B;
  SdmmcCmd16SetBlockLength(unit, val);          // CMD16  the block size is set 8
  // unlock, if the card is locked
  SdmmcCmd42LockUnlock(unit, 0);                // CMD42  unlock card
#endif

  // change the bus width 4-bits
  // 0:1bit, 1:2bit, 2:4bit, 3:8bit
  val = SdmmcGetBusWidthExp(psc);
  re = SdmmcAppCmd6SetBusWidth(unit, val);      // ACMD6

  // clock change
  val = psc->maxclk;
  result = DevSdmmcIoctl(unit, DEV_SDMMC_IOCTL_SET_CLOCK, &val);

fail:
  return result;
}


static sdmmcResult
SdmmcSubSendCommand(int unit, int cmd, uint32_t *ptr)
{
  sdmmcResult   result;

  result = DevSdmmcSendCommand(unit, cmd, ptr);

#if SDMMC_DEBUG_TRANSFER
  if(ptr) {
    printf("# SdmmcSubSendCommand() unit:%d cmd:%2d arg:%x   result:%d\n", unit, cmd, *ptr, result);
  } else {
    printf("# SdmmcSubSendCommand() unit:%d cmd:%2d arg:NIL  result:%d\n", unit, cmd, result);
  }
#endif

  return result;
}
static sdmmcResult
SdmmcRecvResponse(int unit, uint32_t *pResp)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  uint32_t              tout;
  uint32_t              val;

#if SDMMC_DEBUG_TRANSFER
  printf("# SdmmcSubRecvResponse() unit:%d\n", unit);
#endif

  DevSdmmcRecvResponses(unit, pResp);

  return result;
}
static sdmmcResult
SdmmcSubRecvCmdResp1(int unit, uint32_t *ptr)
{
  sdmmcResult   result = SDMMC_ERRNO_TOUT;
  int           re;

  re = DevSdmmcWaitRecvCmd(unit, 100000);
  DevSdmmcRecvResponse1(unit, ptr);
  if(re == DEV_ERRNO_SUCCESS) result = SDMMC_SUCCESS;

  return result;
}
static sdmmcResult
SdmmcSubRecvCmdResp2(int unit, uint32_t *p)
{
  sdmmcResult   result = SDMMC_ERRNO_TOUT;
  int           re;

  if(!p) {
    result = SDMMC_ERRNO_INVALID_ARG;
    goto fail;
  }

  re = DevSdmmcWaitRecvCmd(unit, 100000);
  if(re == DEV_ERRNO_TIMEOUT) goto fail;

  re = DevSdmmcRecvResponses(unit, p);
  if(re == DEV_ERRNO_TIMEOUT) goto fail;

  result = SDMMC_SUCCESS;
fail:
  return result;
}
static sdmmcResult
SdmmcSubRecvCmdResp3(int unit, uint32_t *ptr)
{
  sdmmcResult   result = SDMMC_ERRNO_TOUT;
  int           re;

  re = DevSdmmcWaitRecvCmd(unit, 10000000);
  if(re == DEV_ERRNO_SUCCESS) result = SDMMC_SUCCESS;
  DevSdmmcRecvResponse1(unit, ptr);

  return result;
}
static sdmmcResult
SdmmcSubRecvCmdResp6(int unit, uint32_t *ptr)
{
  sdmmcResult   result;
  int           re;

  re = DevSdmmcWaitRecvCmd(unit, 10000000);
  re = DevSdmmcRecvResponse1(unit, ptr);
  if(re == DEV_ERRNO_SUCCESS) result = SDMMC_SUCCESS;

  return result;
}
static sdmmcResult
SdmmcSubRecvCmdResp7(int unit)
{
  sdmmcResult   result;
  int           re;

  re = DevSdmmcWaitRecvCmd(unit, 10000000);
  if(re == DEV_ERRNO_SUCCESS) result = SDMMC_SUCCESS;

  return result;
}
static sdmmcResult
SdmmcSubWaitSendCmd(int unit)
{
  sdmmcResult   result = SDMMC_ERRNO_TOUT;
  int           re;

  re = DevSdmmcWaitSendCmd(unit, 1000000);
  if(re == DEV_ERRNO_SUCCESS) result = SDMMC_SUCCESS;

  return result;
}
static sdmmcResult
SdmmcSubWaitRecvCmd(int unit)
{
  sdmmcResult   result = SDMMC_ERRNO_UNKNOWN;
  int           re;

  re = DevSdmmcWaitRecvCmd(unit, 1000000);
  if(re == DEV_ERRNO_SUCCESS) result = SDMMC_SUCCESS;
  printf("SdmmcSubWaitRecvCmd  %d %d\n", re, result);

  return result;
}


static sdmmcResult
SdmmcIsLocked(int unit)
{
  int           result = SDMMC_SUCCESS;
  uint32_t      val;

  DevSdmmcRecvResponse1(unit, &val);
  if(val & SDMMC_RESP1_LOCKED_MASK) {
    result = SDMMC_ERRNO_LOCKED;
  }

  return result;
}


static void
SdmmcSplitCsd2(sdmmcCsd2_t *pcsd, uint32_t *ptr)
{
  // bit 128 -- bit 96
  pcsd->csdStructure     = (ptr[0] >> 30) & 0x03;
  pcsd->reserved120      = (ptr[0] >> 24) & 0x3f;
  pcsd->rdAccessTime     = (ptr[0] >> 16) & 0xff;
  pcsd->rdAccessTime2    = (ptr[0] >>  8) & 0xff;
  pcsd->maxDataTransfer  = (ptr[0]      ) & 0xff;

  // bit 95 -- bit 48
  pcsd->cardCmdCls       = (ptr[1] >> 20) & 0xfff;
  pcsd->blkLength        = (ptr[1] >> 16) & 0x0f;
  pcsd->partialBlk       = (ptr[1] >> 15) &    1;
  pcsd->wrBlkMisalign    = (ptr[1] >> 14) &    1;
  pcsd->rdBlkMisalign    = (ptr[1] >> 13) &    1;
  pcsd->dsrImpliment     = (ptr[1] >> 12) &    1;
  pcsd->reserved70       = (ptr[1] >>  6) & 0x3f;
  pcsd->deviceSize       = ((ptr[1] << 16) & 0x3f0000) | ((ptr[2] >> 16) & 0xffff);

  // bit 47 -- bit 32
  pcsd->reserved47       = (ptr[2] >> 15)    & 1;
  pcsd->eraseSingleEn    = (ptr[2] >> 14)    & 1;
  pcsd->eraseSectSize    = (ptr[2] >>  7) & 0x7f;
  pcsd->wrProtectGrpSz   = (ptr[2]      ) & 0x7f;

  pcsd->wrProtectGrpEn   = (ptr[3] >> 31)    & 1;
  pcsd->reserved29       = (ptr[3] >> 29)    & 3;
  pcsd->wrSpeedFactor    = (ptr[3] >> 26)    & 7;
  pcsd->maxWrDataBlkLen  = (ptr[3] >> 22)    & 4;
  pcsd->partialBlkWrAllow= (ptr[3] >> 21)    & 1;
  pcsd->reserved16       = (ptr[3] >> 16) & 0x1f;
  pcsd->fileFormatGrp    = (ptr[3] >> 15)    & 1;
  pcsd->copyFlagp        = (ptr[3] >> 14)    & 1;
  pcsd->permWrProtect    = (ptr[3] >> 13)    & 1;
  pcsd->tempWrProtect    = (ptr[3] >> 12)    & 1;
  pcsd->fileFormat       = (ptr[3] >> 10)    & 3;
  pcsd->reserved8        = (ptr[3] >>  8)    & 3;
  pcsd->crc              = (ptr[3] >>  1) & 0x7f;
  pcsd->reserved0        =  ptr[3]           & 1;

  return;
}


static void
SdmmcSplitCid(sdmmcCid_t *pcid, uint32_t *ptr)
{
  pcid->manufacturerId  = (ptr[0] >> 24) & 0xff;
  pcid->oemAppId        = (ptr[0] >>  8) & 0xffff;
  pcid->productName[0]  = (ptr[0]      ) & 0xff;
  pcid->productName[1]  = (ptr[1] >> 24) & 0xff;
  pcid->productName[2]  = (ptr[1] >> 16) & 0xff;
  pcid->productName[3]  = (ptr[1] >>  8) & 0xff;
  pcid->productName[4]  = (ptr[1]      ) & 0xff;
  pcid->productRevision = (ptr[2] >> 24) & 0xff;
  pcid->productSerial   = ((ptr[2] << 8) & 0xffffff00) | ((ptr[3] >> 24) & 0xff);
  pcid->reserved20      = (ptr[3] >> 20) & 0x4;
  pcid->date            = (ptr[3] >>  8) & 0xfff;
  pcid->crc             = (ptr[3] >>  1) & 0x7f;
  pcid->reserved0       = (ptr[3]      ) & 1;

  return;
}
static void
SdmmcSplitScr(sdmmcScr_t *pscr, uint32_t *ptr)
{
  pscr->scrStructure = (ptr[0] >> 28) & 0x0f;
  pscr->sdSpec       = (ptr[0] >> 24) & 0x0f;
  pscr->eraseStatus  = (ptr[0] >> 23) &    1;
  pscr->cprmSupport  = (ptr[0] >> 20) & 0x07;
  pscr->busWidth     = (ptr[0] >> 16) & 0x0f;
  pscr->sdSpec3      = (ptr[0] >> 15) &    1;
  pscr->extSecurity  = (ptr[0] >> 11) & 0x0f;
  pscr->reserved34   = (ptr[0] >>  2) & 0x1ff;
  pscr->cmdSupport   =  ptr[0]        &    3;
  pscr->reservedManufacturer = ptr[1];

  // extra bits
  pscr->busWidth1bit = (pscr->busWidth >> 0) & 1;
  pscr->busWidth2bit = (pscr->busWidth >> 1) & 1;
  pscr->busWidth4bit = (pscr->busWidth >> 2) & 1;
  pscr->busWidth8bit = (pscr->busWidth >> 3) & 1;

  pscr->sdSpecV10x = 0;
  pscr->sdSpecV110 = 0;
  pscr->sdSpecV200 = 0;
  pscr->sdSpecV30x = 0;
  if(       pscr->sdSpec == 0) {
    pscr->sdSpecV10x = 1;
  } else if(pscr->sdSpec == 1) {
    pscr->sdSpecV110 = 1;
  } else if(pscr->sdSpec == 2) {
    if(pscr->sdSpec == 0) {
      pscr->sdSpecV200 = 1;
    } else {
      pscr->sdSpecV30x = 1;
    }
  }

  return;
}


static void
SdmmcCpy32To8(uint8_t *pDst, uint32_t *pSrc, int count)
{
  for(int i = 0; i < count/4; i++) {
    *pDst++ = (*pSrc >> 24) & 0xff;
    *pDst++ = (*pSrc >> 16) & 0xff;
    *pDst++ = (*pSrc >>  8) & 0xff;
    *pDst++ =  *pSrc        & 0xff;
    pSrc++;
  }

  switch(count & 3) {
  case        0: break;
  case        3:
    *pDst++ = (*pSrc >> 16) & 0xff;
  case        2:
    *pDst++ = (*pSrc >>  8) & 0xff;
  case        1:
    *pDst++ =  *pSrc        & 0xff;
  }

  return;
}


static int
SdmmcGetBusWidthExp(sdmmcSc_t *psc)
{
  int   val = 0;

  if(     psc->scr.busWidth8bit) val = 3;
  else if(psc->scr.busWidth4bit) val = 2;
  /*else if(psc->scr.busWidth1bit) val = 0; */

  val = 2;

  return val;
}


static void
SdmmcDump(int unit, uint32_t lba, int count, uint8_t *ptr)
{
  printf("unit: %d, lba:%x, count:%d", unit, lba, count);
  for(int j = 0; j < 32*count; j++) {
    printf("\n%03x0: ", j);
    for(int i = 0; i < 16; i++) {
      printf("%02x ", *ptr++);
    }
  }
  puts("\n");

  return;
}


#if 0
static void
SdmmcDebugShowCsd(int unit, uint32_t *pCsd)
{
  uint8_t       buf[16], *pBuf;
  uint32_t      *p;

  if((pCsd[0] >> 30) == 0) {
    SdmmcDebugShowCsd1(unit, pCsd);
  } else {
    SdmmcDebugShowCsd22(unit, pCsd);
  }
  return;
}
static void
SdmmcDebugShowCsd1(int unit, uint32_t *pCsd)
{
  uint8_t       buf[16], *pBuf;
  uint32_t      *p;

  p = pCsd;
  pBuf = buf;
  SdmmcCpy32To8(pBuf, p, sizeof(buf));
#if 0
  for(int i = 0; i < sizeof(buf)/4; i++) {
    *pBuf++ = (*p >> 24) & 0xff;
    *pBuf++ = (*p >> 16) & 0xff;
    *pBuf++ = (*p >>  8) & 0xff;
    *pBuf++ =  *p        & 0xff;
    p++;
  }
#endif
  printf("### unit%d CSD\n", unit);

  SdmmcDebugShowHex(0, buf, sizeof(buf));

  puts(" CSD v1.0 is not decoded yet\n");

  return;
}
#endif
static void
SdmmcDebugShowCsd2(sdmmcCsd2_t *pcsd, uint32_t *ptr)
{
  uint8_t       buf[16], *pBuf;
  uint32_t      *p;
  uint32_t      val;
  uint32_t      deviceSize;
  uint32_t      eraseSectSize;

  p = ptr;
  pBuf = buf;
  SdmmcCpy32To8(pBuf, p, sizeof(buf));
#if 0
  for(int i = 0; i < sizeof(buf)/4; i++) {
    *pBuf++ = (*p >> 24) & 0xff;
    *pBuf++ = (*p >> 16) & 0xff;
    *pBuf++ = (*p >>  8) & 0xff;
    *pBuf++ =  *p        & 0xff;
    p++;
  }
#endif

  printf("### CSD\n");
  puts("# hex dump\n");
  SdmmcDebugShowHex(0, buf, sizeof(buf));

  puts("# descriptions\n");

  printf("CSD structure:           %x  Ver%d.0\n",  pcsd->csdStructure, pcsd->csdStructure+1);
  printf("Reserved:               %2x\n",   pcsd->reserved120);
  printf("read access time:       %2x  %3d(d)\n",  pcsd->rdAccessTime,  pcsd->rdAccessTime);
  printf("read access time2:      %2x  %3d(d)\n",  pcsd->rdAccessTime2, pcsd->rdAccessTime2);
  printf("max data transfer:      %2x  %3d(d)\n",  pcsd->maxDataTransfer, pcsd->maxDataTransfer);

  printf("card command classes:  %03x\n",  pcsd->cardCmdCls);
  printf("block length:           %2x  %3d(d)\n",  pcsd->blkLength, pcsd->blkLength);
  printf("partial block (read):    %x\n",  pcsd->partialBlk);
  printf("write block misalign:    %x\n",  pcsd->wrBlkMisalign);
  printf("read block misalign:     %x\n",  pcsd->rdBlkMisalign);
  printf("DSR implemented:         %x\n",  pcsd->dsrImpliment);
  printf("reserved:               %2x\n",  pcsd->reserved70);

  printf("device size:        %6x  %d(d), %dMB\n",
         pcsd->deviceSize, pcsd->deviceSize, pcsd->deviceSize >> 1);
  printf("erase single blk en:     %x\n", pcsd->eraseSingleEn);
  printf("erase sect size:        %2x\n",  pcsd->eraseSectSize);
  printf("write protect grp sz:   %2x\n",  pcsd->wrProtectGrpSz);

  printf("write protect grp en:   %2x\n", pcsd->wrProtectGrpEn);
  printf("reserved                 %x\n", pcsd->reserved29);
  printf("write speed factor:      %x\n", pcsd->wrSpeedFactor);
  printf("max wr data blk len:     %x\n", pcsd->maxWrDataBlkLen);
  printf("part blks write allow:   %x\n", pcsd->partialBlkWrAllow);
  printf("reserved16:              %x\n", pcsd->reserved16);
  printf("file format grp:         %x\n", pcsd->fileFormatGrp);
  printf("copy flag:               %x\n", pcsd->copyFlagp);
  printf("perm wr protect:         %x\n", pcsd->permWrProtect);
  printf("temp wr protect:         %x\n", pcsd->tempWrProtect);
  printf("file format:             %x\n", pcsd->fileFormat);
  printf("reserved:                %x\n", pcsd->reserved8);
  printf("CRC7:                   %02x\n", pcsd->crc);
  printf("reserved0:               %x\n", pcsd->reserved0);

  puts("\n");

  return;
}


#if 0
static void
SdmmcDebugShowCid(int unit, uint32_t *pCid)
{
  uint8_t       buf[16], *pBuf;
  uint32_t      *p;

  p = pCid;
  pBuf = buf;
  for(int i = 0, j = 0; i < sizeof(buf)/4; i++) {
    *pBuf++ = (*p >> 24) & 0xff;
    *pBuf++ = (*p >> 16) & 0xff;
    *pBuf++ = (*p >>  8) & 0xff;
    *pBuf++ =  *p        & 0xff;
    p++;
  }

  printf("### unit%d CID\n", unit);

  puts("# hex dump\n");
  SdmmcDebugShowHex(0, buf, sizeof(buf));

  puts("# descriptions\n");
  printf("Manufacturer ID:    %02x\n", buf[0]);
  printf("OEM/Application ID: %02x%02\n", buf[1], buf[2]);
  printf("Product name:       %02x %02x %02x %02x %02x", buf[3], buf[4], buf[5], buf[6], buf[7]);
  printf("  \"%c%c%c%c%c\"\n", buf[3], buf[4], buf[5], buf[6], buf[7]);
  printf("Product revision:   %02x\n", buf[8]);
  printf("Product serial num: %02x%02x%02x%02x\n", buf[9], buf[10], buf[11], buf[12]);
  printf("Reserved:           %x\n", buf[13]>>4);
  printf("Manufacturing dat:  %02x%02x  %4d/%02d\n",
         buf[13], buf[14],
         2000+(((buf[13]<<4) & 0xf0)|((buf[14]>>4) & 0xf)), buf[14] & 0xf);
  printf("CRC7:               %02x\n", buf[15]>>1);
  puts("\n");

  return;
}
#endif
static void
SdmmcDebugShowCid(sdmmcCid_t *pcid, uint32_t *ptr)
{
  uint8_t       buf[16], *pBuf;
  uint32_t      *p;

  p = ptr;
  pBuf = buf;
  SdmmcCpy32To8(pBuf, p, sizeof(buf));
#if 0
  for(int i = 0, j = 0; i < sizeof(buf)/4; i++) {
    *pBuf++ = (*p >> 24) & 0xff;
    *pBuf++ = (*p >> 16) & 0xff;
    *pBuf++ = (*p >>  8) & 0xff;
    *pBuf++ =  *p        & 0xff;
    p++;
  }
#endif

  printf("### CID\n");

  puts("# hex dump\n");
  SdmmcDebugShowHex(0, buf, sizeof(buf));

  puts("# descriptions\n");
  printf("Manufacturer ID:        %02x\n", pcid->manufacturerId);
  printf("OEM/Application ID:   %04x\n", pcid->oemAppId);
  printf("Product name:           %02x %02x %02x %02x %02x",
         pcid->productName[0], pcid->productName[1], pcid->productName[2],
         pcid->productName[3], pcid->productName[4]);
  printf("  \"%s\"\n", pcid->productName);
  printf("Product revision:       %02x\n", pcid->productRevision);
  printf("Product serial num:   %08x\n", pcid->productSerial);
  printf("Reserved:                %x\n", pcid->reserved20);
  printf("Manufacturing dat:     %03x  %4d/%02d\n",
         pcid->date,
         2000+((pcid->date >> 4) & 0xff), pcid->date & 0xf);
  printf("CRC7:                   %02x\n", pcid->crc);
  printf("Reserved0:               %x\n", pcid->reserved0);
  puts("\n");

  return;
}


static void
SdmmcDebugShowScr(sdmmcScr_t *pscr, uint32_t *ptr)
{
  uint8_t       buf[8], *pBuf;
  uint32_t      *p;
  uint32_t      val;
  uint32_t      deviceSize;
  uint32_t      eraseSectSize;

  p = ptr;
  pBuf = buf;

  SdmmcCpy32To8(pBuf, p, sizeof(buf));

  puts("### SCR\n");
  puts("# hex dump\n");
  SdmmcDebugShowHex(0, buf, sizeof(buf));

  puts("# descriptions\n");

  printf("SCR structure:           %x  Ver%d.0\n",  pscr->scrStructure, pscr->scrStructure+1);
  printf("SD spec:                 %x  %s\n",   pscr->sdSpec,
         (pscr->sdSpec == 0)? "Ver1.0x":
         (pscr->sdSpec == 1)? "Ver1.10":
         (pscr->sdSpec == 2)? "Ver2.00 or 3.0x": "");
  printf("data sta after erases:   %x\n",   pscr->eraseStatus);
  printf("CPRM security support:   %x  %s\n",   pscr->cprmSupport,
         (pscr->cprmSupport == 0)? "No security":
         (pscr->cprmSupport == 1)? "Not used":
         (pscr->cprmSupport == 2)? "SDSC security v1.01":
         (pscr->cprmSupport == 3)? "SDHC security v2.00":
         (pscr->cprmSupport == 4)? "SDXC security v3.xx": "???");
  printf("DAT bus width:           %x %s%s%s%s\n",   pscr->busWidth,
         (pscr->busWidth1bit)? " 1bit": "",
         (pscr->busWidth2bit)? " 2bit": "",
         (pscr->busWidth4bit)? " 4bit": "",
         (pscr->busWidth8bit)? " 8bit": "" );
  printf("SD spec3:                %x  %s\n",   pscr->sdSpec3,
         (pscr->sdSpec == 0)? "":
         (pscr->sdSpec3 == 0)? "Ver2.00":
         (pscr->sdSpec3 == 1)? "Ver3.0x": "");
  printf("ext security support:    %x\n",   pscr->extSecurity);
  printf("reserved34:            %3x\n",    pscr->reserved34);
  printf("command support bits:    %x\n",   pscr->cmdSupport);
  printf("reserved manufactorer:   %x\n",   pscr->reservedManufacturer);
  puts("\n");

  return;
}

static void
SdmmcDebugShowOcr(uint32_t *ptr)
{
  uint8_t       buf[4], *pBuf;
  uint32_t      *p;
  uint32_t      val;
  uint32_t      deviceSize;
  uint32_t      eraseSectSize;

  p = ptr;
  pBuf = buf;

  SdmmcCpy32To8(pBuf, p, sizeof(buf));

  puts("### OCR\n");
  puts("# hex dump\n");
  SdmmcDebugShowHex(0, buf, sizeof(buf));

  puts("# descriptions\n");

  printf("voltage profile:      %s%s%s%s%s%s%s%s%s\n",
         (ptr[0] & (1<<15))? " 2.7": "",
         (ptr[0] & (1<<16))? " 2.8": "",
         (ptr[0] & (1<<17))? " 2.9": "",
         (ptr[0] & (1<<18))? " 3.0": "",
         (ptr[0] & (1<<19))? " 3.1": "",
         (ptr[0] & (1<<20))? " 3.2": "",
         (ptr[0] & (1<<21))? " 3.3": "",
         (ptr[0] & (1<<22))? " 3.4": "",
         (ptr[0] & (1<<23))? " 3.5": "");
  printf("switched to 1.8V:        %x  %s\n",
         (ptr[0] & (1<<24))? 1: 0,
         (ptr[0] & (1<<24))? " yes": "no");
  printf("Card cap Status:         %x\n", (ptr[0] & (1<<30))? 1: 0);
  printf("Card pwr Status:         %x\n\n", (ptr[0] & (1<<31))? 1: 0);

  return;
}


static void
SdmmcDebugShowHex(uint32_t addr, uint8_t *ptr, int size)
{
  uint8_t     *pBuf;
  uint8_t     bufChar[24], *pBufChar;
  uint8_t     c;

  pBuf = ptr;
  pBufChar = bufChar+4;
  bufChar[0] = ' ';
  bufChar[1] = ' ';
  bufChar[2] = ' ';
  bufChar[3] = '|';

  for(int i = 0; i < size; i++) {
    if(!(i & 0xf)) {
      if(i) {
        *pBufChar++ = '|';
        *pBufChar++ = '\n';
        *pBufChar   = '\0';
        puts(bufChar);
        pBufChar = bufChar + 4;
      }
      printf("%08x: ", addr);
    }

    c = *pBuf++;
    if( (c >= 0x20 && c < 0x7f) || c == ' ') {
      *pBufChar++ = c;
    } else {
      *pBufChar++ = '.';
    }
    printf(" %02x", c);
  }

  *pBufChar++ = '|';
  *pBufChar++ = '\n';
  *pBufChar   = '\0';
  puts(bufChar);

  return;
}
