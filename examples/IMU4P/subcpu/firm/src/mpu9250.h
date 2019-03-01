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


#define MPU9250_REG_SMPLRT              0x19

#define MPU9250_REG_CONFIG              0x1a
#define         CONFIG_DLPF_CFG_SHIFT   0
#define         CONFIG_DLPF_CFG_MASK    (7<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_250HZ   (0<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_184HZ   (1<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_92HZ    (2<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_41HZ    (3<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_20HZ    (4<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_10HZ    (5<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_5HZ     (6<<(CONFIG_DLPF_CFG_SHIFT))
#define         CONFIG_DLPF_CFG_3600HZ  (7<<(CONFIG_DLPF_CFG_SHIFT))

#define MPU9250_REG_GYRO_CONFIG         0x1b
#define         GYRO_CONFIG_GYRO_FS_SEL_SHIFT   3
#define         GYRO_CONFIG_GYRO_FS_SEL_MASK    (3<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_250DPS  (0<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_500DPS  (1<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_1000DPS (2<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_GYRO_FS_SEL_2000DPS (3<<(GYRO_CONFIG_GYRO_FS_SEL_SHIFT))
#define         GYRO_CONFIG_FCHOICEB_SHIFT      0
#define         GYRO_CONFIG_FCHOICEB_MASK       (3<<(GYRO_CONFIG_FCHOICEB_SHIFT))
#define         GYRO_CONFIG_FCHOICEB_0          (0<<(GYRO_CONFIG_FCHOICEB_SHIFT))
#define         GYRO_CONFIG_FCHOICEB_1          (1<<(GYRO_CONFIG_FCHOICEB_SHIFT))
#define         GYRO_CONFIG_FCHOICEB_3          (3<<(GYRO_CONFIG_FCHOICEB_SHIFT))

#define MPU9250_REG_ACCEL_CONFIG        0x1c
#define         ACCEL_CONFIG_FS_SEL_SHIFT       3
#define         ACCEL_CONFIG_FS_SEL_MASK        (3<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM2G        (0<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM4G        (1<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM8G        (2<<(ACCEL_CONFIG_FS_SEL_SHIFT))
#define         ACCEL_CONFIG_FS_SEL_PM16G       (3<<(ACCEL_CONFIG_FS_SEL_SHIFT))

#define MPU9250_REG_ACCEL_CONFIG2       0x1d
#define         ACCEL_CONFIG2_FCHOICEB_SHIFT    2
#define         ACCEL_CONFIG2_FCHOICEB_MASK     (3<<(ACCEL_CONFIG2_FCHOICEB_SHIFT))
#define         ACCEL_CONFIG2_FCHOICEB_0        (0<<(ACCEL_CONFIG2_FCHOICEB_SHIFT))
#define         ACCEL_CONFIG2_FCHOICEB_1046HZ   (0<<(ACCEL_CONFIG2_FCHOICEB_SHIFT))
#define         ACCEL_CONFIG2_FCHOICEB_1        (1<<(ACCEL_CONFIG2_FCHOICEB_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_SHIFT   0
#define         ACCEL_CONFIG2_A_DLPFCFG_MASK    (7<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_218HZ   (0<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
#define         ACCEL_CONFIG2_A_DLPFCFG_218HZ2  (1<<(ACCEL_CONFIG2_A_DLPFCFG_SHIFT))
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
#define         INT_PIN_INT_ANYRD_2CLEAR_SHIFT  4
#define         INT_PIN_INT_ANYRD_2CLEAR_MASK   (1<<(INT_PIN_INT_ANYRD_2CLEAR_SHIFT))
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
#define         INT_ENABLE_WOM_EN_SHIFT         6
#define         INT_ENABLE_WOM_EN_MASK          (1<<(INT_ENABLE_WOM_EN_SHIFT))
#define         INT_ENABLE_WOM_EN_NO            (0<<(INT_ENABLE_WOM_EN_SHIFT))
#define         INT_ENABLE_WOM_EN_YES           (1<<(INT_ENABLE_WOM_EN_SHIFT))
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

#define MPU9250_REG_INT_STATUS          0x3a
#define MPU9250_REG_ACCEL_XOUT_H        0x3b
#define MPU9250_REG_ACCEL_XOUT_L        0x3c
#define MPU9250_REG_ACCEL_YOUT_H        0x3d
#define MPU9250_REG_ACCEL_YOUT_L        0x3e
#define MPU9250_REG_ACCEL_ZOUT_H        0x3f
#define MPU9250_REG_ACCEL_ZOUT_L        0x40
#define MPU9250_REG_TEMP_OUT_H          0x41
#define MPU9250_REG_TEMP_OUT_L          0x42
#define MPU9250_REG_GYRO_XOUT_H         0x43
#define MPU9250_REG_GYRO_XOUT_L         0x44
#define MPU9250_REG_GYRO_YOUT_H         0x45
#define MPU9250_REG_GYRO_YOUT_L         0x46
#define MPU9250_REG_GYRO_ZOUT_H         0x47
#define MPU9250_REG_GYRO_ZOUT_L         0x48

#define MPU9250_REG_MOT_DETECT_CTRL     0x69
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT    7
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_MASK     (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_NO       (0<<(MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_EN_YES      (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_EN_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT  6
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_MASK   (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_NO     (0<<(MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT))
#define         MOT_DETECT_CTRL_ACCEL_INTEL_MODE_YES    (1<<(MOT_DETECT_CTRL_ACCEL_INTEL_MODE_SHIFT))

#define MPU9250_REG_PWR_MGMT1           0x6b
#define         PWR_MGMT1_HRESET_SHIFT          1
#define         PWR_MGMT1_HRESET_MASK           (1<<(PWR_MGMT1_HRESET_SHIFT))
#define         PWR_MGMT1_HRESET_NO             (0<<(PWR_MGMT1_HRESET_SHIFT))
#define         PWR_MGMT1_HRESET_YES            (1<<(PWR_MGMT1_HRESET_SHIFT))
#define MPU9250_REG_PWR_MGMT2           0x6c

#define MPU9250_REG_WHOAMI              0x75
#define         WHOAMI_VALUE                    0x71

#define MPU9250_REG_XA_OFFSET_H         0x77
#define MPU9250_REG_XA_OFFSET_L         0x78
#define MPU9250_REG_YA_OFFSET_H         0x79
#define MPU9250_REG_YA_OFFSET_L         0x71
#define MPU9250_REG_ZA_OFFSET_H         0x7b
#define MPU9250_REG_ZA_OFFSET_L         0x7c

int     Mpu9250Probe(int unit);
int     Mpu9250Init(int unit);
int     Mpu9250RecvValue(int unit, imuValue_t *p);
int     Mpu9250ReadValue(int unit, imuValue_t *p);
int     Mpu9250GetSettings(int unit, imuSetting_t *p);
int     Mpu9250SetSettings(int unit, imuSetting_t *p);

#ifdef  _MPU9250_H_
#endif

#endif
