#include <stdint.h>

#include <stm32f1xx.h>

#include "main.h"
#include "startup.h"


// #define __BKPT(value)			__asm volatile ("bkpt "#value)

static void copy_memory(volatile uint32_t *dest, const uint32_t *src, volatile uint32_t *dest_end)
{
	while (dest < dest_end) {
		*dest = *src;
		dest++;
		src++;
	}
}

extern uintptr_t _siram_exec;
extern uintptr_t _startram_exec;
extern uintptr_t _endram_exec;
static void init_ramtext(void)
{
	copy_memory((volatile uint32_t *)&_startram_exec,
			(const uint32_t *)&_siram_exec,
			(volatile uint32_t *)&_endram_exec);
}

extern uintptr_t _start_data;
extern uintptr_t _end_data;
extern uintptr_t _start_bss;
extern uintptr_t _end_bss;
extern uintptr_t _sidata;
static void init_databss(void)
{
	volatile uint32_t *ram_ptr = (volatile uint32_t *)&_start_data;
	const uint32_t *flash_ptr = (const uint32_t *)&_sidata;
	copy_memory(ram_ptr, flash_ptr, (volatile uint32_t *)&_end_data);

	ram_ptr = (volatile uint32_t *)&_start_bss;
	while (ram_ptr < (const uint32_t *)&_end_bss) {
		*ram_ptr = 0u;
		ram_ptr++;
	}
}

typedef void (*init_func_t)(void);
extern init_func_t __init_array_start, __init_array_end;
void init_variables()
{
    init_func_t *func_ptr = &__init_array_start;
    while (func_ptr < &__init_array_end) {
        (*func_ptr)();
        func_ptr++;
    }
}

void __libc_init_array(void);
void reset_handler(void)
{
	__BKPT(0); // Set software breakpoint twice ;)
	asm volatile ("ldr sp, =_estack");
	asm volatile ("bkpt #0");

	init_ramtext();
	init_databss();
	// init_variables();		// init global and static variables
	__libc_init_array();		// init global and static variables

	main(); // main() should never return
	for ( ; ; ) {
		;
	}
}
