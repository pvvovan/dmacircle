GCC_PATH=/mnt/sda1/pvv/gcc/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin
CC=$(GCC_PATH)/arm-none-eabi-gcc
SZ=$(GCC_PATH)/arm-none-eabi-size
OBJ_COPY=$(GCC_PATH)/arm-none-eabi-objcopy
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
	$(OBJ_COPY) -O srec $@ final.srec
	$(SZ) $@

%.o: %.c Makefile *.h
	$(CC) -c $(C_FLAGS) $< -o $@

%.o: %.s Makefile
	$(AS) -c $(C_FLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fR ./*.o ./*.elf ./*.map ./*.srec
