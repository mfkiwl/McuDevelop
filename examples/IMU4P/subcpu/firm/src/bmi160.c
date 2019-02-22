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

#define _BMI160_C_

#include        <stdint.h>

#include        "config.h"
#include        "devErrno.h"
#include        "main.h"
#include        "imu.h"

#include        "bmi160.h"

typedef struct {
  uint8_t       accRange;
  uint8_t       accOdr;
  uint8_t       gyroRange;
  uint8_t       gyroOdr;
} bmi160Setting_t;

static bmi160Setting_t setting[CONFIG_NUM_OF_IMUS];


/************************************************************
 * Bmi160Probe()
 */
int
Bmi160Probe(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;
  bmi160Setting_t       *pSet;

  re = ImuGetValueStandard(unit, BMI160_REG_CHIP_ID, &c, 1);
#if CONFIG_IMU_WAIT_DMA_NONBLOCK
  if(re == DEV_ERRNO_NONBLOCK) {
    while(ImuIsFinishReceiving(unit) != IMU_ERRNO_SUCCESS);
  }
#endif

  if(c == CHIP_ID_BMI160) {
#if CONFIG_IMU_DEBUG_PROBE
    printf("# imu probed unit: %d BMI160\n", unit);
#endif

    pSet = &setting[unit];

    pSet->accOdr    = 0x20 | ACC_CONF_ODR_1600HZ;
    pSet->accRange  = 3;
    pSet->gyroOdr   = 0x20 | GYRO_CONF_ODR_1600HZ;
    pSet->gyroRange = 3;

    result = IMU_ERRNO_SUCCESS;
  }

  return result;
}


/************************************************************
 * Bmi160Init()
 */
int
Bmi160Init(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       reg, val;
  bmi160Setting_t       *pSet;

  pSet = &setting[unit];

  /* reset and wait */
  reg = BMI160_REG_CMD;
  val = CMD_SOFTRESET;
  ImuSetValueStandard(unit, reg, val);

  SystemWaitCounter(2);

  /* flash fifo and reset interrupt */
  reg = BMI160_REG_CMD;
  val = CMD_FIFO_FLUSH;
  ImuSetValueStandard(unit, reg, val);

  reg = BMI160_REG_CMD;
  val = CMD_INT_RESET;
  ImuSetValueStandard(unit, reg, val);

  /* interface mode */
  reg = BMI160_REG_IF_CONF;
  val = IF_CONF_MODE_AUTO_2NDOFF | IF_CONF_SPI3_NO4;
  ImuSetValueStandard(unit, reg, val);

  /* start foc  and 10msec*/
  reg = BMI160_REG_CMD;
  val = 0x03;
  ImuSetValueStandard(unit, reg, val);
  SystemWaitCounter(10);

  /* enable accel measurement  and wait 1 msec */
  reg = BMI160_REG_CMD;
  val = 0x11;
  ImuSetValueStandard(unit, reg, val);
  SystemWaitCounter(2);

  /* enable gyroscope measurement  and wait 50 msec*/
  reg = BMI160_REG_CMD;
  val = 0x15;
  ImuSetValueStandard(unit, reg, val);
  SystemWaitCounter(50);

  /* [5:4]band width 2=800Hz,
   * [3:0]data rate: a=400Hz,b=800,c=1600,d=3200
   * range: 3=2G,5=4G,c=16G
   */
  reg = BMI160_REG_ACC_CONF;
  val = pSet->accOdr;
  ImuSetValueStandard(unit, reg, val);
  reg = BMI160_REG_ACC_RANGE;
  val = pSet->accRange;
  ImuSetValueStandard(unit, reg, val);

  /* [5:4]band bwp: = 800Hz,
   * [3:0]data rate: a=400Hz,b=800,c=1600,d=3200
   * range: 0=2kdeg/s,1=1k,2=500,3=250,4=125
   */
  reg = BMI160_REG_GYRO_CONF;
  val = pSet->gyroOdr;
  ImuSetValueStandard(unit, reg, val);
  reg = BMI160_REG_GYRO_RANGE;
  val = pSet->gyroRange;
  ImuSetValueStandard(unit, reg, val);

  /* interrupt  data ready, map int1, */
  reg = 0x51;
  val = 0x10;
  ImuSetValueStandard(unit, reg, val);
  reg = 0x56;
  val = 0x80;
  ImuSetValueStandard(unit, reg, val);
  /* step counter */
  reg = 0x52;
  val = 0x38;
  ImuSetValueStandard(unit, reg, val);
#if 0
  reg = 0x55;
  val = 0x01;
  ImuSetValueStandard(unit, reg, val);
#endif
#if 1
  /* enable interrupt */
  reg = 0x53;
  val = 0x0b;
  ImuSetValueStandard(unit, reg, val);
#endif

  return result;
}


