/*
 * Copyright (c) 2018 zhtlab
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

#ifndef _IMU_H_
#define _IMU_H_

typedef struct {
  uint16_t      x;
  uint16_t      y;
  uint16_t      z;
} imuAxis_t;

typedef struct {
  imuAxis_t     acc;
  imuAxis_t     gyro;
  uint32_t      ts;
  uint16_t      temp4x;
  float         tempf;
} imuValue_t;



int             ImuInit(int unit);
int             ImuReadValue(int unit, imuValue_t *p);
void            ImuEnableCs(int unit);
void            ImuDisableCs(int unit);

void            Bmi160SetValue(int unit, int reg, uint8_t val);
void            Bmi160GetValue(int unit, int reg, uint8_t *ptr, int size);

#ifdef  _IMU_C_
#endif


#endif
