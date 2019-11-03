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

#include        "sdmmc.h"
#include        "sdmmc_def.h"

#define SDMMC_DEBUG_API         0
#define SDMMC_DEBUG_CMD         0
#define SDMMC_DEBUG_TRANSFER    0
#define SDMMC_DEBUG_DUMP        0


static sdmmc_t  sdmmc;


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

  sdmmc.sc[0].maxclk = 25000000;

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

  result = SDMMC_SUCCESS;  // !!!!!!!!!!!!!!!!!! adhoc

fail:
  return result;
}


sdmmcResult
SdmmcReadBlock(int unit, uint32_t lba, int count, uint8_t *ptr)
{
  sdmmcResult           result = SDMMC_ERRNO_UNKNOWN;
  int                   re;
  uint32_t              val;
  uint32_t              status;

  sdmmcSc_t             *psc;

  psc = &sdmmc.sc[unit];

#if SDMMC_DEBUG_API
  printf("# SdmmcReadBlock() unit:%d, lba:%x, cnt:%x\n", unit, lba, count);
#endif

#if 0
  // get status
  SdmmcCmd13SendStatus(unit, psc->rca, &status);
#endif

  // set the block size
  SdmmcCmd16SetBlockLength(unit, 512);          // CMD16  the block size is set 512
  val = SET_TRANSFER_INFO_READ_BS(SET_TRANSFER_INFO_BLKSIZE_512B);
  DevSdmmcIoctl(unit, DEV_SDMMC_IOCTL_SET_TRANSFER_INFO, &val);

  re = DevSdmmcReadBlock2(unit, lba, count, (uint32_t *)ptr);

  if(count == 1) {
    SdmmcCmd17ReadSingleBlock(unit, lba);
  } else {
    if(psc->supportCmd23) {
      result = SdmmcCmd23SetBlockCount(unit, count);
      if(result != SDMMC_SUCCESS) {
        psc->supportCmd23 = 0;
      }
    }
    SdmmcCmd18ReadMultiBlock(unit, lba);
  }

  result = DevSdmmcWaitRecvData(unit, (uint32_t *)ptr, 400000);

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
  uint32_t              tout;
  uint32_t              val;

  SdmmcSubSendCommand(unit, SDMMC_CMD12_STOP_TRANSMISSION, NULL);
  result = SdmmcSubWaitSendCmd(unit);
  //SdmmcSubRecvCmdResp1(unit, &val);

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

  SdmmcSubSendCommand(unit, SDMMC_CMD16_SET_BLOCKLEN, &blocksize);
  result = SdmmcSubRecvCmdResp1(unit, &val);
  if(result != SDMMC_SUCCESS) goto fail;

  val = SET_TRANSFER_INFO_READ_BS(SET_TRANSFER_INFO_BLKSIZE_8B);
  DevSdmmcIoctl(unit, DEV_SDMMC_IOCTL_SET_TRANSFER_INFO, &val);

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

  SdmmcSubSendCommand(unit, SDMMC_CMD42_LOCK_UNLOCK, &arg);
  result = SdmmcSubRecvCmdResp1(unit, &val);

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

  psc = &sdmmc.sc[unit];

  result = SdmmcCmd55AppCommand(unit, psc->rca);  // CMD55
  if(result < SDMMC_SUCCESS) goto fail;

  SdmmcSubSendCommand(unit, SDMMC_ACMD51_SEND_SCR, NULL);
  result = SdmmcSubRecvCmdResp1(unit, &val);
  DevSdmmcReadData(unit, pScr, 8, 1000000);

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
  } while(!(ocr & SDMMC_ACMD41_READY_MASK));

  /* voltage */
  psc->vccio = SDMMC_VCCIO_3300MV;
  if((ocr & SDMMC_ACMD41_S18_MASK) == SDMMC_ACMD41_S18R_1800MV) {
    psc->vccio = SDMMC_VCCIO_1800MV;
  }

  /* card type */
  psc->cardType = SDMMC_CARDTYPE_SDSC;
  if(psc->cardVersion == SDMMC_CARDVERSIONE_2_X &&
     (ocr & SDMMC_ACMD41_CCS_MASK) == SDMMC_ACMD41_CCS_SDXC) {
    psc->cardType = SDMMC_CARDTYPE_SDHC_SDXC;
  }

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

  result == SDMMC_SUCCESS;

fail:
  return result;
}


static sdmmcResult
SdmmcSubInitCard(int unit)
{
  sdmmcResult   result = SDMMC_ERRNO_UNKNOWN;
  sdmmcSc_t     *psc;

  uint32_t      rca = 1;
  uint32_t      val;

  psc = &sdmmc.sc[unit];

  // get the CID info
  result = SdmmcCmd2AllSendCID(unit, psc->cid); // CMD2

  // get the relative card address
  SdmmcCmd3SendRelativeAddr(unit, &rca);        // CMD3

  // set the relative card address
  psc->rca = rca & 0xffff0000;
  SdmmcCmd9SendCSD(unit, psc->rca, psc->csd);   // cmd9
  //psc->class = psc->csd[1] >> 20;
  SdmmcCmd7SelDeselCard(unit, psc->rca);        // cmd7

  // the block size is set 8
  SdmmcCmd16SetBlockLength(unit, 8);            // CMD16  the block size is set 8

  SdmmcAppCmd51SendSCR(unit, psc->rca, psc->scr); // ACMD51 get scr

#if 0
  // unlock, if the card is locked
  SdmmcCmdLockUnlock(unit, 0);                  // CMD42  unlock card
#endif

  // change the bus width 4-bits
  SdmmcAppCmd6SetBusWidth(unit, 2);             // ACMD6

  // clock change
  val = psc->maxclk;
  result = DevSdmmcIoctl(unit, DEV_SDMMC_IOCTL_SET_CLOCK, &val);

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
