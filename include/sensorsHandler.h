#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "main.h"

#define DIE_TEMP_ALIAS(i) DT_ALIAS(_CONCAT(die_temp, i))
#define DIE_TEMPERATURE_SENSOR(i, _)                                                               \
	IF_ENABLED(DT_NODE_EXISTS(DIE_TEMP_ALIAS(i)), (DEVICE_DT_GET(DIE_TEMP_ALIAS(i)),))

/* support up to 16 cpu die temperature sensors */
extern const struct device *const sensors[];
extern const size_t sensors_count;

uint8_t checkSensorStatus();
uint8_t getNumberOfSensors();

const struct device *const *getDevice();

#endif /* _SENSORS_H_ */