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

#define _ADXL345_C_

#include        <stdint.h>

#include        "config.h"
#include        "devErrno.h"
#include        "main.h"
#include        "imu.h"

#include        "adxl345.h"


/************************************************************
 * Adxl345Probe()
 */
int
Adxl345Probe(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  int           re;
  uint8_t       c;

  re = ImuGetValueStandard(unit, ADXL345_REG_DEVID, &c, 1);
#if CONFIG_IMU_WAIT_DMA_NONBLOCK
  if(re == DEV_ERRNO_NONBLOCK) {
    while(ImuIsFinishReceiving(unit) != IMU_ERRNO_SUCCESS);
  }
#endif

  if(c == DEVID_ADXL345) {
#if CONFIG_IMU_DEBUG_PROBE
    printf("# imu probed unit: %d ADXL345\n", unit);
#endif
    result = IMU_ERRNO_SUCCESS;
  }

  return result;
}


/************************************************************
 * Adxl345Init()
 */
int
Adxl345Init(int unit)
{
  int           result = IMU_ERRNO_UNKNOWN;
  uint8_t       reg, val;
  imuValue_t    imu;

  /* disable interrupt */
  reg = ADXL345_REG_INT_ENABLE;
  val = 0;
  ImuSetValueStandard(unit, reg, val);

  /* power ctl   off first */
  reg = ADXL345_POWER_CTL;
  val = 0x00;
  ImuSetValueStandard(unit, reg, val);

  /* dummy read for clear the interrupt */
  Adxl345ReadValue(unit, &imu);

  SystemWaitCounter(2);

  /* bw rate */
  reg = ADXL345_BW_RATE;
  val = BW_RATE_RATE_100HZ + (CONFIG_IMU_RATE_DEFAULT);
  ImuSetValueStandard(unit, reg, val);

  /* SPI 4lines */
  reg = ADXL345_REG_DATA_FORMAT;
  val = DATA_FORMAT_INT_INVERT_HIGH | DATA_FORMAT_RANGE_PM16G;
  ImuSetValueStandard(unit, reg, val);

  /* interrupt  data ready, map int1, */
  reg = ADXL345_REG_INT_MAP;
  val = 0x00;           /* mapt to INT1 */
  ImuSetValueStandard(unit, reg, val);

  /* enable interrupt */
  reg = ADXL345_REG_INT_ENABLE;
  val = INT_ENABLE_DATA_READY;
  ImuSetValueStandard(unit, reg, val);

  /* power ctl */
  reg = ADXL345_POWER_CTL;
  val = POWER_CTL_MEASURE_EN;
  ImuSetValueStandard(unit, reg, val);

  return result;
}


#define XL      ((ADXL345_REG_DATAX_LOW )-(ADXL345_REG_DATAX_LOW))
#define XH      ((ADXL345_REG_DATAX_HIGH)-(ADXL345_REG_DATAX_LOW))
#define YL      ((ADXL345_REG_DATAY_LOW )-(ADXL345_REG_DATAX_LOW))
#define YH      ((ADXL345_REG_DATAY_HIGH)-(ADXL345_REG_DATAX_LOW))
#define ZL      ((ADXL345_REG_DATAZ_LOW )-(ADXL345_REG_DATAX_LOW))
#define ZH      ((ADXL345_REG_DATAZ_HIGH)-(ADXL345_REG_DATAX_LOW))
int
Adxl345RecvValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  result = ImuGetValueStandard(unit, ADXL345_MB | ADXL345_REG_DATAX_LOW, buf, 6);
  if(result == DEV_ERRNO_NONBLOCK) goto fail;

#if 0
  p->acc.x = (buf[XH] << 8) | buf[XL];
  p->acc.y = (buf[YH] << 8) | buf[YL];
  p->acc.z = (buf[ZH] << 8) | buf[ZL];
  p->gyro.x = 0;
  p->gyro.y = 0;
  p->gyro.z = 0;

  p->ts = 0;
  p->temp4x = 0;
#endif
  Adxl345ReadValue(unit, p);

  result = DEV_ERRNO_SUCCESS;

fail:
  return 0;
}


int
Adxl345ReadValue(int unit, imuValue_t *p)
{
  int           result;
  uint8_t       cmd;
  uint8_t       *buf;
  uint16_t      temp;

  if(!p) goto fail;

  buf = p->raw;

  result = ImuGetValueStandard(unit, ADXL345_MB | ADXL345_REG_DATAX_LOW, buf, 6);
  if(result == DEV_ERRNO_NONBLOCK) goto fail;

  p->acc.x = (buf[XH] << 8) | buf[XL];
  p->acc.y = (buf[YH] << 8) | buf[YL];
  p->acc.z = (buf[ZH] << 8) | buf[ZL];
  p->gyro.x = 0;
  p->gyro.y = 0;
  p->gyro.z = 0;

  p->ts = 0;
  p->temp4x = 0;

  result = DEV_ERRNO_SUCCESS;

fail:
  return 0;
}
