/*
 * Copyright (c) 2018,2019 zhtlab
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

#define _UPDATE_C_


#include	<stdint.h>
#include	<string.h>

#include	"config.h"
#include	"system.h"

#include	"devFlash.h"

#include	"crc32.h"
#include	"peri.h"
#include	"main.h"

#include	"update.h"

extern const uint8_t	strVersionText[];
extern const uint8_t	strProductText[];

#define	UPDATE_SEQ_IDLE			0x00
#define	UPDATE_SEQ_WAIT_SUM		0x01
#define	UPDATE_SEQ_ERASE_BLOCK		0x10
#define	UPDATE_SEQ_ERASE_BLOCK_EXEC	0x11
#define	UPDATE_SEQ_ERASE_BLOCK_SUM	0x12
#define	UPDATE_SEQ_READ_ADDR		0x20
#define	UPDATE_SEQ_READ_LEN		0x21
#define	UPDATE_SEQ_READ_DATA		0x22
#define	UPDATE_SEQ_PROG_ADDR		0x28
#define	UPDATE_SEQ_PROG_LEN		0x29
#define	UPDATE_SEQ_PROG_DATA		0x2a
#define	UPDATE_SEQ_GETCRC_ADDR		0x30
#define	UPDATE_SEQ_GETCRC_LEN		0x31
#define	UPDATE_SEQ_GETINFO_NUM		0x34
#define	UPDATE_SEQ_CHECKBLANK_ADDR	0x38
#define	UPDATE_SEQ_CHECKBLANK_LEN	0x39

#define UPDATE_BLANKVALUE               0xff

#define UPDATE_PROGRAM_DOUBLE_WORD_EN   0
#define UPDATE_PROGRAM_SINGLE_WORD_EN   1
#define UPDATE_PROGRAM_HALF_WORD_EN     0
#define UPDATE_PROGRAM_BYTE_EN          0
#define UPDATE_PROGRAM_BURST_EN         0

static int		seq = UPDATE_SEQ_IDLE;
static uint8_t		cmd, /*lenData, */sum;
static int		lenRecv;
static uint32_t         addr;
static volatile uint32_t        *pAddr32;
static int		lenData;
static int		cntRecv;


