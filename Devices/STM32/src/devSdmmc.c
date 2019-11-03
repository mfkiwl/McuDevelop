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

#define _DEV_SDMMC_C_

#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>

#include        "system.h"
#include        "devErrno.h"

#include        "rtos.h"

#include        "sdmmc_def.h"
#include        "devSdmmc.h"

/*
 *  if enable, the clock is stop after send
 */
#define DEV_SDMMC_POWERSAVE_EN          0

#define DEV_SDMMC_DEBUG_CMDRESP         0


static devSdmmc_t       sdmmc;

#if 0
static uint8_t devSdmmcCmdExtParamTbl[] = {
  SDMMC_CMD_WAITRESP_NO    | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //go idle state
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //SET_BLOCKLEN
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //CMD_READ_SINGLE_BLOCK;
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //CMD_READ_multi block
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //CMD_read_single block
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //CMD_write_multi block
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //CMD_sinegle_multi block
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //Sd ERASE_GRP_end;
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //Sd ERASE_GRP_START;
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //ERASE_GRP_end;
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //ERASE_GRP_START;
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //ERASE
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //stop transfer
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //sel desel
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //open condidtion
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //app command
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //app op cond
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //bus width
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //send csr
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //set rel add
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //send status
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //status register
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //op condition
  SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //hs switch
  SDMMC_CMD_WAITRESP_LONG  | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //send cid
  SDMMC_CMD_WAITRESP_LONG  | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES,     //send csd
};
#endif


const static stm32Dev_SDMMC *tblDevice[] = {SDMMC1_PTR, SDMMC2_PTR};

/**
  * @brief  initilize the devcie
  * @param  unit   unit number
  * @param  param  the pointer of initialize parameters
  * @retval int  0: success
  */
