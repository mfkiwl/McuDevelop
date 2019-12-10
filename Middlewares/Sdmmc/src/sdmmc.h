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

#ifndef _SDMMC_H_
#define _SDMMC_H_


#define SDMMC_CLOCK_400KHZ              400000



typedef struct {
  // bit 128 -- bit 96
  uint8_t       csdStructure;   // 0: Ver1.0, 1: Ver2.0
  uint8_t       reserved120;
  uint8_t       rdAccessTime;
  uint8_t       rdAccessTime2;
  uint8_t       maxDataTransfer;

  // bit 95 -- bit 48
  uint16_t      cardCmdCls;
  uint8_t       blkLength;
  uint8_t       partialBlk: 1;
  uint8_t       wrBlkMisalign: 1;
  uint8_t       rdBlkMisalign: 1;
  uint8_t       dsrImpliment: 1;
  uint8_t       reserved70;
  uint32_t      deviceSize;             // read value + 1

  // bit 47 -- 32
  uint8_t       reserved47: 1;
  uint8_t       eraseSingleEn: 1;
  uint8_t       eraseSectSize;
  uint8_t       wrProtectGrpSz;

  // bit 31 -- bit 0
  uint8_t       wrProtectGrpEn: 1;
  uint8_t       reserved29;
  uint8_t       wrSpeedFactor;
  uint8_t       maxWrDataBlkLen;
  uint8_t       partialBlkWrAllow;
  uint8_t       reserved16;
  uint8_t       fileFormatGrp: 1;
  uint8_t       copyFlagp:     1;
  uint8_t       permWrProtect: 1;
  uint8_t       tempWrProtect: 1;
  uint8_t       fileFormat;
  uint8_t       reserved8;
  uint8_t       crc;
  uint8_t       reserved0: 1;
} sdmmcCsd2_t;


typedef struct {
  uint8_t       manufacturerId;
  uint16_t      oemAppId;
  uint8_t       productName[6];         // with terminator
  uint8_t       productRevision;
  uint32_t      productSerial;
  uint8_t       reserved20;
  uint16_t      date;
  uint8_t       crc;
  uint8_t       reserved0: 1;
} sdmmcCid_t;


typedef struct {
  uint8_t       scrStructure;
  uint8_t       sdSpec;
  uint8_t       eraseStatus: 1;
  uint8_t       cprmSupport;
  uint8_t       busWidth;
#define SDMMC_SCR_BUSWIDTH_1BIT (1<<0)
#define SDMMC_SCR_BUSWIDTH_2BIT (1<<1)
#define SDMMC_SCR_BUSWIDTH_4BIT (1<<2)
#define SDMMC_SCR_BUSWIDTH_8BIT (1<<3)
  uint8_t       sdSpec3: 1;
  uint8_t       extSecurity;
  uint16_t      reserved34;
  uint8_t       cmdSupport;
  uint32_t      reservedManufacturer;

  uint32_t      busWidth1bit: 1;
  uint32_t      busWidth2bit: 1;
  uint32_t      busWidth4bit: 1;
  uint32_t      busWidth8bit: 1;

  uint32_t      supportCMD6:  1;
  uint32_t      supportCMD8:  1;
  uint32_t      supportCMD20: 1;
  uint32_t      supportCMD23: 1;
  uint32_t      supportCMD42: 1;

  uint32_t      sdSpecV10x: 1;
  uint32_t      sdSpecV110: 1;
  uint32_t      sdSpecV200: 1;
  uint32_t      sdSpecV30x: 1;

} sdmmcScr_t;


typedef struct {
  int                   supportCmd23: 1;
  int                   fRecvDone: 1;

  uint8_t               cardVersion;
#define SDMMC_CARDVERSIONE_1_X          0x10
#define SDMMC_CARDVERSIONE_2_X          0x20

  uint8_t               cardType;
#define SDMMC_CARDTYPE_SDSC             0
#define SDMMC_CARDTYPE_SDHC_SDXC        1

  uint8_t               vccio;
#define SDMMC_VCCIO_3300MV              0
#define SDMMC_VCCIO_1800MV              1

  uint8_t               class;

  uint32_t              rca;
  uint32_t              ocr;
  uint32_t              bufCid[4];
  uint32_t              bufCsd[4];
  uint32_t              bufScr[8];

  //sdmmcCsd1_t            csd1;
  sdmmcCsd2_t            csd2;
  sdmmcCid_t             cid;
  sdmmcScr_t             scr;


  int                   maxclk;

} sdmmcSc_t;

typedef struct {
  sdmmcSc_t             sc[2];
} sdmmc_t;

typedef enum  enumSdmmcResult {
  SDMMC_SUCCESS                 = 0,
  SDMMC_ERRNO_UNKNOWN           = -1,
  SDMMC_ERRNO_TOUT              = -2,
  SDMMC_ERRNO_DEVICE_NOT_READY  = -3,
  SDMMC_ERRNO_INVALID_ARG       = -4,
} sdmmcResult;


