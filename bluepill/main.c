#include <stdint.h>

#include <stm32f1xx.h>
#include <FreeRTOS.h>
#include <task.h>

#include "main.h"
#include "led_toggler.h"
#include "interrupts.h"

static void delay_ds(uint32_t ds) __attribute__((long_call, section(".ram_exec")));
static void task1(void *pvParameters);
static void task2(void *pvParameters);

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


const int c_data = 1;
int i_data = 2;
int i2_data = 1;
int bss_value;
static int s_data = 11;
static int s_bss;

int main(void)
{
	init_system_clock(); // 8MHz HSE produces 72MHz CPU clock
	// SysTick_Config(72000000 / 10); // 10Hz system timer irq

	xTaskCreate(task1, "task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(task2, "task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
	vTaskStartScheduler();

	while (s_data++) {
		s_bss++;
		delay_ds(5);
		led_toggle();
	}

	return 0;
}

static void delay_ds(uint32_t ds)
{
	const uint32_t system_timer_ticks_at_start = get_system_timer_ticks();
	while (get_system_timer_ticks() - system_timer_ticks_at_start < ds) {
		;
	}
}

static void task2(void *pvParameters)
{
	(void)pvParameters;
	for ( ; ; ) {
		vTaskDelay((TickType_t)1000 / portTICK_PERIOD_MS);
	}
}

static void task1(void *pvParameters)
{
	(void)pvParameters;
	for ( ; ; ) {
		led_toggle();
		vTaskDelay((TickType_t)500 / portTICK_PERIOD_MS);
	}
}
