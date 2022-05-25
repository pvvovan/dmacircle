CC=/mnt/sda1/pvv/gcc/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc
AS = $(CC) -x assembler-with-cpp
C_FLAGS=-mcpu=cortex-m4 -mthumb -std=gnu18 -g -O0 -Og -Wall -Wextra -mfloat-abi=soft

C_SOURCES = \
main.c \
startup.c

ASM_SOURCES = \
startstack.s

OBJECTS = $(C_SOURCES:.c=.o)

OBJECTS += $(ASM_SOURCES:.s=.o)

all: final.elf

final.elf: blackls.ld $(OBJECTS)
	$(CC) -nostdlib -T $^ $(C_FLAGS) -Wl,-Map=final.map -o $@

%.o: %.c Makefile
	$(CC) -c $(C_FLAGS) $< -o $@

%.o: %.s Makefile
	$(AS) -c $(C_FLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fR ./*.o ./*.elf ./*.map