void
UpdateInit(void)
{
  lenRecv = 1;
  return;
}
int
UpdateSearchInterface(void)
{
  int		result = -1;

  result = 0;

  return result;
}
const static uint8_t	cmdList[] = {
  STM32_CMD_GET, STM32_CMD_GET_VERSION, STM32_CMD_GET_ID,
  STM32_CMD_READ_MEMORY,
  STM32_CMD_GO, STM32_CMD_WRITE_MEMORY, STM32_CMD_ERASE,
  STM32_CMD_WRITE_PROTECT,
  STM32_CMD_WRITE_UNPROTECT,
  STM32_CMD_READOUT_PROTECT,
  STM32_CMD_READOUT_UNPROTECT,
#if CONFIG_BOOTLOADER_CMD_EXTENTIONS_ENABLE
  /* extentions */
  STM32_CMD_CHECK_BLANK,
  STM32_CMD_GET_INFO,
  STM32_CMD_GET_CRC,
#endif
};
const static uint8_t	cmdVersion[] = {
  CONFIG_BOOTLOADER_CMD_VERSION, 0x00, 0x00
};
const static uint8_t	cmdChipID[] = {
  (CONFIG_CHIP_ID) >> 8, (CONFIG_CHIP_ID) & 0xff,
};
#if 0
const static uint8_t	cmdXXX[] = {
  0xff, 0xff, 0x00
};
#endif
void
UpdateLoop(void)
{
  static int    re;
  uint8_t	buf[260], *p;
  uint8_t	c, res = STM32_RES_NACK;
  int		len;
  uint32_t	val;
  int		i, cnt;
  uint8_t	xor = 0;
  volatile uint8_t  *pAddr;

  if(PeriGetRecvSize() >= lenRecv) {
    len = lenRecv;
    PeriRecv(buf, &len);
    c = buf[0];

    switch(seq) {
    case        UPDATE_SEQ_IDLE:
      if(c == STM32_CMD_SYNC) {
        UpdateSendResponse(STM32_RES_ACK);
      } else {
        cmd = c;
        seq = UPDATE_SEQ_WAIT_SUM;
      }
      break;
    case        UPDATE_SEQ_WAIT_SUM:
      seq = UpdateCmdParse(cmd);
      break;

      /*** erase *******************************/
    case	UPDATE_SEQ_ERASE_BLOCK:
      MainSetUpdateLedSpeed(1);
      val  = buf[0] << 8;
      val |= buf[1] << 0;
      sum = buf[0] ^ buf[1];

      DevFlashUnlockErase(0);

      seq = UPDATE_SEQ_ERASE_BLOCK_EXEC;
      if(val == 0xffff) {
        FLASH_PTR->PECR =               FLASH_PECR_ERASE_YES;
        lenRecv = 1;
        seq = UPDATE_SEQ_ERASE_BLOCK_SUM;
      } else if(val == 0xfffe) {
      } else if(val == 0xfffd) {
      } else if(val < 0xfff0) {    /*  adhoc adhoc adhoc */

        cntRecv = val + 1;
        lenRecv = 2;
      }
      break;
    case        UPDATE_SEQ_ERASE_BLOCK_EXEC:
      sum =  buf[0] ^ buf[1];
      val  = buf[0] << 8;
      val |= buf[1] << 0;

      re = DevFlashEraseSector(0, val, 0);

      lenRecv = 2;
      cntRecv--;
      if(cntRecv == 0) {
        seq = UPDATE_SEQ_ERASE_BLOCK_SUM;
        lenRecv = 1;
      }
      break;
    case        UPDATE_SEQ_ERASE_BLOCK_SUM:
      res = (re != DEVFLASH_ERRNO_SUCCESS ||   // re is stored in UPDATE_SEQ_ERASE_BLOCK_EXEC
        sum != buf[0])? STM32_RES_NACK: STM32_RES_ACK;

      UpdateSendResponse(res);
      seq = UPDATE_SEQ_IDLE;
      lenRecv = 1;
      MainSetUpdateLedSpeed(0);
      break;

      /*** read ********************************/
    case        UPDATE_SEQ_READ_ADDR:
      MainSetUpdateLedSpeed(1);
      addr = DevFlashAsm8toB32(buf);
      res = STM32_RES_ACK;
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) != buf[4]) res = STM32_RES_NACK;
      UpdateSendResponse(res);
      seq = UPDATE_SEQ_READ_LEN;
      lenRecv = 2;
      break;
    case        UPDATE_SEQ_READ_LEN:
      UpdateSendResponse(STM32_RES_ACK);
      len = buf[0] + 1;
      PeriSend((const unsigned char *)addr, len);
      seq = UPDATE_SEQ_IDLE;
      lenRecv = 1;
      break;
      /*** program *****************************/
    case        UPDATE_SEQ_PROG_ADDR:
      MainSetUpdateLedSpeed(1);
      addr = DevFlashAsm8toB32(buf) & ~0x3;
      res = STM32_RES_ACK;
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) != buf[4]) res = STM32_RES_NACK;
      UpdateSendResponse(res);
      seq = UPDATE_SEQ_PROG_LEN;
      lenRecv = 1;
      break;

    case        UPDATE_SEQ_PROG_LEN:
      lenData = buf[0];
      len = lenData + 1;
      seq = UPDATE_SEQ_PROG_DATA;
      lenRecv = len + 1;
      break;

    case        UPDATE_SEQ_PROG_DATA:
      xor = lenData;
      for(i = 0; i < lenRecv-1; i++) {
        xor ^= buf[i];
      }

      res = STM32_RES_ACK;
      if(xor != buf[lenRecv-1]) {
        res = STM32_RES_NACK;

      } else {
        DevFlashUnlockProgram(0);

#if UPDATE_PROGRAM_DOUBLE_WORD_EN         /* double word program */
        re = DevFlashProgramX64(0, addr, lenRecv-1, buf);

#elif UPDATE_PROGRAM_SINGLE_WORD_EN       /* single word program */
        re = DevFlashProgramX32(0, addr, lenRecv-1, buf);

#elif UPDATE_PROGRAM_HALF_WORD_EN         /* half-word program */
        re = DevFlashProgramX16(0, addr, lenRecv-1, buf);

#elif UPDATE_PROGRAM_BYTE_EN              /* byte program */
        re = DevFlashProgramX8 (0, addr, lenRecv-1, buf);

#elif UPDATE_PROGRAM_BURST_EN         /* burst program */
        re = UpdateProgX320(pAddr32, buf, lenRecv);
#endif

        if(re != DEVFLASH_ERRNO_SUCCESS) {
          res = STM32_RES_NACK;
        }

      }
      UpdateSendResponse(res);

      MainSetUpdateLedSpeed(0);

      seq = UPDATE_SEQ_IDLE;
      lenRecv = 1;
      break;

      /*** get crc ******************************/
    case	UPDATE_SEQ_GETCRC_ADDR:
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) == buf[4]) {
	res = STM32_RES_ACK;
	val = DevFlashAsm8toB32(buf);
	pAddr = (uint8_t *) val;
	lenRecv = 5;
	seq = UPDATE_SEQ_GETCRC_LEN;
      } else {
	res = STM32_RES_NACK;
	lenRecv = 5;
	seq = UPDATE_SEQ_IDLE;
      }
      UpdateSendResponse(res);
      break;
    case	UPDATE_SEQ_GETCRC_LEN:
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) == buf[4]) {
	res = STM32_RES_ACK;
	UpdateSendResponse(res);

	len  = DevFlashAsm8toB32(buf) + 0x7ffUL;
	len &= ~0x7ffUL;
	val = 0;
	val = Crc32Calc(val, (const uint8_t *)pAddr, len);

	DevFlashAsm32toB8(buf, val);
	buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
	PeriSend(buf, 5);
      } else {
	res = STM32_RES_NACK;
	UpdateSendResponse(res);
      }
      seq = UPDATE_SEQ_IDLE;
      lenRecv = 1;

      break;

    case	UPDATE_SEQ_GETINFO_NUM:
      {
	const uint8_t		*pInfo, **p;
	res = STM32_RES_ACK;
	if(buf[0] != (~buf[1] & 0xff)) {
	  res = STM32_RES_NACK;
	  UpdateSendResponse(res);
	  break;
	}
	switch(buf[0]) {
	case	1:			/* Product name */
	  pInfo = strProductText;
	  break;
	case	2:			/* Bootloader version number */
	  pInfo = strVersionText;
	  break;
	case	3:			/* Firmware version number */
	  p = (const uint8_t **)CONFIG_FIRMWARE_INFO_VERSION_POS;
	  if( ((uint32_t)*p & CONFIG_MEMORY_ROM_BASE_MASK) == CONFIG_MEMORY_ROM_BASE) {
	    pInfo = *p;
	    /*pInfo = systemVersion;*/
	  } else {
	    res = STM32_RES_NACK;
	  }
	  break;
	default:
	  res = STM32_RES_NACK;
	  break;
	}
	UpdateSendResponse(res);
	if(res == STM32_RES_ACK) {
	  len = strlen((char *)pInfo);
	  UpdateSendResponse(len);
	  PeriSend(pInfo, len);
	  UpdateSendResponse(STM32_RES_ACK);
	}

	seq = UPDATE_SEQ_IDLE;
	lenRecv = 1;
      }
      break;

      /*** check blank ******************************/
    case	UPDATE_SEQ_CHECKBLANK_ADDR:
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) == buf[4]) {
	res = STM32_RES_ACK;
	addr = DevFlashAsm8toB32(buf);
	lenRecv = 5;
	seq = UPDATE_SEQ_CHECKBLANK_LEN;
      } else {
	res = STM32_RES_NACK;
	lenRecv = 1;
	seq = UPDATE_SEQ_IDLE;
      }
      UpdateSendResponse(res);
      break;
    case	UPDATE_SEQ_CHECKBLANK_LEN:
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) == buf[4]) {
	res = STM32_RES_ACK;
	UpdateSendResponse(res);

	lenData = DevFlashAsm8toB32(buf);
	val = 0;
        pAddr = (volatile uint8_t *) addr;
	for(i = 0; i < lenData; i++) {
	  if(*pAddr++ != UPDATE_BLANKVALUE) {
	    val = 1;
	    break;
	  }
	}

	DevFlashAsm32toB8(buf, val);
	buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
	PeriSend(buf, 5);
      } else {
	res = STM32_RES_NACK;
	UpdateSendResponse(res);
      }
      seq = UPDATE_SEQ_IDLE;
      lenRecv = 1;

      break;

