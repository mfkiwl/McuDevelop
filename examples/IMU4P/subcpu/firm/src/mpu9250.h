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

#ifndef _MPU9250_H_
#define _MPU9250_H_

#define MPU9250_READ_WRITE_MASK  (1<<7)
#define MPU9250_READ             (1<<7)
#define MPU9250_WRITE            (0<<7)

#if 0
#define MPU9250_REG_IF_CONF          0x6b
#define         IF_CONF_MODE_SHIFT      4
#define         IF_CONF_MODE_MASK       (1<<I(F_CONF_MODE_SHIFT))
#define         IF_CONF_MODE_AUTO_2NDOFF (0<<(IF_CONF_MODE_SHIFT))
#define         IF_CONF_MODE_I2C_2NDOIS  (1<<(IF_CONF_MODE_SHIFT))
#define         IF_CONF_MODE_AUTO_2NDMAG (2<<(IF_CONF_MODE_SHIFT))
#define         IF_CONF_SPI3_SHIFT      0
#define         IF_CONF_SPI3_MASK       (1<<(IF_CONF_SPI3_SHIFT))
#define         IF_CONF_SPI3_NO4        (0<<(IF_CONF_SPI3_SHIFT))
#define         IF_CONF_SPI3_YES        (1<<(IF_CONF_SPI3_SHIFT))
#endif

#define MPU9250_REG_GYRO_CONFIG         0x1c
#define         GYRO_CONFIG_GYRO_FS_SEL_SHIFT   3
#define         GYRO_CONFIG_GYRO_FS_SEL_MASK    (3<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_250DPS  (0<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_500DPS  (1<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_1000DPS (2<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_2000DPS (3<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_FCHOICE_SHIFT       0
#define         GYRO_CONFIG_FCHOICE_MASK        (3<<(GYRO_CONFIG_FCHOICE_SHIFT))
//#define         GYRO_CONFIG_FCHOICE_        (<<(GYRO_CONFIG_FCHOICE_SHIFT))

#define MPU9250_REG_ACCEL_CONFIG        0x1c
#define         ACCEL_CONFIG_FS_SEL_SHIFT       3
#define         ACCEL_CONFIG_FS_SEL_MASK        (3<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM2G        (0<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM4G        (1<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM8G        (2<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM16G       (3<<(ACCEL_CONFIG_FS_SEL_SHIFT))

#define MPU9250_REG_ACCEL_CONFIG2       0x1d
#define         ACCEL_CONFIG2_FCHOICE_SHIFT     3
#define         ACCEL_CONFIG2_FCHOICE_MASK      (1<<(ACCEL_CONFIG2_FCHOICE_SHIFT))
#define         ACCEL_CONFIG2_FCHOICE_NO        (0<<(ACCEL_CONFIG2_FCHOICE_SHIFT))
#define         ACCEL_CONFIG2_FCHOICE_1046HZ    (0<<(ACCEL_CONFIG2_FCHOICE_SHIFT))
#define         ACCEL_CONFIG2_FCHOICE_YES       (1<<(ACCEL_CONFIG2_FCHOICE_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_SHIFT   0
#define         ACCEL_CONFIG2_A_DLPFCFG_MASK    (7<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_218HZ   (0<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
//#define         ACCEL_CONFIG2_A_DLPFCFG_218HZ   (1<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_99HZ    (2<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_44HZ    (3<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_21HZ    (4<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_10HZ    (5<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_5HZ     (6<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_420HZ   (7<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))

#define MPU9250_REG_INT_PIN             0x37
#define         INT_PIN_ACTL_SHIFT              7
#define         INT_PIN_ACTL_MASK               (1<<(INT_PIN_ACTL_SHIFT))
#define         INT_PIN_ACTL_HIGH               (0<<(INT_PIN_ACTL_SHIFT))
#define         INT_PIN_ACTL_LOW                (1<<(INT_PIN_ACTL_SHIFT))
#define         INT_PIN_OPEN_SHIFT              6
#define         INT_PIN_OPEN_MASK               (1<<(INT_PIN_OPEN_SHIFT))
#define         INT_PIN_OPEN_PUSHPULL           (0<<(INT_PIN_OPEN_SHIFT))
#define         INT_PIN_OPEN_OPENDRAIN          (1<<(INT_PIN_OPEN_SHIFT))
#define         INT_PIN_LATCH_INT_EN_SHIFT      5
#define         INT_PIN_LATCH_INT_EN_MASK       (1<<(INT_PIN_LATCH_INT_EN_SHIFT))
#define         INT_PIN_LATCH_INT_EN_EDGE       (0<<(INT_PIN_LATCH_INT_EN_SHIFT))
#define         INT_PIN_LATCH_INT_EN_STATE      (1<<(INT_PIN_LATCH_INT_EN_SHIFT))
#define         INT_PIN_INT_ANYRD_2CLEAR_SHIFT   4
#define         INT_PIN_INT_ANYRD_2CLEAR_MASK    (1<<(INT_PIN_INT_ANYRD_2CLEAR_SHIFT))
#define         INT_PIN_INT_ANYRD_2CLEAR_INT_STATUS  (0<<(INT_PIN_INT_ANYRD_2CLEAR_SHIFT))
#define         INT_PIN_INT_ANYRD_2CLEAR_READ   (1<<(INT_PIN_INT_ANYRD_2CLEAR_SHIFT))
#define         INT_PIN_ACTL_FSYNC_SHIFT        3
#define         INT_PIN_ACTL_FSYNC_MASK         (1<<(INT_PIN_ACTL_FSYNC_SHIFT))
#define         INT_PIN_ACTL_FSYNC_HIGH         (0<<(INT_PIN_ACTL_FSYNC_SHIFT))
#define         INT_PIN_ACTL_FSYNC_LOW          (1<<(INT_PIN_ACTL_FSYNC_SHIFT))
#define         INT_PIN_FSYNC_INT_MODE_EN_SHIFT 2
#define         INT_PIN_FSYNC_INT_MODE_EN_MASK  (1<<(INT_PIN_FSYNC_INT_MODE_EN_SHIFT))
#define         INT_PIN_FSYNC_INT_MODE_EN_NO    (0<<(INT_PIN_FSYNC_INT_MODE_EN_SHIFT))
#define         INT_PIN_FSYNC_INT_MODE_EN_THROUGH   (1<<(INT_PIN_FSYNC_INT_MODE_EN_SHIFT))
#define         INT_PIN_BYPASS_EN_SHIFT         1
#define         INT_PIN_BYPASS_EN_MASK          (1<<(INT_PIN_BYPASS_EN_SHIFT))
#define         INT_PIN_BYPASS_EN_NO            (0<<(INT_PIN_BYPASS_EN_SHIFT))
#define         INT_PIN_BYPASS_EN_YES           (1<<(INT_PIN_BYPASS_EN_SHIFT))

