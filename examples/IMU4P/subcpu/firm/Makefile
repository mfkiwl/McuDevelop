#
# Copyright (c) 2018,2019 zhtlab
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
# target name (binary file name)
#
TARGET_TOP	= firmware
TARGET		= firm


######################################################
# directory definitions
#
MK_DIR		= $(CONFIG_ROOTDIR)/../mk


######################################################
# include
#
-include	../config
-include	../target/$(DEVICE_TYPE)/config
-include	$(MK_DIR)/def.mk
-include	$(MK_DIR)/cortex-m.mk


######################################################
# source files
#
SOURCESASM = $(COMMON_DIR)/start_gcc.s

SOURCES =	\
		$(PRINTF_DIR)/printf.c \
		$(COMMON_DIR)/string_gcc.c \
		$(COMMON_DIR)/fifo.c \
		$(COMMON_DIR)/crc32.c \
		$(CONFIG_ROOTDIR)/../lib/openbsd/string/memcmp.c \
		$(CONFIG_ROOTDIR)/../lib/openbsd/string/strtok.c \
		$(CONFIG_ROOTDIR)/../lib/openbsd/string/strncmp.c \
		$(CONFIG_ROOTDIR)/../lib/openbsd/string/strncpy.c \
		$(CONFIG_ROOTDIR)/../lib/openbsd/stdlib/strtoul.c \
		$(CONFIG_ROOTDIR)/../lib/openbsd/stdlib/ctype_.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devGpio.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devCrs.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devCounter.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devUsart.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devSpi16.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devDma4.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devFlash.c \
		$(DEVICELIB_DIR)/$(VENDOR_SERIES)/src/devCrc.c \
\
		../target/$(DEVICE_TYPE)/src/system_$(DEVICE_TYPE).c \
		src/main.c \
		src/command.c \
		src/intr.c \
		src/imu.c \
		src/bmi160.c \
		src/adxl345.c \
		src/mpu9250.c \
		src/lis3dhh.c


LIBSEXT		= -L$(LIBAEABI_CORTEXM0_DIR) -laeabi-cortexm0

INCDIRS		= -I src -I ../include \
		-I obj/$(DEVICE_TYPE)_$(ENV_CC) \


DEFINES		= -DCPU_$(DEVICE_NAME) -D$(DEVICE_NAME) \
		-DDEVICE_ARCHTYPE=$(DEVICE_ARCHTYPE) \
		-DARM_MATH_M4


ROOTDIR			= $(shell pwd | sed 's/\/cygdrive\/\(\w\)/\1:/')

#OBJDIR = obj/$(DEVICE_TYPE)_$(ENV_CC)


CONFIG_ORIG		= config
CONFIG_H		= config.h
CONFIG_INC		= config.inc

CD			= cd
CAT			= cat
AWK			= awk
ECHO			= echo
MKDIR			= mkdir
MAKE			= make
DATE			= date
MV			= mv
LN			= ln
OBJCOPY			= objcopy
PROGMCU			= $(CONFIG_ROOTDIR)/../bin/progmcu.Linux-x86_64
SHELL			= /bin/bash

SCONV			= $(CONFIG_ROOTDIR)/../bin/sconv
CRC32			= $(CONFIG_ROOTDIR)/../bin/crc32.rb
BINTOS			= $(CONFIG_ROOTDIR)/../bin/bintos.rb

# options
OBJCOPYOPT_SREC2BIN = -I srec -O binary

.PHONY:			obj src depend

all:
	@while read i; do \
		line=`echo $$i | grep -v "^#"` ; \
		if [ ! x"$$line" = x"" ]; then  \
			echo "$$line"; \
			words=( $$line ) ; \
			echo $${words[0]} $${words[1]} ;\
			$(MAKE) $(TARGET_TOP).bin \
				VENDOR_SERIES=$${words[0]} \
				DEVICE_TYPE=$${words[1]} \
				ENV_CC=$${words[2]} \
				DEVICE_ARCHTYPE=$${words[3]} \
				DEVICE_NAME=$${words[4]} \
				; \
		fi ; \
	done < ../target.conf



