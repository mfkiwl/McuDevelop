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
#include        "devCrc.h"

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
    devCrcParam_t       crc;

    ph = &h[0];
    for(int i = 0; i < CONFIG_NUM_OF_IMUS; i++) {
      ph->noFunc = -1;
      ph++;
    }

    crc.cr = (DEVCRC_CR_POLYSIZE_32BIT |
              DEVCRC_CR_REVIN_8BIT | DEVCRC_CR_REVOUT_32BIT);
    crc.poly = DEVCRC_POLY_CRC32;
    crc.init = DEVCRC_INIT_0XFFFFFFFF;
    DevCrcInit(&crc);

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
const static uint8_t         imuHamming[] = {
  /* [5:4]: check bit [3:0]: information */
  0x00, 0x61, 0x52, 0x33, 0x34, 0x55, 0x66, 0x07,
  0x78, 0x19, 0x2a, 0x4b, 0x4c, 0x2d, 0x1e, 0x7f,
};
int
ImuBuildText(int unit, imuValue_t *imu, uint8_t *str, int format)
{
  uint8_t       *p;
  int           n;
  register uint16_t     val16;
  register int          val;
  register uint32_t     val32;
  register uint64_t     val64;

  const static uint8_t  *pTbl;

  uint8_t               msb = 0;

  p = str;

  if(format == MAIN_OUTPUT_FORMAT_HAMMING_CODE) {
    pTbl = &imuHamming[0];
    msb = 0x80;
  } else {
    pTbl = &imuHexText[0];
  }

  /* time stamps */
  val64 = imu->t;
  *p++ = pTbl[(val64 >> 24) & 0xf] | msb;
  *p++ = pTbl[(val64 >> 20) & 0xf];
  *p++ = pTbl[(val64 >> 16) & 0xf];
  *p++ = pTbl[(val64 >> 12) & 0xf];
  *p++ = pTbl[(val64 >>  8) & 0xf];
  *p++ = pTbl[(val64 >>  4) & 0xf];
  *p++ = pTbl[(val64 >>  0) & 0xf];
  *p++ = ' ';

  /* unit */
  *p++ = pTbl[unit & 0xf];
  *p++ = ' ';
  /* cnt */
  val32 = imu->cnt;
  /*
  *p++ = pTbl[(val32 >> 28) & 0xf];
  *p++ = pTbl[(val32 >> 24) & 0xf];
  *p++ = pTbl[(val32 >> 20) & 0xf];
  *p++ = pTbl[(val32 >> 16) & 0xf];
  */
  *p++ = pTbl[(val32 >> 12) & 0xf];
  *p++ = pTbl[(val32 >>  8) & 0xf];
  *p++ = pTbl[(val32 >>  4) & 0xf];
  *p++ = pTbl[(val32 >>  0) & 0xf];
  *p++ = ' ';

  val = imu->capability;
  *p++ = pTbl[(val >> 12) & 0xf];
  *p++ = pTbl[(val >>  8) & 0xf];
  *p++ = pTbl[(val >>  4) & 0xf];
  *p++ = pTbl[(val >>  0) & 0xf];
  *p++ = ' ';

  /* accel */
  val16 = imu->acc.x;
  *p++ = pTbl[(val16 >> 12) & 0xf];
  *p++ = pTbl[(val16 >>  8) & 0xf];
  *p++ = pTbl[(val16 >>  4) & 0xf];
  *p++ = pTbl[(val16 >>  0) & 0xf];
  val16 = imu->acc.y;
  *p++ = pTbl[(val16 >> 12) & 0xf];
  *p++ = pTbl[(val16 >>  8) & 0xf];
  *p++ = pTbl[(val16 >>  4) & 0xf];
  *p++ = pTbl[(val16 >>  0) & 0xf];
  val16 = imu->acc.z;
  *p++ = pTbl[(val16 >> 12) & 0xf];
  *p++ = pTbl[(val16 >>  8) & 0xf];
  *p++ = pTbl[(val16 >>  4) & 0xf];
  *p++ = pTbl[(val16 >>  0) & 0xf];
  *p++ = ' ';

  /* gyro */
  val16 = imu->gyro.x;
  *p++ = pTbl[(val16 >> 12) & 0xf];
  *p++ = pTbl[(val16 >>  8) & 0xf];
  *p++ = pTbl[(val16 >>  4) & 0xf];
  *p++ = pTbl[(val16 >>  0) & 0xf];
  val16 = imu->gyro.y;
  *p++ = pTbl[(val16 >> 12) & 0xf];
  *p++ = pTbl[(val16 >>  8) & 0xf];
  *p++ = pTbl[(val16 >>  4) & 0xf];
  *p++ = pTbl[(val16 >>  0) & 0xf];
  val16 = imu->gyro.z;
  *p++ = pTbl[(val16 >> 12) & 0xf];
  *p++ = pTbl[(val16 >>  8) & 0xf];
  *p++ = pTbl[(val16 >>  4) & 0xf];
  *p++ = pTbl[(val16 >>  0) & 0xf];

  *p++ = ' ';

  /* temp x4  -20deg -- 107.5deg -> 0 -- 255 */
  val = ((int16_t)imu->temp4x >> 1) + 40;
  if(val < 0) val = 0;
  if(val > 215) val = 215;
  *p++ = pTbl[(val >>  4) & 0xf];
  *p++ = pTbl[(val >>  0) & 0xf];
  *p++ = ' ';

#if 0
  /* timestamp in imu */
  val32 = imu->ts;
  *p++ = pTbl[(val32 >> 28) & 0xf];
  *p++ = pTbl[(val32 >> 24) & 0xf];
  *p++ = pTbl[(val32 >> 20) & 0xf];
  *p++ = pTbl[(val32 >> 16) & 0xf];
  *p++ = pTbl[(val32 >> 12) & 0xf];
  *p++ = pTbl[(val32 >>  8) & 0xf];
  *p++ = pTbl[(val32 >>  4) & 0xf];
  *p++ = pTbl[(val32 >>  0) & 0xf];
  *p++ = ' ';
#endif

  /* sum */
  *p++ = 's';
  *p++ = 's';
  *p++ = ' ';

  *p++ = '\n';
  *p++ = '\0';

  return p - str;
}
int
ImuBuildHamming(int unit, imuValue_t *imu, uint8_t *str, int format)
{
  uint8_t       *p;
  int           n;
  register uint16_t     val16;
  register uint32_t     val32;
  register uint64_t     val64;

  const static uint8_t  *pTbl;

  uint8_t               msb = 0;
  uint32_t              sum = 0;

  p = str;

  pTbl = &imuHamming[0];
  msb = 0x80;

  /* time stamps */
  val64 = imu->t;
  *p++ = pTbl[(val64 >>  0) & 0xf] ^ msb;
  *p++ = pTbl[(val64 >>  4) & 0xf];
  *p++ = pTbl[(val64 >>  8) & 0xf];
  *p++ = pTbl[(val64 >> 12) & 0xf];
  *p++ = pTbl[(val64 >> 16) & 0xf];
  *p++ = pTbl[(val64 >> 20) & 0xf];
  *p++ = pTbl[(val64 >> 24) & 0xf];

  /* unit */
  *p++ = pTbl[unit & 0xf];

  /* cnt */
  val32 = imu->cnt;
  *p++ = pTbl[(val32 >>  0) & 0xf];
  *p++ = pTbl[(val32 >>  4) & 0xf];

  /* capability */
  if(format == MAIN_OUTPUT_FORMAT_HAMMING_CRC32_8) {
    imu->capability |= IMU_CAP_CRC32_8;
  } else if(format == MAIN_OUTPUT_FORMAT_HAMMING_CRC32) {
    imu->capability |= IMU_CAP_CRC32;
  }
  val16 = imu->capability;
  *p++ = pTbl[(val16 >>  0) & 0xf];
  *p++ = pTbl[(val16 >>  4) & 0xf];
  *p++ = pTbl[(val16 >>  8) & 0xf];
  *p++ = pTbl[(val16 >> 12) & 0xf];

  /* accel */
  if(imu->capability & IMU_CAP_ACCEL) {
    val16 = imu->acc.x;
    *p++ = pTbl[(val16 >>  0) & 0xf];
    *p++ = pTbl[(val16 >>  4) & 0xf];
    *p++ = pTbl[(val16 >>  8) & 0xf];
    *p++ = pTbl[(val16 >> 12) & 0xf];
    val16 = imu->acc.y;
    *p++ = pTbl[(val16 >>  0) & 0xf];
    *p++ = pTbl[(val16 >>  4) & 0xf];
    *p++ = pTbl[(val16 >>  8) & 0xf];
    *p++ = pTbl[(val16 >> 12) & 0xf];
    val16 = imu->acc.z;
    *p++ = pTbl[(val16 >>  0) & 0xf];
    *p++ = pTbl[(val16 >>  4) & 0xf];
    *p++ = pTbl[(val16 >>  8) & 0xf];
    *p++ = pTbl[(val16 >> 12) & 0xf];
  }

  /* gyro */
  if(imu->capability & IMU_CAP_GYRO) {
    val16 = imu->gyro.x;
    *p++ = pTbl[(val16 >>  0) & 0xf];
    *p++ = pTbl[(val16 >>  4) & 0xf];
    *p++ = pTbl[(val16 >>  8) & 0xf];
    *p++ = pTbl[(val16 >> 12) & 0xf];
    val16 = imu->gyro.y;
    *p++ = pTbl[(val16 >>  0) & 0xf];
    *p++ = pTbl[(val16 >>  4) & 0xf];
    *p++ = pTbl[(val16 >>  8) & 0xf];
    *p++ = pTbl[(val16 >> 12) & 0xf];
    val16 = imu->gyro.z;
    *p++ = pTbl[(val16 >>  0) & 0xf];
    *p++ = pTbl[(val16 >>  4) & 0xf];
    *p++ = pTbl[(val16 >>  8) & 0xf];
    *p++ = pTbl[(val16 >> 12) & 0xf];
  }

  /* temp x4 */
  if(imu->capability & IMU_CAP_TEMPERATURE) {
    val16 = imu->temp4x;
    *p++ = pTbl[(val16 >>  0) & 0xf];
    *p++ = pTbl[(val16 >>  4) & 0xf];
  }

#if 0
  /* timestamp in imu */
  val32 = imu->ts;
  *p++ = pTbl[(val32 >>  0) & 0xf];
  *p++ = pTbl[(val32 >>  4) & 0xf];
  *p++ = pTbl[(val32 >>  8) & 0xf];
  *p++ = pTbl[(val32 >> 12) & 0xf];
  *p++ = pTbl[(val32 >> 16) & 0xf];
  *p++ = pTbl[(val32 >> 20) & 0xf];
  *p++ = pTbl[(val32 >> 24) & 0xf];
  *p++ = pTbl[(val32 >> 28) & 0xf];
  sum ^= val32 >> 24;
  sum ^= val32 >> 16;
  sum ^= val32 >>  8;
  sum ^= val32 >>  0;
#endif

  if(format == MAIN_OUTPUT_FORMAT_HAMMING_CODE) {
    sum ^=  imu->t >>  0;
    sum ^=  imu->t >>  8;
    sum ^=  imu->t >> 16;
    sum ^=  (imu->t >> 24) & 0x0f;
    sum ^= (unit << 4) & 0xf0;
    sum ^= imu->cnt >> 0;
    sum ^= imu->cnt >> 8;
    sum ^= imu->acc.x >> 0;
    sum ^= imu->acc.x >> 8;
    sum ^= imu->acc.y >> 0;
    sum ^= imu->acc.y >> 8;
    sum ^= imu->acc.z >> 0;
    sum ^= imu->acc.z >> 8;
    sum ^= imu->gyro.x >> 0;
    sum ^= imu->gyro.x >> 8;
    sum ^= imu->gyro.y >> 0;
    sum ^= imu->gyro.y >> 8;
    sum ^= imu->gyro.z >> 0;
    sum ^= imu->gyro.z >> 8;
    sum ^= imu->temp4x >> 0;

    /* sum */
    val32 = sum;
    *p++ = pTbl[(val32 >>  0) & 0xf];
    *p++ = pTbl[(val32 >>  4) & 0xf];

  } if(format == MAIN_OUTPUT_FORMAT_HAMMING_CRC32 ||
       format == MAIN_OUTPUT_FORMAT_HAMMING_CRC32_8) {
    uint16_t     val;

    DevCrcSetInit(0xffffffff);

    DevCrcCalc16(__REV16(imu->t & 0xffff));
    val = ((imu->t >> 16) & 0x0fff) | ((unit << 12) & 0xf000);
    DevCrcCalc16(__REV16(val));
    DevCrcCalc8(imu->cnt & 0xff);
    DevCrcCalc16(__REV16(imu->capability));
    if(imu->capability & IMU_CAP_ACCEL) {
      DevCrcCalc16(__REV16(imu->acc.x));
      DevCrcCalc16(__REV16(imu->acc.y));
      DevCrcCalc16(__REV16(imu->acc.z));
    }
    if(imu->capability & IMU_CAP_GYRO) {
      DevCrcCalc16(__REV16(imu->gyro.x));
      DevCrcCalc16(__REV16(imu->gyro.y));
      DevCrcCalc16(__REV16(imu->gyro.z));
    }
    if(imu->capability & IMU_CAP_TEMPERATURE) {
      DevCrcCalc8(imu->temp4x);
    }

    /* sum */
    val32 = ~DevCrcGetValue();
    *p++ = pTbl[(val32 >>  0) & 0xf];
    *p++ = pTbl[(val32 >>  4) & 0xf];
    if(format == MAIN_OUTPUT_FORMAT_HAMMING_CRC32) {
      *p++ = pTbl[(val32 >>  8) & 0xf];
      *p++ = pTbl[(val32 >> 12) & 0xf];
      *p++ = pTbl[(val32 >> 16) & 0xf];
      *p++ = pTbl[(val32 >> 20) & 0xf];
      *p++ = pTbl[(val32 >> 24) & 0xf];
      *p++ = pTbl[(val32 >> 28) & 0xf];
    }

  } else {      /* MAIN_OUTPUT_FORMAT_HAMMING_CRC8 */
    DevCrcSetInit(0xffffffff);

    DevCrcCalc16(__REV16(imu->t & 0xffff));
    DevCrcCalc8((imu->t >> 16) & 0xff);
    DevCrcCalc8(unit);
    DevCrcCalc16(__REV16(imu->cnt));
    DevCrcCalc16(__REV16(imu->capability));
    DevCrcCalc16(__REV16(imu->acc.x));
    DevCrcCalc16(__REV16(imu->acc.y));
    DevCrcCalc16(__REV16(imu->acc.z));
    DevCrcCalc16(__REV16(imu->gyro.x));
    DevCrcCalc16(__REV16(imu->gyro.y));
    DevCrcCalc16(__REV16(imu->gyro.z));
    DevCrcCalc8(imu->temp4x);

    /* sum */
    val32 = ~DevCrcGetValue();
    *p++ = pTbl[(val32 >>  0) & 0xf];
    *p++ = pTbl[(val32 >>  4) & 0xf];
    *p++ = pTbl[(val32 >>  8) & 0xf];
    *p++ = pTbl[(val32 >> 12) & 0xf];
  }

  return p - str;
}
