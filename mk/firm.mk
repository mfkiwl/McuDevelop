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
MK_DIR		= $(CONFIG_ROOTDIR)/../mk


######################################################
# include
#
include		../config
include		../target/$(DEVICE_TYPE)/config
include		$(MK_DIR)/def.mk
include		$(MK_DIR)/cortex-m.mk


ifeq ($(ENV_CC), gcc)			### GCC
MAKEFILE_DEP	= $(MK_DIR)/firm.gcc.mk

else ifeq ($(ENV_CC), iar)		### IAR
MAKEFILE_DEP	= $(MK_DIR)/firm.iar.mk

else					### no environment
$(error no environment)

endif
include		$(MAKEFILE_DEP)


######################################################
# sources
#
#include		$(DESTDIR)/config.mk
include		$(CONFIG_MK)
vpath		%.c $(sort $(dir $(SOURCES)))
vpath		%.s $(sort $(dir $(SOURCESASM)))
vpath		%.S $(sort $(dir $(SOURCESASMC)))

OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SOURCESASM:.s=.o))) \
		  $(addprefix $(OBJDIR)/, $(notdir $(SOURCESASMC:.S=.o))) \
			$(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))

INCDIRS_LIB	= -I $(CMSIS_DIR)/Include \
		-I $(CMSIS_DIR)/DSP/Include \
		-I $(FREERTOS_DIR)/portable/$(PORTABLE_DIR_CC)/$(CONFIG_FREERTOS_MCUARCH) \
		-I $(FREERTOS_DIR)/include \
		-I $(USBDIF_DIR)/include \
		-I $(USBDIF_DIR)/src \
		-I $(SDMMC_DIR)/include \
		-I $(SDMMC_DIR)/src \
		-I $(FATFS_DIR)/source \
		-I $(DEVICELIB_DIR)/$(DEVICE_TYPE)/include \
		-I $(DEVICELIB_DIR)/$(VENDOR_SERIES)/include \
		-I $(DEVICELIB_DIR)/$(VENDOR_SERIES)/src \
		-I $(DEVICELIB_DIR)/include \
		-I $(DEVICE_DIR)/src \
		-I $(COMMON_DIR) \
		-I $(LLVM_DIR) \
		-DULONG_MAX=UINT32_MAX

CFLAGS		= $(CFLAGS_DEV) $(DEFINES) $(INCDIRS) $(INCDIRS_LIB) $(DEPFLAGS) $(LSTFLAGS)

LDFLAGS		= $(LDFLAGS_CFG) $(LDFLAGS_SEC)  $(LDFLAGS_MAP)  $(LIBSEXT)

DEPENDS		= $(OBJS:.o=.d)


######################################################
# tool definitions
#
CP		= /bin/cp
RM		= /bin/rm
SHELL		= /bin/bash


######################################################
# make special target definitions
#
.SUFFIXES:	.c .s .S .o .d
.PHONY:		clean depend


######################################################
# entry point
#
all: $(OBJDIR)/$(TARGET).srec $(OBJDIR)/$(TARGET).bin


######################################################
# ld, file convert
#
$(OBJDIR)/$(TARGET).srec: $(OBJDIR)/$(TARGET).elf
	$(ELFTOOL) $(ELFTOOLFLAGS_SREC) $< $@

$(OBJDIR)/$(TARGET).bin: $(OBJDIR)/$(TARGET).elf
	$(ELFTOOL) $(ELFTOOLFLAGS_BIN) $< $@

$(OBJDIR)/$(TARGET).elf: $(OBJS) $(LDCONFIG)
	$(LD) $(CPU) $(OBJS) $(LIBS) $(LDFLAGS) -o $@
	$(SIZE)	$@


######################################################
# cc
#
ifeq ($(ENV_CC), gcc)			### GCC
$(OBJDIR)/%.o: %.c
	$(CC) -c $(CPU) -o $@ $(CFLAGS) $<

else ifeq ($(ENV_CC), iar)		### IAR
$(OBJDIR)/%.o: %.c
	$(CC) -o $@ $(CFLAGS) $<

else
$(error no environment)

endif

$(OBJDIR)/%.o: %.s
	$(AS)  -c $(CPU) $(ASFLAGS) -o $@ $<
$(OBJDIR)/%.o: %.S
	$(AS)  -c $(CPU) $(ASFLAGS) -o $@ $<


######################################################
# Makefile
#
$(OBJS): $(MK_DIR)/firm.mk $(MAKEFILE_DEP)


######################################################
# clean, etc
#
clean:
	$(RM) -f $(OBJS) ${OBJS:.o=.ss} ${OBJS:.o=.lst}


######################################################
# depend
#
$(OBJDIR)/%.d: %.s
	echo "${@:.d=.o} $@: $(DESTDIR)/config.inc" > $@
$(OBJDIR)/%.d: %.S
	echo "${@:.d=.o} $@: $(DESTDIR)/config.inc" > $@

-include $(DEPENDS)
