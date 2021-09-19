

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
//#include "nvs_flash.h"
//#include "protocol_examples_common.h"
#include "esp_sntp.h"

//extern TaskHandle_t xHandleNTP;

void obtain_time(void);
void initialize_sntp(void);
void vTaskNTP(void *pvParameters);