int
DevSdmmcInit(int unit, devSdmmcParam_t *param)
{
  int                   result = DEV_ERRNO_UNKNOWN;

  devSdmmcUnit_t        *psc;
  stm32Dev_SDMMC        *p;
  uint32_t              clkcr;

  systemClockFreq_t     clk;
  uint32_t              val;

  if(unit < 0) {
    memset(&sdmmc, 0, sizeof(sdmmc));
    goto end;
  }

  if(unit >= SDMMC_NUM_MAX) {
    result = DEV_ERRNO_UNIT_EXCEEDED;
    goto fail;
  }

  psc = &sdmmc.sc[unit];
  psc->dev = (stm32Dev_SDMMC *)tblDevice[unit];

  p = psc->dev;

#if 0
  val = 128;                    // for backup
  if(param->clk) {
    SystemGetClockValue(&clk);
    val = clk.pll1.Q / DEV_SDMMC_CLOCK_400KHZ;
    val = (val >= 2)? val-2: 0;
  }
#endif
  // 400kHz first
  SystemGetClockValue(&clk);
  val = clk.pll1.Q / DEV_SDMMC_CLOCK_400KHZ;
  val = (val >= 2)? val-2: 0;

  clkcr = SDMMC_CLKCR_CLKEN_YES | SDMMC_CLKCR_CLKDIV_VAL(val);
  if(param->clkPolNeg) clkcr |= SDMMC_CLKCR_NEGEDGE_YES;
  if(param->hwFlow)    clkcr |= SDMMC_CLKCR_HWFC_EN_YES;
#if DEV_SDMMC_POWERSAVE_EN
  clkcr |= SDMMC_CLKCR_PWRSAV_YES;
#endif
  clkcr |= SDMMC_CLKCR_WIDBUS_1BIT;
  psc->param.szBlock = 512;

  p->CLKCR = clkcr;
  p->POWER = SDMMC_POWER_PWRCTRL_ON;
  p->MASK  = 0x003fc5ff;

  psc->toutData = 100000000;
  psc->DCTRL = /*SDMMC_DCTRL_SDIOEN_YES |*/ SDMMC_DCTRL_DTMODE_BLOCK | SDMMC_DCTRL_DBLOCKSIZE_512B;
  psc->up = 1;

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
DevSdmmcDeinit(int unit)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;

  if(unit >= SDMMC_NUM_MAX) {
    result = DEV_ERRNO_UNIT_EXCEEDED;
    goto fail;
  }

  psc = &sdmmc.sc[unit];
  psc->up = 0;

  psc->dev->CLKCR = 0;

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
DevSdmmcSendCommand(int unit, int cmd, void *param)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;
  stm32Dev_SDMMC        *p;
  uint32_t              reg;
  uint32_t              val = 0;

  if(unit >= SDMMC_NUM_MAX) goto fail;
  if(param) val = *(uint32_t *)param;

  psc = &sdmmc.sc[unit];
  p = psc->dev;

  // add the STM SDMMC module depended parameters
  switch(cmd) {
  case        SDMMC_CMD2_ALL_SEND_CID:
  case        SDMMC_CMD9_SEND_CSD:
  case        SDMMC_CMD26_PROGRAM_CID:
  case        SDMMC_CMD27_PROGRAM_CSD:
    reg = SDMMC_CMD_WAITRESP_LONG  | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES;
    break;
  case        SDMMC_CMD0_GO_IDLE_STATE:
    reg = SDMMC_CMD_WAITRESP_NO    | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES;
    break;
  default:
    reg = SDMMC_CMD_WAITRESP_SHORT | SDMMC_CMD_WAITINT_NO | SDMMC_CMD_CPSMEN_YES;
  }
  reg |= SDMMC_CMD_CMDINDEX_VAL(cmd);

  // clear all interrupt status
  p->ICR = SDMMC_ICR_FAILS_MASK;

#if DEV_SDMMC_DEBUG_CMDRESP
  printf("#  DevSdmmcSendCommand() cmd:%2d cmd:%x, arg:%x\n", cmd, reg, val);
#endif

  // send the command
  p->ARG = val;
  p->CMD = reg;

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
DevSdmmcRecvCommandResponse(int unit, uint32_t *cmdresp)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];
  *cmdresp = psc->dev->RESPCMD;

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
DevSdmmcRecvResponse1(int unit, uint32_t *resp1)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];
  *resp1 = psc->dev->RESP1;

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}
int
DevSdmmcRecvResponses(int unit, uint32_t *pResp)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];

  if(pResp) {
    pResp[0] = psc->dev->RESP1;
    pResp[1] = psc->dev->RESP2;
    pResp[2] = psc->dev->RESP3;
    pResp[3] = psc->dev->RESP4;
  }

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}







int
DevSdmmcWaitSendCmd(int unit, int tout)
{
  int                   result = DEV_ERRNO_TIMEOUT;

  devSdmmcUnit_t        *psc;
  uint32_t              val;

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];

  // wait to finish sending
  while(1) {
    val = psc->dev->STA;
    if(val & SDMMC_STA_CMDSENT_MASK) break;
    if(val & SDMMC_STA_FAILS_MASK) {
      goto fail;
    }
    if(tout-- <= 0) {
      result = DEV_ERRNO_TIMEOUT;
      goto fail;
    }
  }

  result = DEV_ERRNO_SUCCESS;

fail:
#if DEV_SDMMC_DEBUG_CMDRESP
  printf("DevSdmmcWaitSendCmd() sta:%x respcmd: result:%d\n",
         val, psc->dev->RESPCMD, result);
#endif
  // claer all interrupt status
  psc->dev->ICR = SDMMC_ICR_FAILS_MASK;

  return result;
}
int
DevSdmmcWaitRecvCmd(int unit, int tout)
{
  int                   result = DEV_ERRNO_TIMEOUT;

  devSdmmcUnit_t        *psc;
  uint32_t              val;

  unit = 0;

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];

  // wait to finish sending
  while(1) {
    val = psc->dev->STA;
    if(!(val & SDMMC_STA_CMDACT_MASK)) {
      uint32_t  cmd;
      if(val & SDMMC_STA_CMDREND_MASK) break;
      cmd = psc->dev->RESPCMD;
      if(cmd == SDMMC_RESPCMD_INVALID) {
        if(val & SDMMC_STA_CCRCFAIL_MASK) break;    // errata STM32F730
      }
    }
    if(val & SDMMC_STA_CTIMEOUT_MASK) {
      result = DEV_ERRNO_TIMEOUT;
      goto fail;
    }
    if(val & SDMMC_STA_FAILS_MASK) {
      goto fail;
    }
    if(tout-- <= 0) {
      result = DEV_ERRNO_TIMEOUT;
      goto fail;
    }
  }

  result = DEV_ERRNO_SUCCESS;

