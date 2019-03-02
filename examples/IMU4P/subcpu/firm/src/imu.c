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
#include        "devErrno.h"
#include        "devSpi16.h"

#include        "imu.h"
#include        "main.h"

#include        "bmi160.h"
#include        "adxl345.h"
#include        "mpu9250.h"
#include        "lis3dhh.h"


typedef struct {
  int           (*probe)(imuHandler_t *ph);
  int           (*init)(imuHandler_t *ph);
  int           (*recv)(imuHandler_t *ph, imuValue_t *p);
  int           (*read)(imuHandler_t *ph, imuValue_t *p);
  int           (*get)(imuHandler_t *ph, imuSetting_t *p);
  int           (*set)(imuHandler_t *ph, imuSetting_t *p);
} imuFunc_t;


imuHandler_t    h[CONFIG_NUM_OF_IMUS];

#define IMU_NUMOF_SENSORS       (CONFIG_NUM_OF_IMUS)
const static imuFunc_t  imuFunc[] = {
  {Bmi160Probe, Bmi160Init, Bmi160RecvValue, Bmi160ReadValue, Bmi160GetSettings, Bmi160SetSettings},
  {Lis3dhhProbe, Lis3dhhInit, Lis3dhhRecvValue, Lis3dhhReadValue, Lis3dhhGetSettings, Lis3dhhSetSettings},
  {Adxl345Probe, Adxl345Init, Adxl345RecvValue, Adxl345ReadValue, NULL, NULL},
  {Mpu9250Probe, Mpu9250Init, Mpu9250RecvValue, Mpu9250ReadValue, Mpu9250GetSettings, Mpu9250SetSettings},
  {NULL, NULL, NULL, NULL, NULL},
};


int
ImuProbe(int unit)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  int                   re;
  const imuFunc_t       *pFunc = imuFunc;
  int                   i = 0;
  int                   (*cb)(imuHandler_t *ph, imuSetting_t *p);

  h[unit].unit = unit;

  while(pFunc->probe) {
    if(pFunc->probe(&h[unit]) == IMU_ERRNO_SUCCESS) {
      h[unit].noFunc = i;
      result = IMU_ERRNO_SUCCESS;
      goto end;
    }
    pFunc++;
    i++;
  }

end:
  return result;
}


int
ImuInit(int unit)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc;
  int                   no;
  imuHandler_t          *ph;

  /* initialize variables */
  if(unit < 0) {
    ph = &h[0];
    for(int i = 0; i < CONFIG_NUM_OF_IMUS; i++) {
      ph->noFunc = -1;
      ph++;
    }
    goto end;
  }

  ph = &h[unit];
  no = h[unit].noFunc;

  if(no >= 0) {
    pFunc = &imuFunc[no];
    result = pFunc->init(ph);
  }

end:
  return result;
}


int
ImuRecvValue(int unit, imuValue_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc = imuFunc;
  int                   (*cb)(imuHandler_t *ph, imuValue_t *p);
  int                   no;
  imuHandler_t          *ph;

  if(unit < 0 ||  unit >= IMU_NUMOF_SENSORS) {
    result = IMU_ERRNO_UNIT;
    goto fail;
  }

  ph = &h[unit];
  no = h[unit].noFunc;

  if(no >= 0) {
    cb = pFunc[no].recv;
    if(cb) result = cb(ph, p);
  }

fail:
  return result;
}


int
ImuReadValue(int unit, imuValue_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc = imuFunc;
  int                   (*cb)(imuHandler_t *ph, imuValue_t *p);

  int                   no;
  imuHandler_t          *ph;

  if(unit < 0 ||  unit >= IMU_NUMOF_SENSORS) {
    result = IMU_ERRNO_UNIT;
    goto fail;
  }

  ph = &h[unit];
  no = ph->noFunc;

  if(no >= 0) {
    cb = pFunc[no].read;
    if(cb) result = cb(ph, p);
  }

fail:
  return result;
}


int
ImuGetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc = imuFunc;
  int                   (*cb)(imuHandler_t *ph, imuSetting_t *p);

  int                   no;
  imuHandler_t          *ph;

  if(unit < 0 ||  unit >= IMU_NUMOF_SENSORS) {
    result = IMU_ERRNO_UNIT;
    goto fail;
  }

  ph = &h[unit];
  no = ph->noFunc;

  if(no >= 0) {
    cb = pFunc[no].get;
    if(cb) result = cb(ph, p);
  }

fail:
  return result;
}


