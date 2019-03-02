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

#ifndef _LIS3DHH_H_
#define _LIS3DHH_H_

#define LIS3DHH_READ_WRITE_MASK  (1<<7)
#define LIS3DHH_READ             (1<<7)
#define LIS3DHH_WRITE            (0<<7)

#define LIS3DHH_REG_WHO_AM_I    0x0f
#define         WHO_AM_I_LIS3DHH        0x11
#define LIS3DHH_REG_CTRL_REG1   0x20
#define         CTRL_REG1_NORM_MOD_EN_SHIFT 7
#define         CTRL_REG1_NORM_MOD_EN_MASK  (1<<(CTRL_REG1_NORM_MOD_EN_SHIFT))
#define         CTRL_REG1_NORM_MOD_EN_NO    (0<<(CTRL_REG1_NORM_MOD_EN_SHIFT))
#define         CTRL_REG1_NORM_MOD_EN_YES   (1<<(CTRL_REG1_NORM_MOD_EN_SHIFT))
#define         CTRL_REG1_IF_ADD_INC_SHIFT  6
#define         CTRL_REG1_IF_ADD_INC_MASK   (1<<(CTRL_REG1_IF_ADD_INC_SHIFT))
#define         CTRL_REG1_IF_ADD_INC_NO     (0<<(CTRL_REG1_IF_ADD_INC_SHIFT))
#define         CTRL_REG1_IF_ADD_INC_YES    (1<<(CTRL_REG1_IF_ADD_INC_SHIFT))
#define         CTRL_REG1_BOOT_SHIFT    4
#define         CTRL_REG1_BOOT_MASK     (1<<(CTRL_REG1_BOOT_SHIFT))
#define         CTRL_REG1_BOOT_NO       (0<<(CTRL_REG1_BOOT_SHIFT))
#define         CTRL_REG1_BOOT_YES      (1<<(CTRL_REG1_BOOT_SHIFT))
#define         CTRL_REG1_SW_RESET_SHIFT 2
#define         CTRL_REG1_SW_RESET_MASK (1<<(CTRL_REG1_SW_RESET_SHIFT))
#define         CTRL_REG1_SW_RESET_NO   (0<<(CTRL_REG1_SW_RESET_SHIFT))
#define         CTRL_REG1_SW_RESET_YES  (1<<(CTRL_REG1_SW_RESET_SHIFT))
#define         CTRL_REG1_DRDY_PULSE_SHIFT 1
#define         CTRL_REG1_DRDY_PULSE_MASK (1<<(CTRL_REG1_DRDY_PULSE_SHIFT))
#define         CTRL_REG1_DRDY_PULSE_NO   (0<<(CTRL_REG1_DRDY_PULSE_SHIFT))
#define         CTRL_REG1_DRDY_PULSE_YES  (1<<(CTRL_REG1_DRDY_PULSE_SHIFT))
#define         CTRL_REG1_BDU_SHIFT     0
#define         CTRL_REG1_BDU_MASK      (1<<(CTRL_REG1_BDU_SHIFT))
#define         CTRL_REG1_BDU_NO        (0<<(CTRL_REG1_BDU_SHIFT))
#define         CTRL_REG1_BDU_YES       (1<<(CTRL_REG1_BDU_SHIFT))
#define LIS3DHH_REG_INT1_CTRL   0x21
#define         INT1_CTRL_DRDY_SHIFT    7
#define         INT1_CTRL_DRDY_MASK     (1<<(INT1_CTRL_DRDY_SHIFT))
#define         INT1_CTRL_DRDY_NO       (0<<(INT1_CTRL_DRDY_SHIFT))
#define         INT1_CTRL_DRDY_YES      (1<<(INT1_CTRL_DRDY_SHIFT))
#define         INT1_CTRL_BOOT_SHIFT    6
#define         INT1_CTRL_BOOT_MASK     (1<<(INT1_CTRL_BOOT_SHIFT))
#define         INT1_CTRL_BOOT_NO       (0<<(INT1_CTRL_BOOT_SHIFT))
#define         INT1_CTRL_BOOT_YES      (1<<(INT1_CTRL_BOOT_SHIFT))
#define         INT1_CTRL_OVR_SHIFT     5
#define         INT1_CTRL_OVR_MASK      (1<<(INT1_CTRL_OVR_SHIFT))
#define         INT1_CTRL_OVR_NO        (0<<(INT1_CTRL_OVR_SHIFT))
#define         INT1_CTRL_OVR_YES       (1<<(INT1_CTRL_OVR_SHIFT))
#define         INT1_CTRL_FSS5_SHIFT    4
#define         INT1_CTRL_FSS5_MASK     (1<<(INT1_CTRL_FSS5_SHIFT))
#define         INT1_CTRL_FSS5_NO       (0<<(INT1_CTRL_FSS5_SHIFT))
#define         INT1_CTRL_FSS5_YES      (1<<(INT1_CTRL_FSS5_SHIFT))
#define         INT1_CTRL_FTH_SHIFT     3
#define         INT1_CTRL_FTH_MASK      (1<<(INT1_CTRL_FTH_SHIFT))
#define         INT1_CTRL_FTH_NO        (0<<(INT1_CTRL_FTH_SHIFT))
#define         INT1_CTRL_FTH_YES       (1<<(INT1_CTRL_FTH_SHIFT))
#define         INT1_CTRL_EXT_SHIFT     2
#define         INT1_CTRL_EXT_MASK      (1<<(INT1_CTRL_EXT_SHIFT))
#define         INT1_CTRL_EXT_NO        (0<<(INT1_CTRL_EXT_SHIFT))
#define         INT1_CTRL_EXT_YES       (1<<(INT1_CTRL_BOOT_SHIFT))
#define LIS3DHH_REG_INT2_CTRL   0x22
#define         INT2_CTRL_DRDY_SHIFT    7
#define         INT2_CTRL_DRDY_MASK     (1<<(INT2_CTRL_DRDY_SHIFT))
#define         INT2_CTRL_DRDY_NO       (0<<(INT2_CTRL_DRDY_SHIFT))
#define         INT2_CTRL_DRDY_YES      (1<<(INT2_CTRL_DRDY_SHIFT))
#define         INT2_CTRL_BOOT_SHIFT    6
#define         INT2_CTRL_BOOT_MASK     (1<<(INT2_CTRL_BOOT_SHIFT))
#define         INT2_CTRL_BOOT_NO       (0<<(INT2_CTRL_BOOT_SHIFT))
#define         INT2_CTRL_BOOT_YES      (1<<(INT2_CTRL_BOOT_SHIFT))
#define         INT2_CTRL_OVR_SHIFT     5
#define         INT2_CTRL_OVR_MASK      (1<<(INT2_CTRL_OVR_SHIFT))
#define         INT2_CTRL_OVR_NO        (0<<(INT2_CTRL_OVR_SHIFT))
#define         INT2_CTRL_OVR_YES       (1<<(INT2_CTRL_OVR_SHIFT))
#define         INT2_CTRL_FSS5_SHIFT    4
#define         INT2_CTRL_FSS5_MASK     (1<<(INT2_CTRL_FSS5_SHIFT))
#define         INT2_CTRL_FSS5_NO       (0<<(INT2_CTRL_FSS5_SHIFT))
#define         INT2_CTRL_FSS5_YES      (1<<(INT2_CTRL_FSS5_SHIFT))
#define         INT2_CTRL_FTH_SHIFT     3
#define         INT2_CTRL_FTH_MASK      (1<<(INT2_CTRL_FTH_SHIFT))
#define         INT2_CTRL_FTH_NO        (0<<(INT2_CTRL_FTH_SHIFT))
#define         INT2_CTRL_FTH_YES       (1<<(INT2_CTRL_FTH_SHIFT))
#define LIS3DHH_REG_CTRL_REG4   0x23
#define LIS3DHH_REG_CTRL_REG5   0x24
#define LIS3DHH_REG_OUT_TEMP_LOW    0x25
#define LIS3DHH_REG_OUT_TEMP_HIGH   0x26
#define LIS3DHH_REG_STATUS      0x27
#define LIS3DHH_REG_ACC_X_LOW   0x28
#define LIS3DHH_REG_ACC_X_HIGH  0x29
#define LIS3DHH_REG_ACC_Y_LOW   0x2a
#define LIS3DHH_REG_ACC_Y_HIGH  0x2b
#define LIS3DHH_REG_ACC_Z_LOW   0x2c
#define LIS3DHH_REG_ACC_Z_HIGH  0x2d
#define LIS3DHH_REG_FIFO_CTRL   0x2e
#define LIS3DHH_REG_FIFO_SRC    0x2f


int     Lis3dhhProbe(imuHandler_t *ph);
int     Lis3dhhInit(imuHandler_t *ph);
int     Lis3dhhRecvValue(imuHandler_t *ph, imuValue_t *p);
int     Lis3dhhReadValue(imuHandler_t *ph, imuValue_t *p);
int     Lis3dhhGetSettings(imuHandler_t *ph, imuSetting_t *p);
int     Lis3dhhSetSettings(imuHandler_t *ph, imuSetting_t *p);


#ifdef  _LIS3DHH_H_
#endif

#endif
