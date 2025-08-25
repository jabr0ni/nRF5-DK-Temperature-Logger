/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "main.h"

int main(void)
{
	uint8_t numberOfSensors = 0;
	initLED();
	initButtons();
	// initFlash();
	init_timers();

	// Check device sensor status
	if(!checkSensorStatus()) {
		printk("Sensor not ready, exiting...\n");
		return 0;
	}
	else{
		printk("All sensors ready.\n");
		printf("Number of temperature sensors: %d\n", numberOfSensors);
	}
	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