fail:
#if DEV_SDMMC_DEBUG_CMDRESP
  printf("#  devSdmmcWaitRecvCmd() sta:%x, respcmd:%2d, result:%d\n",
         val, psc->dev->RESPCMD, result);
#endif
  // claer all interrupt status
  psc->dev->ICR = SDMMC_ICR_FAILS_MASK;

  return result;
}


int
DevSdmmcWaitRecvData(int unit, uint32_t *ptr, int tout)
{
  int                   result = DEV_ERRNO_TIMEOUT;

  devSdmmcUnit_t        *psc;
  uint32_t              val;

  if(unit >= SDMMC_NUM_MAX) goto fail;
  if((uint32_t)ptr & 3) goto fail;

  psc = &sdmmc.sc[unit];

  // wait to finish sending
  while(1) {
    val = psc->dev->STA;

    if((val & (SDMMC_STA_DATAEND_MASK | SDMMC_STA_RXFIFOHF_MASK)) == (SDMMC_STA_DATAEND_MASK)) {
      break;
    }

    if(val & SDMMC_STA_RXFIFOF_MASK) {
      for(int i = 0; i < 32; i++) *ptr++ = psc->dev->FIFO;
    } else if(val & SDMMC_STA_RXFIFOHF_MASK) {
      for(int i = 0; i < 8; i++) *ptr++ = psc->dev->FIFO;
    }

    if((val & SDMMC_STA_DTIMEOUT_MASK) || tout-- <= 0) {
      result = DEV_ERRNO_TIMEOUT;
      goto fail;
    }
    if(val & (SDMMC_STA_DCRCFAIL_MASK|SDMMC_STA_RXOVERR_MASK)) {
      goto fail;
    }
  }

  result = DEV_ERRNO_SUCCESS;

fail:
#if DEV_SDMMC_DEBUG_CMDRESP
  printf("DevSdmmcWaitRecvData() sta:%x, result:%d\n",
         val, result);
#endif
  // claer all interrupt status
  psc->dev->ICR = SDMMC_ICR_FAILS_MASK;

  return result;
}


int
DevSdmmcReadBlock(int unit, uint32_t lba, uint32_t count, uint32_t *ptr)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;
  stm32Dev_SDMMC        *p;
  uint32_t              val;

#if DEV_SDMMC_DEBUG_CMDRESP
  printf("# DevSdmmcReadBlock() unit:%d, lba:%x, cnt:%d\n", unit, lba, count);
#endif

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];
  p = psc->dev;

  // initialize
  p->DCTRL = 0;

  // set parameters
  p->DTIMER = psc->toutData;
  p->DLEN = SDMMC_DLEN_VAL(count * psc->param.szBlock);
  val  = psc->DCTRL;
  val |= SDMMC_DCTRL_DTEN_YES;
  p->DCTRL = val;
  p->ARG = lba;
  p->ICR = SDMMC_ICR_FAILS_MASK;

  // flush
  for(int i = 0; i < 32; i++) p->FIFO;

  // send command
  if(count == 1) {
    p->CMD = SDMMC_CMD_READSINGLE;
  } else {
    p->CMD = SDMMC_CMD_READMULTI;
  }

  // wait the command response
  DevSdmmcWaitRecvCmd(unit, 10000);

  // wait to finish receiving
  DevSdmmcWaitRecvData(unit, ptr, 400000);


#if 0
  if(count > 1) {
    // stop receiving
    p->ARG = 0;
    p->CMD = SDMMC_CMD_STOPTRANFERING;
    DevSdmmcWaitRecvCmd(unit, 10000);
  }
#endif

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}
int
DevSdmmcReadBlock2(int unit, uint32_t lba, uint32_t count, uint32_t *ptr)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;
  stm32Dev_SDMMC        *p;
  uint32_t              val;

