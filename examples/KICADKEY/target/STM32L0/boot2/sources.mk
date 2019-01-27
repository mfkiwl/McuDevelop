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
		$(CONFIG_ROOTDIR)/../boot/$(TARGET)/main.c \
		$(CONFIG_ROOTDIR)/../boot/$(TARGET)/peri.c \
		../target/$(DEVICE_TYPE)/$(TARGET)/system.c \
		../target/$(DEVICE_TYPE)/$(TARGET)/update.c \
		../target/$(DEVICE_TYPE)/$(TARGET)/periI2c.c \
		../target/$(DEVICE_TYPE)/$(TARGET)/periUart.c \
		$(CONFIG_ROOTDIR)/../lib/common/string_gcc.c \
		$(CONFIG_ROOTDIR)/../lib/common/crc32.c \
		$(CONFIG_ROOTDIR)/../lib/common/fifo.c


DEFINES		= -DCPU_$(DEVICE_NAME) -D$(DEVICE_NAME) \
		-DDEVICE_ARCHTYPE=$(DEVICE_ARCHTYPE) \
		-DCRC32_BUILD_TABLE=$(CRC32_BUILD_TABLE) \
		-D$(COMPILER)=1

INCDIRS		= -I obj/$(DEVICE_TYPE)_$(ENV_CC) \
		-I . \
		-I include \
		-I $(CONFIG_ROOTDIR)/../boot/$(TARGET) \
		-I $(CONFIG_ROOTDIR)/../target/$(DEVICE_TYPE)/$(TARGET) \
		-I $(CONFIG_ROOTDIR)/../CMSIS/Include \
		-I $(CONFIG_ROOTDIR)/../Devices/$(DEVICE_TYPE)/include \
		-I $(CONFIG_ROOTDIR)/../Devices/$(VENDOR_SERIES)/include \
		-I $(CONFIG_ROOTDIR)/../Devices/$(VENDOR_SERIES)/src \
		-I $(DEVICE_DIR) \
		-I $(CONFIG_ROOTDIR)/../Middlewares/Usbdif/include \
		-I $(CONFIG_ROOTDIR)/../Middlewares/Usbdif/src \
		-I $(COMMON_DIR) \