#define SDMMC_IOCTL_REQ_IS_MEDIUM_READY         0x01
#define SDMMC_IOCTL_REQ_IS_WRITABLE             0x02

sdmmcResult     SdmmcInit(int unit);
sdmmcResult     SdmmcReadBlock(int unit, uint32_t lba, int count, uint8_t *ptr);
sdmmcResult     SdmmcWriteBlock(int unit, uint32_t lba, int count, uint8_t *ptr);
sdmmcResult     SdmmcIoctl(int unit, int req, void *ptr);
void            SdmmcCallback(int unit, int req, void *ptr);



// CMD
sdmmcResult     SdmmcCmd0GoIdleState(int unit);
sdmmcResult     SdmmcCmd2AllSendCID(int unit, uint32_t *pCid);
sdmmcResult     SdmmcCmd3SendRelativeAddr(int unit, uint32_t *pRCA);
sdmmcResult     SdmmcCmd7SelDeselCard(int unit, uint32_t addr);
sdmmcResult     SdmmcCmd8SendIfCond(int unit, uint32_t arg);
sdmmcResult     SdmmcCmd9SendCSD(int unit, uint32_t addr, uint32_t *pCsd);
sdmmcResult     SdmmcCmd13SendStatus(int unit, uint32_t arg, uint32_t *status);
sdmmcResult     SdmmcCmd16SetBlockLength(int unit, uint32_t blocksize);
sdmmcResult     SdmmcCmd42LockUnlock(int unit, uint32_t arg);
sdmmcResult     SdmmcCmd55AppCommand(int unit, uint32_t arg);

sdmmcResult     SdmmcAppCmd6SetBusWidth(int unit, uint32_t arg);
sdmmcResult     SdmmcAppCmd41SdSendOpCond(int unit, uint32_t SdType, uint32_t *ocr);
sdmmcResult     SdmmcAppCmd51SendSCR(int unit, uint32_t arg, uint32_t *scr);

sdmmcResult     SdmmcCmd17ReadSingleBlock(int unit, uint32_t addr);
sdmmcResult     SdmmcCmd18ReadMultiBlock(int unit, uint32_t addr);
sdmmcResult     SdmmcCmd23SetBlockCount(int unit, uint32_t cnt);
sdmmcResult     SdmmcCmd24WriteSingleBlock(int unit, uint32_t addr);
sdmmcResult     SdmmcCmd25WriteMultiBlock(int unit, uint32_t addr);
sdmmcResult     SdmmcCmd12StopTransmission(int unit);

sdmmcResult     SdmmcGetResponse(int unit, uint32_t *resp);

#ifdef _SDMMC_C_
// local sub routine
static sdmmcResult      SdmmcSubGetCapability(int unit);
static sdmmcResult      SdmmcSubInitCard(int unit);
static sdmmcResult      SdmmcSubSendCommand(int unit, int cmd, uint32_t *ptr);
static sdmmcResult      SdmmcRecvResponse(int unit, uint32_t *pResp);
static sdmmcResult      SdmmcSubRecvCmdResp1(int unit, uint32_t *ptr);
static sdmmcResult      SdmmcSubRecvCmdResp2(int unit, uint32_t *p);
static sdmmcResult      SdmmcSubRecvCmdResp3(int unit, uint32_t *ptr);
static sdmmcResult      SdmmcSubRecvCmdResp6(int unit, uint32_t *ptr);
static sdmmcResult      SdmmcSubRecvCmdResp7(int unit);
static sdmmcResult      SdmmcSubWaitSendCmd(int unit);
static sdmmcResult      SdmmcSubWaitRecvCmd(int unit);

static void             SdmmcSplitCsd2(sdmmcCsd2_t *pcsd, uint32_t *ptr);
static void             SdmmcSplitCid(sdmmcCid_t *pcid, uint32_t *ptr);
static void             SdmmcSplitScr(sdmmcScr_t *pscr, uint32_t *ptr);

static void             SdmmcCpy32To8(uint8_t *pDst, uint32_t *pSrc, int count);

static void             SdmmcDump(int unit, uint32_t lba, int count, uint8_t *ptr);
static void             SdmmcDebugShowCsd2(sdmmcCsd2_t *pcsd, uint32_t *ptr);
static void             SdmmcDebugShowCid(sdmmcCid_t *pcid, uint32_t *ptr);
static void             SdmmcDebugShowScr(sdmmcScr_t *pscr, uint32_t *ptr);
static void             SdmmcDebugShowOcr(uint32_t *ptr);

static void             SdmmcDebugShowHex(uint32_t addr, uint8_t *ptr, int size);

#endif


#endif