int
ImuSetSettings(int unit, imuSetting_t *p)
{
  int                   result = IMU_ERRNO_UNKNOWN;
  const imuFunc_t       *pFunc = imuFunc;
  int                   (*cb)(imuHandler_t *ph, imuSetting_t *p);

  int                   no;
  imuHandler_t          *ph;

  if(unit < 0 ||  unit >= IMU_NUMOF_SENSORS) {
    result = IMU_ERRNO_UNIT;
    goto fail;
  }

  ph = &h[unit];
  no = h[unit].noFunc;

  if(no >= 0) {
    cb = pFunc[no].set;
    if(cb) result = cb(ph, p);
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
int
ImuGetValue(int unit, int reg, uint8_t *ptr, int size)
{
  int           re;
  uint8_t       cmd;

  cmd = reg;

  ImuEnableCs(unit);
  DevSpiSend(1, &cmd, 1);
  re = DevSpiRecv(1,  ptr, size);
  if(re != DEV_ERRNO_NONBLOCK) {
    ImuDisableCs(unit);
  }

  return re;
}
int
ImuReadValueNonblockEnd(int unit)
{
  int   result;

  result = DevSpiRecvStopDma(1);
  ImuDisableCs(unit);

  return result;
}
int
ImuIsFinishReceiving(int unit)
{
  int   result;

  result = DevSpiRecvIsFinishDma(1);

  return result;
}
void
ImuSetValueStandard(int unit, int reg, uint8_t val)
{
  uint8_t       cmd;

  cmd = reg & ~IMU_SPI_READ_WRITE_MASK;  /* write */

  ImuSetValue(unit, cmd, val);

  return;
}
int
ImuGetValueStandard(int unit, int reg, uint8_t *ptr, int size)
{
  int           re;
  uint8_t       cmd;

  cmd = reg | IMU_SPI_READ;              /* read */

  re = ImuGetValue(unit, cmd, ptr, size);

  return re;
}


const static uint8_t         imuHexText[] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};
int
ImuBuildText(int unit, uint64_t ts1, uint32_t ts0, imuValue_t *imu, uint8_t *str)
{
  uint8_t       *p;
  int           n;
  register uint16_t     val;
  register uint32_t     val32;

  p = str;

  /* time stamps */
  val32 = ts1;
#if 0
  *p++ = imuHexText[(val32 >> 28) & 0xf];
  *p++ = imuHexText[(val32 >> 24) & 0xf];
  *p++ = imuHexText[(val32 >> 20) & 0xf];
  *p++ = imuHexText[(val32 >> 16) & 0xf];
  *p++ = imuHexText[(val32 >> 12) & 0xf];
  *p++ = imuHexText[(val32 >>  8) & 0xf];
#endif
  *p++ = imuHexText[(val32 >>  4) & 0xf];
  *p++ = imuHexText[(val32 >>  0) & 0xf];
  val = ts0;
  *p++ = imuHexText[(val >> 12) & 0xf];
  *p++ = imuHexText[(val >>  8) & 0xf];
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];
  *p++ = ' ';

  /* unit */
  *p++ = imuHexText[unit & 0xf];
  *p++ = ' ';
  /* cnt */
  val32 = imu->cnt;
  /*
  *p++ = imuHexText[(val32 >> 28) & 0xf];
  *p++ = imuHexText[(val32 >> 24) & 0xf];
  *p++ = imuHexText[(val32 >> 20) & 0xf];
  *p++ = imuHexText[(val32 >> 16) & 0xf];
  */
  *p++ = imuHexText[(val32 >> 12) & 0xf];
  *p++ = imuHexText[(val32 >>  8) & 0xf];
  *p++ = imuHexText[(val32 >>  4) & 0xf];
  *p++ = imuHexText[(val32 >>  0) & 0xf];
  *p++ = ' ';

  /* accel */
  val = imu->acc.x;
  *p++ = imuHexText[(val >> 12) & 0xf];
  *p++ = imuHexText[(val >>  8) & 0xf];
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];
  val = imu->acc.y;
  *p++ = imuHexText[(val >> 12) & 0xf];
  *p++ = imuHexText[(val >>  8) & 0xf];
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];
  val = imu->acc.z;
  *p++ = imuHexText[(val >> 12) & 0xf];
  *p++ = imuHexText[(val >>  8) & 0xf];
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];
  *p++ = ' ';

  /* gyro */
  val = imu->gyro.x;
  *p++ = imuHexText[(val >> 12) & 0xf];
  *p++ = imuHexText[(val >>  8) & 0xf];
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];
  val = imu->gyro.y;
  *p++ = imuHexText[(val >> 12) & 0xf];
  *p++ = imuHexText[(val >>  8) & 0xf];
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];
  val = imu->gyro.z;
  *p++ = imuHexText[(val >> 12) & 0xf];
  *p++ = imuHexText[(val >>  8) & 0xf];
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];

  *p++ = ' ';

  /* temp x4 */
  val = imu->temp4x;
  *p++ = imuHexText[(val >>  4) & 0xf];
  *p++ = imuHexText[(val >>  0) & 0xf];
  *p++ = ' ';

#if 0
  /* timestamp in imu */
  val32 = imu->ts;
  *p++ = imuHexText[(val32 >> 28) & 0xf];
  *p++ = imuHexText[(val32 >> 24) & 0xf];
  *p++ = imuHexText[(val32 >> 20) & 0xf];
  *p++ = imuHexText[(val32 >> 16) & 0xf];
  *p++ = imuHexText[(val32 >> 12) & 0xf];
  *p++ = imuHexText[(val32 >>  8) & 0xf];
  *p++ = imuHexText[(val32 >>  4) & 0xf];
  *p++ = imuHexText[(val32 >>  0) & 0xf];
  *p++ = ' ';
#endif
  *p++ = '\n';
  *p   = '\0';

  return 0;
}
