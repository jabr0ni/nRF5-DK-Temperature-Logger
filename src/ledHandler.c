#include "ledHandler.h"

// Get LED configuration from the devicetree
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// Configure LED GPIO
void initLED()
{
    if (!device_is_ready(led.port)) {
        printk("LED: device %s not ready.\n", led.port->name);
        return;
    } else {
        printf("LED ready (%s)\n", led.port->name);
    }

    if (!gpio_is_ready_dt(&led)) {
		return;
	}
    
	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE))
    {
        printk("LED: Failed to configure pin (%d)\n", led.pin);
		return;
	}
    else{
        printf("LED pin %d configured successfully.\n", led.pin);
    }
}

// Callback for LED toggle timer
void toggleLED()
{
    gpio_pin_toggle_dt(&led);
}