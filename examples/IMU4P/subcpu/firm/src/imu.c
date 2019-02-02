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
#include        "bmi160.h"
#include        "gpio.h"
#include        "devSpi16.h"

#include        "imu.h"

int
ImuInit(int unit)
{
  uint8_t       reg, val;

  /* interface mode */
  reg = BMI160_REG_IF_CONF;
  val = IF_CONF_MODE_AUTO_2NDOFF | IF_CONF_SPI3_NO4;
  Bmi160SetValue(unit, reg, val);

  /* start foc */
  reg = BMI160_REG_CMD;
  val = 0x03;
  Bmi160SetValue(unit, reg, val);

  //wait 10 msec
  SystemWaitCounter(10*4);      /*adhoc */

  /* enable accel measurement */
  reg = BMI160_REG_CMD;
  val = 0x11;
  Bmi160SetValue(unit, reg, val);

  //wait 1 msec
  SystemWaitCounter(1*4);      /*adhoc */

  /* enable gyroscope measurement */
  reg = BMI160_REG_CMD;
  val = 0x15;
  Bmi160SetValue(unit, reg, val);

  //wait 50 msec
  SystemWaitCounter(50*4);      /*adhoc */

  /* [5:4]band width 2=800Hz,
   * [3:0]data rate: a=400Hz,b=800,c=1600,d=3200
   * range: 3=2G,5=4G,c=16G
   */
  reg = 0x40;
  val = 0x2c;
  Bmi160SetValue(unit, reg, val);
  reg = 0x41;
  val = 0x03;
  Bmi160SetValue(unit, reg, val);

  /* [5:4]acc_bwp: = 800Hz,
   * [3:0]data rate: a=400Hz,b=800,c=1600,d=3200
   * range: 0=2kdeg/s,1=1k,2=500,3=250,4=125
   */
  reg = 0x42;
  val = 0x2c;
  Bmi160SetValue(unit, reg, val);
  reg = 0x41;
  val = 0x03;
  Bmi160SetValue(unit, reg, val);

  /* interrupt  data ready, map int1, */
  reg = 0x51;
  val = 0x10;
  Bmi160SetValue(unit, reg, val);
  reg = 0x56;
  val = 0x80;
  Bmi160SetValue(unit, reg, val);
  /* step counter */
  reg = 0x52;
  val = 0x38;
  Bmi160SetValue(unit, reg, val);
#if 0
  reg = 0x55;
  val = 0x01;
  Bmi160SetValue(unit, reg, val);
#endif
#if 1
  /* enable interrupt */
  reg = 0x53;
  val = 0x0b;
  Bmi160SetValue(unit, reg, val);
#endif
  return 0;
}



int
ImuReadValue(int unit)
{
  uint8_t       cmd;
  uint8_t       buf[32];
  uint32_t      reg = BMI160_REG_CHIP_ID | BMI160_READ;
  uint32_t      val = 0xaa55ddbb;

  Bmi160GetValue(unit, 0x0c, buf, 1);

  return 0;
}


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


void
Bmi160SetValue(int unit, int reg, uint8_t val)
{
  uint8_t       buf[2];
  buf[0] = reg & ~BMI160_READ_WRITE_MASK;  /* read */
  buf[1] = val;

  ImuEnableCs(unit);
  DevSpiSend(1, buf, 2);
  ImuDisableCs(unit);

  return;
}

void
Bmi160GetValue(int unit, int reg, uint8_t *ptr, int size)
{
  uint8_t       cmd;

  cmd = reg | BMI160_READ;

  ImuEnableCs(unit);
  DevSpiSend(1, &cmd, 1);
  DevSpiRecv(1,  ptr, size);
  ImuDisableCs(unit);

  return;
}


