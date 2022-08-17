GCC_PATH = /mnt/sda1/pvv/gcc/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin/
CC = $(GCC_PATH)arm-none-eabi-gcc
CXX = $(GCC_PATH)arm-none-eabi-g++
SZ = $(GCC_PATH)arm-none-eabi-size
OBJ_COPY = $(GCC_PATH)arm-none-eabi-objcopy
AS = $(CC) -x assembler-with-cpp
OUTDIR = out
DEBUG = 1

WARN = -Wall -Wextra -pedantic -Wsign-conversion -Wconversion -Werror=pedantic -pedantic-errors -Wpedantic

CC_FLAGS = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -specs=nano.specs \
-fstack-usage $(CC_DEFINES) -fcallgraph-info -MD -MP -MF"$(@:%.o=%.d)"

C_FLAGS = -std=gnu11 $(CC_FLAGS) $(WARN)

CXX_FLAGS = -std=gnu++17 -fno-exceptions -fno-rtti -fno-use-cxa-atexit $(CC_FLAGS) $(WARN)

LD_FLAGS = -specs=nosys.specs -static -Wl,-Map=$(OUTDIR)/final.map,--cref \
-Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group $(WARN)

ifeq ($(DEBUG), 1)
CC_FLAGS += -g3 -O0 -Og -gdwarf-2
else
CC_FLAGS += -O2 -ffunction-sections -fdata-sections
LD_FLAGS += -Wl,--orphan-handling=error -Wl,--gc-sections
endif

C_SOURCES = \
main.c \
startup.c \
interrupts.c \
../FreeRTOS-Kernel/tasks.c \
../FreeRTOS-Kernel/list.c \
../FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.c \
../FreeRTOS-Kernel/portable/MemMang/heap_1.c

CXX_SOURCES = \
led_toggler.cpp \
adc_handle.cpp

ASM_SOURCES = \
startstack.s

CC_INCLUDES = \
-iquote . \
-I ../STM32CubeF1/Drivers/CMSIS/Include \
-I ../STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
-I ../FreeRTOS-Kernel/include \
-I ../FreeRTOS-Kernel/portable/GCC/ARM_CM3

CC_DEFINES =  \
-DSTM32F103xB

OBJECTS = $(addprefix $(OUTDIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(OUTDIR)/, $(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))

OBJECTS += $(addprefix $(OUTDIR)/, $(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


$(shell test -d $(OUTDIR) || mkdir $(OUTDIR))
all: $(OUTDIR)/final.elf

$(OUTDIR)/final.elf: blue_ls.ld $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -T $^ -o $@
	$(OBJ_COPY) -O srec $@ $(OUTDIR)/final.srec
	$(SZ) $@

$(OUTDIR)/%.o: %.c Makefile
	$(CC) -c $(C_FLAGS) $(CC_INCLUDES) $< -o $@

$(OUTDIR)/%.o: %.cpp Makefile
	$(CXX) -c $(CXX_FLAGS) $(CC_INCLUDES) $< -o $@

$(OUTDIR)/%.o: %.s Makefile
	$(AS) -c $(C_FLAGS) $< -o $@

.PHONY: clean
clean:
	-rm -fR $(OUTDIR)


-include $(wildcard $(OUTDIR)/*.d)
