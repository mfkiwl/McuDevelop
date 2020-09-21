#ifndef	_UPDATE_H_
#define	_UPDATE_H_
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


enum STM32_CMD {
  STM32_CMD_SYNC		= 0x7f,
  STM32_CMD_GET			= 0x00,
  STM32_CMD_GET_VERSION		= 0x01,
  STM32_CMD_GET_ID		= 0x02,
  STM32_CMD_READ_MEMORY		= 0x11,
  STM32_CMD_GO			= 0x21,
  STM32_CMD_WRITE_MEMORY	= 0x31,
  STM32_CMD_ERASE		= 0x44,
  STM32_CMD_WRITE_PROTECT	= 0x63,
  STM32_CMD_WRITE_UNPROTECT	= 0x73,
  STM32_CMD_READOUT_PROTECT	= 0x82,
  STM32_CMD_READOUT_UNPROTECT	= 0x92,

  STM32_CMD_WRITE_MEMORY_NO_STRETCH	= 0x32,
  STM32_CMD_ERASE_NO_STRETCH		= 0x45,
  STM32_CMD_WRITE_PROTECT_NO_STRETCH	= 0x64,
  STM32_CMD_WRITE_UNPROTECT_NO_STRETCH	= 0x74,
  STM32_CMD_READOUT_PROTECT_NO_STRETCH	= 0x83,
  STM32_CMD_READOUT_UNPROTECT_NO_STRETCH	= 0x93,

  /* special extensions */
  STM32_CMD_CHECK_BLANK		= 0x0d,
  STM32_CMD_GET_INFO		= 0x0e,
  STM32_CMD_GET_CRC		= 0x0f,
};
enum STM32_RES {
  STM32_RES_ACK			= 0x79,
  STM32_RES_NACK		= 0x1f,
};
  
  


void		UpdateInit(void);
int		UpdateSearchInterface(void);
void		UpdateLoop(void);

int		UpdateCmdParse(uint8_t cmd);

#ifdef	_UPDATE_C_
static uint16_t UpdateAsm8toL16(uint8_t *p);
static uint16_t UpdateAsm8toB16(uint8_t *p);
static uint32_t	UpdateAsm8toL32(uint8_t *p);
static uint32_t	UpdateAsm8toB32(uint8_t *p);
/*static void	UpdateAsm32toL8(uint8_t *p, uint32_t val);*/
static void	UpdateAsm32toB8(uint8_t *p, uint32_t val);
static int	UpdateSendResponse(uint8_t res);

static int      UpdateProgX64(volatile uint32_t *pAddr32, uint8_t *p, int len);
static int      UpdateProgX32(volatile uint32_t *pAddr32, uint8_t *p, int len);
static int      UpdateProgX16(volatile uint16_t *pAddr,   uint8_t *p, int len);
static int      UpdateProgX8 (volatile uint8_t  *pAddr,   uint8_t *p, int len);

#endif

#endif
