/*
 * Copyright (c) 2019 zhtlab
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

#define _ICM42605_C_

#include        <stdint.h>
#include        <stdlib.h>
#include        <stdio.h>
#include        <cmsis_gcc.h>

#include        "config.h"
#include        "devErrno.h"
#include        "main.h"
#include        "imu.h"

#include        "icm42605.h"

typedef struct {
  uint8_t  accConfig0;
  uint8_t  gyroConfig0;
  uint16_t odr;
} icm42605Setting_t;


/************************************************************
 * Icm42605Probe()
 */
int
Icm42605Probe(imuHandler_t *ph)
{
  int           result = IMU_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;
  icm42605Setting_t       *pSet;
  int           unit;

  unit = ph->unit;

  re = ImuGetValueStandard(unit, ICM42605_REG_WHOAMI, &c, 1);
#if CONFIG_IMU_WAIT_DMA_NONBLOCK
  if(re == DEV_ERRNO_NONBLOCK) {
    while(ImuIsFinishReceiving(unit) != IMU_ERRNO_SUCCESS);
  }
#endif

  if(c == WHOAMI_VALUE_ICM42605
#if WHOAMI_VALUE_TESTID
     || c == WHOAMI_VALUE_TESTID
#endif
     ) {
#if CONFIG_IMU_DEBUG_PROBE
    printf("# imu probed unit: %d ICM42605\n", unit);
#endif

    if((ph->pConfig = malloc(sizeof(icm42605Setting_t))) == NULL) {
      goto fail;
    }
    pSet = ph->pConfig;

    pSet->odr = 1000;
    pSet->accConfig0  = ACCEL_CONFIG0_FSSEL_2G     | ACCEL_CONFIG0_ODR_1000HZ;
    pSet->gyroConfig0 = GYRO_CONFIG0_FSSEL_2000DPS | GYRO_CONFIG0_ODR_1000HZ;

    result = IMU_ERRNO_SUCCESS;
  }

fail:
  return result;
}


/************************************************************
 * Icm42605Init()
 */
int
Icm42605Init(imuHandler_t *ph)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       reg, val;
  icm42605Setting_t       *pSet;
  int           unit;

  unit = ph->unit;

  pSet = ph->pConfig;

  /* reset first */
  reg = ICM42605_REG_DEVICE_CONFIG;
  val = DEVICE_CONFIG_SOFT_RESET_YES;
  ImuSetValueStandard(unit, reg, val);

  SystemWaitCounter(2);

  /* power manegement */
  reg = ICM42605_REG_PWR_MGMT0;
  val = (PWR_MGMT0_TEMP_DIS_NO | PWR_MGMT0_IDLE_NO |
         PWR_MGMT0_GYRO_MODE_LOWNOISE | PWR_MGMT0_ACCEL_MODE_LOWNOISE);
  ImuSetValueStandard(unit, reg, val);

  /* configuration accel */
  reg = ICM42605_REG_ACCEL_CONFIG0;
  val = pSet->accConfig0;
  ImuSetValueStandard(unit, reg, val);

  /* configuration gyro */
  reg = ICM42605_REG_GYRO_CONFIG0;
  val = pSet->gyroConfig0;
  ImuSetValueStandard(unit, reg, val);

  /* interrupt */
  reg = ICM42605_REG_INT_SOURCE0;
  val = INT_SOURCE0_UI_DRDY_INT1_EN_YES;
  ImuSetValueStandard(unit, reg, val);
  reg = ICM42605_REG_INT_CONFIG;
  val = (INT_CONFIG_INT1_MODE_PULSE |
         INT_CONFIG_INT1_DRIVE_CIRCUIT_DRIVE |
         INT_CONFIG_INT1_POLARITY_HIGH);
  ImuSetValueStandard(unit, reg, val);

  return result;
}


#define ICM42605_REG_ACCEL_DATA_X1      0x1f

