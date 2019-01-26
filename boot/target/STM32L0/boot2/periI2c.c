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

#define _PERII2C_C_

#include        <stdint.h>

#include        "config.h"
#include        "system.h"
#include        "gpio.h"

#include        "peri.h"
#include        "periI2c.h"

#define	PERII2C_UNIT_MAX               3

extern struct _stPeri   peri;

static stm32Dev_I2C     *pu[3] = {
  I2C1_PTR, I2C1_PTR, I2C2_PTR
};
const static uint16_t	gpioInitScl[] = {
  PORT_CTRL(GPIO_I2C1_SCL, GPIO_MODE_FUNC, GPIO_PULL_OD, GPIO_FUNC(7)),
  PORT_CTRL(GPIO_I2C2_SCL, GPIO_MODE_FUNC, GPIO_PULL_OD, GPIO_FUNC(7)),
};
const static uint16_t	gpioInitSda[] = {
  PORT_CTRL(GPIO_I2C1_SDA, GPIO_MODE_FUNC, GPIO_PULL_OD, GPIO_FUNC(7)),
  PORT_CTRL(GPIO_I2C2_SDA, GPIO_MODE_FUNC, GPIO_PULL_OD, GPIO_FUNC(7)),
};
const static uint16_t	gpioUninitScl[] = {
  PORT_CTRL(GPIO_I2C1_SCL, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_I2C2_SCL, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
};
const static uint16_t	gpioUninitSda[] = {
  PORT_CTRL(GPIO_I2C1_SDA, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
  PORT_CTRL(GPIO_I2C2_SDA, GPIO_MODE_INPUT, GPIO_PULL_NONE, 0),
};


int
PeriI2cInit(int unit)
{
  int				result = PERI_ERROR_UNKNOWN;
  uint32_t			val;
  stm32Dev_I2C                  *u;

  if(unit >= PERII2C_UNIT_MAX) goto fail;

  u = pu[unit];

  /* gpio function setting */
  SystemGpioSet(gpioInitScl[unit]);
  SystemGpioSet(gpioInitSda[unit]);

  /* i2c setting */
  u->OAR1 = I2C_OAR1_OA1EN_YES |
    I2C_OAR1_OA1_7BIT_VAL(CONFIG_BOOTLOADER_PERI_I2C_ADDR);
  u->CR2 = I2C_CR2_AUTOEND_YES | I2C_CR2_NACK;
  u->CR1 |= I2C_CR1_GCEN_YES | I2C_CR1_NOSTRETCH_YES;

  /* enable */
  u->CR1 |= I2C_CR1_PE_YES;

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriI2cUninit(int unit)
{
  int		result = PERI_ERROR_UNKNOWN;

  stm32Dev_I2C                  *u;
  uint16_t			gpio;

  if(unit >= PERII2C_UNIT_MAX) goto fail;

  u = pu[unit];
  u->CR1 = 0;				/* disable I2C */
  u->CR2 = 0;
  u->OAR1 = 0;

  SystemGpioSet(gpioUninitScl[unit]);
  SystemGpioSet(gpioUninitSda[unit]);

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriI2cProbe(int unit)
{
  int		result = PERI_ERROR_UNKNOWN;
  char		ch;
  int		len;

  if(unit >= PERII2C_UNIT_MAX) goto fail;

  len = sizeof(ch);

  PeriI2cRecv(unit, &ch, &len);
  if(len >= 1) {
    result = PERI_SUCCESS;
  }
  
fail:
  return result;
}


int
PeriI2cSend(int unit, const uint8_t *ptr, int len)
{
  int		result = PERI_ERROR_UNKNOWN;
  int		i;
  stm32Dev_I2C                  *u;

  if(unit >= PERII2C_UNIT_MAX) goto fail;

  u = pu[unit];

  for(i = 0; i < len; i++) {
    while(!(u->ISR & I2C_ISR_TXE_MASK));
    u->TXDR = ptr[i];
  }

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriI2cRecv(int unit, uint8_t *ptr, int *len)
{
  int           result = PERI_ERROR_UNKNOWN;
  int           i = 0;
  int           t = 0;

  stm32Dev_I2C  *u;
  int           sz;

  if(unit >= PERII2C_UNIT_MAX) goto fail;

  *len = 0;

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriI2cIoctl(int unit, int req, void *ptr, int len)
{
  int		result = PERI_ERROR_UNKNOWN;
  uint32_t	*pL;

  if(!ptr || len < 0) goto fail;

  if(unit >= PERII2C_UNIT_MAX) goto fail;

  pL = (uint32_t *) ptr;

  switch(req) {
  case	PERII2C_GET_RECV_SIZE:
    *pL = PERI_RX_BUF_SIZE - 0;       /* adhoc adhoc */
    break;
  }

  result = PERI_SUCCESS;
fail:
  return result;
}