int
Bmi160RecvValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint32_t      reg = 0 | BMI160_READ;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  result = ImuGetValueStandard(unit, BMI160_REG_CHIP_ID, buf, 34);
  if(result == DEV_ERRNO_NONBLOCK) goto fail;

#if 0
  p->gyro.x = (buf[BMI160_REG_GYRO_X_HIGH] << 8) | buf[BMI160_REG_GYRO_X_LOW];
  p->gyro.y = (buf[BMI160_REG_GYRO_Y_HIGH] << 8) | buf[BMI160_REG_GYRO_Y_LOW];
  p->gyro.z = (buf[BMI160_REG_GYRO_Z_HIGH] << 8) | buf[BMI160_REG_GYRO_Z_LOW];

  p->acc.x = (buf[BMI160_REG_ACC_X_HIGH] << 8) | buf[BMI160_REG_ACC_X_LOW];
  p->acc.y = (buf[BMI160_REG_ACC_Y_HIGH] << 8) | buf[BMI160_REG_ACC_Y_LOW];
  p->acc.z = (buf[BMI160_REG_ACC_Z_HIGH] << 8) | buf[BMI160_REG_ACC_Z_LOW];

  p->ts = (buf[BMI160_REG_TIME_HIGH] << 16) |
    (buf[BMI160_REG_TIME_MID] << 8) | buf[BMI160_REG_TIME_LOW];

  temp = (buf[BMI160_REG_TEMP_HIGH] << 8) | buf[BMI160_REG_TEMP_LOW] ;
  p->temp4x = (23 << 2) + (temp >> 7);
#endif
  Bmi160ReadValue(unit, p);

  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
Bmi160ReadValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       *buf;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  p->gyro.x = (buf[BMI160_REG_GYRO_X_HIGH] << 8) | buf[BMI160_REG_GYRO_X_LOW];
  p->gyro.y = (buf[BMI160_REG_GYRO_Y_HIGH] << 8) | buf[BMI160_REG_GYRO_Y_LOW];
  p->gyro.z = (buf[BMI160_REG_GYRO_Z_HIGH] << 8) | buf[BMI160_REG_GYRO_Z_LOW];

  p->acc.x = (buf[BMI160_REG_ACC_X_HIGH] << 8) | buf[BMI160_REG_ACC_X_LOW];
  p->acc.y = (buf[BMI160_REG_ACC_Y_HIGH] << 8) | buf[BMI160_REG_ACC_Y_LOW];
  p->acc.z = (buf[BMI160_REG_ACC_Z_HIGH] << 8) | buf[BMI160_REG_ACC_Z_LOW];

  p->ts = (buf[BMI160_REG_TIME_HIGH] << 16) |
    (buf[BMI160_REG_TIME_MID] << 8) | buf[BMI160_REG_TIME_LOW];

  temp = (buf[BMI160_REG_TEMP_HIGH] << 8) | buf[BMI160_REG_TEMP_LOW] ;
  p->temp4x = (23 << 2) + (temp >> 7);

  result = DEV_ERRNO_SUCCESS;

fail:
  return result;
}


