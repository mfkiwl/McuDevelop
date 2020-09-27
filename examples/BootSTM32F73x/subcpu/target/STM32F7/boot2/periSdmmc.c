/*
 * Copyright (c) 2020 zhtlab
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

#define _PERISDMMC_C_

#include        <stdint.h>
#include        <string.h>

#include        "config.h"
#include        "system.h"
#include        "gpio.h"

#include        "peri.h"
#include        "updateSdmmc.h"
#include        "periSdmmc.h"

extern struct _stPeri   peri;

#define PERISDMMC_UNIT_MAX  (SDMMC_NUM_MAX)

static stm32Dev_SDMMC *const pu[PERISDMMC_UNIT_MAX] = {
#ifdef SDMMC0_PTR
  SDMMC0_PTR,
#endif
#ifdef SDMMC1_PTR
  SDMMC1_PTR,
#endif
};
const static uint16_t   gpioInitSdmmc1[] = {
  PORT_CTRL(GPIO_SDMMC_CLK,     GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC(12)),
  PORT_CTRL(GPIO_SDMMC_CMD,     GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC(12)),
  PORT_CTRL(GPIO_SDMMC_D0,      GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC(12)),
  PORT_CTRL(GPIO_SDMMC_D1,      GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC(12)),
  PORT_CTRL(GPIO_SDMMC_D2,      GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC(12)),
  PORT_CTRL(GPIO_SDMMC_D3,      GPIO_MODE_FUNC,   GPIO_PULL_NONE, GPIO_FUNC(12)),
  PORT_CTRL(GPIO_SDMMC_CDETX,   GPIO_MODE_INPUT,  GPIO_PULL_UP,   GPIO_VAL_0),

  PORT_CTRL_END,
};
const static uint16_t   gpioUninitSdmmc1[] = {
  PORT_CTRL(GPIO_SDMMC_CLK,     GPIO_MODE_INPUT,  GPIO_PULL_NONE, GPIO_VAL_0),
  PORT_CTRL(GPIO_SDMMC_CMD,     GPIO_MODE_INPUT,  GPIO_PULL_NONE, GPIO_VAL_0),
  PORT_CTRL(GPIO_SDMMC_D0,      GPIO_MODE_INPUT,  GPIO_PULL_NONE, GPIO_VAL_0),
  PORT_CTRL(GPIO_SDMMC_D1,      GPIO_MODE_INPUT,  GPIO_PULL_NONE, GPIO_VAL_0),
  PORT_CTRL(GPIO_SDMMC_D2,      GPIO_MODE_INPUT,  GPIO_PULL_NONE, GPIO_VAL_0),
  PORT_CTRL(GPIO_SDMMC_D3,      GPIO_MODE_INPUT,  GPIO_PULL_NONE, GPIO_VAL_0),
  PORT_CTRL(GPIO_SDMMC_CDETX,   GPIO_MODE_INPUT,  GPIO_PULL_NONE, GPIO_VAL_0),

  PORT_CTRL_END,
};


int
PeriSdmmcInit(int unit)
{
  int                   result = PERI_ERROR_UNKNOWN;
  stm32Dev_SDMMC        *u;

  if(unit >= PERISDMMC_UNIT_MAX) goto fail;

  switch(unit) {
  case  SDMMC1_NUM:
    RCC_PTR->APB2ENR |= RCC_APB2ENR_SDMMC1EN_YES;
    SystemGpioSets((uint16_t *)gpioInitSdmmc1);
    break;
  case  SDMMC2_NUM:
    RCC_PTR->APB2ENR |= RCC_APB2ENR_SDMMC2EN_YES;
    //SystemGpioSets((uint16_t *)gpioInitSdmmc2);
    break;
  }

  u = pu[unit];

  // init FATFS
  UpdateSdmmcInit();            // test test test test

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriSdmmcUninit(int unit)
{
  int           result = PERI_ERROR_UNKNOWN;

  stm32Dev_SDMMC        *u;

  if(unit >= PERISDMMC_UNIT_MAX) goto fail;

  u = pu[unit];

  // uninit FATFS

  switch(unit) {
  case  SDMMC1_NUM:
    SystemGpioSets((uint16_t *)gpioUninitSdmmc1);
    RCC_PTR->APB2ENR &= ~RCC_APB2ENR_SDMMC1EN_MASK;
    break;
  case  SDMMC2_NUM:
    //SystemGpioSets((uint16_t *)gpioUninitSdmmc2);
    RCC_PTR->APB2ENR &= ~RCC_APB2ENR_SDMMC2EN_MASK;
    break;
  }

  result = PERI_SUCCESS;

fail:
  return result;
}


int
PeriSdmmcProbe(int unit)
{
  int           result = PERI_ERROR_UNKNOWN;
  uint8_t       c;
  int           len;

  if(unit >= PERISDMMC_UNIT_MAX) goto fail;

  // mount,update,umount

fail:
  return result;
}
