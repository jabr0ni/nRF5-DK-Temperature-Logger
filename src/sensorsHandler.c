#include "main.h"
#include "sensorsHandler.h"

const struct device *const sensors[] = {LISTIFY(16, DIE_TEMPERATURE_SENSOR, ())};
const size_t sensors_count = ARRAY_SIZE(sensors);

const struct device *const *getDevice()
{
	return sensors;
}

uint8_t getNumberOfSensors()
{
	return sensors_count;
}

uint8_t checkSensorStatus()
{
	for (size_t i = 0; i < sensors_count; i++) {
		if (!device_is_ready(sensors[i])) {
			printk("sensor: device %s not ready.\n", sensors[i]->name);
			return 0;
		}
		else {
			printf("Sensor ready (%s)\n", sensors[i]->name);
		}
	}
	return 1;
}

