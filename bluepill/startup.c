#include <stdint.h>

#include "main.h"
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

static void init_system_clock(void)
{
	RCC->CR |= RCC_CR_HSEON; // Enable High Speed External clock
	while ((RCC->CR & RCC_CR_HSERDY) == 0u) { } // wait until HSE oscillator is stable

	FLASH->ACR |= FLASH_ACR_PRFTBE; // Enable the FLASH prefetch buffer
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_2; // Set 2 wait states if 48MHz < SYSCLK <= 72MHz

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // Bits 7:4 HPRE: AHB prescaler
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // Bits 13:11 PPRE2: APB high-speed prescaler (APB2)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
	RCC->CFGR &= ~RCC_CFGR_PLLXTPRE; // Bit 17 PLLXTPRE: HSE divider for PLL entry
	RCC->CFGR |= RCC_CFGR_PLLSRC; // Bit 16 PLLSRC: PLL entry clock source
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // Bits 21:18 PLLMUL: PLL multiplication factor

	RCC->CR |= RCC_CR_PLLON; // Bit 24 PLLON: PLL enable
	while ((RCC->CR & RCC_CR_PLLRDY) == 0u) { } // Bit 25 PLLRDY: PLL clock ready flag

	RCC->CFGR &= ~RCC_CFGR_SW; // Bits 1:0 SW: System clock switch
	RCC->CFGR |= RCC_CFGR_SW_PLL; // 10: PLL selected as system clock
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { } // Bits 3:2 SWS: System clock switch status
}

void reset_handler(void)
{
	__BKPT(0); // Set software breakpoint twice ;)
	asm volatile ("ldr sp, =_estack");
	asm volatile ("bkpt 0");

	init_memory();

	init_system_clock();
	SysTick_Config(72000000 / 10);

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

uint32_t system_timer_ticks = 0u;
void sys_tick_handler(void)
{
	system_timer_ticks++;
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
	(uint32_t)sys_tick_handler	// System tick timer	0x0000_003C
};
