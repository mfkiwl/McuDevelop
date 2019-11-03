/*
 * Copyright (c) 2019 zhtlab
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

#ifndef _ICM42605_H_
#define _ICM42605_H_

#define ICM42605_READ_WRITE_MASK  (1<<7)
#define ICM42605_READ             (1<<7)
#define ICM42605_WRITE            (0<<7)

#define ICM42605_REG_DEVICE_CONFIG      0x11
#define         DEVICE_CONFIG_SPI_MODE_SHIFT    4
#define         DEVICE_CONFIG_SPI_MODE_MASK     (1<<(DEVICE_CONFIG_SPI_MODE_SHIFT))
#define         DEVICE_CONFIG_SPI_MODE_03       (0<<(DEVICE_CONFIG_SPI_MODE_SHIFT))
#define         DEVICE_CONFIG_SPI_MODE_12       (1<<(DEVICE_CONFIG_SPI_MODE_SHIFT))
#define         DEVICE_CONFIG_SOFT_RESET_SHIFT  0
#define         DEVICE_CONFIG_SOFT_RESET_MASK   (1<<(DEVICE_CONFIG_SOFT_RESET_SHIFT))
#define         DEVICE_CONFIG_SOFT_RESET_YES    (1<<(DEVICE_CONFIG_SOFT_RESET_SHIFT))
#define ICM42605_REG_DRIVE_CONFIG       0x13
#define ICM42605_REG_INT_CONFIG         0x14
#define         INT_CONFIG_INT1_MODE_SHIFT      2
#define         INT_CONFIG_INT1_MODE_MASK       (1<<(INT_CONFIG_INT1_MODE_SHIFT))
#define         INT_CONFIG_INT1_MODE_PULSE      (0<<(INT_CONFIG_INT1_MODE_SHIFT))
#define         INT_CONFIG_INT1_MODE_LATCH      (1<<(INT_CONFIG_INT1_MODE_SHIFT))
#define         INT_CONFIG_INT1_DRIVE_CIRCUIT_SHIFT   1
#define         INT_CONFIG_INT1_DRIVE_CIRCUIT_MASK    (1<<(INT_CONFIG_INT1_DRIVE_CIRCUIT_SHIFT))
#define         INT_CONFIG_INT1_DRIVE_CIRCUIT_OD      (0<<(INT_CONFIG_INT1_DRIVE_CIRCUIT_SHIFT))
#define         INT_CONFIG_INT1_DRIVE_CIRCUIT_DRIVE   (1<<(INT_CONFIG_INT1_DRIVE_CIRCUIT_SHIFT))
#define         INT_CONFIG_INT1_POLARITY_SHIFT      0
#define         INT_CONFIG_INT1_POLARITY_MASK       (1<<(INT_CONFIG_INT1_POLARITY_SHIFT))
#define         INT_CONFIG_INT1_POLARITY_LOW        (0<<(INT_CONFIG_INT1_POLARITY_SHIFT))
#define         INT_CONFIG_INT1_POLARITY_HIGH       (1<<(INT_CONFIG_INT1_POLARITY_SHIFT))
#define ICM42605_REG_FIFO_CONFIG        0x15

#define ICM42605_REG_TEMP_OUT_H         0x1d
#define ICM42605_REG_TEMP_OUT_L         0x1e
#define ICM42605_REG_ACCEL_DATA_X1      0x1f
#define ICM42605_REG_ACCEL_DATA_X0      0x20
#define ICM42605_REG_ACCEL_DATA_Y1      0x21
#define ICM42605_REG_ACCEL_DATA_Y0      0x22
#define ICM42605_REG_ACCEL_DATA_Z1      0x23
#define ICM42605_REG_ACCEL_DATA_Z0      0x24
#define ICM42605_REG_GYRO_DATA_X1       0x25
#define ICM42605_REG_GYRO_DATA_X0       0x26
#define ICM42605_REG_GYRO_DATA_Y1       0x27
#define ICM42605_REG_GYRO_DATA_Y0       0x28
#define ICM42605_REG_GYRO_DATA_Z1       0x29
#define ICM42605_REG_GYRO_DATA_Z0       0x2a
#define ICM42605_REG_TMST_FSYNCH        0x2b
#define ICM42605_REG_TMST_FSYNCL        0x2c
#define ICM42605_REG_INT_STATUS         0x2d

#define ICM42605_REG_INT_STATUS2        0x37
#define ICM42605_REG_INT_STATUS3        0x38

#define ICM42605_REG_INTF_CONFIG0       0x4c
#define         INTF_CONFIG0_SIFS_CFG_SHIFT        0
#define         INTF_CONFIG0_SIFS_CFG_MASK         (3<<(INTF_CONFIG0_SIFS_CFG_SHIFT))
#define         INTF_CONFIG0_SIFS_CFG_DIS_NO       (0<<(INTF_CONFIG0_SIFS_CFG_SHIFT))
#define         INTF_CONFIG0_SIFS_CFG_SPI_DIS      (2<<(INTF_CONFIG0_SIFS_CFG_SHIFT))
#define         INTF_CONFIG0_SIFS_CFG_I2C_DIS      (3<<(INTF_CONFIG0_SIFS_CFG_SHIFT))
#define ICM42605_REG_INTF_CONFIG1       0x4d

#define ICM42605_REG_PWR_MGMT0          0x4e
#define         PWR_MGMT0_TEMP_DIS_SHIFT        5
#define         PWR_MGMT0_TEMP_DIS_MASK         (1<<(PWR_MGMT0_TEMP_DIS_SHIFT))
#define         PWR_MGMT0_TEMP_DIS_NO           (0<<(PWR_MGMT0_TEMP_DIS_SHIFT))
#define         PWR_MGMT0_TEMP_DIS_YES          (1<<(PWR_MGMT0_TEMP_DIS_SHIFT))
#define         PWR_MGMT0_IDLE_SHIFT            4
#define         PWR_MGMT0_IDLE_MASK             (1<<(PWR_MGMT0_IDLE_SHIFT))
#define         PWR_MGMT0_IDLE_NO               (0<<(PWR_MGMT0_IDLE_SHIFT))
#define         PWR_MGMT0_IDLE_YES              (1<<(PWR_MGMT0_IDLE_SHIFT))
#define         PWR_MGMT0_GYRO_MODE_SHIFT       2
#define         PWR_MGMT0_GYRO_MODE_MASK        (3<<(PWR_MGMT0_GYRO_MODE_SHIFT))
#define         PWR_MGMT0_GYRO_MODE_OFF         (0<<(PWR_MGMT0_GYRO_MODE_SHIFT))
#define         PWR_MGMT0_GYRO_MODE_STANDBY     (1<<(PWR_MGMT0_GYRO_MODE_SHIFT))
#define         PWR_MGMT0_GYRO_MODE_RESERVED2   (2<<(PWR_MGMT0_GYRO_MODE_SHIFT))
#define         PWR_MGMT0_GYRO_MODE_LOWNOISE    (3<<(PWR_MGMT0_GYRO_MODE_SHIFT))
#define         PWR_MGMT0_ACCEL_MODE_SHIFT      0
#define         PWR_MGMT0_ACCEL_MODE_MASK       (3<<(PWR_MGMT0_ACCEL_MODE_SHIFT))
#define         PWR_MGMT0_ACCEL_MODE_OFF        (0<<(PWR_MGMT0_ACCEL_MODE_SHIFT))
#define         PWR_MGMT0_ACCEL_MODE_OFF1       (1<<(PWR_MGMT0_ACCEL_MODE_SHIFT))
#define         PWR_MGMT0_ACCEL_MODE_LOWPOWER   (2<<(PWR_MGMT0_ACCEL_MODE_SHIFT))
#define         PWR_MGMT0_ACCEL_MODE_LOWNOISE   (3<<(PWR_MGMT0_ACCEL_MODE_SHIFT))

#define ICM42605_REG_GYRO_CONFIG0       0x4f
#define         GYRO_CONFIG0_FSSEL_SHIFT        5
#define         GYRO_CONFIG0_FSSEL_MASK         (7<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_2000DPS      (0<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_1000DPS      (1<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_500DPS       (2<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_250DPS       (3<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_125DPS       (4<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_62DPS        (5<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_31DPS        (6<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_FSSEL_15DPS        (7<<(GYRO_CONFIG0_FSSEL_SHIFT))
#define         GYRO_CONFIG0_ODR_SHIFT          0
#define         GYRO_CONFIG0_ODR_MASK           (15<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_8000HZ         ( 3<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_4000HZ         ( 4<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_2000HZ         ( 5<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_1000HZ         ( 6<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_200HZ          ( 7<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_100HZ          ( 8<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_50HZ           ( 9<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_25HZ           (10<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_12HZ           (11<<(GYRO_CONFIG0_ODR_SHIFT))
#define         GYRO_CONFIG0_ODR_500HZ          (15<<(GYRO_CONFIG0_ODR_SHIFT))
#define ICM42605_REG_ACCEL_CONFIG0      0x50
#define         ACCEL_CONFIG0_FSSEL_SHIFT       5
#define         ACCEL_CONFIG0_FSSEL_MASK        (7<<(ACCEL_CONFIG0_FSSEL_SHIFT))
#define         ACCEL_CONFIG0_FSSEL_16G         (0<<(ACCEL_CONFIG0_FSSEL_SHIFT))
#define         ACCEL_CONFIG0_FSSEL_8G          (1<<(ACCEL_CONFIG0_FSSEL_SHIFT))
#define         ACCEL_CONFIG0_FSSEL_4G          (2<<(ACCEL_CONFIG0_FSSEL_SHIFT))
#define         ACCEL_CONFIG0_FSSEL_2G          (3<<(ACCEL_CONFIG0_FSSEL_SHIFT))
#define         ACCEL_CONFIG0_ODR_SHIFT         0
#define         ACCEL_CONFIG0_ODR_MASK          (15<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_8000HZ        ( 3<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_4000HZ        ( 4<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_2000HZ        ( 5<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_1000HZ        ( 6<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_200HZ         ( 7<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_100HZ         ( 8<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_50HZ          ( 9<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_25HZ          (10<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_12HZ          (11<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_6HZ           (12<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_3HZ           (13<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_1HZ           (14<<(ACCEL_CONFIG0_ODR_SHIFT))
#define         ACCEL_CONFIG0_ODR_500HZ         (15<<(ACCEL_CONFIG0_ODR_SHIFT))
#define ICM42605_REG_GYRO_CONFIG1       0x51
#define ICM42605_REG_GYRO_ACCEL_CONFIG0 0x52
#define ICM42605_REG_ACCEL_CONFIG1      0x53

#define ICM42605_REG_TMST_CONFIG        0x54
#define ICM42605_REG_APEX_CONFIG0       0x56
#define ICM42605_REG_SMD_CONFIG         0x57

#define ICM42605_REG_FIFO_CONFIG1       0x5f
#define ICM42605_REG_FIFO_CONFIG2       0x60
#define ICM42605_REG_FIFO_CONFIG3       0x61
#define ICM42605_REG_FSYNC_CONFIG       0x62

#define ICM42605_REG_INT_CONFIG0        0x63
#define ICM42605_REG_INT_CONFIG1        0x64
#define ICM42605_REG_INT_SOURCE0        0x65
#define         INT_SOURCE0_UI_DRDY_INT1_EN_SHIFT     3
#define         INT_SOURCE0_UI_DRDY_INT1_EN_MASK      (1<<(INT_SOURCE0_UI_DRDY_INT1_EN_SHIFT))
#define         INT_SOURCE0_UI_DRDY_INT1_EN_NO        (0<<(INT_SOURCE0_UI_DRDY_INT1_EN_SHIFT))
#define         INT_SOURCE0_UI_DRDY_INT1_EN_YES       (1<<(INT_SOURCE0_UI_DRDY_INT1_EN_SHIFT))
#define ICM42605_REG_INT_SOURCE1        0x66
#define ICM42605_REG_INT_SOURCE3        0x68
#define ICM42605_REG_INT_SOURCE4        0x69

#define ICM42605_REG_WHOAMI             0x75
#define         WHOAMI_VALUE                    0x71
#define         WHOAMI_VALUE_ICM20600           0x11
#define         WHOAMI_VALUE_ICM20601           0xac
#define         WHOAMI_VALUE_ICM20602           0x12
#define         WHOAMI_VALUE_ICM20609           0xa6
//#define         WHOAMI_VALUE_ICM20648           0xe0
//#define         WHOAMI_VALUE_ICM20649           0xe1
#define         WHOAMI_VALUE_ICG20660           0x91
#define         WHOAMI_VALUE_ICM20689           0x98
#define         WHOAMI_VALUE_ICM20690           0x20
#define         WHOAMI_VALUE_IAM20680           0xa9
#define         WHOAMI_VALUE_ICM20789           0x03
#define         WHOAMI_VALUE_ICM42605           0x42
#define         WHOAMI_VALUE_TESTID             0x00

#define ICM42605_REG_BANKSEL            0x76

int     Icm42605Probe(imuHandler_t *ph);
int     Icm42605Init(imuHandler_t *ph);
int     Icm42605RecvValue(imuHandler_t *ph, imuValue_t *p);
int     Icm42605ReadValue(imuHandler_t *ph, imuValue_t *p);
int     Icm42605GetSettings(imuHandler_t *ph, imuSetting_t *p);
int     Icm42605SetSettings(imuHandler_t *ph, imuSetting_t *p);

#ifdef  _ICM42605_C_
#endif

#endif
