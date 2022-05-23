CC=/mnt/sda1/pvv/gcc/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc
C_FLAGS=-mcpu=cortex-m4 -mthumb -std=gnu18 -g -O0 -Og -Wall -Wextra

C_SOURCES = \
main.c \
startup.c

OBJECTS = $(C_SOURCES:.c=.o)

all: final.elf

final.elf: $(OBJECTS) Makefile

%.o: %.c Makefile
	$(CC) -c $(C_FLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fR ./*.o
