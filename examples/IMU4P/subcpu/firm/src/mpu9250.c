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

#define _MPU9250_C_

#include        <stdint.h>
#include        <stdlib.h>
#include        <stdio.h>
#include        <cmsis_gcc.h>

#include        "config.h"
#include        "devErrno.h"
#include        "main.h"
#include        "imu.h"

#include        "mpu9250.h"

typedef struct {
  uint8_t  accConfig;
  uint8_t  accConfig2;
  uint8_t  gyroConfig;
  uint8_t  sampleDiv;
} mpu9250Setting_t;


/************************************************************
 * Mpu9250Probe()
 */
int
Mpu9250Probe(imuHandler_t *ph)
{
  int           result = IMU_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;
  mpu9250Setting_t       *pSet;
  int           unit;

  unit = ph->unit;

  re = ImuGetValueStandard(unit, MPU9250_REG_WHOAMI, &c, 1);
#if CONFIG_IMU_WAIT_DMA_NONBLOCK
  if(re == DEV_ERRNO_NONBLOCK) {
    while(ImuIsFinishReceiving(unit) != IMU_ERRNO_SUCCESS);
  }
#endif

  if(c == WHOAMI_VALUE) {
#if CONFIG_IMU_DEBUG_PROBE
    printf("# imu probed unit: %d MPU8250\n", unit);
#endif

    if((ph->pConfig = malloc(sizeof(mpu9250Setting_t))) == NULL) {
      goto fail;
    }
    pSet = ph->pConfig;

    pSet->sampleDiv = 0;
    pSet->accConfig  = ACCEL_CONFIG_FS_SEL_PM8G;
    pSet->accConfig2 = ACCEL_CONFIG2_FCHOICEB_0 | ACCEL_CONFIG2_A_DLPFCFG_218HZ;
    pSet->gyroConfig = GYRO_CONFIG_FCHOICEB_0 | GYRO_CONFIG_GYRO_FS_SEL_250DPS;

    result = IMU_ERRNO_SUCCESS;
  }

fail:
  return result;
}


/************************************************************
 * Mpu9250Init()
 */
int
Mpu9250Init(imuHandler_t *ph)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       reg, val;
  mpu9250Setting_t       *pSet;
  int           unit;

  unit = ph->unit;

  pSet = ph->pConfig;

  /* reset first */
  reg = MPU9250_REG_PWR_MGMT1;
  val = PWR_MGMT1_HRESET_YES;
  ImuSetValueStandard(unit, reg, val);

  SystemWaitCounter(2);

  /* power management */
  reg = MPU9250_REG_PWR_MGMT1;
  val = 0;
  ImuSetValueStandard(unit, reg, val);
  reg = MPU9250_REG_PWR_MGMT2;
  val = 0;
  ImuSetValueStandard(unit, reg, val);

  /* sampling rate */
  reg = MPU9250_REG_SMPLRT;
  val = pSet->sampleDiv;
  val = 0;
  ImuSetValueStandard(unit, reg, val);

  /* config */
  reg = MPU9250_REG_CONFIG;
  val = CONFIG_DLPF_CFG_184HZ;
  ImuSetValueStandard(unit, reg, val);

  /* configuration accel */
  reg = MPU9250_REG_ACCEL_CONFIG;
  val = pSet->accConfig;
  ImuSetValueStandard(unit, reg, val);
  reg = MPU9250_REG_ACCEL_CONFIG2;
  val = pSet->accConfig2;
  ImuSetValueStandard(unit, reg, val);

  /* configuration gyro */
  reg = MPU9250_REG_GYRO_CONFIG;
  val = pSet->gyroConfig;
  ImuSetValueStandard(unit, reg, val);

  /* interrupt */
  reg = MPU9250_REG_INT_PIN;
  val = (INT_PIN_ACTL_HIGH |
         INT_PIN_OPEN_PUSHPULL |
         INT_PIN_LATCH_INT_EN_EDGE |
         INT_PIN_INT_ANYRD_2CLEAR_INT_STATUS);
  ImuSetValueStandard(unit, reg, val);
  reg = MPU9250_REG_INT_ENABLE;
  val = INT_ENABLE_RAW_RDY_EN_YES;
  ImuSetValueStandard(unit, reg, val);

  return result;
}


