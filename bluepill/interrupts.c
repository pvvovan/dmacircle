#include <stdint.h>

#include <stm32f1xx.h>

#include "startup.h"
#include "main.h"


static __IO uint32_t system_timer_ticks = 0u;
static void system_timer_isr(void)
{
	system_timer_ticks++;
}

uint32_t get_system_timer_ticks(void)
{
	return system_timer_ticks;
}

static void default_handler(void)
{
	for ( ; ; ) {
		;
	}
}

const uint32_t isr_vector_table[116] __attribute__ ((section(".isr_vector"))) =
{
	(uint32_t)&reset_handler,

	(uint32_t)&default_handler,	// NMI			0x0000 0008
	(uint32_t)&default_handler,	// HardFault		0x0000 000C
	(uint32_t)&default_handler,	// MemManage		0x0000 0010
	(uint32_t)&default_handler,	// BusFault		0x0000 0014
	(uint32_t)&default_handler,	// UsageFault		0x0000 0018
	0u,				// Reserved		0x0000 001C - 0x0000 002B
	0u,
	0u,
	0u,
	(uint32_t)&default_handler,	// SVCall		0x0000 002C
	(uint32_t)&default_handler,	// Debug Monitor	0x0000_0030
	0u,				// Reserved		0x0000_0034
	(uint32_t)&default_handler,	// PendSV		0x0000_0038
	(uint32_t)system_timer_isr	// System tick timer	0x0000_003C
};
