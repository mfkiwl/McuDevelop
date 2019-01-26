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

SOURCESASM	= $(COMMON_DIR)/start_gcc.s
SOURCES		= \
		$(CONFIG_ROOTDIR)/../boot/boot2/main.c \
		$(CONFIG_ROOTDIR)/../boot/boot2/peri.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/system.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/update.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/periI2c.c \
		$(CONFIG_ROOTDIR)/../boot/target/$(DEVICE_TYPE)/boot2/periUart.c \
		$(CONFIG_ROOTDIR)/../boot/common/string_gcc.c \
		$(CONFIG_ROOTDIR)/../boot/common/crc32.c \
		$(CONFIG_ROOTDIR)/../boot/common/fifo.c


DEFINES		= -DCPU_$(DEVICE_NAME) -D$(DEVICE_NAME) \
		-DDEVICE_ARCHTYPE=$(DEVICE_ARCHTYPE) \
		-DCRC32_BUILD_TABLE=$(CRC32_BUILD_TABLE) \
		-D$(COMPILER)=1

INCDIRS		= -I obj/$(DEVICE_TYPE)_$(ENV_CC) \
		-I $(CONFIG_ROOTDIR)/../boot/$(TARGET) \
		-I include \
		-I $(COMMON_DIR) \
		-I $(CONFIG_ROOTDIR)/../boot/../CMSIS/Include \
		-I $(CONFIG_ROOTDIR)/../Devices/$(DEVICE_TYPE)/include \
		-I $(CONFIG_ROOTDIR)/../Devices/$(VENDOR_SERIES)/include \
		-I $(CONFIG_ROOTDIR)/../boot/target/$(VENDOR_SERIES)/src \
		-I $(DEVICE_DIR)

#		-I target/STM32H7/STM32H7xx_HAL_Driver/Inc \
		-I target/STM32H7/Middlewares/ST/STM32_USB_Device_Library/Core/Inc \
		-I target/STM32H7/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc \
