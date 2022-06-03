#include <stdint.h>

#include "main.h"
#include "startup.h"
#include "stm32f1xx.h"


// #define __BKPT(value)			__asm volatile ("bkpt "#value)

extern uintptr_t _start_data;
extern uintptr_t _end_data;
extern uintptr_t _start_bss;
extern uintptr_t _end_bss;
extern uintptr_t _sidata;

static void init_memory(void)
{
	volatile uint32_t *ram_ptr = (volatile uint32_t *)&_start_data;
	const uint32_t *flash_ptr = (const uint32_t *)&_sidata;

	while (ram_ptr < (volatile uint32_t *)&_end_data) {
		*ram_ptr = *flash_ptr;
		ram_ptr++;
		flash_ptr++;
	}

	ram_ptr = (volatile uint32_t *)&_start_bss;
	while (ram_ptr < (const uint32_t *)&_end_bss) {
		*ram_ptr = 0u;
		ram_ptr++;
	}
}

void __libc_init_array(void);

void reset_handler(void)
{
	__BKPT(0); // Set software breakpoint twice ;)
	asm volatile ("ldr sp, =_estack");
	asm volatile ("bkpt 0");

	init_memory();
	__libc_init_array();

	main(); // main() should never return
	for ( ; ; ) {
		;
	}
}
