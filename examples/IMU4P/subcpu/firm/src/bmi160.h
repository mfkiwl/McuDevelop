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

#ifndef _BMI160_H_
#define _BMI160_H_

#define BMI160_READ_WRITE_MASK  (1<<7)
#define BMI160_READ             (1<<7)
#define BMI160_WRITE            (0<<7)

#define BMI160_REG_CHIP_ID          0x00
#define BMI160_REG_GYRO_X_LOW       0x0c
#define BMI160_REG_GYRO_X_HIGH      0x0d
#define BMI160_REG_GYRO_Y_LOW       0x0e
#define BMI160_REG_GYRO_Y_HIGH      0x0f
#define BMI160_REG_GYRO_Z_LOW       0x10
#define BMI160_REG_GYRO_Z_HIGH      0x11
#define BMI160_REG_ACC_X_LOW        0x12
#define BMI160_REG_ACC_X_HIGH       0x13
#define BMI160_REG_ACC_Y_LOW        0x14
#define BMI160_REG_ACC_Y_HIGH       0x15
#define BMI160_REG_ACC_Z_LOW        0x16
#define BMI160_REG_ACC_Z_HIGH       0x17
#define BMI160_REG_TIME_LOW         0x18
#define BMI160_REG_TIME_MID         0x19
#define BMI160_REG_TIME_HIGH        0x1a
#define BMI160_REG_TEMP_LOW         0x20
#define BMI160_REG_TEMP_HIGH        0x21

#define BMI160_REG_IF_CONF          0x6b
#define         IF_CONF_MODE_SHIFT      4
#define         IF_CONF_MODE_MASK       (1<<IF_CONF_MODE_SHIFT)
#define         IF_CONF_MODE_AUTO_2NDOFF (0<<IF_CONF_MODE_SHIFT)
#define         IF_CONF_MODE_I2C_2NDOIS  (1<<IF_CONF_MODE_SHIFT)
#define         IF_CONF_MODE_AUTO_2NDMAG (2<<IF_CONF_MODE_SHIFT)
#define         IF_CONF_SPI3_SHIFT      0
#define         IF_CONF_SPI3_MASK       (1<<IF_CONF_SPI3_SHIFT)
#define         IF_CONF_SPI3_NO4        (0<<IF_CONF_SPI3_SHIFT)
#define         IF_CONF_SPI3_YES        (1<<IF_CONF_SPI3_SHIFT)


#define BMI160_REG_CMD          0x7e
#define         CMD_PROG_NVM H          0xa0
#define         CMD_FIFO_FLUSH          0xb0
#define         CMD_INT_RESET           0xb1
#define         CMD_SOFTRESET           0xb6

#endif