#if DEV_SDMMC_DEBUG_CMDRESP
  printf("# DevSdmmcReadBlock() uint:%d, lba:%x, cnt:count\n", unit, lba, count);
#endif

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];
  p = psc->dev;

  // initialize
  p->DCTRL = 0;

  // set parameters
  p->DTIMER = psc->toutData;
  p->DLEN = SDMMC_DLEN_VAL(count * psc->param.szBlock);
  val  = psc->DCTRL;
  val |= SDMMC_DCTRL_DTDIR_CARD2CTRL | SDMMC_DCTRL_DTEN_YES;
  p->DCTRL = val;
  p->ARG = lba;
  p->ICR = SDMMC_ICR_FAILS_MASK;

#if 0
  // flush
  for(int i = 0; i < 32; i++) p->FIFO;
#endif


end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
DevSdmmcWriteBlock(int unit, uint32_t lba, uint32_t count, uint32_t *ptr)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;

#if DEV_SDMMC_DEBUG_CMDRESP
  printf("# DevSdmmcWriteBlock() uint:%d, lba:%x, cnt:count\n", unit, lba, count);
#endif

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];



 end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
DevSdmmcIoctl(int unit, int req, void *ptr)
{
  int                   result = DEV_ERRNO_UNKNOWN;
  devSdmmcUnit_t        *psc;
  //  systemClockFreq_t     clk;

  //devSdmmcIoctlTransferInfo_t   *p;
  uint32_t              reg, val, bs;

#if DEV_SDMMC_DEBUG_CMDRESP
  printf("# DevSdmmcIoctl() uint:%d, req:%x\n", unit, req);
#endif

  if(unit >= SDMMC_NUM_MAX) goto fail;

  psc = &sdmmc.sc[unit];

  switch(req) {
  case        DEV_SDMMC_IOCTL_SET_TRANSFER_INFO:
#if 0
    p = (devSdmmcIoctlTransferInfo_t *) ptr;
    psc->DCTRL &= ~SDMMC_DCTRL_DBLOCKSIZE_MASK;
    psc->DCTRL |=  SDMMC_DCTRL_DBLOCKSIZE_VAL(9);
#if 0
    if(p->szBlock == 512) {
    }
#endif
    psc->toutData = p->toutData;
#endif
    val = *(uint32_t *) ptr;
    bs = val & SET_TRANSFER_INFO_BLKSIZE_MASK;
    psc->DCTRL &= ~(SDMMC_DCTRL_DBLOCKSIZE_MASK | SDMMC_DCTRL_DTDIR_SHIFT);
    psc->DCTRL |=   SDMMC_DCTRL_DBLOCKSIZE_VAL(bs);
    psc->DCTRL |= ((val & SET_TRANSFER_INFO_DIR_MASK)?
                   SDMMC_DCTRL_DTDIR_CTRL2CARD:
                   SDMMC_DCTRL_DTDIR_CARD2CTRL);
    psc->param.szBlock = 1<<bs;

    break;

  case        DEV_SDMMC_IOCTL_TRANSFER:
#if 0
    p = (devSdmmcIoctlTransferInfo_t *) ptr;

    p->cntBlock;
    val = psc->DCTRL;
    val |= SDMMC_DCTRL_DTMODE_BLOCK | SDMMC_DCTRL_DTEN_YES ;
    if(p->dir == DEV_SDMMC_IOCTL_TRANSTERINFO_DIR_CARD_TO_CTRL) {
      val |= SDMMC_DCTRL_DTDIR_CARD2CTRL;
    }
#endif
    break;

  case        DEV_SDMMC_IOCTL_SET_CLOCK:
    val = *(uint32_t *)ptr;
    if(val == 0) {
      result = DEV_ERRNO_PARAM_EXCEEDED;
      goto fail;
    }
    psc->param.clk = val;
    DevSdmmcChangeClock(psc);
    break;

  case        DEV_SDMMC_IOCTL_SET_BUSWIDTH:
    psc->param.busWidth = *(uint32_t *)ptr;
    val = psc->dev->CLKCR & ~SDMMC_CLKCR_WIDBUS_MASK;
    switch(psc->param.busWidth) {
    case        8: val |= SDMMC_CLKCR_WIDBUS_8BIT; break;
    case        4: val |= SDMMC_CLKCR_WIDBUS_4BIT; break;
    default:       val |= SDMMC_CLKCR_WIDBUS_1BIT; break;
    }
    psc->dev->CLKCR = val;
    break;

  case        DEV_SDMMC_IOCTL_SET_DATA_TRANSMISSION:
    val = *(uint32_t *)ptr;
    reg = psc->DCTRL;
    if((val & SET_DATA_TRANSMISSION_MASK) == SET_DATA_TRANSMISSION_READ) {
      reg |= SDMMC_DCTRL_DTDIR_CARD2CTRL | SDMMC_DCTRL_DTEN_YES;
    } else {
      reg |= SDMMC_DCTRL_DTDIR_CTRL2CARD | SDMMC_DCTRL_DTEN_YES;
    }
    psc->dev->DCTRL = reg;
    break;
  }

end:
  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}



