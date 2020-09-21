#define _DEV_FLASHF730_C_
#define _DEV_FLASH_C_

#include        <stdint.h>

#include        "config.h"
#include        "system.h"
#include        "devFlash.h"


int
DevFlashUnlockErase(int unit)
{
  int                   result = DEVFLASH_ERRNO_SUCCESS;

  while(FLASH_PTR->SR & FLASH_SR_BSY_MASK);
  FLASH_PTR->SR = FLASH_PTR->SR;

  if(FLASH_PTR->PECR & FLASH_PECR_PELOCK_MASK) {
    FLASH_PTR->KEYR = FLASH_PKEY1;
    FLASH_PTR->KEYR = FLASH_PKEY2;
  }

  return result;
}


int
DevFlashUnlockProgram(int unit)
{
  int                   result = DEVFLASH_ERRNO_SUCCESS;

  while(FLASH_PTR->SR & FLASH_SR_BSY_MASK);
  FLASH_PTR->SR = FLASH_PTR->SR;

  if(FLASH_PTR->PECR & FLASH_PECR_PRGLOCK_MASK) {
    FLASH_PTR->KEYR = FLASH_PRGKEY1;
    FLASH_PTR->KEYR = FLASH_PRGKEY2;
  }

  return result;
}


int
DevFlashUnlockOption(int unit)
{
  int                   result = DEVFLASH_ERRNO_SUCCESS;

  while(FLASH_PTR->SR & FLASH_SR_BSY_MASK);
  FLASH_PTR->SR = FLASH_PTR->SR;

  if(FLASH_PTR->PECR & FLASH_PECR_OPTLOCK_MASK) {
    FLASH_PTR->KEYR = FLASH_OPTKEY1;
    FLASH_PTR->KEYR = FLASH_OPTKEY2;
  }

  return result;
}


int
DevFlashEraseSector(int unit, uint32_t sect, int size)
{
  int                   result = DEVFLASH_ERRNO_SUCCESS;
  __IO uint32_t         *pAddr32;
  int                   remain;

  FLASH_PTR->PECR = FLASH_PECR_SNB_SECT(sect) | FLASH_PECR_SER_YES;
  FLASH_PTR->PECR |= FLASH_PECR_STRT_YES;

  while(FLASH_PTR->SR & FLASH_SR_BSY_MASK);

  FLASH_PTR->PECR = 0;

  return result;
}


int
DevFlashProgramX64(int unit, uint32_t addr, int size, uint8_t *p)
{
  int                   result = DEVFLASH_ERRNO_SUCCESS;
  int                   i;
  uint32_t              val, val2;
  volatile uint32_t     *pAddr;

  FLASH_PTR->PECR = FLASH_PECR_PSIZE_X64 | FLASH_PECR_PROG_YES;

  for(i = 0; i < size; i += 2*sizeof(uint32_t)) {

    if(FLASH_PTR->SR & (FLASH_SR_OPERR_MASK | FLASH_SR_WRPERR_MASK | FLASH_SR_PGAERR_MASK | FLASH_SR_PGPERR_MASK)) {
      result = DEVFLASH_ERRNO_UNKNOWN;
      break;
    }
    if(FLASH_PTR->SR & FLASH_SR_EOP_MASK) {
      FLASH_PTR->SR |= FLASH_SR_EOP_MASK;
    }

    val = DevFlashAsm8toL32(p);
    p += 4;
    val2 = DevFlashAsm8toL32(p);
    p += 4;

    *pAddr     = val;
    *(pAddr+1) = val2;
    __DSB();

    pAddr += 2;
  }
  FLASH_PTR->PECR = 0;

  return result;
}


