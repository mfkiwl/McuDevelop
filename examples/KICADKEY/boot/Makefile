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

-include	../config
-include	../target/$(DEVICE_TYPE)/config

TARGET_TOP		= bootloader
#UARTDEV		= /dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_066DFF574953867567105732-if02	# NUCLEO 144 (STM32F7)
UARTDEV			= /dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_066CFF353830523457254131-if02	# NUCLEO 144 (STM32H7)
#UARTDEV		= /dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_0676FF525750877267035147-if02   # NUCLEO  xx (STM32L4)

ROOTDIR			= $(shell pwd | sed 's/\/cygdrive\/\(\w\)/\1:/')

OBJDIR = obj/$(DEVICE_TYPE)_$(ENV_CC)

CONFIG_ORIG		= config
CONFIG_MK		= config.mk
CONFIG_H		= config.h
CONFIG_INC		= config.inc

MK_DIR			= $(CONFIG_ROOTDIR)/../mk
BOOT_MK			= $(MK_DIR)/boot.mk

CD			= cd
CAT			= cat
AWK			= awk
ECHO			= echo
MKDIR			= mkdir
MAKE			= make
DATE			= date
RM			= rm
MV			= mv
LN			= ln
OBJCOPY			= objcopy
PROGMCU			= $(CONFIG_ROOTDIR)/../bin/progmcu.Linux-x86_64
SHELL			= /bin/bash

SCONV			= $(CONFIG_ROOTDIR)/../bin/sconv.rb

# options
OBJCOPYOPT_SREC2BIN = -I srec -O binary

.PHONY:			obj boot1 boot2 depend

all:
	@while read i; do \
		line=`echo $$i | grep -v "^#"` ; \
		if [ ! x"$$line" = x"" ]; then  \
			echo "$$line"; \
			words=( $$line ) ; \
			echo $${words[0]} $${words[1]} ;\
			echo $(MAKE) $(TARGET_TOP).bin \
				VENDOR_SERIES=$${words[0]} \
				DEVICE_TYPE=$${words[1]} \
				ENV_CC=$${words[2]} \
				DEVICE_ARCHTYPE=$${words[3]} \
				DEVICE_NAME=$${words[4]} \
				DEVICE_PERI=$${words[5]} ; \
			$(MAKE) $(TARGET_TOP).bin \
				VENDOR_SERIES=$${words[0]} \
				DEVICE_TYPE=$${words[1]} \
				ENV_CC=$${words[2]} \
				DEVICE_ARCHTYPE=$${words[3]} \
				DEVICE_NAME=$${words[4]} \
				DEVICE_PERI=$${words[5]} ; \
		fi ; \
	done < ../target.conf

$(TARGET_TOP).bin: $(OBJDIR)/$(TARGET_TOP).bin

$(OBJDIR)/$(TARGET_TOP).bin: $(OBJDIR)/$(TARGET_TOP).srec
	$(RM) -f $(OBJDIR)/$(TARGET_TOP).*.bin
	$(OBJCOPY) -v $(OBJCOPYOPT_SREC2BIN) $< $(@:.bin=.plain.bin)
	$(ECHO) "Add the information header"
	$(ECHO) "Product: $(CONFIG_PRODUCT_NAME_TEXT)" > $@
	$(ECHO) "Type: Bootloader" >> $@
	$(ECHO) "FlashStart: $(CONFIG_MEMORY_ROM_BASE)" >> $@
	$(ECHO) "File: $@" >> $@
	$(ECHO) "Version: $(CONFIG_VERSION_BOOT_TEXT)" >> $@
	$(DATE) '+Date: %Y/%m/%d %H:%M:%S %Z' >> $@
	$(ECHO) -n -e '\032' >> $@
	$(ECHO) -n -e '\000' >> $@
	$(CAT) $(@:.bin=.plain.bin) >> $@
	$(LN) $@ $(OBJDIR)/$(TARGET_TOP).$(CONFIG_VERSION_BOOT_TEXT).bin

$(OBJDIR)/$(TARGET_TOP).srec: Makefile $(OBJDIR)/boot1/boot1.srec $(OBJDIR)/boot2/boot2.srec
	$(CAT) $(OBJDIR)/boot1/boot1.srec | grep -v -e "^S7" > $@
	$(SCONV) --off=$(CONFIG_MEMORY_ROM_BOOT2_OFFSET) -f $(OBJDIR)/boot2/boot2.srec | grep -v "^S7">> $@
	$(ECHO) "S70500000000FA" >> $@

