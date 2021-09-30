/*
 * mdns_task.c
 *
 *  Created on: 12 сент. 2021 г.
 *      Author: ivanov
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "netdb.h"
//
//#include "protocol_examples_common.h"
#include "mdns.h"



#include "mdns_task.h"






#define CONFIG_MDNS_INSTANCE "ESP32 with mDNS"







static const char *TAG_mDNS = "mDNS";
static char* generate_hostname(void)
{
#ifndef CONFIG_MDNS_ADD_MAC_TO_HOSTNAME
    return strdup("Netping110");
#else
    uint8_t mac[6];
    char   *hostname;
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    if (-1 == asprintf(&hostname, "%s-%02X%02X%02X", CONFIG_MDNS_HOSTNAME, mac[3], mac[4], mac[5])) {
        abort();
    }
    return hostname;
#endif
}
//static void mdns_print_results(mdns_result_t * results){
//    mdns_result_t * r = results;
//    mdns_ip_addr_t * a = NULL;
//    int i = 1, t;
//    while(r){
//        printf("%d: Interface: %s, Type: %s\n", i++, if_str[r->tcpip_if], ip_protocol_str[r->ip_protocol]);
//        if(r->instance_name){
//            printf("  PTR : %s\n", r->instance_name);
//        }
//        if(r->hostname){
//            printf("  SRV : %s.local:%u\n", r->hostname, r->port);
//        }
//        if(r->txt_count){
//            printf("  TXT : [%u] ", r->txt_count);
//            for(t=0; t<r->txt_count; t++){
//                printf("%s=%s; ", r->txt[t].key, r->txt[t].value?r->txt[t].value:"NULL");
//            }
//            printf("\n");
//        }
//        a = r->addr;
//        while(a){
//            if(a->addr.type == ESP_IPADDR_TYPE_V6){
//                printf("  AAAA: " IPV6STR "\n", IPV62STR(a->addr.u_addr.ip6));
//            } else {
//                printf("  A   : " IPSTR "\n", IP2STR(&(a->addr.u_addr.ip4)));
//            }
//            a = a->next;
//        }
//        r = r->next;
//    }
//
//}
static void query_mdns_service(const char * service_name, const char * proto)
{
    ESP_LOGI(TAG_mDNS, "Query PTR: %s.%s.local", service_name, proto);

    mdns_result_t * results = NULL;
    esp_err_t err = mdns_query_ptr(service_name, proto, 3000, 20,  &results);
    if(err){
        ESP_LOGE(TAG_mDNS, "Query Failed: %s", esp_err_to_name(err));
        return;
    }
    if(!results){
        ESP_LOGW(TAG_mDNS, "No results found!");
        return;
    }

  //  mdns_print_results(results);
    mdns_query_results_free(results);
}

static void query_mdns_host(const char * host_name)
{
    ESP_LOGI(TAG_mDNS, "Query A: %s.local", host_name);

    struct esp_ip4_addr addr;
    addr.addr = 0;

    esp_err_t err = mdns_query_a(host_name, 2000,  &addr);
    if(err){
        if(err == ESP_ERR_NOT_FOUND){
            ESP_LOGW(TAG_mDNS, "%s: Host was not found!", esp_err_to_name(err));
            return;
        }
        ESP_LOGE(TAG_mDNS, "Query Failed: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG_mDNS, "Query A: %s.local resolved to: " IPSTR, host_name, IP2STR(&addr));
}
void initialise_mdns(void)
{
    char* hostname = generate_hostname();
    //initialize mDNS
    ESP_ERROR_CHECK( mdns_init() );
    //set mDNS hostname (required if you want to advertise services)
    ESP_ERROR_CHECK( mdns_hostname_set(hostname) );
    ESP_LOGI(TAG_mDNS, "mdns hostname set to: [%s]", hostname);
    //set default mDNS instance name
    ESP_ERROR_CHECK( mdns_instance_name_set(CONFIG_MDNS_INSTANCE) );

    //structure with TXT records
    mdns_txt_item_t serviceTxtData[3] = {
        {"board","esp32"},
        {"u","user"},
        {"p","password"}
    };

    //initialize service
    ESP_ERROR_CHECK( mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80, serviceTxtData, 3) );
    //add another TXT item
    ESP_ERROR_CHECK( mdns_service_txt_item_set("_http", "_tcp", "path", "/foobar") );
    //change TXT item value
    ESP_ERROR_CHECK( mdns_service_txt_item_set("_http", "_tcp", "u", "admin") );
    free(hostname);
}
static void check_button(void)
{
  //  static bool old_level = true;
//    bool new_level = gpio_get_level(EXAMPLE_BUTTON_GPIO);
//    if (!new_level && old_level) {
        query_mdns_host("esp32");
        query_mdns_service("_arduino", "_tcp");
        query_mdns_service("_http", "_tcp");
        query_mdns_service("_printer", "_tcp");
        query_mdns_service("_ipp", "_tcp");
        query_mdns_service("_afpovertcp", "_tcp");
        query_mdns_service("_smb", "_tcp");
        query_mdns_service("_ftp", "_tcp");
        query_mdns_service("_nfs", "_tcp");
//    }
//    old_level = new_level;
}
void mdns_example_task(void *pvParameters)
{
#if CONFIG_MDNS_RESOLVE_TEST_SERVICES == 1
    /* Send initial queries that are started by CI tester */
    query_mdns_host("tinytester");
    query_mdns_host_with_gethostbyname("tinytester-lwip.local");
    query_mdns_host_with_getaddrinfo("tinytester-lwip.local");
#endif

    while(1) {
     //   check_button();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
