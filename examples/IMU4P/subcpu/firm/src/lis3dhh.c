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
#include        <stdlib.h>
#include        <stdio.h>

#include        "config.h"
#include        "devErrno.h"
#include        "main.h"
#include        "imu.h"

#include        "lis3dhh.h"

typedef struct {
  uint8_t       accRange;
  uint8_t       accOdr;
} lis3dhhSetting_t;


/************************************************************
 * Lis3dhhProbe()
 */
int
Lis3dhhProbe(imuHandler_t *ph)
{
  int           result = IMU_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;
  lis3dhhSetting_t       *pSet;
  int           unit;

  unit = ph->unit;

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

    if((ph->pConfig = malloc(sizeof(lis3dhhSetting_t))) == NULL) {
      goto fail;
    }
    pSet = ph->pConfig;

    result = IMU_ERRNO_SUCCESS;
  }

fail:
  return result;
}


/************************************************************
 * Lis3dhhInit()
 */
int
Lis3dhhInit(imuHandler_t *ph)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       reg, val;
  lis3dhhSetting_t       *pSet;
  int           unit;

  unit = ph->unit;

  pSet = ph->pConfig;

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
  val = (INT1_CTRL_DRDY_YES);
  ImuSetValueStandard(unit, reg, val);

  return result;
}


#define TOP     LIS3DHH_REG_CTRL_REG5
#define BOTTOM  LIS3DHH_REG_ACC_Z_HIGH

#define AXL     ((LIS3DHH_REG_ACC_X_LOW) -(TOP))
#define AXH     ((LIS3DHH_REG_ACC_X_HIGH)-(TOP))
#define AYL     ((LIS3DHH_REG_ACC_Y_LOW) -(TOP))
#define AYH     ((LIS3DHH_REG_ACC_Y_HIGH)-(TOP))
#define AZL     ((LIS3DHH_REG_ACC_Z_LOW) -(TOP))
#define AZH     ((LIS3DHH_REG_ACC_Z_HIGH)-(TOP))

#define READ_LEN  ((BOTTOM) - (TOP) + 1)
int
Lis3dhhRecvValue(imuHandler_t *ph, imuValue_t *p)
{
  int           result;
  uint8_t       *buf;
  int           unit;

  if(!p) goto fail;

  unit = ph->unit;
  buf = p->raw;

  result = ImuGetValueStandard(ph->unit, TOP, buf, READ_LEN);
  if(result != DEV_ERRNO_NONBLOCK) {
    Lis3dhhReadValue(ph, p);
    result = DEV_ERRNO_SUCCESS;
  }

fail:
  return result;
}


int
Lis3dhhReadValue(imuHandler_t *ph, imuValue_t *p)
{
  int           result;
  uint16_t      temp;
  uint8_t       *buf;
  uint16_t      *src, *dest;

  if(!p) goto fail;

  /* the data of *src byte order must be little endian */
  src  = (uint16_t *) &p->raw[AXL];
  dest = (uint16_t *) &p->acc.x;
  *dest++ = *src++;
  *dest++ = *src++;
  *dest++ = *src++;

  temp = (buf[LIS3DHH_REG_OUT_TEMP_HIGH] << 8) |
    buf[LIS3DHH_REG_OUT_TEMP_LOW] ;
  p->temp4x = (25 << 2) + (temp >> 7);

  p->capability = (IMU_CAP_TEMPERATURE | IMU_CAP_ACCEL);


  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
Lis3dhhGetSettings(imuHandler_t *ph, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  int                   freqVal;
  lis3dhhSetting_t       *pSet;

  pSet = ph->pConfig;

  p->acc_fs =  250;     /* fixed PM2.5G */
  p->freq = 1100;       /* fixed 1.1kHz */
  p->mag_fs = -1;       /* n/a */

  return IMU_ERRNO_SUCCESS;
}


int
Lis3dhhSetSettings(imuHandler_t *ph, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  lis3dhhSetting_t       *pSet;

  /*pSet = &setting[unit];*/

  return IMU_ERRNO_SUCCESS;
}
