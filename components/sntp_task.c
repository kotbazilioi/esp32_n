

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

#include "sntp_task.h"


static const char *TAG_SNTP = "SNTP_TASK";

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG_SNTP, "Notification of a time synchronization event");
}
void initialize_sntp(void) {
	ESP_LOGI(TAG_SNTP, "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, "pool.ntp.org");
	sntp_setservername(1, "ntp3.stratum2.ru");
	sntp_set_time_sync_notification_cb(time_sync_notification_cb);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
	sntp_init();
}
void obtain_time(void) {

	// wait for time to be set
	time_t now = 0;
	struct tm timeinfo = { 0 };
	int retry = 0;
	const int retry_count = 20;
	while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET
			&& ++retry < retry_count) {
		ESP_LOGI(TAG_SNTP, "Waiting for system time to be set... (%d/%d)", retry,
				retry_count);
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
	if (retry==retry_count)sntp_restart();
	ESP_LOGD(TAG_SNTP, "Time OK)");
	time(&now);
	localtime_r(&now, &timeinfo);

	//   ESP_ERROR_CHECK( example_disconnect() );
}

// xTaskCreate(&vTaskNTP, "vTaskNTP", 2048, NULL, 10, &xHandleNTP);


void vTaskNTP(void *pvParameters) {
	initialize_sntp();
	for (;;) {
		obtain_time();

		vTaskDelay(60 * 1000 / portTICK_PERIOD_MS);
	}
}
