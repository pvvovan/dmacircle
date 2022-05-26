#include <stdint.h>

#include "main.h"


#define __BKPT(value)			__asm volatile ("bkpt "#value)

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

void reset_handler(void)
{
	__BKPT(0); // Set software breakpoint twice ;)
	asm volatile ("bkpt 0");

	init_memory();

	main();
	for ( ; ; ) {
		;
	}
}

void default_handler(void)
{
	for ( ; ; ) {
		;
	}
}

const uint32_t isr_vector_table[116] __attribute__ ((section(".isr_vector"))) =
{
	(uint32_t)&reset_handler,

	(uint32_t)&default_handler,	// NMI		0x0000 0008
	(uint32_t)&default_handler,	// HardFault	0x0000 000C
	(uint32_t)&default_handler,	// MemManage	0x0000 0010
	(uint32_t)&default_handler,	// BusFault	0x0000 0014
	(uint32_t)&default_handler,	// UsageFault	0x0000 0018
	0,				// Reserved	0x0000 001C - 0x0000 002B
	0,
	0,
	0,
	(uint32_t)&default_handler	// SVCall	0x0000 002C
};
