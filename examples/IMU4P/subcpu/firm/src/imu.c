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
} imuFunc_t;


static int     imuFuncNum[4] = {
  -1, -1, -1, -1
};
const static imuFunc_t  imuFunc[] = {
  {Bmi160Probe, Bmi160Init, Bmi160ReadValue},
  {Adxl345Probe, Adxl345Init, Adxl345ReadValue},
  //{Mpu9250Probe, Mpu9250Init, Mpu9250ReadValue},
  {NULL, NULL, NULL},
};


int
ImuInit(int unit)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  int                   re;
  const imuFunc_t       *pFunc = imuFunc;
  int                   i = 0;

  while(pFunc->probe) {
    if(pFunc->probe(unit) == IMU_ERRNO_SUCCESS) {
      pFunc->init(unit);
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
ImuReadValue(int unit, imuValue_t *p)
{
  int                   result;
  const imuFunc_t       *pFunc = imuFunc;

  if(imuFuncNum[unit] >= 0) {
    pFunc[imuFuncNum[unit]].read(unit, p);
  }

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