$(TARGET_TOP).bin: $(DESTDIR)/$(TARGET_TOP).bin

$(DESTDIR)/$(TARGET_TOP).bin: $(OBJDIR)/firm_crc.bin
	$(RM) -f $(DESTDIR)/*.bin
	$(ECHO) "Add the information header"
	$(ECHO) "Product: $(CONFIG_PRODUCT_NAME_TEXT)" > $@
	$(ECHO) "Type: Firmware" >> $@
	$(ECHO) "FlashStart: $(CONFIG_MEMORY_MAINFIRM_START)" >> $@
	$(ECHO) "File: $@" >> $@
	$(ECHO) "Version: $(CONFIG_VERSION_FIRM_TEXT)" >> $@
	$(DATE) '+Date: %Y/%m/%d %H:%M:%S %Z' >> $@
	$(ECHO) -n -e '\032' >> $@
	$(ECHO) -n -e '\000' >> $@
	$(CAT) $< >> $@
	$(LN) $@ $(DESTDIR)/$(TARGET_TOP).$(CONFIG_VERSION_FIRM_TEXT).bin

$(OBJDIR)/firm_crc.bin: $(OBJDIR)/firm_crc.srec
	$(OBJCOPY) -v $(OBJCOPYOPT_SREC2BIN) $< $@

$(OBJDIR)/firm_crc.srec: $(CONFIG_ROOTDIR)/../mk/firm.mk $(OBJDIR)/firm.srec
	$(CAT) $(OBJDIR)/firm.srec | grep -v -e "^S7" > $@
	$(CRC32) $(OBJDIR)/firm.bin --bin | $(BINTOS)  --noend --start-addr=$(CONFIG_FIRMWARE_INFO_TOP_MK) >> $@
	$(ECHO) "S70500000000FA" >> $@

$(OBJDIR)/firm.srec: obj $(CONFIG_MK) make_config
	$(MAKE) -f $(CONFIG_ROOTDIR)/../mk/firm.mk TARGETCODE=$(CONFIG_TARGETCODE_FIRM) TARGET=firm

make_config: $(DESTDIR)/$(CONFIG_H) $(DESTDIR)/$(CONFIG_INC)

obj:
	$(MKDIR) -p $(DESTDIR)/firm

$(CONFIG_MK): Makefile ../config
	echo "SOURCES = $(SOURCES)" > $@
	echo "SOURCESASM = $(SOURCESASM)" >> $@
	echo "INCDIRS = $(INCDIRS)" >> $@
	echo "DEFINES = $(DEFINES)" >> $@
	echo "LIBSEXT = $(LIBSEXT)" >> $@


$(DESTDIR)/$(CONFIG_H): ../$(CONFIG_ORIG) ../target/$(DEVICE_TYPE)/$(CONFIG_ORIG)
	echo	"#ifndef	_CONFIG_H_" > $@
	echo	"#define	_CONFIG_H_" >> $@
	cat $^ | $(AWK) '$$1 ~ /^[a-zA-Z_].+/ {print "#define	" $$1 "	" $$3}' >> $@
	echo	"#endif /* _CONFIG_H_ */" >> $@
$(DESTDIR)/$(CONFIG_INC): ../$(CONFIG_ORIG) ../target/$(DEVICE_TYPE)/$(CONFIG_ORIG)
	echo	'#ifndef	_config_inc_' > $@
	echo	'#define	_config_inc_' >> $@
	echo	''			      >> $@
	cat $^ | $(AWK) '$$1 ~ /^[a-zA-Z_].+/ && $$3 ~ /^[0-9\"\047][0-9]*/{print "#define	" $$1 "		" $$3; }' >> $@
	echo	'#endif' >> $@


clean:
	$(RM)	 $(CONFIG_H) $(CONFIG_INC)
	/bin/$(RM) -fr obj

prog:
	$(PROGMCU) --force --stm32 --uart -e -p -k -f obj/STM32L0_gcc/firmware.bin --uartdev=/dev/ttyUSB0 -R
