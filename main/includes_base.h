/*
 * includes_base.h
 *
 *  Created on: 18 сент. 2021 г.
 *      Author: ivanov
 */

#ifndef MAIN_INCLUDES_BASE_H_
#define MAIN_INCLUDES_BASE_H_


#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "esp_eth.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"


//////////////////////////////// LBS ////////////////////////

#include <esp_ota_ops.h>

//NVS-----------------
#include "nvs_task.h"
//---------------------

//--- SMNP library (adapted)
//#include "port/esp32/lwipopts.h"

#include "lwip/apps/snmp_opts.h"
#include "lwip/snmp.h"
#include "lwip/apps/snmp.h"
#include "lwip/apps/snmp_core.h"
#include "lwip/apps/snmp_mib2.h"

#include "lwip/apps/snmp_scalar.h"
/* library for the app */
//#include "simple_snmp_agent.h"
/* library for static IP */
#include "lwip/sockets.h"
#include "my_mib.h"


//#include <esp_http_server.h>
//#include "data.h"





#include "mdns_task.h"
#include "sntp_task.h"

#define ENABLE 1
#define DISABLE 0



#endif /* MAIN_INCLUDES_BASE_H_ */
