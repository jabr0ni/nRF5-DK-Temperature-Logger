#include "timers.h"
#include "main.h"

struct k_timer led_toggle_timer, temperature_timer;

void init_timers(void)
{
    // Initialize the LED system status timer
    k_timer_init(&led_toggle_timer, led_timer_handler, NULL);
    k_timer_start(&led_toggle_timer, K_SECONDS(1), K_SECONDS(1));

    // sampling timer for temperature reading
    k_timer_init(&temperature_timer, temperature_timer_handler, NULL);
}

void led_timer_handler(struct k_timer *dummy)
{	
    toggleLED();
}

void temperature_timer_handler(struct k_timer *dummy)
{
	// submit work order for temperature readings
    k_work_submit((struct k_work*)getTemperatureWork_Handler());
}