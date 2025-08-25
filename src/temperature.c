#include "main.h"
#include "temperature.h"
#include "flashHandler.h"

/**
 * K_WORK_DEFINE() Initialize a statically-defined work item. Does the same as:
  		struct k_work temp_work;
		k_work_init(&temp_work, temp_work_handler);
 * Instead of initialising at run time, this macro initializes the work item at compile time.
 * Therefore don't need to call k_work_init() in main() and can call k_work_submit() directly.
 */
K_WORK_DEFINE(get_temperature_work, getTemperature_Handler);

static temperature_data_t temperature_data_array[MAX_ENTRIES];
temperature_data_t temperature_data;

const struct k_work *getTemperatureWork_Handler()
{
	return &get_temperature_work;
}

void getDieTemperature()
{
	static uint8_t index = 0;
    struct sensor_value value;
	int rc;

	for (size_t i = 0; i < getNumberOfSensors(); i++) {
		rc = sensor_sample_fetch_chan(sensors[i], SENSOR_CHAN_DIE_TEMP);
		if (rc) {
			printk("Failed to fetch sample (%d)\n", rc);
			return;
		}

		rc = sensor_channel_get(sensors[i], SENSOR_CHAN_DIE_TEMP, &value);
		if (rc) {
			printk("Failed to get data (%d)\n", rc);
			return;
		}
	}
	
    temperature_data.timestamp = k_uptime_get();
    temperature_data.temperature = sensor_value_to_double(&value);
	
	printf("%llu Temperature: %f C\n", temperature_data.timestamp, temperature_data.temperature);

	// writeToFlash();
	/** WRITE TO FLASH
	 * Temperature val (double) = 8 bytes
	 * Timestamp (int64_t) = 8 bytes
	 * Total = 12 bytes
	 * 
	 * Flash should write to rolling buffer, overwriting oldest data when full.
	 * 
	 */

	temperature_data_array[index] = temperature_data;
	index = (index + 1) % MAX_ENTRIES; // wrap around if exceeds max
}

void getTemperature_Handler(struct k_work *work)
{
	getDieTemperature();
}

temperature_data_t* getTemperatureDataArray()
{
	return temperature_data_array;
}