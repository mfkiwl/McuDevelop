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

#define	_BOOT1_C_

#include	<stdint.h>
#include	"config.h"
#include	"crc32.h"
#include	"system.h"

#ifdef	COMPILER_GCC
#define	__ATTR_SECTION	__attribute__((section(".text2")))
#define	__ATTR_INTR	__attribute__((interrupt("Boot1SysTickIntr")))
#endif

#ifdef	COMPILER_IAR
#define	__ATTR_SECTION 
#define	__ATTR_INTR
#endif

struct _stFirmInfo {
  uint32_t	crc;
  uint32_t	size;
  uint32_t	dummy0;
  uint32_t	dummy1;
};

const char	strProductText[] = CONFIG_PRODUCT_NAME_TEXT;
const char	strVersionText[] = CONFIG_VERSION_BOOT_TEXT;

static uint32_t	boot1Cnt1ms = 1;
uint32_t Src,SrcEnd, Dest;

__ATTR_SECTION static void
Boot1Increment1ms(void)
{
  if(SysTick->CTRL & (1<<SysTick_CTRL_COUNTFLAG_Pos)) {
    boot1Cnt1ms--;
  }
  return;
}
__ATTR_SECTION uint32_t
Boot1GetCounter(void)
{
  return boot1Cnt1ms;
}

__ATTR_SECTION int
Boot1IsFirmCrcCorrect(void)
{
  int			result = 0;
  const uint8_t		*ptr;
  const uint8_t		*pFirm;
  int			len;
  int			size;
  uint32_t		crc;
  const static uint8_t	buf[] = {
    0xff, 0xff, 0xff, 0xff
  };
  const struct _stFirmInfo	*pFirmInfo;

  pFirmInfo = (struct _stFirmInfo *) (CONFIG_FIRMWARE_INFO_TOP);

  pFirm = (uint8_t *) CONFIG_FIRMWARE_TOP;
  size  = pFirmInfo->size;
  if(((unsigned long)pFirm & 3) ||
     (unsigned long) size > (CONFIG_MEMORY_ROM_MAINFIRM_SIZE)) goto fail;

  crc = 0;
  ptr = pFirm;
  len = (CONFIG_FIRMWARE_INFO_TOP) - (CONFIG_FIRMWARE_VECTOR_TOP);
  crc = Crc32Calc(crc, ptr, len);

  ptr = buf;
  len = 4;
  crc = Crc32Calc(crc, ptr, len);

  ptr = pFirm + ((CONFIG_FIRMWARE_INFO_SIZE_POS) - (CONFIG_FIRMWARE_VECTOR_TOP));
  len = size  - ((CONFIG_FIRMWARE_INFO_SIZE_POS) - (CONFIG_FIRMWARE_VECTOR_TOP));
  crc = Crc32Calc(crc, ptr, len);

  result = (crc == pFirmInfo->crc)? 1: 0;

fail:
  return result;
}


__ATTR_SECTION void
Boot1Jump(uint32_t sp, uint32_t pc)
{
  void	(*func)(void);

  __disable_irq();

  func = (void (*)(void))pc;
  (*func)();			/* never return */

  while(1);
}


__ATTR_SECTION void
MainEntry(int forceUpdate)
{
  uint32_t		*pSrc, *pSrcEnd, *pDest;
  register uint32_t	sp, pc;
  int			isPowerSwPushed;
  uint32_t		tout;

  /* init the systick counter */

  SystemInit();

  SystemGpioInit();

  if(!forceUpdate) {
    /************************************************
     *  Jump to the main firmware entry, if exist and crc is correct
     */
    pSrc    = (uint32_t *) CONFIG_FIRMWARE_VECTOR_TOP;
    sp = *(pSrc + 0);
    pc = *(pSrc + 1);

    if(sp != FLASH_ERASED_VALUE32 &&
       pc != FLASH_ERASED_VALUE32 &&
       !(sp & 3) &&
       (pc & 0xfff00000) == POR_PC_ENTRY_BASE) {
      if(Boot1IsFirmCrcCorrect()) {
	tout = Boot1GetCounter();
	while((tout - Boot1GetCounter()) < (CONFIG_FIRMUPDATE_POWERSW_TIMEOUT)) {
	  Boot1Increment1ms();
	  /* reset WDT */
	  SystemWdtClear();

	  isPowerSwPushed = SystemGpioGetPowerSw();
	  if(!isPowerSwPushed) {
	    SystemGpioSetPowerLedOff();
	    Boot1Jump(sp, pc);		/* never return */
	  } else {
	    SystemGpioSetPowerLedOn();
	  }
	}
      }
    }
  }

  /* POWER_LED = off, UPDATE_LED = on */
  SystemGpioSetPowerLedOff();
  SystemGpioSetUpdateLedOn();

  /************************************************
   *  Copy the updater to ram and jump to updater entry
   */
  pSrc    = (uint32_t *)  CONFIG_MEMORY_ROM_BOOT2_START;
  pSrcEnd = (uint32_t *) (CONFIG_MEMORY_ROM_BOOT2_END+1);
  pDest   = (uint32_t *)  CONFIG_MEMORY_RAM_BOOT2_START;

  Src    = (uint32_t )  CONFIG_MEMORY_ROM_BOOT2_START;
  SrcEnd = (uint32_t ) (CONFIG_MEMORY_ROM_BOOT2_END+1);
  Dest   = (uint32_t )  CONFIG_MEMORY_RAM_BOOT2_START;


  sp = *(pSrc + 0);
  pc = *(pSrc + 1);
  if(sp != FLASH_ERASED_VALUE32 && pc != FLASH_ERASED_VALUE32) {

    while(pSrc != pSrcEnd) {
      *pDest++ = *pSrc++;
    }

    Boot1Jump(sp, pc);          /* never return */
  }

  /************************************************
   *  Updater is not exist,  halt
   */
  while(1);
}


__ATTR_SECTION __ATTR_INTR void
SystemSysTickIntr(void)
{
  Boot1Increment1ms();

  return;
}


void
IntrException(void)
{
  int		i;
  int		num;

#define	INTR_NVIC_INTSTATUS_REG		((unsigned long *) 0xe000ed04)
#define		NVIC_INTSTATUS_VECTACTIVE_SHIFT	(0)
#define		NVIC_INTSTATUS_VECTACTIVE_MASK	(0x1ff << (NVIC_INTSTATUS_VECTACTIVE_SHIFT))

  num = *INTR_NVIC_INTSTATUS_REG & NVIC_INTSTATUS_VECTACTIVE_MASK;

  while(1) {
    for(i = 0; i< num; i++) {
      SystemGpioSetPowerLedOn();
      SystemGpioSetUpdateLedOn();
      SystemGpioSetPowerLedOff();
      SystemGpioSetUpdateLedOff();
    }
    for(i = 0; i< 10; i++) {
      SystemGpioSetPowerLedOff();
      SystemGpioSetUpdateLedOff();
    }
  }
}
