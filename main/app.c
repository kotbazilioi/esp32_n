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
input_port_t IN_PORT[2]={0};
#define PORT_O1 16
#define PORT_O2 6


#define PORT_I0 34
#define PORT_I1 32

void gpio1_task(void *pvParameters)
{
	gpio_set_direction(PORT_O2, GPIO_MODE_OUTPUT);
	gpio_set_direction(PORT_I0, GPIO_MODE_INPUT);
	while(1)
		{
	//	IN_PORT[0]=gpio_get_level(PORT_I0);
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
	gpio_set_direction(PORT_I1, GPIO_MODE_INPUT);
	while(1)
		{
	//	IN_PORT[1]=gpio_get_level(PORT_I1);
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
void read_in (input_port_t* inpin,uint8_t pin)
{
	inpin->sost_raw=pin;
	if (inpin->semple_count<inpin->filtr_time)
		{
		inpin->semple_count++;
		if (inpin->sost_raw!=0)
			{
			inpin->filtr_count++;
			}
		}
	else
	{
		inpin->semple_count=0;
		inpin->sost_filtr_old=inpin->sost_filtr;
		if (inpin->filtr_count<(inpin->filtr_time/2))
			{
			inpin->sost_filtr=0;
			}
		else
			{
			inpin->sost_filtr=1;
			}
		inpin->filtr_count=0;
	while(inpin->event!=0)
		{
		 vTaskDelay(30 / portTICK_PERIOD_MS);
		}
		if (inpin->sost_filtr_old>inpin->sost_filtr)
			{
			inpin->sost_fall=1;
			inpin->sost_rise=0;
			inpin->event=1;
			}
		if (inpin->sost_filtr_old<inpin->sost_filtr)
			{
			inpin->sost_fall=0;
			inpin->sost_rise=1;
			inpin->event=1;
			}
		if (inpin->event==0)
			{
			inpin->sost_fall=0;
			inpin->sost_rise=0;
			}
	}
}
void input_port(void *pvParameters)
{
	IN_PORT[0].filtr_time=20;
	IN_PORT[1].filtr_time=20;


	while(1)
		{
		read_in(&(IN_PORT[0]),gpio_get_level(PORT_I0));
		read_in(&(IN_PORT[1]),gpio_get_level(PORT_I1));

		 vTaskDelay(30 / portTICK_PERIOD_MS);
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

	 xTaskCreate(&gpio1_task, "gpio1_task", 1024, NULL, 10, NULL);
	 xTaskCreate(&gpio2_task, "gpio2_task", 1024, NULL, 10, NULL);
	 xTaskCreate(&input_port, "input_port", 1024, NULL, 10, NULL);
	 xTaskCreate(&log_task, "log_task", 2024, NULL, 10, NULL);
	 xTaskCreate(&send_smtp_task, "send_smtp_task", 4096, NULL, 10, NULL);



 vTaskDelay(1000 / portTICK_PERIOD_MS);
       reple_to_save.type_event=POWER_ON;
       reple_to_save.dicr=1;

	  printf("all app run\n\r");


	 vTaskDelete(NULL);
 }
