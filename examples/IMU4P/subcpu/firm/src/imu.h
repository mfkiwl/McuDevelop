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

#define IMU_ERRNO_SUCCESS       0
#define IMU_ERRNO_UNKNOWN       (-1)
#define IMU_ERRNO_UNIT          (-2)


#define IMU_SPI_READ_WRITE_MASK (1<<7)
#define IMU_SPI_READ            (1<<7)
#define IMU_SPI_WRITE           (0<<7)


typedef struct {
  uint16_t      x;
  uint16_t      y;
  uint16_t      z;
} imuAxis_t;

typedef struct {
  uint32_t      cnt;
  imuAxis_t     acc;
  imuAxis_t     gyro;
  uint32_t      ts;
  uint16_t      temp4x;
} imuValue_t;


/* full scale range */
typedef struct {
#define IMU_SETTING_INVALID     (0xffff)
  uint16_t      acc_fs;         /* in 10mG */
  uint16_t      gyro_fs;        /* in 10DPS */
  uint16_t      mag_fs;         /* in mT */
  uint16_t      freq;           /* in Hz */
  uint32_t      reserved[14];
} imuSetting_t;


int             ImuProbe(int unit);
int             ImuInit(int unit);
int             ImuReadValue(int unit, imuValue_t *p);
int             ImuGetSettings(int unit, imuSetting_t *p);
int             ImuSetSettings(int unit, imuSetting_t *p);
void            ImuEnableCs(int unit);
void            ImuDisableCs(int unit);

void            ImuGenCsPulse(int unit, uint32_t ms);
void            ImuSetValue(int unit, int reg, uint8_t val);
void            ImuGetValue(int unit, int reg, uint8_t *ptr, int size);
void            ImuSetValueStandard(int unit, int reg, uint8_t val);
void            ImuGetValueStandard(int unit, int reg, uint8_t *ptr, int size);
int             ImuBuildText(int unit, uint64_t ts1, uint32_t ts0, imuValue_t *imu, uint8_t *str);


#ifdef  _IMU_C_
#endif


#endif
