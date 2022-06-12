#include "led_toggler.h"
#include "gpio.hpp"
#include "adc_handle.hpp"


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
	bool is_adc_poll = false;
	static bool called_once{false};
	if (!called_once) {
		called_once = true;
		gpo_init();
		if (is_adc_poll) {
			adc_init();
		} else {
			adc_start_dma();
		}
	}

	ini_val++;
	gpo.Toggle<13>(GPIOC);
	if (is_adc_poll) {
		adc_convert();
	}
	ini_val--;
}
