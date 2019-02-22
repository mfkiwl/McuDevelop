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

#ifndef _ADXL345_H_
#define _ADXL345_H_

#define ADXL345_READ_WRITE_MASK  (1<<7)
#define ADXL345_READ             (1<<7)
#define ADXL345_WRITE            (0<<7)
#define ADXL345_MB               (1<<6)

#define ADXL345_REG_DEVID            0x00
#define         DEVID_ADXL345                   0xe5

#define ADXL345_BW_RATE              0x2c
#define         BW_RATE_RATE_SHIFT              (0)
#define         BW_RATE_RATE_MASK               (0xf<<(BW_RATE_RATE_SHIFT))
#define         BW_RATE_RATE_0_1HZ              ( 0<<(BW_RATE_RATE_SHIFT))
#define         BW_RATE_RATE_100HZ              (10<<(BW_RATE_RATE_SHIFT))
#define         BW_RATE_RATE_200HZ              (11<<(BW_RATE_RATE_SHIFT))
#define         BW_RATE_RATE_400HZ              (12<<(BW_RATE_RATE_SHIFT))
#define         BW_RATE_RATE_800HZ              (13<<(BW_RATE_RATE_SHIFT))
#define         BW_RATE_RATE_1600HZ             (14<<(BW_RATE_RATE_SHIFT))
#define         BW_RATE_RATE_3200HZ             (15<<(BW_RATE_RATE_SHIFT))

#define ADXL345_POWER_CTL            0x2d
#define         POWER_CTL_MEASURE_SHIFT         (3)
#define         POWER_CTL_MEASURE_MASK          (1<<(POWER_CTL_MEASURE_SHIFT))
#define         POWER_CTL_MEASURE_DIS           (0<<(POWER_CTL_MEASURE_SHIFT))
#define         POWER_CTL_MEASURE_EN            (1<<(POWER_CTL_MEASURE_SHIFT))
#define         POWER_CTL_SLEEP                 (1<<2)
#define         POWER_CTL_WAKEUP_SHIFT          (0)
#define         POWER_CTL_WAKEUP_MASK           (3<<(POWER_CTL_WAKEUP_SHIFT))
#define         POWER_CTL_WAKEUP_8HZ            (0<<(POWER_CTL_WAKEUP_SHIFT))
#define         POWER_CTL_WAKEUP_4HZ            (1<<(POWER_CTL_WAKEUP_SHIFT))
#define         POWER_CTL_WAKEUP_2HZ            (2<<(POWER_CTL_WAKEUP_SHIFT))
#define         POWER_CTL_WAKEUP_1HZ            (3<<(POWER_CTL_WAKEUP_SHIFT))

#define ADXL345_REG_INT_ENABLE       0x2e
#define         INT_ENABLE_DATA_READY           (1<<7)
#define ADXL345_REG_INT_MAP          0x2f
#define ADXL345_REG_INT_SOURCE       0x30

#define ADXL345_REG_DATA_FORMAT      0x31
#define         DATA_FORMAT_INT_INVERT_SHIFT   (5)
#define         DATA_FORMAT_INT_INVERT_MASK    (1<<(DATA_FORMAT_INT_INVERT_SHIFT))
#define         DATA_FORMAT_INT_INVERT_HIGH    (0<<(DATA_FORMAT_INT_INVERT_SHIFT))
#define         DATA_FORMAT_INT_INVERT_LOW     (1<<(DATA_FORMAT_INT_INVERT_SHIFT))
#define         DATA_FORMAT_JUSTIFY_SHIFT      (2)
#define         DATA_FORMAT_JUSTIFY_MASK       (1<<(DATA_FORMAT_JUSTIFY_SHIFT))
#define         DATA_FORMAT_JUSTIFY_LSB        (0<<(DATA_FORMAT_JUSTIFY_SHIFT))
#define         DATA_FORMAT_JUSTIFY_MSB        (1<<(DATA_FORMAT_JUSTIFY_SHIFT))
#define         DATA_FORMAT_RANGE_SHIFT        (0)
#define         DATA_FORMAT_RANGE_MASK         (3<<(DATA_FORMAT_RANGE_SHIFT))
#define         DATA_FORMAT_RANGE_PM2G         (0<<(DATA_FORMAT_RANGE_SHIFT))
#define         DATA_FORMAT_RANGE_PM4G         (1<<(DATA_FORMAT_RANGE_SHIFT))
#define         DATA_FORMAT_RANGE_PM8G         (2<<(DATA_FORMAT_RANGE_SHIFT))
#define         DATA_FORMAT_RANGE_PM16G        (3<<(DATA_FORMAT_RANGE_SHIFT))
#define ADXL345_REG_DATAX_LOW        0x32
#define ADXL345_REG_DATAX_HIGH       0x33
#define ADXL345_REG_DATAY_LOW        0x34
#define ADXL345_REG_DATAY_HIGH       0x35
#define ADXL345_REG_DATAZ_LOW        0x36
#define ADXL345_REG_DATAZ_HIGH       0x37


int     Adxl345Probe(int unit);
int     Adxl345Init(int unit);
int     Adxl345RecvValue(int unit, imuValue_t *p);
int     Adxl345ReadValue(int unit, imuValue_t *p);
void    Adxl345SetValue(int unit, int reg, uint8_t val);
void    Adxl345GetValue(int unit, int reg, uint8_t *ptr, int size);

#ifdef  _ADXL345_H_
#endif

#endif
