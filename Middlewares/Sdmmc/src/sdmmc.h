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
  int                   supportCmd23: 1;

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
  uint32_t              cid[4];
  uint32_t              csd[4];
  uint32_t              scr[8];

  int                   maxclk;
  uint8_t               xxx;

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

static void             SdmmcDump(int unit, uint32_t lba, int count, uint8_t *ptr);
#endif


#endif