int
DevFlashProgramX32(int unit, uint32_t addr, int size, uint8_t *p)
{
  int                   result = DEVFLASH_ERRNO_SUCCESS;
  int                   i;
  uint32_t              val;
  volatile uint32_t     *pAddr;

  pAddr = (volatile uint32_t *) addr;

  FLASH_PTR->PECR = FLASH_PECR_PSIZE_X32 | FLASH_PECR_PROG_YES;

  for(i = 0; i < size; i += sizeof(uint32_t)) {

    if(FLASH_PTR->SR & (FLASH_SR_OPERR_MASK | FLASH_SR_WRPERR_MASK | FLASH_SR_PGAERR_MASK | FLASH_SR_PGPERR_MASK)) {
      result = DEVFLASH_ERRNO_UNKNOWN;
      break;
    }
    if(FLASH_PTR->SR & FLASH_SR_EOP_MASK) {
      FLASH_PTR->SR |= FLASH_SR_EOP_MASK;
    }

    val = DevFlashAsm8toL32(p);
    *pAddr = val;
    __DSB();

    p += 4;
    pAddr++;
  }

  FLASH_PTR->PECR = 0;

  return result;
}


int
DevFlashProgramX16(int unit, uint32_t addr, int size, uint8_t *p)
{
  int                   i;
  int                   result = DEVFLASH_ERRNO_SUCCESS;
  uint32_t              val;
  volatile uint16_t     *pAddr;

  pAddr = (volatile uint16_t *) addr;

  FLASH_PTR->PECR = FLASH_PECR_PSIZE_X16 | FLASH_PECR_PROG_YES;

  for(i = 0; i < size; i += sizeof(uint16_t)) {

    if(FLASH_PTR->SR & (FLASH_SR_OPERR_MASK | FLASH_SR_WRPERR_MASK | FLASH_SR_PGAERR_MASK | FLASH_SR_PGPERR_MASK)) {
      result = DEVFLASH_ERRNO_UNKNOWN;
      break;
    }
    if(FLASH_PTR->SR & FLASH_SR_EOP_MASK) {
      FLASH_PTR->SR |= FLASH_SR_EOP_MASK;
    }

    val = DevFlashAsm8toL16(p);
    *pAddr = val;
    __DSB();

    p += sizeof(uint16_t);
    pAddr++;
  }

  FLASH_PTR->PECR = 0;

  return result;
}


int
DevFlashProgramX8(int unit, uint32_t addr, int size, uint8_t *p)
{
  int                   i;
  int                   result = DEVFLASH_ERRNO_SUCCESS;
  uint32_t              val;
  volatile uint8_t      *pAddr;

  pAddr = (volatile uint8_t *) addr;

  FLASH_PTR->PECR = FLASH_PECR_PSIZE_X8 | FLASH_PECR_PROG_YES;

  for(i = 0; i < size; i += sizeof(uint8_t)) {

    if(FLASH_PTR->SR & (FLASH_SR_OPERR_MASK | FLASH_SR_WRPERR_MASK | FLASH_SR_PGAERR_MASK | FLASH_SR_PGPERR_MASK)) {
      result = DEVFLASH_ERRNO_UNKNOWN;
      break;
    }
    if(FLASH_PTR->SR & FLASH_SR_EOP_MASK) {
      FLASH_PTR->SR |= FLASH_SR_EOP_MASK;
    }

    *pAddr = *p;
    __DSB();

    p += sizeof(uint8_t);
    pAddr++;
  }

  FLASH_PTR->PECR = 0;

  return result;
}


#if 0
static int
DevFlashProgramX32C16(volatile uint32_t *pAddr32, uint8_t *buf, int len)
{
  int           i;
  uint8_t       *p;
  int           res = STM32_RES_ACK;
  uint32_t      val;

  FLASH_PTR->PECR = FLASH_PECR_PROG_YES | FLASH_PECR_FPRG_YES;

  p = buf;
  for(i = 0; i < len; i += 0x40) {
    while(FLASH_PTR->SR & FLASH_SR_BSY_MASK);
    FLASH_PTR->SR = FLASH_PTR->SR;
    /* write the first word */
    val = DevFlashAsm8toL32(p);
    p += 4;
    *pAddr32++ = val;

    for(int j = 0; j < 0x40/4-1; j++) {
      val = DevFlashAsm8toL32(p);
      p += 4;
      *pAddr32 = val;
    }
    pAddr32 += 15;

  }
  FLASH_PTR->PECR = 0;

  return res;
}
#endif