#define TOP     (MPU9250_REG_ACCEL_XOUT_H)
#define AXH     ((MPU9250_REG_ACCEL_XOUT_H)-(TOP))
#define AXL     ((MPU9250_REG_ACCEL_XOUT_L)-(TOP))
#define AYH     ((MPU9250_REG_ACCEL_YOUT_H)-(TOP))
#define AYL     ((MPU9250_REG_ACCEL_YOUT_L)-(TOP))
#define AZH     ((MPU9250_REG_ACCEL_ZOUT_H)-(TOP))
#define AZL     ((MPU9250_REG_ACCEL_ZOUT_L)-(TOP))
#define TH      ((MPU9250_REG_TEMP_OUT_H)  -(TOP))
#define TL      ((MPU9250_REG_TEMP_OUT_L)  -(TOP))
#define GXH     ((MPU9250_REG_GYRO_XOUT_H) -(TOP))
#define GXL     ((MPU9250_REG_GYRO_XOUT_L) -(TOP))
#define GYH     ((MPU9250_REG_GYRO_YOUT_H) -(TOP))
#define GYL     ((MPU9250_REG_GYRO_YOUT_L) -(TOP))
#define GZH     ((MPU9250_REG_GYRO_ZOUT_H) -(TOP))
#define GZL     ((MPU9250_REG_GYRO_ZOUT_L) -(TOP))
int
Mpu9250RecvValue(imuHandler_t *ph, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint16_t      temp;
  int           unit;

  if(!p) goto fail;

  unit = ph->unit;
  buf = p->raw;

#define READ_LEN  ((MPU9250_REG_GYRO_ZOUT_L) - (MPU9250_REG_ACCEL_XOUT_H) + 1)
  result = ImuGetValueStandard(unit, (MPU9250_REG_ACCEL_XOUT_H), buf, READ_LEN);
  if(result != DEV_ERRNO_NONBLOCK) {
    Mpu9250ReadValue(ph, p);
    result = DEV_ERRNO_SUCCESS;
  }

fail:
  return result;
}
int
Mpu9250ReadValue(imuHandler_t *ph, imuValue_t *p)
{
  int           result;
  uint16_t      temp;
  uint16_t      *src, *dest;

  if(!p) goto fail;

  /* the data of *src byte order must be little endian
   * however, *src is bigendian, so swap high and low byte
   */
  src  = (uint16_t *) &p->raw[AXH];
  dest = (uint16_t *) &p->acc.x;
  *dest++ = __REV16(*src++);
  *dest++ = __REV16(*src++);
  *dest++ = __REV16(*src++);

  temp    = __REV16(*src++);
  p->temp4x = (23 << 2) + (temp >> 7);

  dest = (uint16_t *) &p->gyro.x;
  *dest++ = __REV16(*src++);
  *dest++ = __REV16(*src++);
  *dest++ = __REV16(*src++);

  p->capability = (IMU_CAP_TEMPERATURE |
                   IMU_CAP_GYRO | IMU_CAP_ACCEL);

  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
Mpu9250GetSettings(imuHandler_t *ph, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  mpu9250Setting_t       *pSet;

  pSet = ph->pConfig;

  switch(pSet->accConfig & ACCEL_CONFIG_FS_SEL_MASK) {
  case ACCEL_CONFIG_FS_SEL_PM4G:  p->acc_fs =  400; break;
  case ACCEL_CONFIG_FS_SEL_PM8G:  p->acc_fs =  800; break;
  case ACCEL_CONFIG_FS_SEL_PM16G: p->acc_fs = 1600; break;
  default:                        p->acc_fs =  200; break;
  }
  switch(pSet->gyroConfig & GYRO_CONFIG_GYRO_FS_SEL_MASK) {
  case GYRO_CONFIG_GYRO_FS_SEL_2000DPS: p->gyro_fs =  200; break;
  case GYRO_CONFIG_GYRO_FS_SEL_1000DPS: p->gyro_fs =  100; break;
  case GYRO_CONFIG_GYRO_FS_SEL_500DPS:  p->gyro_fs =   50; break;
  case GYRO_CONFIG_GYRO_FS_SEL_250DPS:  p->gyro_fs =   25; break;
  default:                              p->gyro_fs =  200; break;
  }
  p->freq = 1000 / (pSet->sampleDiv+1);

  p->mag_fs = -1;       /* n/a */

  return IMU_ERRNO_SUCCESS;
}


int
Mpu9250SetSettings(imuHandler_t *ph, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  mpu9250Setting_t       *pSet;

  pSet = ph->pConfig;

  if(p->acc_fs != IMU_SETTING_INVALID) {
    pSet->accConfig &= ~ACCEL_CONFIG_FS_SEL_MASK;
    if(     p->acc_fs >= 1600) pSet->accConfig |= ACCEL_CONFIG_FS_SEL_PM16G;
    else if(p->acc_fs >=  800) pSet->accConfig |= ACCEL_CONFIG_FS_SEL_PM8G;
    else if(p->acc_fs >=  400) pSet->accConfig |= ACCEL_CONFIG_FS_SEL_PM4G;
    else                       pSet->accConfig |= ACCEL_CONFIG_FS_SEL_PM2G;
  }
  if(p->gyro_fs != IMU_SETTING_INVALID) {
    pSet->gyroConfig &= ~GYRO_CONFIG_GYRO_FS_SEL_MASK;
    if(     p->gyro_fs >= 200) pSet->gyroConfig |= GYRO_CONFIG_GYRO_FS_SEL_2000DPS;
    else if(p->gyro_fs >= 100) pSet->gyroConfig |= GYRO_CONFIG_GYRO_FS_SEL_1000DPS;
    else if(p->gyro_fs >=  50) pSet->gyroConfig |= GYRO_CONFIG_GYRO_FS_SEL_500DPS;
    else                       pSet->gyroConfig |= GYRO_CONFIG_GYRO_FS_SEL_250DPS;
  }
  if(p->freq != IMU_SETTING_INVALID) {
    pSet->sampleDiv = 1000/p->freq - 1;
  }

  return IMU_ERRNO_SUCCESS;
}