#define TOP     (ICM42605_REG_TEMP_OUT_H)
#define TH      ((ICM42605_REG_TEMP_OUT_H)   -(TOP))
#define TL      ((ICM42605_REG_TEMP_OUT_L)   -(TOP))
#define AXH     ((ICM42605_REG_ACCEL_DATA_X1)-(TOP))
#define AXL     ((ICM42605_REG_ACCEL_DATA_X0)-(TOP))
#define AYH     ((ICM42605_REG_ACCEL_DATA_Y1)-(TOP))
#define AYL     ((ICM42605_REG_ACCEL_DATA_Y0)-(TOP))
#define AZH     ((ICM42605_REG_ACCEL_DATA_Z1)-(TOP))
#define AZL     ((ICM42605_REG_ACCEL_DATA_Z0)-(TOP))
#define GXH     ((ICM42605_REG_GYRO_DATA_X1) -(TOP))
#define GXL     ((ICM42605_REG_GYRO_DATA_X0) -(TOP))
#define GYH     ((ICM42605_REG_GYRO_DATA_Y1) -(TOP))
#define GYL     ((ICM42605_REG_GYRO_DATA_Y0) -(TOP))
#define GZH     ((ICM42605_REG_GYRO_DATA_Z1) -(TOP))
#define GZL     ((ICM42605_REG_GYRO_DATA_Z0) -(TOP))
#define BOT     (ICM42605_REG_GYRO_DATA_Z0)
int
Icm42605RecvValue(imuHandler_t *ph, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint16_t      temp;
  int           unit;

  if(!p) goto fail;

  unit = ph->unit;
  buf = p->raw;

#define READ_LEN  ((BOT) - (TOP) + 1)
  result = ImuGetValueStandard(unit, (TOP), buf, READ_LEN);
  if(result != DEV_ERRNO_NONBLOCK) {
    Icm42605ReadValue(ph, p);
    result = DEV_ERRNO_SUCCESS;
  }

fail:
  return result;
}
int
Icm42605ReadValue(imuHandler_t *ph, imuValue_t *p)
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
Icm42605GetSettings(imuHandler_t *ph, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  icm42605Setting_t       *pSet;

  pSet = ph->pConfig;

  switch(pSet->accConfig0 & ACCEL_CONFIG0_FSSEL_MASK) {
  case ACCEL_CONFIG0_FSSEL_4G:  p->acc_fs =  400; break;
  case ACCEL_CONFIG0_FSSEL_8G:  p->acc_fs =  800; break;
  case ACCEL_CONFIG0_FSSEL_16G: p->acc_fs = 1600; break;
  default:                      p->acc_fs =  200; break;
  }
  switch(pSet->gyroConfig0 & GYRO_CONFIG0_FSSEL_MASK) {
  case GYRO_CONFIG0_FSSEL_2000DPS: p->gyro_fs =  200; break;
  case GYRO_CONFIG0_FSSEL_1000DPS: p->gyro_fs =  100; break;
  case GYRO_CONFIG0_FSSEL_500DPS:  p->gyro_fs =   50; break;
  case GYRO_CONFIG0_FSSEL_250DPS:  p->gyro_fs =   25; break;
  case GYRO_CONFIG0_FSSEL_125DPS:  p->gyro_fs =   12; break;
  case GYRO_CONFIG0_FSSEL_62DPS:   p->gyro_fs =    6; break;
  case GYRO_CONFIG0_FSSEL_31DPS:   p->gyro_fs =    3; break;
  case GYRO_CONFIG0_FSSEL_15DPS:   p->gyro_fs =    1; break;
  default:                         p->gyro_fs =  200; break;
  }
  p->freq = pSet->odr;

  p->mag_fs = -1;       /* n/a */

  return IMU_ERRNO_SUCCESS;
}


int
Icm42605SetSettings(imuHandler_t *ph, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  icm42605Setting_t       *pSet;
  uint8_t               accOdr, gyroOdr;

  pSet = ph->pConfig;

  if(p->acc_fs != IMU_SETTING_INVALID) {
    pSet->accConfig0 &= ~ACCEL_CONFIG0_FSSEL_MASK;
    if(     p->acc_fs >= 1600) pSet->accConfig0 |= ACCEL_CONFIG0_FSSEL_16G;
    else if(p->acc_fs >=  800) pSet->accConfig0 |= ACCEL_CONFIG0_FSSEL_8G;
    else if(p->acc_fs >=  400) pSet->accConfig0 |= ACCEL_CONFIG0_FSSEL_4G;
    else                       pSet->accConfig0 |= ACCEL_CONFIG0_FSSEL_2G;
  }
  if(p->gyro_fs != IMU_SETTING_INVALID) {
    pSet->gyroConfig0 &= ~GYRO_CONFIG0_FSSEL_MASK;
    if(     p->gyro_fs >= 200) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_2000DPS;
    else if(p->gyro_fs >= 100) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_1000DPS;
    else if(p->gyro_fs >=  50) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_500DPS;
    else if(p->gyro_fs >=  25) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_250DPS;
    else if(p->gyro_fs >=  12) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_125DPS;
    else if(p->gyro_fs >=   6) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_62DPS;
    else if(p->gyro_fs >=   3) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_31DPS;
    else if(p->gyro_fs >=   1) pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_15DPS;
    else                       pSet->gyroConfig0 |= GYRO_CONFIG0_FSSEL_250DPS;
  }

  if(p->freq != IMU_SETTING_INVALID) {
    pSet->odr = p->freq;
    if(       p->freq >= 8000) {        /* 8000Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_8000HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_8000HZ;
    } else if(p->freq >= 4000) {        /* 4000Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_4000HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_4000HZ;
    } else if(p->freq >= 2000) {        /* 2000Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_2000HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_2000HZ;
    } else if(p->freq >= 1000) {        /* 1000Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_1000HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_1000HZ;
    } else if(p->freq >=  500) {        /*  500Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_500HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_500HZ;
    } else if(p->freq >=  200) {        /*  200Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_200HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_200HZ;
    } else if(p->freq >=  100) {        /*  100Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_100HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_100HZ;
    } else if(p->freq >=   50) {        /*   50Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_50HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_50HZ;
    } else if(p->freq >=   25) {        /*   25Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_25HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_25HZ;
    } else if(p->freq >=   12) {        /*   12.5Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_12HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_12HZ;
    } else if(p->freq >=   12) {        /*    6.2Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_6HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_12HZ;
    } else if(p->freq >=   12) {        /*    3.1Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_3HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_12HZ;
    } else {                           /*     1.5Hz */
      accOdr  |= ACCEL_CONFIG0_ODR_1HZ;
      gyroOdr |= GYRO_CONFIG0_ODR_12HZ;
    }
    pSet->accConfig0 &= ~ACCEL_CONFIG0_ODR_MASK;
    pSet->accConfig0 |=  accOdr;
    pSet->gyroConfig0  &= ~GYRO_CONFIG0_ODR_MASK;
    pSet->gyroConfig0  |=  gyroOdr;
  }

  return IMU_ERRNO_SUCCESS;
}
