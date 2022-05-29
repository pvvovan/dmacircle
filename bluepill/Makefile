GCC_PATH=/mnt/sda1/pvv/gcc/gcc-arm-none-eabi-10.3-2021.07/bin
CC=$(GCC_PATH)/arm-none-eabi-gcc
SZ=$(GCC_PATH)/arm-none-eabi-size
OBJ_COPY=$(GCC_PATH)/arm-none-eabi-objcopy
AS = $(CC) -x assembler-with-cpp
C_FLAGS=-mcpu=cortex-m4 -mthumb -std=gnu18 -g -O0 -Og -Wall -Wextra -pedantic -Wsign-conversion -Wconversion -Werror=pedantic -pedantic-errors -mfloat-abi=soft $(C_DEFINES)

C_SOURCES = \
main.c \
startup.c \
interrupts.c

ASM_SOURCES = \
startstack.s

C_INCLUDES = \
-I. \
-I../STM32CubeF1/Drivers/CMSIS/Include \
-I../STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include

C_DEFINES =  \
-DSTM32F103xB

OBJECTS = $(C_SOURCES:.c=.o)

OBJECTS += $(ASM_SOURCES:.s=.o)

all: final.elf

final.elf: bluels.ld $(OBJECTS)
	$(CC) -nostdlib -T $^ $(C_FLAGS) -Wl,-Map,final.map -o $@
	$(OBJ_COPY) -O srec $@ final.srec
	$(SZ) $@

%.o: %.c Makefile *.h
	$(CC) -c $(C_FLAGS) $(C_INCLUDES) $< -o $@

%.o: %.s Makefile
	$(AS) -c $(C_FLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fR ./*.o ./*.elf ./*.map ./*.srec
