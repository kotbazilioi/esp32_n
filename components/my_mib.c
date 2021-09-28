//--- global library
#include <string.h>

//--- RTOS library and Espressif ESP32 library
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

//--- GPIO library
#include "driver/gpio.h"

//--- LwIP library
//#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

//--- SMNP library (adapted)
//#include "port/esp32/lwipopts.h"
//#include "lwip/my_opt.h"
#include "lwip/apps/snmp_opts.h"
#include "lwip/snmp.h"
#include "lwip/apps/snmp.h"
#include "lwip/apps/snmp_core.h"
#include "lwip/apps/snmp_mib2.h"
#include "lwip/apps/snmp_scalar.h"

#include "my_mib.h"
#include "../main/app.h"
u32_t *leds, *switches, *xgpio;
/*
 * SNMP general function prototype
 * Add here any function to be usable in specific private MIB node.
 */
static s16_t get_in0_value(struct snmp_node_instance* instance, void* value);
static s16_t get_in1_value(struct snmp_node_instance* instance, void* value);
static s16_t get_single_value(struct snmp_node_instance* instance, void* value);
static snmp_err_t set_leds_value(struct snmp_node_instance* instance, u16_t len, void *value);
static s16_t snmpengine_scalars_get_value(const struct snmp_scalar_array_node_def *node, void *value) ;

static const struct snmp_scalar_array_node_def snmpengine_scalars_nodes[] = {
  {1, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY}, /* snmpEngineID */
  {2, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY}, /* snmpEngineBoots */
  {3, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY}, /* snmpEngineTime */
  {4, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY}, /* snmpEngineMaxMessageSize */
};
static const struct snmp_scalar_array_node snmpengine_scalars = SNMP_SCALAR_CREATE_ARRAY_NODE(4, snmpengine_scalars_nodes, snmpengine_scalars_get_value, NULL, NULL);

/* led .1.3.6.1.4.1.25728.1.1.1 */
//static const struct snmp_scalar_node leds_node = SNMP25_SCALAR_CREATE_NODE_READONLY(1, SNMP_ASN1_TYPE_INTEGER, get_leds_value); // ready-only example
static const struct snmp_scalar_node leds_node = SNMP_SCALAR_CREATE_NODE(1, SNMP_NODE_INSTANCE_READ_WRITE, SNMP_ASN1_TYPE_INTEGER, get_in0_value, NULL, set_leds_value); //read-and-write example
/* switch .1.3.6.1.4.1.25728.1.1.2 */
static const struct snmp_scalar_node switches_node = SNMP_SCALAR_CREATE_NODE_READONLY(2, SNMP_ASN1_TYPE_INTEGER, get_in1_value);
/* xgpio .1.3.6.1.4.1.25728.1.1.3 */
static const struct snmp_scalar_node single_node = SNMP_SCALAR_CREATE_NODE_READONLY(3, SNMP_ASN1_TYPE_INTEGER,get_single_value);

/* all private nodes  .1.3.6.1.4.1.25728.1.1 .. (.1,.2,.3) .0 */
static const struct snmp_node* const gpio_nodes[] = { &snmpengine_scalars.node.node,&switches_node.node.node, &leds_node.node.node, &single_node.node.node};
static const struct snmp_tree_node gpio_tree_node = SNMP_CREATE_TREE_NODE(1, gpio_nodes);

/* define private mib */
static const u32_t my_base_oid[] = { 1, 3, 6, 1, 4, 1, 25728, 8900,1,1 };
const struct snmp_mib gpio_mib = SNMP_MIB_CREATE(my_base_oid, &gpio_tree_node.node);


static s16_t snmpengine_scalars_get_value(const struct snmp_scalar_array_node_def *node, void *value)
{
  const char *engineid;
  u8_t engineid_len;

  switch (node->oid) {
    case 1: /* snmpEngineID */
    	 *(s32_t *)value =111;// snmpv3_get_engine_boots_internal();
//      snmpv3_get_engine_id(&engineid, &engineid_len);
//      MEMCPY(value, engineid, engineid_len);
//      return value;
    	  return sizeof(s32_t);
    case 2: /* snmpEngineBoots */
      *(s32_t *)value = 112;//snmpv3_get_engine_boots_internal();
      return sizeof(s32_t);
    case 3: /* snmpEngineTime */
      *(s32_t *)value = 113;//snmpv3_get_engine_time_internal();
      return sizeof(s32_t);
    case 4: /* snmpEngineMaxMessageSize */
      *(s32_t *)value = 114;//SNMP_FRAMEWORKMIB_SNMPENGINEMAXMESSAGESIZE;
      return sizeof(s32_t);
    default:
  //    LWIP_DEBUGF(SNMP_MIB_DEBUG, ("snmpengine_scalars_get_value(): unknown id: %"S32_F"\n", node->oid));
      return 0;
  }
}


/* leds value .1.3.6.1.4.1.25728.1.1.0 */
static s16_t get_in0_value(struct snmp_node_instance* instance, void* value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = IN_PORT[0].sost_filtr;//(u32_t) gpio_get_level((gpio_num_t) *(&leds)); /* get structure values */
	return sizeof(*uint_ptr);
}

/* switches value .1.3.6.1.4.1.25728.1.2.0 */
static s16_t get_in1_value(struct snmp_node_instance* instance, void* value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = IN_PORT[1].sost_filtr;//(u32_t) gpio_get_level((gpio_num_t) *(&switches)); /* get structure values */
	return sizeof(*uint_ptr);
}

/* xgpio value .1.3.6.1.4.1.25728.1.3.0 */
static s16_t get_single_value(struct snmp_node_instance* instance, void* value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = 152;
	return sizeof(*uint_ptr);
}

static snmp_err_t set_leds_value(struct snmp_node_instance* instance, u16_t len, void *value) {
	u32_t val = *((u32_t*)value);
	gpio_set_level((gpio_num_t) *(&leds), (u32_t) val); /* write a value in struture */
	return SNMP_ERR_NOERROR;
}
