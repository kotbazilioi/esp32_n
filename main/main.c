/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "includes_base.h"
#include "http_var.h"
#include "update.h"
#define TAG_boot "main start"



#define DEVICE_USE_STATIC_IP	false
#define DEVICE_IP          "172.16.0.210"
#define DEVICE_GW          "172.16.0.254"
#define DEVICE_NETMASK     "255.255.255.0"

#define CONFIG_BLINK_GPIO 4
#define BLINK_GPIO CONFIG_BLINK_GPIO

/*
 * TODO: Setup SNMP server
 * Define a specific address to send SNMP broadcast package.
*/

#define TRAP_DESTINATION_INDEX 0
//----- ^^^

/* transport information to my_mib.c */
extern const struct snmp_mib gpio_mib;

#define SNMP_SERVER_IP "172.16.0.250"

static const char *TAG_SNMP = "Snmp_agent";
/* ----- TODO: Global variables for SNMP Trap vvv
* Define your own vars SNMP_SYSDESCR for System Description, SNMP_SYSCONTACT
* for your contact mail, SNMP_SYSNAME for your system name, SNMP_SYSLOCATION
* for your location. Also consider the size of each string in _LEN functions.
*/
static const struct snmp_mib *my_snmp_mibs[] = { &mib2, &gpio_mib };
//1.3.6.1.2.1.1.1.0
const u8_t * SNMP_SYSDESCR = (u8_t*) "simple_snmp_agent";
const u16_t SNMP_SYSDESCR_LEN = sizeof("simple_snmp_agent");
//1.3.6.1.2.1.1.4.0
u8_t * SNMP_SYSCONTACT = (u8_t*) "yourmail@contact.com";
u16_t SNMP_SYSCONTACT_LEN = sizeof("yourmail@contact.com");
//1.3.6.1.2.1.1.5.0
u8_t * SNMP_SYSNAME = (u8_t*) "ESP32_Core_board_V2";
u16_t SNMP_SYSNAME_LEN = sizeof("ESP32_Core_board_V2");
//1.3.6.1.2.1.1.6.0
u8_t * SNMP_SYSLOCATION = (u8_t*) "Your Institute or Company";
u16_t SNMP_SYSLOCATION_LEN = sizeof("Your Institute or Company");
/*
* ----- TODO: Global variables for SNMP Trap ^^^
*/
char PAGE_BODY[256]={0};
struct tm timeinfo;
u16_t snmp_buffer = 64;
TaskHandle_t xHandleNTP = NULL;



static void initialize_snmp(void)
{

	snmp_mib2_set_syscontact(SNMP_SYSCONTACT, &SNMP_SYSCONTACT_LEN, snmp_buffer);
	snmp_mib2_set_syslocation(SNMP_SYSLOCATION, &SNMP_SYSLOCATION_LEN, snmp_buffer);
	snmp_set_auth_traps_enabled(ENABLE);
	snmp_mib2_set_sysdescr(SNMP_SYSDESCR, &SNMP_SYSDESCR_LEN);
	snmp_mib2_set_sysname(SNMP_SYSNAME, &SNMP_SYSNAME_LEN, snmp_buffer);

	ip_addr_t gw = { 0 };
    ipaddr_aton(SNMP_SERVER_IP,&gw);

	snmp_trap_dst_ip_set(TRAP_DESTINATION_INDEX, &gw);
	snmp_trap_dst_enable(TRAP_DESTINATION_INDEX, ENABLE);
	snmp_set_mibs(my_snmp_mibs, LWIP_ARRAYSIZE(my_snmp_mibs));

    snmp_init();
	ESP_LOGI(TAG_SNMP, "initialize_snmp() finished.");

}

void app_main(void)
{

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    const esp_partition_t *running = esp_ota_get_running_partition();
    ESP_LOGI(TAG_boot, "Current running partition: %s", running->label);
    esp_app_desc_t app_desc;
    esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
    if(ret == ESP_OK)
    {
      ESP_LOGI(TAG_boot, "Proj %s, ver %s, date %s, time %s", app_desc.project_name, app_desc.version, app_desc.date, app_desc.time);
    }

	static httpd_handle_t server = NULL;

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    /* Register event handlers to stop the server when Wi-Fi or Ethernet is disconnected,
     * and re-start it upon connection.
     */
#ifdef CONFIG_EXAMPLE_CONNECT_WIFI
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
#endif // CONFIG_EXAMPLE_CONNECT_WIFI
#ifdef CONFIG_EXAMPLE_CONNECT_ETHERNET
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ETHERNET_EVENT_DISCONNECTED, &disconnect_handler, &server));
#endif // CONFIG_EXAMPLE_CONNECT_ETHERNET

    /* Start the server for the first time */
    server = start_webserver();
    time_t now;
      initialise_mdns();
      initialize_snmp();
      gpio_set_direction(5, GPIO_MODE_OUTPUT);
     // xTaskCreate(&mdns_example_task, "mdns_example_task", 2048, NULL, 5, NULL);
      xTaskCreate(&vTaskNTP, "vTaskNTP", 2048, NULL, 10, &xHandleNTP);
      xTaskCreate(&mdns_example_task, "mdns_example_task", 2048, NULL, 5, NULL);
      xTaskCreate(&nvs_task, "nvs_task", 2048, NULL, 5, NULL);
      gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
           while(1) {
          	 time(&now);
          	 localtime_r(&now, &timeinfo);
          	 sprintf(PAGE_BODY,"\n\r%d : %d : %d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);

               /* Blink off (output low) */
             //  printf("Turning off the LED\n");
               gpio_set_level(BLINK_GPIO, 0);
               vTaskDelay(1000 / portTICK_PERIOD_MS);
               /* Blink on (output high) */
           //    printf("Turning on the LED\n");
               gpio_set_level(BLINK_GPIO, 1);
               vTaskDelay(1000 / portTICK_PERIOD_MS);
           }


}

const __attribute__((used)) __attribute__((section(".rodata_custom_desc"))) char updater_js[]=
		"75hd95kuDbvf8y3k"
		"function fw_is_compatible(oldver){return oldver.startsWith('v32.');}/* **************************** */"
		"48fe99uA6k88eSDa";

