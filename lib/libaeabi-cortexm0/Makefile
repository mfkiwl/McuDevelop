AS=arm-none-eabi-as -mthumb

SRC_FILES=$(wildcard *.S)


.PHONY: all clean

all: libaeabi-cortexm0.a

libaeabi-cortexm0.a: $(SRC_FILES:%.S=%.o)
	ar rcs libaeabi-cortexm0.a $(SRC_FILES:%.S=%.o)

%.o: %.S
	$(AS) $< -o $@

clean:
	-rm -f *.o *.a
