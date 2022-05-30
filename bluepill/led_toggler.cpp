#include "led_toggler.h"
#include "Gpio.hpp"


static Gpio gpio{};

static void init()
{
	gpio.Init<13>(GPIOC, Gpio::Mode::output);
}

void led_toggle()
{
	static bool gpio_ready = false;
	if (!gpio_ready) {
		gpio_ready = true;
		init();
	}

	gpio.Toggle<13>(GPIOC);
}
