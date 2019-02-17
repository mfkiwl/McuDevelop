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

#define _IMU_C_

#include        <stdint.h>

#include        "system.h"
#include        "devGpio.h"
#include        "gpio.h"
#include        "devSpi16.h"

#include        "imu.h"

#include        "bmi160.h"
#include        "adxl345.h"
#include        "mpu9250.h"


typedef struct {
  int           (*probe)(int unit);
  int           (*init)(int unit);
  int           (*read)(int unit, imuValue_t *p);
  int           (*get)(int unit, imuSetting_t *p);
  int           (*set)(int unit, imuSetting_t *p);
} imuFunc_t;


#define IMU_NUMOF_SENSORS       (CONFIG_NUM_OF_IMUS)
static int     imuFuncNum[IMU_NUMOF_SENSORS] = {
  -1
};
const static imuFunc_t  imuFunc[] = {
  {Bmi160Probe, Bmi160Init, Bmi160ReadValue, Bmi160GetSettings, Bmi160SetSettings},
  {Adxl345Probe, Adxl345Init, Adxl345ReadValue, NULL, NULL},
  {Mpu9250Probe, Mpu9250Init, Mpu9250ReadValue, Mpu9250GetSettings, Mpu9250SetSettings},
  {NULL, NULL, NULL, NULL, NULL},
};


int
ImuProbe(int unit)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  int                   re;
  const imuFunc_t       *pFunc = imuFunc;
  int                   i = 0;
  int                   (*cb)(int unit, imuSetting_t *p);

  while(pFunc->probe) {
    if(pFunc->probe(unit) == IMU_ERRNO_SUCCESS) {
      //pFunc->init(unit);
      imuFuncNum[unit] = i;
      result = IMU_ERRNO_SUCCESS;
      break;
    }
    pFunc++;
    i++;
  }

  return result;
}


int
ImuInit(int unit)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc;

  if(imuFuncNum[unit] >= 0) {
    pFunc = &imuFunc[imuFuncNum[unit]];
    result = pFunc->init(unit);
  }

  return result;
}


int
ImuReadValue(int unit, imuValue_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc = imuFunc;
  int                   (*cb)(int unit, imuValue_t *p);

  if(unit < 0 ||  unit >= IMU_NUMOF_SENSORS) {
    result = IMU_ERRNO_UNIT;
    goto fail;
  }

  if(imuFuncNum[unit] >= 0) {
    cb = pFunc[imuFuncNum[unit]].read;
    if(cb) result = cb(unit, p);
  }

fail:
  return result;
}


int
ImuGetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc = imuFunc;
  int                   (*cb)(int unit, imuSetting_t *p);

  if(unit < 0 ||  unit >= IMU_NUMOF_SENSORS) {
    result = IMU_ERRNO_UNIT;
    goto fail;
  }

  if(imuFuncNum[unit] >= 0) {
    cb = pFunc[imuFuncNum[unit]].get;
    if(cb) result = cb(unit, p);
  }

fail:
  return result;
}


int
ImuSetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc = imuFunc;
  int                   (*cb)(int unit, imuSetting_t *p);

  if(unit < 0 ||  unit >= IMU_NUMOF_SENSORS) {
    result = IMU_ERRNO_UNIT;
    goto fail;
  }

  if(imuFuncNum[unit] >= 0) {
    cb = pFunc[imuFuncNum[unit]].set;
    if(cb) result = cb(unit, p);
  }

fail:
  return result;
}


/************************************************************
 * ImuEnableCs()
 * ImuDisableCs()
 */
void
ImuEnableCs(int unit)
{
  if(unit == 0) {
    GpioSetSpiCs0XEn();
  } else if(unit == 1) {
    GpioSetSpiCs1XEn();
  } else if(unit == 2) {
    GpioSetSpiCs2XEn();
  } else if(unit == 3) {
    GpioSetSpiCs3XEn();
  }

  return;
}
void
ImuDisableCs(int unit)
{
  if(unit == 0) {
    GpioSetSpiCs0XDis();
  } else if(unit == 1) {
    GpioSetSpiCs1XDis();
  } else if(unit == 2) {
    GpioSetSpiCs2XDis();
  } else if(unit == 3) {
    GpioSetSpiCs3XDis();
  }

  return;
}


/************************************************************
 * ImuGenCsPulse()
 * ImuSetValue()
 * ImuGetValue()
 * ImuSetValueStandard()
 * ImuGetValueStandard()
 */
void
ImuGenCsPulse(int unit, uint32_t ms)
{
  ImuEnableCs(unit);
  SystemWaitCounter(ms);
  ImuDisableCs(unit);

  return;
}
void
ImuSetValue(int unit, int reg, uint8_t val)
{
  uint8_t       buf[2];

  buf[0] = reg;
  buf[1] = val;

  ImuEnableCs(unit);
  DevSpiSend(1, buf, 2);
  ImuDisableCs(unit);

  return;
}
void
ImuGetValue(int unit, int reg, uint8_t *ptr, int size)
{
  uint8_t       cmd;

  cmd = reg;

  ImuEnableCs(unit);
  DevSpiSend(1, &cmd, 1);
  DevSpiRecv(1,  ptr, size);
  ImuDisableCs(unit);

  return;
}
void
ImuSetValueStandard(int unit, int reg, uint8_t val)
{
  uint8_t       cmd;

  cmd = reg & ~IMU_SPI_READ_WRITE_MASK;  /* write */

  ImuSetValue(unit, cmd, val);

  return;
}
void
ImuGetValueStandard(int unit, int reg, uint8_t *ptr, int size)
{
  uint8_t       cmd;

  cmd = reg | IMU_SPI_READ;              /* read */

  ImuGetValue(unit, cmd, ptr, size);

  return;
}


uint8_t         imuHexText[] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};
int
ImuBuildText(int unit, uint64_t ts1, uint32_t ts0, imuValue_t *imu, uint8_t *str)
{
  uint8_t       *p;
  int           n;

  p = str;

  /* time stamps */
  for(int i = 32; i >= 0; i -= 4) {
    *p++ = imuHexText[(ts1 >> i) & 0xf];
  }
  for(int i = 12; i >= 0; i -= 4) {
    *p++ = imuHexText[(ts0 >> i) & 0xf];
  }
  *p++ = ' ';

  /* unit */
  *p++ = imuHexText[unit & 0xf];
  *p++ = ' ';

  /* cnt */
  for(int i = 28; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->cnt >> i) & 0xf];
  }
  *p++ = ' ';

  /* accel */
  for(int i = 12; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->acc.x >> i) & 0xf];
  }
  for(int i = 12; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->acc.y >> i) & 0xf];
  }
  for(int i = 12; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->acc.z >> i) & 0xf];
  }
  *p++ = ' ';

  /* gyro */
  for(int i = 12; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->gyro.x >> i) & 0xf];
  }
  for(int i = 12; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->gyro.y >> i) & 0xf];
  }
  for(int i = 12; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->gyro.z >> i) & 0xf];
  }
  *p++ = ' ';

  /* temp x4 */
  for(int i = 4; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->temp4x >> i) & 0xf];
  }
  *p++ = ' ';

  /* timestamp in imu */
  for(int i = 28; i >= 0; i -= 4) {
    *p++ = imuHexText[(imu->ts >> i) & 0xf];
  }
  *p++ = ' ';

  *p++ = '\n';
  *p = '\0';

  return 0;
}
