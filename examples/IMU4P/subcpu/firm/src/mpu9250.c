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

#include        "config.h"
#include        "devErrno.h"
#include        "main.h"
#include        "imu.h"

#include        "mpu9250.h"

typedef struct {
  uint8_t  accConfig;
  uint8_t  gyroConfig;
  uint8_t  sampleDiv;
} mpu9250Setting_t;

static mpu9250Setting_t setting[CONFIG_NUM_OF_IMUS];


/************************************************************
 * Mpu9250Probe()
 */
int
Mpu9250Probe(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;
  mpu9250Setting_t       *pSet;

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
    pSet = &setting[unit];

    pSet->sampleDiv = 0;
    pSet->accConfig = ACCEL_CONFIG_FS_SEL_PM2G;
    pSet->gyroConfig = GYRO_CONFIG_FCHOICEB_0 | GYRO_CONFIG_GYRO_FS_SEL_250DPS;

    result = IMU_ERRNO_SUCCESS;
  }

  return result;
}


/************************************************************
 * Mpu9250Init()
 */
int
Mpu9250Init(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       reg, val;
  mpu9250Setting_t       *pSet;

  pSet = &setting[unit];

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
  ImuSetValueStandard(unit, reg, val);

  /* config */
  reg = MPU9250_REG_CONFIG;
  val = CONFIG_DLPF_CFG_184HZ | GYRO_CONFIG_FCHOICEB_0;
  ImuSetValueStandard(unit, reg, val);

  /* configuration accel */
  reg = MPU9250_REG_ACCEL_CONFIG;
  val = pSet->accConfig;
  ImuSetValueStandard(unit, reg, val);
  reg = MPU9250_REG_ACCEL_CONFIG2;
  val = ACCEL_CONFIG2_FCHOICEB_0 | ACCEL_CONFIG2_A_DLPFCFG_218HZ;
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


#define AXL     ((MPU9250_REG_ACCEL_XOUT_L)-(MPU9250_REG_INT_STATUS))
#define AXH     ((MPU9250_REG_ACCEL_XOUT_H)-(MPU9250_REG_INT_STATUS))
#define AYL     ((MPU9250_REG_ACCEL_YOUT_L)-(MPU9250_REG_INT_STATUS))
#define AYH     ((MPU9250_REG_ACCEL_YOUT_H)-(MPU9250_REG_INT_STATUS))
#define AZL     ((MPU9250_REG_ACCEL_ZOUT_L)-(MPU9250_REG_INT_STATUS))
#define AZH     ((MPU9250_REG_ACCEL_ZOUT_H)-(MPU9250_REG_INT_STATUS))
#define GXL     ((MPU9250_REG_GYRO_XOUT_L) -(MPU9250_REG_INT_STATUS))
#define GXH     ((MPU9250_REG_GYRO_XOUT_H) -(MPU9250_REG_INT_STATUS))
#define GYL     ((MPU9250_REG_GYRO_YOUT_L) -(MPU9250_REG_INT_STATUS))
#define GYH     ((MPU9250_REG_GYRO_YOUT_H) -(MPU9250_REG_INT_STATUS))
#define GZL     ((MPU9250_REG_GYRO_ZOUT_L) -(MPU9250_REG_INT_STATUS))
#define GZH     ((MPU9250_REG_GYRO_ZOUT_H) -(MPU9250_REG_INT_STATUS))
#define TL      ((MPU9250_REG_TEMP_OUT_L)  -(MPU9250_REG_INT_STATUS))
#define TH      ((MPU9250_REG_TEMP_OUT_H)  -(MPU9250_REG_INT_STATUS))
int
Mpu9250RecvValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  result = ImuGetValueStandard(unit, MPU9250_REG_INT_STATUS, buf, 38);
  if(result == DEV_ERRNO_NONBLOCK) goto fail;

#if 0
  memset(p, 0, sizeof(imuValue_t));

  p->acc.x  = (buf[AXH] << 8) | buf[AXL];
  p->acc.y  = (buf[AYH] << 8) | buf[AYL];
  p->acc.z  = (buf[AZH] << 8) | buf[AZL];
  p->gyro.x = (buf[GXH] << 8) | buf[GXL];
  p->gyro.y = (buf[GYH] << 8) | buf[GYL];
  p->gyro.z = (buf[GZH] << 8) | buf[GZL];

  temp = (buf[TH] << 8) | buf[TL];
  p->temp4x = (23 << 2) + (temp >> 7);
#endif

#if 0
  p->ts = (buf[TH] << 8) | buf[TL];
#endif
  Mpu9250ReadValue(unit, p);

  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}
int
Mpu9250ReadValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  p->acc.x  = (buf[AXH] << 8) | buf[AXL];
  p->acc.y  = (buf[AYH] << 8) | buf[AYL];
  p->acc.z  = (buf[AZH] << 8) | buf[AZL];
  p->gyro.x = (buf[GXH] << 8) | buf[GXL];
  p->gyro.y = (buf[GYH] << 8) | buf[GYL];
  p->gyro.z = (buf[GZH] << 8) | buf[GZL];

  temp = (buf[TH] << 8) | buf[TL];
  p->temp4x = (23 << 2) + (temp >> 7);

#if 0
  p->ts = (buf[TH] << 8) | buf[TL];
#endif

  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
Mpu9250GetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  mpu9250Setting_t       *pSet;

  pSet = &setting[unit];


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
Mpu9250SetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  mpu9250Setting_t       *pSet;

  pSet = &setting[unit];

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
