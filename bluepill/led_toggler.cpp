#include "led_toggler.h"
#include "gpio.hpp"


static gpio gpo{};

static void init()
{
	gpo.Init<13>(GPIOC, gpio::output::general_push_pull, gpio::speed::max2MHz);
}

void led_toggle()
{
	static bool gpio_ready = false;
	if (!gpio_ready) {
		gpio_ready = true;
		init();
	}

	gpo.Toggle<13>(GPIOC);
}