$(OBJDIR)/boot1/boot1.srec: $(BOOT_MK) obj make_config
	@echo "### compile boot1"
	@$(MAKE) -f $(BOOT_MK) TARGETCODE=$(CONFIG_TARGETCODE_BOOT1) TARGET=boot1;

ifeq ($(VENDOR_SERIES), Kinetis)
$(OBJDIR)/boot2/boot2.srec: $(BOOT_MK) obj make_config
	$(CD) target/Kinetis; \
	$(MAKE) all \
		VENDOR_SERIES=$(VENDOR_SERIES) \
		DEVICE_TYPE=$(DEVICE_TYPE) \
		ENV_CC=$(ENV_CC) \
		DEVICE_ARCHTYPE=$(DEVICE_ARCHTYPE) \
		DEVICE_NAME=$(DEVICE_NAME) \
		DEVICE_PERI=$(DEVICE_PERI) \
		TARGETCODE=$(TARGETCODE_BOOT2) \
		TARGET=boot2; \
	$(CD) ../..
else
$(OBJDIR)/boot2/boot2.srec: $(BOOT_MK) obj make_config
	@echo "### compile boot2"
	@$(MAKE) -f $(BOOT_MK) TARGETCODE=$(CONFIG_TARGETCODE_BOOT2) TARGET=boot2
endif

make_config: $(OBJDIR)/$(CONFIG_MK) \
		$(OBJDIR)/$(CONFIG_H) $(OBJDIR)/$(CONFIG_INC)

obj:
	$(MKDIR) -p $(OBJDIR)/boot1
	$(MKDIR) -p $(OBJDIR)/boot2

obj/$(DEVICE_TYPE)_$(ENV_CC)/$(CONFIG_MK): ../$(CONFIG_ORIG) ../target/$(DEVICE_TYPE)/$(CONFIG_ORIG)
	cat $^ | $(AWK) '$$1 ~ /^[a-zA-Z_].+/ {print $$1 "		= " $$3}' >> $@
	for i in `echo $(DEVICE_PERI) | tr -t ',' ' '` ; \
	do \
		echo "CONFIG_PERIPHERAL_$$i	= 1# defined in target.conf" ; \
	done  >> $@
obj/$(DEVICE_TYPE)_$(ENV_CC)/$(CONFIG_H): ../$(CONFIG_ORIG) ../target/$(DEVICE_TYPE)/$(CONFIG_ORIG)
	echo	"#ifndef	_CONFIG_H_" > $@
	echo	"#define	_CONFIG_H_" >> $@
	cat $^ | $(AWK) '$$1 ~ /^[a-zA-Z_].+/ {print "#define	" $$1 "	" $$3}' >> $@
	for i in `echo $(DEVICE_PERI) | tr -t ',' ' '` ; \
	do \
		echo "#define	CONFIG_PERIPHERAL_$$i	1	/* defined in target.conf */" ; \
	done  >> $@
	echo	"#endif /* _CONFIG_H_ */" >> $@
obj/$(DEVICE_TYPE)_$(ENV_CC)/$(CONFIG_INC): ../$(CONFIG_ORIG) ../target/$(DEVICE_TYPE)/$(CONFIG_ORIG)
	echo	'#ifndef	_config_inc_' > $@
	echo	'#define	_config_inc_' >> $@
	echo	''			      >> $@
	cat $^ | $(AWK) '$$1 ~ /^[a-zA-Z_].+/ && $$3 ~ /^[0-9\"\047][0-9]*/{print "#define	" $$1 "		" $$3; }' >> $@
	for i in `echo $(DEVICE_PERI) | tr -t ',' ' '` ; \
	do \
		echo "#define	CONFIG_PERIPHERAL_$$i	1	/* defined in target.conf */" ; \
	done  >> $@
	echo	'#endif' >> $@

clean:
	$(RM) -fr obj


prog:
#	../progmcu/obj/progmcu.Linux-x86_64 --force --stm32 --uart --uartdev=$(UARTDEV) -e -p -k -f obj/STM32H7_gcc/bootloader.bin
	$(PROGMCU) --force --stm32 --uart --uartdev=/dev/ttyACM0 -e -p -k -f obj/STM32L4_gcc/bootloader.bin
