### target infromation
include		../../../config

TARGET		= boot1

CONFIGFILE	= ../../../config
LOGFILE		= log
DEPENDFILE	= _depend
TMPDIR		= /tmp
TOOLBINDIR	= ..

#TARGET		= 
LOGFILE		= log

SOURCESASM	= start_iar.s

SOURCES		= boot1.c system_MKL27Z4.c crc32.c


SOURCESCPP	= 

SOURCES		= $(wildcard *.c)
OBJDIR		= ../../../obj/$(DEVICE_TYPE)_$(ENV_CC)/boot1

OBJS		= $(addprefix $(OBJDIR)/, $(SOURCESASM:.s=.o)) \
			$(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

LIBS		= 
ICFFILE		= boot1.icf

CFLAGS		= --silent -Oh \
		--cpu=$(DEVICE_ARCHTYPE) -e --fpu=None \
		-DCPU_$(DEVICE_NAME) \
		-DBL_TARGET_FLASH \
		-I . -I ../../../obj \
		-I ../../sdk.kinetis12/include \
		-I ../../sdk.kinetis12/include/device \
		-I ../../sdk.kinetis12/include/device/$(DEVICE_TYPE)/startup \
		-I $(IAR)/CMSIS/Include \
		--no_cse --no_unroll --no_code_motion --no_tbaa \
		--no_clustering --no_scheduling --endian=little \
		-e --diag_suppress=Pa050 \
		--dlib_config "$(IAR)/inc/c/DLib_Config_Normal.h"

#		--dlib_config "C:/Program Files/IAR Systems/Embedded Workbench 6.0 Evaluation/arm/INC/c/DLib_Config_Normal.h"

ASFLAGS		= --cpu $(DEVICE_ARCHTYPE) --fpu None \
		-S -s+ -M\<\> -w+ -r \
		-L. -t8 \
		-Iapp/dfu_bootloader \
		-DBL_TARGET_FLASH


LDFLAGS		= --silent --config $(ICFFILE) \
		--log libraries,initialization,modules,redirects,sections,veneers,unused_fragments \
		--log_file $(OBJDIR)/$(TARGET).log  --map $(OBJDIR)/$(TARGET).map \
		--redirect __aeabi_idiv=__iar_small___aeabi_idiv --redirect \
				__aeabi_uidiv=__iar_small___aeabi_uidiv --redirect \
				__aeabi_idivmod=__iar_small___aeabi_idivmod --redirect \
				__aeabi_uidivmod=__iar_small___aeabi_uidivmod \
		--entry Reset_Handler \
		--no_exceptions --merge_duplicate_sections --vfe \
		--diag_suppress Lp048,Lp049

#		--redirect _Printf=_PrintfSmall --redirect _Scanf=_ScanfSmall \
		--config_def __ram_vector_table__=1 \
		--config_def __stack_size__=0x400 \
		--config_def __heap_size__=0x100 \
#		--semihosting \
#		--entry __iar_program_start --vfe

ELFTOOLFLAGS_SREC	= --silent --srec --srec-s3only --srec-len=32
ELFTOOLFLAGS_BIN	= --silent --bin

CFLAGSDEP	= -I . -I .. -I $(IAR)/CMSIS/Include

INSTALLDIR	= ../../binary

CC		= iccarm
CPP		= iccarm
AS		= iasmarm
LK		= ilinkarm
CCDEP		= $(CROSS_COMPILE)gcc
GCC		= cc
CP		= cp
STRIP		= $(CROSS_COMPILE)strip
RM		= /bin/rm
ELFTOOL		= ielftool
SIZE		= size

.SUFFIXES:	.c .cpp .s .o .dat
.PHONY:		clean depend

all: rmlog $(OBJDIR)/$(TARGET).elf


$(OBJDIR)/$(TARGET).srec: $(OBJDIR)/$(TARGET).elf
	$(ELFTOOL) $(ELFTOOLFLAGS_SREC) $< $@

$(OBJDIR)/$(TARGET).bin: $(OBJDIR)/$(TARGET).elf
	$(ELFTOOL) $(ELFTOOLFLAGS_BIN) $< $@

$(OBJDIR)/$(TARGET).elf: $(OBJS) $(OBJDIR)/$(DEPENDFILE) $(ICFFILE) # $(CONFIGFILE)
	$(LK) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
	$(SIZE)	$@

$(OBJDIR)/%.o: %.c
	$(CC) -o $@ $(CFLAGS) -lA ${@:.o=.ss} -lCN ${@:.o=.lst} --dependencies=m ${@:.o=.d} $<

$(OBJDIR)/%.o: %.s
	$(AS) -o $@ $(ASFLAGS) $<
	mv ${<:.s=.lst} $(OBJDIR)/.

rmlog:
	$(RM) -f $(LOGFILE)

log: $(OBJS)
	$(RM) -f $(LOGFILE)

clean:
	$(RM) -f $(DEPENDFILE)

$(OBJDIR)/$(DEPENDFILE): $(OBJS)
	cat $(OBJS:.o=.d) | sed 's/\\/\//g'| sed 's/\(\w\):\//\/cygdrive\/\1\//g' > $@
#	echo "MKL27Z4/startup_MKL27Z4.o: MKL27Z4/startup_MKL27Z4.s ../../../obj/config.inc" >> $@


ifeq ($(OBJDIR)/$(DEPENDFILE), $(wildcard $(OBJDIR)/$(DEPENDFILE)))
-include $(OBJDIR)/$(DEPENDFILE)
endif

install: $(TARGET)
	$(STRIP) $(TARGET)
	$(CP) -f $(TARGET) $(INSTALLDIR)/.