#define MPU9250_REG_INT_ENABLE          0x38
#define         INT_ENABLE_WOM_EN_SHIFT        6
#define         INT_ENABLE_WOM_EN_MASK         (1<<(INT_ENABLE_WOM_EN_SHIFT))
#define         INT_ENABLE_WOM_EN_NO           (0<<(INT_ENABLE_WOM_EN_SHIFT))
#define         INT_ENABLE_WOM_EN_YES          (1<<(INT_ENABLE_WOM_EN_SHIFT))
#define         INT_ENABLE_FIFO_OVERFLOW_EN_SHIFT       4
#define         INT_ENABLE_FIFO_OVERFLOW_EN_MASK        (1<<(INT_ENABLE_FIFO_OVERFLOW_EN_SHIFT))
#define         INT_ENABLE_FIFO_OVERFLOW_EN_NO          (0<<(INT_ENABLE_FIFO_OVERFLOW_EN_SHIFT))
#define         INT_ENABLE_FIFO_OVERFLOW_EN_YES         (1<<(INT_ENABLE_FIFO_OVERFLOW_EN_SHIFT))
#define         INT_ENABLE_FSYNC_INT_EN_SHIFT   3
#define         INT_ENABLE_FSYNC_INT_EN_MASK    (1<<(INT_ENABLE_FSYNC_INT_EN_SHIFT))
#define         INT_ENABLE_FSYNC_INT_EN_NO      (0<<(INT_ENABLE_FSYNC_INT_EN_SHIFT))
#define         INT_ENABLE_FSYNC_INT_EN_YES     (1<<(INT_ENABLE_FSYNC_INT_EN_SHIFT))
#define         INT_ENABLE_RAW_RDY_EN_SHIFT     0
#define         INT_ENABLE_RAW_RDY_EN_MASK      (1<<(INT_ENABLE_RAW_RDY_EN_SHIFT))
#define         INT_ENABLE_RAW_RDY_EN_NO        (0<<(INT_ENABLE_RAW_RDY_EN_SHIFT))
#define         INT_ENABLE_RAW_RDY_EN_YES       (1<<(INT_ENABLE_RAW_RDY_EN_SHIFT))


#define MPU9250_REG_ACCEL_XOUT_L        0x3b
#define MPU9250_REG_ACCEL_XOUT_H        0x3c
#define MPU9250_REG_ACCEL_YOUT_L        0x3d
#define MPU9250_REG_ACCEL_YOUT_H        0x3e
#define MPU9250_REG_ACCEL_ZOUT_L        0x3f
#define MPU9250_REG_ACCEL_ZOUT_H        0x40
#define MPU9250_REG_TEMP_OUT_L          0x41
#define MPU9250_REG_TEMP_OUT_H          0x42
#define MPU9250_REG_GYRO_XOUT_L         0x43
#define MPU9250_REG_GYRO_XOUT_H         0x44
#define MPU9250_REG_GYRO_YOUT_L         0x45
#define MPU9250_REG_GYRO_YOUT_H         0x46
#define MPU9250_REG_GYRO_ZOUT_L         0x47
#define MPU9250_REG_GYRO_ZOUT_H         0x48

#define MPU9250_REG_MOT_DETECT_CTRL     0x69
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT    7
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_MASK     (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_NO       (0<<(MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_YES      (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT  6
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_MASK   (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_NO     (0<<(MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_YES    (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT))

#define MPU9250_REG_PWR_MGMT1        0x6b
#define MPU9250_REG_PWR_MGMT2        0x6c

#define MPU9250_REG_WHOAMI           0x75
#define         WHOAMI_VALUE                    0x71


int     Mpu9250Probe(int unit);
int     Mpu9250Init(int unit);
int     Mpu9250ReadValue(int unit, imuValue_t *p);
void    Mpu9250SetValue(int unit, int reg, uint8_t val);
void    Mpu9250GetValue(int unit, int reg, uint8_t *ptr, int size);

#ifdef  _MPU9250_H_
#endif

#endif