int
DevSdmmcReadData(int unit, uint32_t *ptr, int len, int tout)
{
  int                   result = DEV_ERRNO_TIMEOUT;

  devSdmmcUnit_t        *psc;
  stm32Dev_SDMMC        *p;
  uint32_t              val;
  uint32_t              cnt;

  if(unit >= SDMMC_NUM_MAX) goto fail;
  if((uint32_t)ptr & 3) goto fail;

  psc = &sdmmc.sc[unit];
  p = psc->dev;

  cnt = (len + 3) >> 2;

  p->DTIMER = psc->toutData;
  p->DLEN = SDMMC_DLEN_VAL(len);
  val  = psc->DCTRL;
  val |= SDMMC_DCTRL_DTDIR_CARD2CTRL | SDMMC_DCTRL_DTEN_YES;
  p->DCTRL = val;

  // wait to finish sending
  while(1) {
    val = psc->dev->STA;
    if(val & SDMMC_STA_RXDAVL_MASK) {
      *ptr++ = psc->dev->FIFO;
      cnt--;
      if(cnt <= 0) break;
    }

    if((val & SDMMC_STA_DTIMEOUT_MASK) || tout-- <= 0) {
      result = DEV_ERRNO_TIMEOUT;
      goto fail;
    }
    if(val & (SDMMC_STA_DCRCFAIL_MASK|SDMMC_STA_RXOVERR_MASK)) {
      goto fail;
    }
  }

  result = DEV_ERRNO_SUCCESS;

fail:
#if DEV_SDMMC_DEBUG_CMDRESP
  printf("DevSdmmcReadData() sta:%x, result:%d\n",
         val, result);
#endif
  // claer all interrupt status

  return result;
}


void
DevSdmmcInterrupt0(void)
{
  DevSdmmcInterrupt(0);
  return;
}
void
DevSdmmcInterrupt1(void)
{
  DevSdmmcInterrupt(1);
  return;
}
static void
DevSdmmcInterrupt(int unit)
{
  devSdmmcUnit_t        *psc;
  uint32_t              reg;
  uint32_t              intr;

  psc = &sdmmc.sc[unit];
  intr = psc->dev->STA;
  psc->dev->ICR = intr;

  //printf("\nx %x\n", intr);

  if(intr & SDMMC_STA_CMDSENT_MASK) {
    //RtosQueueSend(rtosQueueId id, void *ptr, 100);
  }



  return;
}


static void
DevSdmmcChangeClock(devSdmmcUnit_t *psc)
{
  systemClockFreq_t     clk;
  uint32_t              val;
  uint32_t              reg;

  SystemGetClockValue(&clk);
  val = clk.pll1.Q / psc->param.clk;
  val = (val >= 2)? val-2: 0;
  reg  = psc->dev->CLKCR;
  reg &= ~SDMMC_CLKCR_CLKDIV_MASK;
  reg |=  SDMMC_CLKCR_CLKDIV_VAL(val);
  psc->dev->CLKCR = reg;

  return;
}
