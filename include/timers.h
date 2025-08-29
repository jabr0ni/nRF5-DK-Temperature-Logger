#ifndef TIMERS_H
#define TIMERS_H

#include "ledHandler.h"
#include "temperature.h"
#include "main.h"

// Timer config for LED blinking
extern struct k_timer led_toggle_timer, temperature_timer;

void init_timers(void);
/**
 * LED toggle timer callback
*/
void led_timer_handler(struct k_timer *dummy);
void temperature_timer_handler(struct k_timer *dummy);


#endif /* TIMERS_H */