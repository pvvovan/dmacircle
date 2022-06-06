#include "led_toggler.h"
#include "gpio.hpp"


static gpio gpo{};

static void gpo_init()
{
	gpo.Init<13>(GPIOC, gpio::output::general_push_pull, gpio::speed::max2MHz);
}


volatile int glo_val{11};
int get_val()
{
	return glo_val * 2;
}

volatile int ini_val = get_val();

class demo_class {
public:
	demo_class() {
		ini_val = 44;
	}
};

static demo_class cl{};

void led_toggle()
{
	static bool gpio_ready = false;
	if (!gpio_ready) {
		gpio_ready = true;
		gpo_init();
	}

	ini_val++;
	gpo.Toggle<13>(GPIOC);
	ini_val--;
}
