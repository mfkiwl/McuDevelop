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

ROOTDIR			= $(shell pwd | sed 's/\/cygdrive\/\(\w\)/\1:/')

OBJDIR			= obj/$(DEVICE_TYPE)_$(ENV_CC)

TARGET			= bootfirm
TARGET_BOOT		= boot/$(OBJDIR)/bootloader
TARGET_FIRM		= firm/$(OBJDIR)/firm/firm_crc
TEMP_FILE		= file.temp

DIRECTORIES		= $(CONFIG_ROOTDIR)/lib/libaeabi-cortexm0 boot firm

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
PUSHD			= pushd
POPD			= popd


top: compile all


all:
	@while read i; do \
		line=`echo $$i | grep -v "^#"` ; \
		if [ ! x"$$line" = x"" ]; then  \
			words=( $$line ) ; \
			$(MAKE) bootfirm_cat \
				DEVICE_TYPE=$${words[1]} \
				ENV_CC=$${words[2]} \
				; \
		fi ; \
	done < target.conf


compile: $(DIRECTORIES)
	@for i in $(DIRECTORIES); \
	do \
		echo \#\#\# compile in $$i; \
		$(PUSHD) $$i; $(MAKE) all; $(POPD); \
	done


bootfirm_cat:
	@echo \#\#\# concatinate bootloader and firmware
	@$(MKDIR) -p obj
	@$(MOT2BIN) $(TARGET_BOOT).srec > $(TEMP_FILE);
	@$(0XFF) | $(DD) bs=1k count=16 >>  $(TEMP_FILE)  2> /dev/null
	@$(DD) if=$(TEMP_FILE) of=$(TEMP_FILE)2 bs=1k count=16  2> /dev/null
	@$(CAT) $(TARGET_FIRM).bin >> $(TEMP_FILE)2
	@$(MV) -f $(TEMP_FILE)2 obj/$(TARGET).bin
	@$(RM) $(TEMP_FILE)


clean:
	@for i in $(DIRECTORIES); \
	do \
		echo \#\#\# clean in $$i; \
		$(PUSHD) $$i; $(MAKE) clean; $(POPD); \
	done
	$(RM) -rf obj


prog:
	dfu-util -D obj/bootfirm.bin -a 0 --dfuse-address 0x8000000
