#include <stdio.h>

#include "main.h"

const int c_data = 1;
int i_data = 2;
int i2_data = 1;
int bss_value;
static int s_data = 11;
static int s_bss;

int main(void)
{
	volatile double d = 1.1;
	d *= 1.37;
	fprintf(stdout, "demo start: %f\n", d);
	while (s_data++) {
		s_bss++;
	}
	fprintf(stdout, "%s", "demo end\n");
	return 0;
}