int
Bmi160GetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  int                   freqVal;
  bmi160Setting_t       *pSet;

  pSet = &setting[unit];

  switch(pSet->accRange & ACC_CONF_RANGE_MASK) {
  case ACC_CONF_RANGE_PM4G:  p->acc_fs =  400; break;
  case ACC_CONF_RANGE_PM8G:  p->acc_fs =  800; break;
  case ACC_CONF_RANGE_PM16G: p->acc_fs = 1600; break;
  default:                   p->acc_fs =  200; break;
  }
  switch(pSet->gyroRange & GYRO_CONF_RANGE_MASK) {
  case GYRO_CONF_RANGE_PM2000DPS: p->gyro_fs =  200; break;
  case GYRO_CONF_RANGE_PM1000DPS: p->gyro_fs =  100; break;
  case GYRO_CONF_RANGE_PM500DPS:  p->gyro_fs =   50; break;
  case GYRO_CONF_RANGE_PM250DPS:  p->gyro_fs =   25; break;
  case GYRO_CONF_RANGE_PM125DPS:  p->gyro_fs =   12; break;
  default:                        p->gyro_fs =  200; break;
  }
  freqVal = GYRO_CONF_ODR_3200HZ - ((pSet->gyroOdr & GYRO_CONF_ODR_MASK) >> GYRO_CONF_ODR_SHIFT);
  p->freq = 3200 >> (freqVal);

  return IMU_ERRNO_SUCCESS;
}


int
Bmi160SetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  bmi160Setting_t       *pSet;

  pSet = &setting[unit];

  if(p->acc_fs != IMU_SETTING_INVALID) {
    pSet->accRange &= ~ACC_CONF_RANGE_MASK;
    if(     p->acc_fs >= 1600) pSet->accRange |= ACC_CONF_RANGE_PM16G;
    else if(p->acc_fs >=  800) pSet->accRange |= ACC_CONF_RANGE_PM8G;
    else if(p->acc_fs >=  400) pSet->accRange |= ACC_CONF_RANGE_PM4G;
    else                       pSet->accRange |= ACC_CONF_RANGE_PM2G;
  }
  if(p->gyro_fs != IMU_SETTING_INVALID) {
    pSet->gyroRange &= ~GYRO_CONF_RANGE_MASK;
    if(     p->gyro_fs >= 200) pSet->gyroRange |= GYRO_CONF_RANGE_PM2000DPS;
    else if(p->gyro_fs >= 100) pSet->gyroRange |= GYRO_CONF_RANGE_PM1000DPS;
    else if(p->gyro_fs >=  50) pSet->gyroRange |= GYRO_CONF_RANGE_PM500DPS;
    else if(p->gyro_fs >=  25) pSet->gyroRange |= GYRO_CONF_RANGE_PM250DPS;
    else                       pSet->gyroRange |= GYRO_CONF_RANGE_PM125DPS;
  }
  if(p->freq != IMU_SETTING_INVALID) {
    pSet->accOdr  &= ~ACC_CONF_ODR_MASK;
    pSet->gyroOdr &= ~GYRO_CONF_ODR_MASK;
    if(       p->freq >= 3200) {        /* 3200Hz */
      pSet->accOdr  |= ACC_CONF_ODR_1600HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_3200HZ;
    } else if(p->freq >= 1600) {        /* 1600Hz */
      pSet->accOdr  |= ACC_CONF_ODR_1600HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_1600HZ;
    } else if(p->freq >= 800) {         /*  800Hz */
      pSet->accOdr  |= ACC_CONF_ODR_800HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_800HZ;
    } else if(p->freq >= 400) {         /*  400Hz */
      pSet->accOdr  |= ACC_CONF_ODR_400HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_400HZ;
    } else if(p->freq >= 200) {         /*  200Hz */
      pSet->accOdr  |= ACC_CONF_ODR_200HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_200HZ;
    } else if(p->freq >= 100) {         /*  100Hz */
      pSet->accOdr  |= ACC_CONF_ODR_100HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_100HZ;
    } else if(p->freq >=  50) {         /*  50Hz */
      pSet->accOdr  |= ACC_CONF_ODR_50HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_50HZ;
    } else {                            /*  25Hz */
      pSet->accOdr  |= ACC_CONF_ODR_25HZ;
      pSet->gyroOdr |= GYRO_CONF_ODR_25HZ;
    }
  }

  return IMU_ERRNO_SUCCESS;
}
