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

######################################################
# directory definitions
#
TMPDIR		:= /tmp
DESTDIR		:= obj/$(DEVICE_TYPE)_$(ENV_CC)
OBJDIR		:= obj/$(DEVICE_TYPE)_$(ENV_CC)/$(TARGET)
CMSIS_DIR	:= $(CONFIG_ROOTDIR)/../CMSIS
DEVICE_DIR	:= ../target/$(DEVICE_TYPE)
DEVICELIB_DIR	:= $(CONFIG_ROOTDIR)/../Devices
FREERTOS_DIR	:= $(CONFIG_ROOTDIR)/../Middlewares/FreeRTOS/Source
USBDIF_DIR	:= $(CONFIG_ROOTDIR)/../Middlewares/Usbdif
SDMMC_DIR	:= $(CONFIG_ROOTDIR)/../Middlewares/Sdmmc
FATFS_DIR	:= $(CONFIG_ROOTDIR)/../Middlewares/Fatfs
COMMON_DIR	:= $(CONFIG_ROOTDIR)/../lib/common
PRINTF_DIR	:= $(CONFIG_ROOTDIR)/../lib/printf-MarcoPaland
OPENBSD_DIR	:= $(CONFIG_ROOTDIR)/../lib/openbsd
LIBAEABI_CORTEXM0_DIR	:= $(CONFIG_ROOTDIR)/../lib/libaeabi-cortexm0
LLVM_DIR	:= $(CONFIG_ROOTDIR)/../lib/llvm

CONFIG_MK	:= $(DESTDIR)/config.mk
