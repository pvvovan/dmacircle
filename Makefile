CC=/mnt/sda1/pvv/gcc/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc
C_FLAGS=-mcpu=cortex-m4 -mthumb -std=gnu18 -g -O0 -Og

all: *.o

*.o: *.c
	$(CC) -c $(C_FLAGS) $^

.PHONY: clean
clean:
	rm -rf ./*.o
