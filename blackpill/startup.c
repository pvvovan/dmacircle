#include <stdint.h>

#include "main.h"


void startup(void)
{

}

void reset_handler(void)
{
	startup();
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

uint32_t isr_vector_table[121] __attribute__ ((section(".isr_vector"))) =
{
	(uint32_t)&reset_handler,

	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler
};
