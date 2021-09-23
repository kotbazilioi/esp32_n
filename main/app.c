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
#include "tcpip_adapter.h"
TaskHandle_t xHandleNTP = NULL;
SemaphoreHandle_t S_gpio_port1 = NULL;
SemaphoreHandle_t S_gpio_port2 = NULL;
uint8_t port_data[2];
uint8_t chipid[6];
uint32_t  serial_id;
tcpip_adapter_ip_info_t ipInfo;
esp_netif_dns_type_t dns_info;
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


	 esp_mac_type_t eth = 3;
	 esp_read_mac(chipid,eth);
	 serial_id = (chipid[3]<<24)|(chipid[2]<<16)|(chipid[1]<<8)|(chipid[0]);
	 tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ipInfo);
	 FW_data.net.V_IP_CONFIG[0]=(0xff&(ipInfo.ip.addr));
	 FW_data.net.V_IP_CONFIG[1]=(0xff&(ipInfo.ip.addr>>8));
	 FW_data.net.V_IP_CONFIG[2]=(0xff&(ipInfo.ip.addr>>16));
	 FW_data.net.V_IP_CONFIG[3]=(0xff&(ipInfo.ip.addr>>24));

	 FW_data.net.V_IP_MASK[0]=(0xff&(ipInfo.netmask.addr));
     FW_data.net.V_IP_MASK[1]=(0xff&(ipInfo.netmask.addr>>8));
	 FW_data.net.V_IP_MASK[2]=(0xff&(ipInfo.netmask.addr>>16));
	 FW_data.net.V_IP_MASK[3]=(0xff&(ipInfo.netmask.addr>>24));


	 FW_data.net.V_IP_GET[0]=(0xff&(ipInfo.gw.addr));
	 FW_data.net.V_IP_GET[1]=(0xff&(ipInfo.gw.addr>>8));
	 FW_data.net.V_IP_GET[2]=(0xff&(ipInfo.gw.addr>>16));
	 FW_data.net.V_IP_GET[3]=(0xff&(ipInfo.gw.addr>>24));

	 tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_ETH, 0, &dns_info) ;

	 FW_data.net.V_IP_DNS[0]=(0xff&(dns_info));
     FW_data.net.V_IP_DNS[1]=(0xff&(dns_info>>8));
	 FW_data.net.V_IP_DNS[2]=(0xff&(dns_info>>16));
	 FW_data.net.V_IP_DNS[3]=(0xff&(dns_info>>24));

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
