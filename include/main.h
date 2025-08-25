#include <stdio.h>
#include <devicetree_generated.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include "temperature.h"
#include "sensorsHandler.h"
#include "ledHandler.h"
#include "buttonHandler.h"
#include "flashHandler.h"
#include "timers.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   10
