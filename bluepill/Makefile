GCC_PATH=/mnt/sda1/pvv/gcc/gcc-arm-none-eabi-10.3-2021.07/bin/
CC=$(GCC_PATH)arm-none-eabi-gcc
CXX=$(GCC_PATH)arm-none-eabi-g++
SZ=$(GCC_PATH)arm-none-eabi-size
OBJ_COPY=$(GCC_PATH)arm-none-eabi-objcopy
AS = $(CC) -x assembler-with-cpp

WARN=-Wall -Wextra -pedantic -Wsign-conversion -Wconversion -Werror=pedantic -pedantic-errors -Wno-pedantic

CC_FLAGS=-mcpu=cortex-m3 -mthumb -g3 -O0 -Og -mfloat-abi=soft \
-fstack-usage -ffunction-sections -fdata-sections --specs=nano.specs $(CC_DEFINES) \
-fcallgraph-info

C_FLAGS=-std=gnu18 $(CC_FLAGS) $(WARN)
CXX_FLAGS=-std=gnu++17 -fno-threadsafe-statics -fno-exceptions -fno-use-cxa-atexit $(CC_FLAGS) $(WARN)

C_SOURCES = \
main.c \
startup.c \
interrupts.c

CXX_SOURCES = \
led_toggler.cpp

ASM_SOURCES = \
startstack.s

CC_INCLUDES = \
-I. \
-I../STM32CubeF1/Drivers/CMSIS/Include \
-I../STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include

CC_DEFINES =  \
-DSTM32F103xB

OBJECTS = $(C_SOURCES:.c=.o)
OBJECTS += $(CXX_SOURCES:.cpp=.o)
OBJECTS += $(ASM_SOURCES:.s=.o)

all: final.elf

final.elf: bluels.ld $(OBJECTS)
	$(CXX) --specs=nosys.specs -static -T $^ $(CXX_FLAGS) -Wl,-Map,final.map -o $@ -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	$(OBJ_COPY) -O srec $@ final.srec
	$(SZ) $@

%.o: %.c Makefile *.h
	$(CC) -c $(C_FLAGS) $(CC_INCLUDES) $< -o $@

%.o: %.cpp Makefile *.h *.hpp
	$(CXX) -c $(CXX_FLAGS) $(CC_INCLUDES) $< -o $@

%.o: %.s Makefile
	$(AS) -c $(C_FLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fR ./*.o ./*.elf ./*.map ./*.srec
