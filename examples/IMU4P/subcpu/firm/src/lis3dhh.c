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

#define _LIS3DHH_C_

#include        <stdint.h>

#include        "config.h"
#include        "devErrno.h"
#include        "main.h"
#include        "imu.h"

#include        "lis3dhh.h"

typedef struct {
  uint8_t       accRange;
  uint8_t       accOdr;
} lis3dhhSetting_t;

static lis3dhhSetting_t setting[CONFIG_NUM_OF_IMUS];

/************************************************************
 * Lis3dhhProbe()
 */
int
Lis3dhhProbe(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;
  lis3dhhSetting_t       *pSet;

  re = ImuGetValueStandard(unit, LIS3DHH_REG_WHO_AM_I, &c, 1);
#if CONFIG_IMU_WAIT_DMA_NONBLOCK
  if(re == DEV_ERRNO_NONBLOCK) {
    while(ImuIsFinishReceiving(unit) != IMU_ERRNO_SUCCESS);
  }
#endif

  if(c == WHO_AM_I_LIS3DHH) {
#if CONFIG_IMU_DEBUG_PROBE
    printf("# imu probed unit: %d LIS3DHH\n", unit);
#endif

    pSet = &setting[unit];

    result = IMU_ERRNO_SUCCESS;
  }

  return result;
}


/************************************************************
 * Lis3dhhInit()
 */
int
Lis3dhhInit(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       reg, val;
  lis3dhhSetting_t       *pSet;

  pSet = &setting[unit];

  /* reset and wait */
  reg = LIS3DHH_REG_CTRL_REG1;
  val = CTRL_REG1_SW_RESET_YES;
  ImuSetValueStandard(unit, reg, val);

  SystemWaitCounter(10);

  /* normal mode, addr inc, drdy_pulse, cont update */
  reg = LIS3DHH_REG_CTRL_REG1;
  val = (CTRL_REG1_NORM_MOD_EN_YES | CTRL_REG1_IF_ADD_INC_YES |
         CTRL_REG1_DRDY_PULSE_YES | CTRL_REG1_BDU_YES);
  ImuSetValueStandard(unit, reg, val);

  SystemWaitCounter(10);

  /* int1 drdy enable */
  reg = LIS3DHH_REG_INT1_CTRL;
  val = (CTRL_REG1_NORM_MOD_EN_YES | CTRL_REG1_IF_ADD_INC_YES |
         CTRL_REG1_DRDY_PULSE_YES | CTRL_REG1_BDU_YES);
  ImuSetValueStandard(unit, reg, val);

  return result;
}


int
Lis3dhhRecvValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint32_t      reg = 0 | LIS3DHH_READ;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  result = ImuGetValueStandard(unit, LIS3DHH_REG_OUT_TEMP_LOW, buf, 9);
  if(result == DEV_ERRNO_NONBLOCK) goto fail;

  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
Lis3dhhReadValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       *buf;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
Lis3dhhGetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  int                   freqVal;
  lis3dhhSetting_t       *pSet;

  pSet = &setting[unit];
  return IMU_ERRNO_SUCCESS;
}


int
Lis3dhhSetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  lis3dhhSetting_t       *pSet;

  pSet = &setting[unit];

  return IMU_ERRNO_SUCCESS;
}
