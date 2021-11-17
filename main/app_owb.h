/*
 * app_owb.h
 *
 *  Created on: 8 ����. 2021 �.
 *      Author: ivanov
 */
#include "config_pj.h"

#if MAIN_APP_OWB_H_
#define GPIO_DS18B20_0       (4)
#define MAX_DEVICES          (2)
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD        (10000)   // milliseconds


extern float readings[MAX_DEVICES];

 void app_owb(void *pvParameters);

#endif /* MAIN_APP_OWB_H_ */
