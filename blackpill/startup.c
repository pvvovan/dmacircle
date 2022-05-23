#include <stdint.h>

#include "main.h"


#define STACK_POINT		(0x20000000u + (128u * 1024u))

static void startup(void)
{

}

static void reset_handler(void)
{
	startup();
	main();
	for ( ; ; ) {
		;
	}
}

static void default_handler(void)
{
	for ( ; ; ) {
		;
	}
}

const uint32_t isr_vector_table[121] __attribute__ ((section(".isr_vector"))) =
{
	STACK_POINT,
	(uint32_t)&reset_handler,
	(uint32_t)&default_handler
};
