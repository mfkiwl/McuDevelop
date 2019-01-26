#define	_UPDATE_C_
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


#include	<stdint.h>
#include	<string.h>

#include	"config.h"
#include	"system.h"

#include	"crc32.h"
#include	"peri.h"
#include	"main.h"

#include	"update.h"

extern const uint8_t	systemVersion[];
extern const uint8_t	systemProduct[];

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

int		seq = UPDATE_SEQ_IDLE;
uint8_t		cmd, /*lenData, */sum;
int		lenRecv;
uint32_t	addr;
uint8_t		*pAddr;
uint32_t	*pAddr32;
uint64_t	*pAddr64;
int		lenData;
int		cntRecv;

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
  uint64_t	buf64[260/4];
  uint8_t	*buf, *p;
  uint8_t	c, res;
  int		len;
  uint32_t	val;
  uint64_t	valLL;
  int		i, cnt;
  uint8_t	xor = 0;

  buf = (uint8_t *)buf64;

  if(PeriGetRecvSize() >= lenRecv) {
    len = lenRecv;
    PeriRecv(buf, &len);
    c = buf[0];

    switch(seq) {
    case	UPDATE_SEQ_IDLE:
      if(c == STM32_CMD_SYNC) {
	UpdateSendResponse(STM32_RES_ACK);
      } else {
	cmd = c;
	seq = UPDATE_SEQ_WAIT_SUM;
      }
      break;
    case	UPDATE_SEQ_WAIT_SUM:
      seq = UpdateCmdParse(cmd);
      break;

      /*** erase *******************************/
    case	UPDATE_SEQ_ERASE_BLOCK:
      MainSetUpdateLedSpeed(1);
      val  = buf[0] << 8;
      val |= buf[1] << 0;
      sum = buf[0] ^ buf[1];

      while(FLASH->SR & FLASH_SR_BSY_Msk);
      FLASH->SR = FLASH->SR;
      if(FLASH->CR & CR_LOCK_MASK) {
	FLASH->KEYR = KEYR_KEY1;
	FLASH->KEYR = KEYR_KEY2;
      }

      seq = UPDATE_SEQ_ERASE_BLOCK_EXEC;
      if(val == 0xffff) {
	FLASH->CR =               CR_MER1_YES;
	FLASH->CR = CR_STRT_YES | CR_MER1_YES;
	lenRecv = 1;
	seq = UPDATE_SEQ_ERASE_BLOCK_SUM;
      } else if(val == 0xfffe) {
      } else if(val == 0xfffd) {
      } else if(val < 0xfff0) {    /*  adhoc adhoc adhoc */
	cntRecv = val + 1;
	lenRecv = 2;
      }
      while(FLASH->SR & FLASH_SR_BSY_Msk);
      break;
    case	UPDATE_SEQ_ERASE_BLOCK_EXEC:
      if(!(FLASH->SR & SR_PGSERR_MASK)) {
	sum = buf[0] ^ buf[1];
	val  = buf[0] << 8;
	val |= buf[1] << 0;
	while(FLASH->SR & FLASH_SR_BSY_Msk);
	FLASH->CR  = CR_PER_YES | CR_PNB_SECT(val);
	FLASH->CR |= FLASH_CR_STRT;
	while(FLASH->SR & FLASH_SR_BSY_Msk);
      }
      lenRecv = 2;
      cntRecv--;
      if(cntRecv == 0) {
	seq = UPDATE_SEQ_ERASE_BLOCK_SUM;
	lenRecv = 1;
      }
      break;
    case	UPDATE_SEQ_ERASE_BLOCK_SUM:
      while(FLASH->SR & FLASH_SR_BSY_Msk);
      res = ((FLASH->SR & SR_PGSERR_MASK) ||
	     sum != buf[0])? STM32_RES_NACK: STM32_RES_ACK;
      UpdateSendResponse(res);
      seq = UPDATE_SEQ_IDLE;
      lenRecv = 1;
      MainSetUpdateLedSpeed(0);
      break;

      /*** read ********************************/
    case	UPDATE_SEQ_READ_ADDR:
      val = UpdateAsm8toB32(buf);
      res = STM32_RES_ACK;
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) != buf[4]) res = STM32_RES_NACK;
      UpdateSendResponse(res);
      seq = UPDATE_SEQ_READ_LEN;
      lenRecv = 2;
      pAddr = (uint8_t *) val;
      break;
    case	UPDATE_SEQ_READ_LEN:
      UpdateSendResponse(STM32_RES_ACK);
      len = buf[0] + 1;
      PeriSend(pAddr, len);
      seq = UPDATE_SEQ_IDLE;
      lenRecv = 1;
      break;
      /*** program *****************************/
    case	UPDATE_SEQ_PROG_ADDR:
      MainSetUpdateLedSpeed(1);
      val = UpdateAsm8toB32(buf);
      res = STM32_RES_ACK;
      if((buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) != buf[4]) res = STM32_RES_NACK;
      UpdateSendResponse(res);
      seq = UPDATE_SEQ_PROG_LEN;
      addr = val;
      pAddr32 = (uint32_t *) (val & ~0x3);
      pAddr64 = (uint64_t *) (val & ~0x7);
      lenRecv = 1;
      break;

    case	UPDATE_SEQ_PROG_LEN:
      lenData = buf[0];
      len = lenData + 1;
      seq = UPDATE_SEQ_PROG_DATA;
      lenRecv = len + 1;
      break;

    case	UPDATE_SEQ_PROG_DATA:
      xor = lenData;
      for(i = 0; i < lenRecv-1; i++) {
	xor ^= buf[i];
      }
      res = STM32_RES_NACK;
      if(xor == buf[lenRecv-1]) {
	while(FLASH->SR & FLASH_SR_BSY_Msk);
	FLASH->SR = FLASH->SR;
	if(FLASH->CR & FLASH_CR_LOCK_Msk) {
	  FLASH->KEYR = KEYR_KEY1;
	  FLASH->KEYR = KEYR_KEY2;
	}

	memset(buf+lenRecv-1, 0xff, 0x100-(lenRecv-1));
	switch(2) {
	case 0: UpdateProgram128Halfword(addr, buf64); break;	/* 2.1V -- 2.7V */
	case 1: UpdateProgram256Byte(addr, buf64); break;	/* 1.8V -- 2.1V */
	case 2: UpdateProgram64Word(addr, buf64); break;	/* 2.7V -- 3.6V */
	case 3: UpdateProgram32DWord(addr, buf64); break;	/* 2.7V -- 3.6V */
	}
	res = STM32_RES_ACK;
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
	val = UpdateAsm8toB32(buf);
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

	len  = UpdateAsm8toB32(buf) + 0x7ffUL;
	len &= ~0x7ffUL;
	val = 0;
	val = Crc32Calc(val, (const uint8_t *)pAddr, len);

	UpdateAsm32toB8(buf, val);
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
	uint32_t		val;
	res = STM32_RES_ACK;
	if(buf[0] != (~buf[1] & 0xff)) {
	  res = STM32_RES_NACK;
	  UpdateSendResponse(res);
	  break;
	}
	switch(buf[0]) {
	case	BOOT_PRODUCT:		/* Product name */
	  pInfo = systemProduct;
	  break;
	case	BOOT_VERSION:		/* Bootloader version number */
	  pInfo = systemVersion;
	  break;
	case	FIRM_VERSION:		/* Firmware version number */
	  p = (const uint8_t **)CONFIG_FIRMWARE_INFO_VERSION_POS;
	  val = (uint32_t)*p;
	  if(val >= CONFIG_MEMORY_ROM_MAINFIRM_START &&
	     val < CONFIG_MEMORY_ROM_MAINFIRM_END) {
	    pInfo = *p;
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
	val = UpdateAsm8toB32(buf);
	pAddr = (uint8_t *) val;
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

	lenData = UpdateAsm8toB32(buf);
	val = 0;
	for(i = 0; i < lenData; i++) {
	  if(*pAddr++ != 0xff) {
	    val = 1;
	    break;
	  }
	}

	UpdateAsm32toB8(buf, val);
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
    /*NVIC_SystemReset();*/
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




static uint32_t
UpdateAsm8toL32(uint8_t *p)
{
  uint32_t	val;
  val  = (*p++ <<  0);
  val |= (*p++ <<  8);
  val |= (*p++ << 16);
  val |= (*p   << 24);
  return val;
}
static uint32_t
UpdateAsm8toB32(uint8_t *p)
{
  uint32_t	val;
  val  = (*p++ << 24);
  val |= (*p++ << 16);
  val |= (*p++ <<  8);
  val |= (*p   <<  0);
  return val;
}
static void
UpdateAsm32toB8(uint8_t *p, uint32_t val)
{
  p[0] = val >> 24;
  p[1] = val >> 16;
  p[2] = val >>  8;
  p[3] = val >>  0;
  return;
}
#if 0
static void
UpdateAsm32toL8(uint8_t *p, uint32_t val)
{
  p[0] = val >>  0;
  p[1] = val >>  8;
  p[2] = val >> 16;
  p[3] = val >> 24;
  return;
}
#endif

static int
UpdateSendResponse(uint8_t res)
{
  return PeriSend(&res, 1);
}



static int
UpdateProgram64Word(uint32_t addr, uint32_t *buf)
{
  int		result = 0;
  int		i;
  uint32_t	*pDest, *pSrc;

  pSrc  = (uint32_t *) buf;
  pDest = (uint32_t *) (addr & ~3);

  FLASH->CR = (2 << FLASH_CR_PSIZE_Pos);

  for(i = 0; i < 64; i++) {
    while(FLASH->SR & FLASH_SR_BSY_Msk);
    FLASH->CR |= FLASH_CR_PG;
    *pDest++ = *pSrc++;
    __DSB();
    FLASH->CR &= ~FLASH_CR_PG;
  }

  return result;
}


#if 0
/*  this codes does not work yet */
static int
UpdateProgram32DWord(uint32_t addr, uint64_t *buf)
{
  int		result = 0;

  int		i;
  volatile uint32_t	*pDest, *pSrc;

  pSrc  = (uint32_t *) buf;
  pDest = (uint32_t *) (addr & ~7);

  FLASH->CR = (3 << FLASH_CR_PSIZE_Pos);

  for(i = 0; i < 32; i++) {
    while(FLASH->SR & FLASH_SR_BSY_Msk);
    FLASH->CR |= FLASH_CR_PG;
    *pDest++ = *pSrc++;
    *pDest++ = *pSrc++;
    __DSB();
    FLASH->CR &= ~FLASH_CR_PG;
    buf++;
  }

  return result;
}
#endif
