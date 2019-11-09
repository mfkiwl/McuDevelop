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

#ifndef _DEV_SDMMC_H_
#define _DEV_SDMMC_H_



typedef struct {
  int           pio: 1;
  int           dma: 1;

  int           hwFlow: 1;
  int           clkPolNeg: 1;
  int           clkPowerEn: 1;

  int           busWidth;
  int           szBlock;
  int           clk;
#define DEV_SDMMC_CLOCK_400KHZ          400000
} devSdmmcParam_t;


typedef struct {
  uint32_t      cntBlock;
  uint32_t      szBlock;
  uint32_t      dir;
#define DEV_SDMMC_IOCTL_TRANSTERINFO_DIR_CARD_TO_CTRL    0
#define DEV_SDMMC_IOCTL_TRANSTERINFO_DIR_CTRL_TO_CARD    1
  //uint32_t      toutData;
  void          *ptrDma;

} devSdmmcIoctlTransferInfo_t;


#ifdef _DEV_SDMMC_C_


typedef struct {
  int                   unit;
  stm32Dev_SDMMC        *dev;

  int                   up: 1;
  devSdmmcParam_t       param;

  uint32_t              DCTRL;
  uint32_t              toutData;

} devSdmmcUnit_t;


typedef struct {
  devSdmmcUnit_t        sc[2];

} devSdmmc_t;





#endif


enum {
  DEV_SDMMC_IOCTL_POWERON               =0x0010,
  DEV_SDMMC_IOCTL_POWEROFF,
  DEV_SDMMC_IOCTL_BUSWIDTHON            =0x0020,
  DEV_SDMMC_IOCTL_BUSWIDTHOFF,

  DEV_SDMMC_IOCTL_GET_CARD_STATUS,


};


int             DevSdmmcInit(int unit, devSdmmcParam_t *param);
int             DevSdmmcSendCommand(int unit, int cmd, void *param);
int             DevSdmmcRecvCommandResponse(int unit, uint32_t *cmdresp);
int             DevSdmmcRecvResponse1(int unit, uint32_t *resp1);
int             DevSdmmcRecvResponses(int unit, uint32_t *pResp);
int             DevSdmmcWaitSendCmd(int unit, int tout);
int             DevSdmmcWaitRecvCmd(int unit, int tout);
int             DevSdmmcWaitRecvData(int unit, uint32_t *ptr, int tout);
int             DevSdmmcGetCmdResp1(int unit, uint32_t *resp, int tout);

int             DevSdmmcReadBlock(int unit, uint32_t lba, uint32_t count, uint32_t *ptr);
int             DevSdmmcReadBlock2(int unit, uint32_t lba, uint32_t count, uint32_t *ptr);
int             DevSdmmcWriteBlock(int unit, uint32_t lba, uint32_t count, uint32_t *ptr);
int             DevSdmmcIoctl(int unit, int req, void *ptr);
#define DEV_SDMMC_IOCTL_SET_TRANSFER_INFO       0x0100
#define         SET_TRANSFER_INFO_DIR_SHIFT             8
#define         SET_TRANSFER_INFO_DIR_MASK              (1<<(SET_TRANSFER_INFO_DIR_SHIFT))
#define         SET_TRANSFER_INFO_DIR_READ              (0<<(SET_TRANSFER_INFO_DIR_SHIFT))
#define         SET_TRANSFER_INFO_DIR_WRITE             (1<<(SET_TRANSFER_INFO_DIR_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_SHIFT         0
#define         SET_TRANSFER_INFO_BLKSIZE_MASK          (0xf<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_1B            ( 0<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_2B            ( 1<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_4B            ( 2<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_8B            ( 3<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_16B           ( 4<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_32B           ( 5<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_64B           ( 6<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_128B          ( 7<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_256B          ( 8<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_512B          ( 9<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT))
#define         SET_TRANSFER_INFO_BLKSIZE_VAL(x)        (( (x)<<(SET_TRANSFER_INFO_BLKSIZE_SHIFT)) & SET_TRANSFER_INFO_BLKSIZE_MASK)
#define         SET_TRANSFER_INFO_WRITE_BS(x)           ((SET_TRANSFER_INFO_DIR_WRITE) | (SET_TRANSFER_INFO_BLKSIZE_VAL(x)))
#define         SET_TRANSFER_INFO_READ_BS(x)            ((SET_TRANSFER_INFO_DIR_READ)  | (SET_TRANSFER_INFO_BLKSIZE_VAL(x)))
#define DEV_SDMMC_IOCTL_SET_CLOCK               0x0300
#define DEV_SDMMC_IOCTL_SET_DATA_TRANSMISSION   0x0400
#define         SET_DATA_TRANSMISSION_SHIFT             8
#define         SET_DATA_TRANSMISSION_MASK              (1<<(SET_DATA_TRANSMISSION_SHIFT))
#define         SET_DATA_TRANSMISSION_READ              (0<<(SET_DATA_TRANSMISSION_SHIFT))
#define         SET_DATA_TRANSMISSION_WRITE             (1<<(SET_DATA_TRANSMISSION_SHIFT))
#define DEV_SDMMC_IOCTL_SET_BUSWIDTH            0x0500
void            DevSdmmcInterrupt0(void);
void            DevSdmmcInterrupt1(void);

int             DevSdmmcReadData(int unit, uint32_t *ptr, int len, int tout);

#ifdef  _DEV_SDMMC_C_
static void     DevSdmmcInterrupt(int unit);
static void     DevSdmmcChangeClock(devSdmmcUnit_t *psc);
static int      DevSdmmcWaitRecvDataPio(devSdmmcUnit_t *psc, uint32_t *ptr, int tout);
static int      DevSdmmcWaitRecvDataDma(devSdmmcUnit_t *psc, uint32_t *ptr, int tout);
#endif


#endif
