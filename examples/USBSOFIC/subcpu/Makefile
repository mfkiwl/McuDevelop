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

include		config

DEVICE_TYPE		= STM32F7
ENV_CC			= gcc

ROOTDIR			= $(shell pwd | sed 's/\/cygdrive\/\(\w\)/\1:/')

OBJDIR			= obj/$(DEVICE_TYPE)_$(ENV_CC)

TARGET			= bootfirm.bin
TARGET_BOOT		= boot/$(OBJDIR)/bootloader
TARGET_FIRM		= firm/$(OBJDIR)/firm/firm_crc
TEMP_FILE		= file.temp

DIRECTORIES		= firm boot

CONFIG_ORIG		= config
CONFIG_H		= config.h
CONFIG_INC		= config.inc


SHELL			= /bin/bash
CD			= cd
DD			= dd
CAT			= cat
RM			= rm -f
MV			= mv
AWK			= awk
MAKE			= make
MOT2BIN			= $(CONFIG_ROOTDIR)/bin/mot2bin.rb
0XFF			= $(CONFIG_ROOTDIR)/bin/0xff
MKDIR			= mkdir

#top: make_config all bootfirm
top: all bootfirm


all: $(DIRECTORIES)
	for i in $(DIRECTORIES); \
	do \
		$(CD) $$i; $(MAKE) $@; $(CD) ..; \
	done


bootfirm:
	$(MAKE) bootfirm_cat;


bootfirm_cat:
	$(MKDIR) -p obj
	$(MOT2BIN) $(TARGET_BOOT).srec > $(TEMP_FILE)
	$(0XFF) | $(DD) bs=1k count=16 >>  $(TEMP_FILE)
	$(DD) if=$(TEMP_FILE) of=$(TEMP_FILE)2 bs=1k count=16
	$(CAT) $(TARGET_FIRM).bin >> $(TEMP_FILE)2
	$(MV) -f $(TEMP_FILE)2 obj/bootfirm.bin
	$(RM) $(TEMP_FILE)


clean:
	for i in $(DIRECTORIES); \
	do \
		$(CD) $$i; $(MAKE) $@; $(CD) ..; \
	done
	$(RM) -rf obj
#	$(RM)	 $(CONFIG_H) $(CONFIG_INC)

PROGMCU		= bin/progmcu.Linux-x86_64

prog:
	@echo type \"../progusbhid/src/progusbhid -e -p --wholearea -f obj/bootfirm.xxxxx\"    to burn
	$(PROGMCU) -e -p -B -f obj/bootfirm.bin --force --allarea --stm --uart --uartdev=/dev/ttyUSB0 #-D 0x80000000
