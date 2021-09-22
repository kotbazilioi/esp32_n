/*
 * app.c
 *
 *  Created on: 22 сент. 2021 г.
 *      Author: ivanov
 */
#include "includes_base.h"
#include "LOGS.h"
#include "app.h"
#include "sntp_task.h"
#include "smtp.h"
TaskHandle_t xHandleNTP = NULL;
SemaphoreHandle_t S_gpio_port1 = NULL;
SemaphoreHandle_t S_gpio_port2 = NULL;
uint8_t port_data[2];
#define PORT_O1 16
#define PORT_O2 6
void gpio1_task(void *pvParameters)
{
	gpio_set_direction(PORT_O2, GPIO_MODE_OUTPUT);
	while(1)
		{

		if( S_gpio_port1 != NULL )
				    {

				        if( xSemaphoreTake( S_gpio_port1, ( TickType_t ) 100 ) == pdTRUE )
				        {
		 	 	 	 	 gpio_set_level(PORT_O1,port_data[0]);

				            xSemaphoreGive( S_gpio_port1 );
				        }
				        else
				        {
				            /* We could not obtain the semaphore and can therefore not access
				            the shared resource safely. */
				        }
				    }
	     vTaskDelay(300 / portTICK_PERIOD_MS);
		}
}
void gpio2_task(void *pvParameters)
{
	gpio_set_direction(PORT_O2, GPIO_MODE_OUTPUT);
	while(1)
		{

		if( S_gpio_port2 != NULL )
				    {

				        if( xSemaphoreTake( S_gpio_port2, ( TickType_t ) 100 ) == pdTRUE )
				        {
			        	 gpio_set_level(PORT_O2,port_data[1]);

				            xSemaphoreGive( S_gpio_port2 );
				        }
				        else
				        {
				            /* We could not obtain the semaphore and can therefore not access
				            the shared resource safely. */
				        }
				    }
	     vTaskDelay(300 / portTICK_PERIOD_MS);
		}
}

 void start_task(void *pvParameters)
 {

	 flag_global_save_log = xSemaphoreCreateMutex ();
	 S_gpio_port1 = xSemaphoreCreateMutex ();
	 S_gpio_port2 = xSemaphoreCreateMutex ();

	 xTaskCreate(&vTaskNTP, "vTaskNTP", 2048, NULL, 5, &xHandleNTP);
	 xTaskCreate(&mdns_example_task, "mdns_example_task", 2048, NULL, 5, NULL);
	 xTaskCreate(&nvs_task, "nvs_task", 2048, NULL, 5, NULL);

	 xTaskCreate(&gpio1_task, "gpio1_task", 512, NULL, 10, NULL);
	 xTaskCreate(&gpio2_task, "gpio2_task", 512, NULL, 10, NULL);
	 vTaskDelay(1000 / portTICK_PERIOD_MS);
	  my_smtp_test();

	 vTaskDelete(NULL);
 }
