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

#include        "main.h"
#include        "imu.h"

#include        "mpu9250.h"


/************************************************************
 * Mpu9250Probe()
 */
int
Mpu9250Probe(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       c;

  ImuGetValueStandard(unit, MPU9250_REG_WHOAMI, &c, 1);
  if(c == WHOAMI_VALUE) {
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

  /* power management */
  reg = MPU9250_REG_PWR_MGMT1;
  val = 0;
  ImuSetValueStandard(unit, reg, val);
  reg = MPU9250_REG_PWR_MGMT2;
  val = 0;
  ImuSetValueStandard(unit, reg, val);

  /* configuration accel */
  reg = MPU9250_REG_ACCEL_CONFIG;
  val = ACCEL_CONFIG_FS_SEL_PM2G;
  ImuSetValueStandard(unit, reg, val);
  reg = MPU9250_REG_ACCEL_CONFIG2;
  val = ACCEL_CONFIG2_FCHOICE_YES | ACCEL_CONFIG2_A_DLPFCFG_420HZ;
  ImuSetValueStandard(unit, reg, val);

  /* configuration gyro */
  reg = MPU9250_REG_GYRO_CONFIG;
  val = GYRO_CONFIG_GYRO_FS_SEL_250DPS;
  ImuSetValueStandard(unit, reg, val);

  /* interrupt */
  reg = MPU9250_REG_INT_PIN;
  val = INT_PIN_ACTL_HIGH | INT_PIN_OPEN_PUSHPULL | INT_PIN_LATCH_INT_EN_EDGE;
  ImuSetValueStandard(unit, reg, val);
  reg = MPU9250_REG_INT_ENABLE;
  val = INT_ENABLE_RAW_RDY_EN_YES;
  ImuSetValueStandard(unit, reg, val);

  return result;
}


#define AXL     ((MPU9250_REG_ACCEL_XOUT_L)-(MPU9250_REG_ACCEL_XOUT_H))
#define AXH     ((MPU9250_REG_ACCEL_XOUT_H)-(MPU9250_REG_ACCEL_XOUT_H))
#define AYL     ((MPU9250_REG_ACCEL_YOUT_L)-(MPU9250_REG_ACCEL_XOUT_H))
#define AYH     ((MPU9250_REG_ACCEL_YOUT_H)-(MPU9250_REG_ACCEL_XOUT_H))
#define AZL     ((MPU9250_REG_ACCEL_ZOUT_L)-(MPU9250_REG_ACCEL_XOUT_H))
#define AZH     ((MPU9250_REG_ACCEL_ZOUT_H)-(MPU9250_REG_ACCEL_XOUT_H))
#define GXL     ((MPU9250_REG_GYRO_XOUT_L)-(MPU9250_REG_ACCEL_XOUT_H))
#define GXH     ((MPU9250_REG_GYRO_XOUT_H)-(MPU9250_REG_ACCEL_XOUT_H))
#define GYL     ((MPU9250_REG_GYRO_YOUT_L)-(MPU9250_REG_ACCEL_XOUT_H))
#define GYH     ((MPU9250_REG_GYRO_YOUT_H)-(MPU9250_REG_ACCEL_XOUT_H))
#define GZL     ((MPU9250_REG_GYRO_ZOUT_L)-(MPU9250_REG_ACCEL_XOUT_H))
#define GZH     ((MPU9250_REG_GYRO_ZOUT_H)-(MPU9250_REG_ACCEL_XOUT_H))
#define TL      ((MPU9250_REG_TEMP_OUT_L)-(MPU9250_REG_ACCEL_XOUT_H))
#define TH      ((MPU9250_REG_TEMP_OUT_H)-(MPU9250_REG_ACCEL_XOUT_H))
int
Mpu9250ReadValue(int unit, imuValue_t *p)
{
  uint8_t       cmd;
  uint8_t       buf[34];
  uint32_t      reg = 0 | MPU9250_READ;
  uint16_t      temp;

  if(!p) goto fail;

  ImuGetValueStandard(unit, MPU9250_REG_ACCEL_XOUT_L, buf, 14);

  memset(p, 0, sizeof(imuValue_t));

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

fail:
  return 0;
}
