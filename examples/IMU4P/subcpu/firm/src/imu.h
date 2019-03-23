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
  uint64_t      t;              /* timestamp of TIM IC */
  uint32_t      cnt;
  uint8_t       raw[64];
  uint16_t      capability;
#define IMU_CAP_EXTENSION    (1<<15)
#define IMU_CAP_CRC32_8      (1<<14)
#define IMU_CAP_CRC32        (1<<13)
#define IMU_CAP_TEMPERATURE  (1<< 3)
#define IMU_CAP_MAGNETIC     (1<< 2)
#define IMU_CAP_GYRO         (1<< 1)
#define IMU_CAP_ACCEL        (1<< 0)
  imuAxis_t     acc;
  imuAxis_t     gyro;
  uint32_t      ts;             /* timestamp of sensor */
  uint16_t      temp4x;         /* 0x00 = 0deg, 0x28 = 10deg */
  uint16_t      flag;
#define       IMU_FLAG_OVERWRITE      (1<<0)
} imuValue_t;


typedef struct {
  int8_t        unit;           /* unit number */
  int8_t        noFunc;         /* function number */

  void          *pConfig;       /* pointer of the device setting */
} imuHandler_t;


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
int             ImuRecvValue(int unit, imuValue_t *p);
int             ImuReadValueNonblockEnd(int unit);
int             ImuIsFinishReceiving(int unit);
int             ImuReadValue(int unit, imuValue_t *p);
int             ImuGetSettings(int unit, imuSetting_t *p);
int             ImuSetSettings(int unit, imuSetting_t *p);
void            ImuEnableCs(int unit);
void            ImuDisableCs(int unit);

void            ImuGenCsPulse(int unit, uint32_t ms);
void            ImuSetValue(int unit, int reg, uint8_t val);
int             ImuGetValue(int unit, int reg, uint8_t *ptr, int size);
void            ImuGetValueNonblockEnd(int unit);
void            ImuSetValueStandard(int unit, int reg, uint8_t val);
int             ImuGetValueStandard(int unit, int reg, uint8_t *ptr, int size);
int             ImuBuildText(int unit, imuValue_t *imu, uint8_t *str, int format);
int             ImuBuildHamming(int unit, imuValue_t *imu, uint8_t *str, int format);

void            ImuDmaInterrupt(void);


#ifdef  _IMU_C_
#endif


#endif
