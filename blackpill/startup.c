#include <stdint.h>

#include "main.h"


#define STACK_POINT		(0x20000000u + (128u * 1024u))

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
	STACK_POINT,
	(uint32_t)&reset_handler,
	(uint32_t)&default_handler
};
