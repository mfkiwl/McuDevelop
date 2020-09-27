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

#ifndef _DEV_FLASH_H_
#define _DEV_FLASH_H_

#define DEVFLASH_ERRNO_SUCCESS          (0)
#define DEVFLASH_ERRNO_UNKNOWN          (-1)


int             DevFlashInit(int unit);

int             DevFlashUnlockErase(int unit);
int             DevFlashUnlockProgram(int unit);
int             DevFlashUnlockOption(int unit);

int             DevFlashErase(int unit, uint32_t addr, int size);
int             DevFlashEraseSector(int unit, uint32_t sect, int size);
int             DevFlashEraseChip(int unit);
int             DevFlashProgram(int unit, uint32_t addr, int size, uint8_t *pBuf);
int             DevFlashWrite32(int unit, uint32_t addr, uint32_t word);

int             DevFlashEraseEeprom(int unit, uint32_t addr, int size);

int             DevFlashProgramX64(int unit, uint32_t addr, int size, uint8_t *p);
int             DevFlashProgramX32(int unit, uint32_t addr, int size, uint8_t *p);
int             DevFlashProgramX16(int unit, uint32_t addr, int size, uint8_t *p);
int             DevFlashProgramX8 (int unit, uint32_t addr, int size, uint8_t *p);


uint16_t        DevFlashAsm8toL16(uint8_t *p);
uint16_t        DevFlashAsm8toB16(uint8_t *p);
uint32_t        DevFlashAsm8toL32(uint8_t *p);
uint32_t        DevFlashAsm8toB32(uint8_t *p);
void            DevFlashAsm32toB8(uint8_t *p, uint32_t val);
void            DevFlashAsm32toL8(uint8_t *p, uint32_t val);



#ifdef  _DEV_FLASH_C_

/* prototypes */



uint16_t
DevFlashAsm8toL16(uint8_t *p)
{
  uint16_t	val;
  val  = (*p++ <<  0);
  val |= (*p++ <<  8);
  return val;
}
uint16_t
DevFlashAsm8toB16(uint8_t *p)
{
  uint16_t	val;
  val |= (*p++ <<  8);
  val |= (*p   <<  0);
  return val;
}
uint32_t
DevFlashAsm8toL32(uint8_t *p)
{
  uint32_t	val;
  val  = (*p++ <<  0);
  val |= (*p++ <<  8);
  val |= (*p++ << 16);
  val |= (*p   << 24);
  return val;
}
uint32_t
DevFlashAsm8toB32(uint8_t *p)
{
  uint32_t	val;
  val  = (*p++ << 24);
  val |= (*p++ << 16);
  val |= (*p++ <<  8);
  val |= (*p   <<  0);
  return val;
}
void
DevFlashAsm32toB8(uint8_t *p, uint32_t val)
{
  p[0] = val >> 24;
  p[1] = val >> 16;
  p[2] = val >>  8;
  p[3] = val >>  0;
  return;
}
#if 0
static void
DevFlashAsm32toL8(uint8_t *p, uint32_t val)
{
  p[0] = val >>  0;
  p[1] = val >>  8;
  p[2] = val >> 16;
  p[3] = val >> 24;
  return;
}
#endif

#endif

#endif
