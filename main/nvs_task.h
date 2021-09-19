/*
 * nvs_task.h
 *
 *  Created on: 16 сент. 2021 г.
 *      Author: ivanov
 */

#ifndef COMPONENTS_NVS_TASK_H_
#define COMPONENTS_NVS_TASK_H_

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"


void nvs_task(void *pvParameters);




#endif /* COMPONENTS_NVS_TASK_H_ */
