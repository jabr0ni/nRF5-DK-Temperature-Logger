#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include "main.h"

typedef struct {
	int64_t timestamp;		// 8 bytes
	double temperature;		// 8 bytes

}temperature_data_t;

void getTemperature_Handler(struct k_work *work);
void getDieTemperature(void);

temperature_data_t* getTemperatureDataArray();

const struct k_work *getTemperatureWork_Handler(void);





#endif /* _TEMPERATURE_H_ */