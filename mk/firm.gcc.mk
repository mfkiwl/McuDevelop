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

# Makefile for gcc

BINDIR		= /usr/bin
COMPILER	= COMPILER_GCC
PORTABLE_DIR_CC	= GCC
#PORTABLE_ARCH	= ARM$(FREERTOS_ARCH)
#PORTABLE_ARCHDIR	= ARM_$(FREERTOS_ARCH)
#PORTABLE_DIR	= $(FREERTOS_DIR)/Source/portable/$(PORTABLE_CC)/$(PORTABLE_ARCHDIR)
#PORTABLE_TARGET	= $(PORTABLE_CC)_$(PORTABLE_ARCH)

DEPFLAGS	= -MMD -MP -MT"$@ ${@:.o=.d}" -MF"${@:.o=.d}"
LSTFLAGS	= -Wa,-a,-ad,-alms=$(OBJDIR)/$(notdir $(<:.c=.lst))
CFLAGS_DEV	= -ffunction-sections -fdata-sections -Os -gdwarf-2 \
			-mthumb -mcpu=$(CPU_CORTEX_ARCH) -mlittle-endian

ASFLAGS		= $(CPU) #-acdghlmns=${@:.o=.lst}

#LIBS		= -lc -lm -lnosys #-specs=nano.specs
LIBS		= -nostdlib
LDCONFIG	= ../target/$(DEVICE_TYPE)/src/$(TARGET).ld
LDFLAGS_MAP	= -Wl,-Map=${@:.elf=.map},--cref
LDFLAGS_SEC	= -Wl,--gc-sections
LDFLAGS_CFG	= -T$(LDCONFIG)

ARFLAGS		= rcs

ELFTOOLFLAGS_SREC	= -S -O srec --srec-forceS3 --srec-len=32
ELFTOOLFLAGS_BIN	= -S -O binary

CC		= $(BINDIR)/$(CROSS)gcc
CPP		= $(BINDIR)/$(CROSS)gcc
AS		= $(BINDIR)/$(CROSS)gcc -x assembler-with-cpp
#AS		= $(BINDIR)/$(CROSS)as
LD		= $(BINDIR)/$(CROSS)gcc
AR		= $(BINDIR)/$(CROSS)ar
CCDEP		= $(BINDIR)/$(CROSS)gcc
GCC		= $(BINDIR)/$(CROSS)cc
STRIP		= $(BINDIR)/$(CROSS)strip
ELFTOOL		= $(BINDIR)/$(CROSS)objcopy
SIZE		= $(BINDIR)/$(CROSS)size
