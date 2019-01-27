#
# Copyright (c) 2018 zhtlab
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

SOURCESASM	= common/start_gcc.s
SOURCES		= \
		$(CONFIG_ROOTDIR)/../boot/boot2/main.c \
		$(CONFIG_ROOTDIR)/../boot/boot2/peri.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/system.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/update.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/periI2c.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/periUart.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/periUsb.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/usbd_cdc_if.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/usbd_desc.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/usbd_conf.c \
		$(CONFIG_ROOTDIR)/../boot/common/string_gcc.c \
		$(CONFIG_ROOTDIR)/../boot/common/crc32.c \
		$(CONFIG_ROOTDIR)/../boot/common/fifo.c

#		target/$(DEVICE_TYPE)/boot2/usb_device.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
		target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \


#		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c \


#		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \


DEFINES		= -DCPU_$(DEVICE_NAME) -D$(DEVICE_NAME) \
		-DDEVICE_ARCHTYPE=$(DEVICE_ARCHTYPE) \
		-DCRC32_BUILD_TABLE=$(CRC32_BUILD_TABLE) \
		-D$(COMPILER)=1 \
		-DUSE_VENDOR_HAL

INCDIRS		= -I obj/$(DEVICE_TYPE)_$(ENV_CC) \
		-I $(TARGET) -I include \
		-I $(COMMON_DIR) \
		-I $(CONFIG_ROOTDIR)/../CMSIS/Include \
		-I $(CONFIG_ROOTDIR)/../Devices/$(DEVICE_TYPE)/include \
		-I $(CONFIG_ROOTDIR)/../Devices/$(VENDOR_SERIES)/include \
		-I $(CONFIG_ROOTDIR)/../Devices/STM32/include \
		-I $(CONFIG_ROOTDIR)/../boot/boot2 \
		-I $(DEVICE_DIR)


#		-I target/STM32H7/STM32H7xx_HAL_Driver/Inc \
#-I target/STM32H7/Middlewares/ST/STM32_USB_Device_Library/Core/Inc \
		-I target/STM32H7/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc \

#		-I target/$(DEVICE_TYPE)/Include \
