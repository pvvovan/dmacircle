#include "main.h"

const int c_data = 1;
int i_data = 2;
int i2_data = 1;
int bss_value;
static int s_data = 11;
static int s_bss;

int main(void)
{
	while (s_data++) {
		s_bss++;
	}
	return 0;
}
