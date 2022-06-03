#include "led_toggler.h"
#include "gpio.hpp"


static gpio gpo{};

static void gpo_init()
{
	gpo.Init<13>(GPIOC, gpio::output::general_push_pull, gpio::speed::max2MHz);
}


volatile int myg_val = 11;
int get_val()
{
	return myg_val;
}

volatile int glo_what = get_val();

void led_toggle()
{
	static bool gpio_ready = false;
	if (!gpio_ready) {
		gpio_ready = true;
		gpo_init();
	}

	glo_what++;
	gpo.Toggle<13>(GPIOC);
	glo_what--;
}