#if 0
    default:
      seq = UPDATE_SEQ_IDLE;
#endif
    }
  }

fail:
  return;
}


int
UpdateCmdParse(uint8_t cmd)
{
  int		seq = -1;

  uint8_t	res;
  int		len;

  UpdateSendResponse(STM32_RES_ACK);

  seq = UPDATE_SEQ_IDLE;

  switch(cmd) {
  case	STM32_CMD_ERASE:
    seq = UPDATE_SEQ_ERASE_BLOCK;
    lenRecv = 2;
    break;
  case	STM32_CMD_READ_MEMORY:
    seq = UPDATE_SEQ_READ_ADDR;
    lenRecv = 5;
    break;
  case	STM32_CMD_WRITE_MEMORY:
    seq = UPDATE_SEQ_PROG_ADDR;
    lenRecv = 5;
    break;

    /* version, id */
  case	STM32_CMD_GET:
    len = sizeof(cmdList);
    res = len;
    PeriSend(&res, 1);
    res = CONFIG_BOOTLOADER_CMD_VERSION;		/* version */
    PeriSend(&res, 1);
    PeriSend(cmdList, len);
    UpdateSendResponse(STM32_RES_ACK);
    break;
  case	STM32_CMD_GET_VERSION:
    len = sizeof(cmdVersion);
    PeriSend(cmdVersion, len);
    UpdateSendResponse(STM32_RES_ACK);
    break;
  case	STM32_CMD_GET_ID:
    res = 0x01;
    UpdateSendResponse(res);
    len = sizeof(cmdChipID);
    PeriSend(cmdChipID, len);
    UpdateSendResponse(STM32_RES_ACK);
    break;
  case	STM32_CMD_GO:
    /* never return */
    NVIC_SystemReset();
    break;

    /* extensions */
  case	STM32_CMD_GET_CRC:
    seq = UPDATE_SEQ_GETCRC_ADDR;
    lenRecv = 5;
    break;

  case	STM32_CMD_GET_INFO:
    seq = UPDATE_SEQ_GETINFO_NUM;
    lenRecv = 2;
    break;

  case	STM32_CMD_CHECK_BLANK:
    seq = UPDATE_SEQ_CHECKBLANK_ADDR;
    lenRecv = 5;
    break;
  }

  return seq;
}


static int
UpdateSendResponse(uint8_t res)
{
  return PeriSend(&res, 1);
}
