/**
 * @file
 * lwip Private MIB 
 *
 * @todo create MIB file for this example
 * @note the lwip enterprise tree root (26381) is owned by the lwIP project.
 * It is NOT allowed to allocate new objects under this ID (26381) without our,
 * the lwip developers, permission!
 *
 * Please apply for your own ID with IANA: http://www.iana.org/numbers.html
 *  
 * lwip        OBJECT IDENTIFIER ::= { enterprises 26381 }
 * example     OBJECT IDENTIFIER ::= { lwip 1 }
 */

/*
 * Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Christiaan Simons <christiaan.simons@axon.tv>
 */

#include "private_mib.h"

#include "lwipopts.h"
#include "lwip/apps/snmp.h"
#include "..\main\app.h"
//#if LWIP_SNMP

#include <string.h>
#include <stdio.h>

#include "lwip/apps/snmp_table.h"
#include "lwip/apps/snmp_scalar.h"

#include "../main/nvs_task.h"
#include "../main/app_owb.h"
#include "../main/LOGS.h"
//#include "../main/app.h"
#define SENSOR_COUNT 4

#define TERMO_COUNT MAX_DEVICES

#define SENSOR_MAX      10
#define termo_max      10
#define SENSOR_NAME_LEN 20

struct sensor_inf {
	u8_t num;

	char file[SENSOR_NAME_LEN + 1];

#if !SENSORS_USE_FILES
	/** When not using files, contains the value of the sensor */
	s32_t value;
#endif /* !SENSORS_USE_FILES */
};

static struct sensor_inf sensors[SENSOR_MAX];

static s16_t sensor_count_get_value(struct snmp_node_instance *instance,
		void *value);
static snmp_err_t sensor_table_get_cell_instance(const u32_t *column,
		const u32_t *row_oid, u8_t row_oid_len,
		struct snmp_node_instance *cell_instance);
static snmp_err_t sensor_table_get_next_cell_instance(const u32_t *column,
		struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance);
static s16_t sensor_table_get_value(struct snmp_node_instance *instance,
		void *value);
static snmp_err_t sensor_table_set_value(struct snmp_node_instance *instance,
		u16_t len, void *value);

static struct sensor_inf termo[termo_max];

static s16_t termo_count_get_value(struct snmp_node_instance *instance,
		void *value);
static snmp_err_t termo_table_get_cell_instance(const u32_t *column,
		const u32_t *row_oid, u8_t row_oid_len,
		struct snmp_node_instance *cell_instance);
static snmp_err_t termo_table_get_next_cell_instance(const u32_t *column,
		struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance);
static s16_t termo_table_get_value(struct snmp_node_instance *instance,
		void *value);
static snmp_err_t termo_table_set_value(struct snmp_node_instance *instance,
		u16_t len, void *value);

struct snmp_varbind vbt_tr1, vbt_tr2, vbt_tr6, vbt_tr15, vbt_tr18, vbt_tr19;

/* sensorentry .1.3.6.1.4.1.26381.1.1.1 (.level0.level1)
 where level 0 is the table column (temperature/file name)
 and level 1 the table row (sensor index) */
//static const struct snmp_table_col_def sensor_table_columns[] = {
//  { 1, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_WRITE },
//  { 2, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY  }
//};
static const struct snmp_table_col_def sensor_table_columns[] = { { 1,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY }, { 2,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY }, { 3,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_WRITE }, { 6,
		SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY }, { 9,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_WRITE }, { 12,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_WRITE }, { 13,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_WRITE } };

static const struct snmp_table_col_def termo_table_columns[] = { { 1,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY }, { 2,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY }, { 3,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY }, { 4,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY }, { 5,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY }, { 6,
		SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY }, { 7,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY } };

/* sensortable .1.3.6.1.4.1.26381.1.1 */
static const struct snmp_table_node sensor_table = SNMP_TABLE_CREATE(1,
		sensor_table_columns, sensor_table_get_cell_instance,
		sensor_table_get_next_cell_instance, sensor_table_get_value,
		snmp_set_test_ok, sensor_table_set_value);

/* sensorcount .1.3.6.1.4.1.26381.1.2 */
static const struct snmp_scalar_node sensor_count =
		SNMP_SCALAR_CREATE_NODE_READONLY(2, SNMP_ASN1_TYPE_INTEGER,
				sensor_count_get_value);

static const struct snmp_table_node termo_table = SNMP_TABLE_CREATE(1,
		termo_table_columns, termo_table_get_cell_instance,
		termo_table_get_next_cell_instance, termo_table_get_value,
		snmp_set_test_ok, termo_table_set_value);

/* sensorcount .1.3.6.1.4.1.26381.1.2 */
static const struct snmp_scalar_node termo_count =
		SNMP_SCALAR_CREATE_NODE_READONLY(2, SNMP_ASN1_TYPE_INTEGER,
				termo_count_get_value);

/* example .1.3.6.1.4.1.26381.1 */
static const struct snmp_node *const io_nodes[] = { &sensor_table.node.node,
		&sensor_count.node.node };

static const struct snmp_node *const termo_nodes[] = { &termo_table.node.node,
		&termo_count.node.node };

static s16_t get_hres_value(struct snmp_node_instance *instance, void *value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = 1;
	return sizeof(*uint_ptr);
}

static snmp_err_t set_hres_value(struct snmp_node_instance *instance, u16_t len,
		void *value) {
	u32_t val = *((u32_t*) value);
	if (val == 1) {
		esp_restart();
		return SNMP_ERR_NOERROR;
	} else {
		return SNMP_ERR_GENERROR;

	}
}
static s16_t get_tres_value(struct snmp_node_instance *instance, void *value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = 1;
	return sizeof(*uint_ptr);
}

static snmp_err_t set_tres_value(struct snmp_node_instance *instance, u16_t len,
		void *value) {
	u32_t val = *((u32_t*) value);
	if (val == 1) {
		xTaskCreate(&start_task, "start_task", 12048, NULL, 10, NULL);
		return SNMP_ERR_NOERROR;
	} else {
		return SNMP_ERR_GENERROR;

	}

}

static s16_t get_sres_value(struct snmp_node_instance *instance, void *value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = 1;
	return sizeof(*uint_ptr);
}

static snmp_err_t set_sres_value(struct snmp_node_instance *instance, u16_t len,
		void *value) {
	u32_t val = *((u32_t*) value);
	if (val == 1) {
		esp_restart();
		return SNMP_ERR_NOERROR;
	} else {
		return SNMP_ERR_GENERROR;

	}
}

static const struct snmp_scalar_node sres_node = SNMP_SCALAR_CREATE_NODE(1,
		SNMP_NODE_INSTANCE_READ_WRITE, SNMP_ASN1_TYPE_INTEGER, get_sres_value,
		NULL, set_sres_value); //read-and-write example

static const struct snmp_scalar_node tres_node = SNMP_SCALAR_CREATE_NODE(2,
		SNMP_NODE_INSTANCE_READ_WRITE, SNMP_ASN1_TYPE_INTEGER, get_tres_value,
		NULL, set_tres_value);

static const struct snmp_scalar_node hres_node = SNMP_SCALAR_CREATE_NODE(3,
		SNMP_NODE_INSTANCE_READ_WRITE, SNMP_ASN1_TYPE_INTEGER, get_hres_value,
		NULL, set_hres_value);

static const struct snmp_node *const np_nodes[] = { &sres_node.node.node,
		&tres_node.node.node, &hres_node.node.node };

static const struct snmp_tree_node io_node = SNMP_CREATE_TREE_NODE(1, io_nodes);
static const struct snmp_tree_node np_node = SNMP_CREATE_TREE_NODE(1, np_nodes);
static const struct snmp_tree_node termo_node = SNMP_CREATE_TREE_NODE(1,
		termo_nodes);
///{ 1,3,6,1,4,1,26381,1 }
static const u32_t prvmib_base_oid[] = { 1, 3, 6, 1, 4, 1, 25728, 8800 };
static const u32_t prvmib_np_base_oid[] = { 1, 3, 6, 1, 4, 1, 25728, 911 };

static const u32_t termo_base_oid[] = { 1, 3, 6, 1, 4, 1, 25728, 8900 };
const struct snmp_mib mib_private = SNMP_MIB_CREATE(prvmib_base_oid,
		&io_node.node);
const struct snmp_mib mib_np_private = SNMP_MIB_CREATE(prvmib_np_base_oid,
		&np_node.node);
const struct snmp_mib mib_termo_private = SNMP_MIB_CREATE(termo_base_oid,
		&termo_node.node);

/**
 * Initialises this private MIB before use.
 * @see main.c
 */
void lwip_privmib_init(void) {

	u8_t i;

	memset(sensors, 0, sizeof(sensors));

	printf("SNMP private MIB start, detecting sensors.\n");

	for (i = 0; i < SENSOR_COUNT; i++) {
		sensors[i].num = (u8_t)(i + 1);
		snprintf(sensors[i].file, sizeof(sensors[i].file), "%d.txt", i);
		/* initialize sensor value to != zero */
		sensors[i].value = 11 * (i + 1);
		/* !SENSORS_USE_FILES */
	}

	memset(termo, 0, sizeof(termo));

	printf("SNMP private MIB start, detecting sensors.\n");

	for (i = 0; i < TERMO_COUNT; i++) {
		termo[i].num = (u8_t)(i + 1);
		snprintf(termo[i].file, sizeof(termo[i].file), "%d.txt", i);
		/* initialize sensor value to != zero */
		termo[i].value = 11 * (i + 1);
		/* !SENSORS_USE_FILES */
	}

}

/* sensorcount .1.3.6.1.4.1.26381.1.2 */
static s16_t sensor_count_get_value(struct snmp_node_instance *instance,
		void *value) {
	size_t count = 0;
	u32_t *uint_ptr = (u32_t*) value;

	LWIP_UNUSED_ARG(instance);

	for (count = 0; count < LWIP_ARRAYSIZE(sensors); count++) {
		if (sensors[count].num == 0) {
			*uint_ptr = (u32_t) count;
			return sizeof(*uint_ptr);
		}
	}

	return 0;
}
/* sensorcount .1.3.6.1.4.1.26381.1.2 */
static s16_t termo_count_get_value(struct snmp_node_instance *instance,
		void *value) {
	size_t count = 0;
	u32_t *uint_ptr = (u32_t*) value;

	LWIP_UNUSED_ARG(instance);

	for (count = 0; count < LWIP_ARRAYSIZE(termo); count++) {
		if (termo[count].num == 0) {
			*uint_ptr = (u32_t) count;
			return sizeof(*uint_ptr);
		}
	}

	return 0;
}
/* sensortable .1.3.6.1.4.1.26381.1.1 */
/* list of allowed value ranges for incoming OID */
static const struct snmp_oid_range sensor_table_oid_ranges[] = { { 1, SENSOR_MAX
		+ 1 } };

static const struct snmp_oid_range termo_table_oid_ranges[] = { { 1, termo_max
		+ 1 } };

static snmp_err_t sensor_table_get_cell_instance(const u32_t *column,
		const u32_t *row_oid, u8_t row_oid_len,
		struct snmp_node_instance *cell_instance) {
	u32_t sensor_num;
	size_t i;

	LWIP_UNUSED_ARG(column);

	/* check if incoming OID length and if values are in plausible range */
	if (!snmp_oid_in_range(row_oid, row_oid_len, sensor_table_oid_ranges,
			LWIP_ARRAYSIZE(sensor_table_oid_ranges))) {
		return SNMP_ERR_NOSUCHINSTANCE;
	}

	/* get sensor index from incoming OID */
	sensor_num = row_oid[0];

	/* find sensor with index */
	for (i = 0; i < LWIP_ARRAYSIZE(sensors); i++) {
		if (sensors[i].num != 0) {
			if (sensors[i].num == sensor_num) {
				/* store sensor index for subsequent operations (get/test/set) */
				cell_instance->reference.u32 = (u32_t) i;
				return SNMP_ERR_NOERROR;
			}
		}
	}

	/* not found */
	return SNMP_ERR_NOSUCHINSTANCE;
}

static snmp_err_t termo_table_get_cell_instance(const u32_t *column,
		const u32_t *row_oid, u8_t row_oid_len,
		struct snmp_node_instance *cell_instance) {
	u32_t termo_num;
	size_t i;

	LWIP_UNUSED_ARG(column);

	/* check if incoming OID length and if values are in plausible range */
	if (!snmp_oid_in_range(row_oid, row_oid_len, termo_table_oid_ranges,
			LWIP_ARRAYSIZE(termo_table_oid_ranges))) {
		return SNMP_ERR_NOSUCHINSTANCE;
	}

	/* get sensor index from incoming OID */
	termo_num = row_oid[0];

	/* find sensor with index */
	for (i = 0; i < LWIP_ARRAYSIZE(termo); i++) {
		if (termo[i].num != 0) {
			if (termo[i].num == termo_num) {
				/* store sensor index for subsequent operations (get/test/set) */
				cell_instance->reference.u32 = (u32_t) i;
				return SNMP_ERR_NOERROR;
			}
		}
	}

	/* not found */
	return SNMP_ERR_NOSUCHINSTANCE;
}

static snmp_err_t sensor_table_get_next_cell_instance(const u32_t *column,
		struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance) {
	size_t i;
	struct snmp_next_oid_state state;
	u32_t result_temp[LWIP_ARRAYSIZE(sensor_table_oid_ranges)];

	LWIP_UNUSED_ARG(column);

	/* init struct to search next oid */
	snmp_next_oid_init(&state, row_oid->id, row_oid->len, result_temp,
			LWIP_ARRAYSIZE(sensor_table_oid_ranges));

	/* iterate over all possible OIDs to find the next one */
	for (i = 0; i < LWIP_ARRAYSIZE(sensors); i++) {
		if (sensors[i].num != 0) {
			u32_t test_oid[LWIP_ARRAYSIZE(sensor_table_oid_ranges)];

			test_oid[0] = sensors[i].num;

			/* check generated OID: is it a candidate for the next one? */
			snmp_next_oid_check(&state, test_oid,
					LWIP_ARRAYSIZE(sensor_table_oid_ranges), (void*) i);
		}
	}

	/* did we find a next one? */
	if (state.status == SNMP_NEXT_OID_STATUS_SUCCESS) {
		snmp_oid_assign(row_oid, state.next_oid, state.next_oid_len);
		/* store sensor index for subsequent operations (get/test/set) */
		cell_instance->reference.u32 = LWIP_CONST_CAST(u32_t, state.reference);
		return SNMP_ERR_NOERROR;
	}

	/* not found */
	return SNMP_ERR_NOSUCHINSTANCE;
}

static snmp_err_t termo_table_get_next_cell_instance(const u32_t *column,
		struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance) {
	size_t i;
	struct snmp_next_oid_state state;
	u32_t result_temp[LWIP_ARRAYSIZE(termo_table_oid_ranges)];

	LWIP_UNUSED_ARG(column);

	/* init struct to search next oid */
	snmp_next_oid_init(&state, row_oid->id, row_oid->len, result_temp,
			LWIP_ARRAYSIZE(termo_table_oid_ranges));

	/* iterate over all possible OIDs to find the next one */
	for (i = 0; i < LWIP_ARRAYSIZE(termo); i++) {
		if (termo[i].num != 0) {
			u32_t test_oid[LWIP_ARRAYSIZE(termo_table_oid_ranges)];

			test_oid[0] = termo[i].num;

			/* check generated OID: is it a candidate for the next one? */
			snmp_next_oid_check(&state, test_oid,
					LWIP_ARRAYSIZE(termo_table_oid_ranges), (void*) i);
		}
	}

	/* did we find a next one? */
	if (state.status == SNMP_NEXT_OID_STATUS_SUCCESS) {
		snmp_oid_assign(row_oid, state.next_oid, state.next_oid_len);
		/* store sensor index for subsequent operations (get/test/set) */
		cell_instance->reference.u32 = LWIP_CONST_CAST(u32_t, state.reference);
		return SNMP_ERR_NOERROR;
	}

	/* not found */
	return SNMP_ERR_NOSUCHINSTANCE;
}

static s16_t sensor_table_get_value(struct snmp_node_instance *instance,
		void *value) {
	char buf[32];
	u32_t i = instance->reference.u32;
	s32_t *temperature = (s32_t*) value;

	switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
	case 1: /* sensor value */

		if (i < 5) {
			*temperature = i;
			return sizeof(s32_t);
		} else {
			return 0;
		}
	case 2: /* sensor value */
		if (i < 2) {
			*temperature = FW_data.gpio.OUT_PORT[i].realtime;
			return sizeof(s32_t);
		} else if (i < 4) {
			*temperature = FW_data.gpio.IN_PORT[i - 2].sost_filtr;
			return sizeof(s32_t);
		} else {
			return 0;
		}

	case 3: /* sensor value */
		if (i < 2) {
			*temperature = 1;
			return sizeof(s32_t);
		} else if (i < 4) {
			*temperature = 0;
			return sizeof(s32_t);
		} else {
			return 0;
		}

	case 6: /* file name */
		if (i < 2) {
			sprintf(buf, "OUTPUT #%d ", i);
			MEMCPY(value, buf, strlen(buf));
			return (s16_t) strlen(buf);
		} else if (i < 4) {
			sprintf(buf, "INPUT #%d ", i - 2);
			MEMCPY(value, buf, strlen(buf));
			return (s16_t) strlen(buf);
		} else {
			return 0;
		}
	case 9: /* sensor value */
		if (i < 2) {
			*temperature = FW_data.gpio.OUT_PORT[i].count;
			return sizeof(s32_t);
		} else if (i < 4) {
			*temperature = FW_data.gpio.IN_PORT[i - 2].count;
			return sizeof(s32_t);
		} else {
			return 0;
		}
	default:
		return 0;
	}
}
static s16_t termo_table_get_value(struct snmp_node_instance *instance,
		void *value) {
	char buf[32];
	u32_t i = instance->reference.u32;
	s32_t *temperature = (s32_t*) value;

	switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
	case 1: /* »дентификатор термодатчика, число от 1 до 8 включительно */

		if (i < 2) {
			*temperature = i; // (FW_data.termo[1].id[0]<<56)|(FW_data.termo[1].id[1]<<48)|(FW_data.termo[1].id[2]<<40)|(FW_data.termo[1].id[3]<<32)|(FW_data.termo[1].id[4]<<24)|(FW_data.termo[1].id[5]<<16)|(FW_data.termo[1].id[6]<<8)|(FW_data.termo[1].id[7]);
			return sizeof(s32_t);
		} else {
			return 0;
		}
	case 2: /* “екуща€ температура на термодатчике, ∞C, где n Ч номер датчика */
		if (i < 2) {
			*temperature = FW_data.termo[i].temper;
			return sizeof(s32_t);
		} else {
			return 0;
		}

	case 3: /* —татус термодатчика, где n Ч номер датчика: */
		if (i < 2) {
			*temperature = FW_data.termo[i].status;
			return sizeof(s32_t);
		} else {
			return 0;
		}

	case 4: /* Ќижн€€ граница диапазона нормальных значений температуры, ∞C, где n Ч номер датчика */
		if (i < 2) {
			*temperature = FW_data.termo[i].t_dw;
			return sizeof(s32_t);
		} else {
			return 0;
		}
	case 5: /* ¬ерхн€€ граница диапазона нормальных значений температуры, ∞C, где n Ч номер датчика */
		if (i < 2) {
			*temperature = FW_data.termo[i].t_up;
			return sizeof(s32_t);
		} else {
			return 0;
		}
	case 6: /* file name */
		if (i < 2) {

			MEMCPY(value, FW_data.termo[i].name, strlen(FW_data.termo[i].name));
			return (s16_t) strlen(FW_data.termo[i].name);
		} else {
			return 0;
		}
	case 7: /* sensor value */
		if (i < 2) {
			*temperature = 1000 * FW_data.termo[i].ftemper;
			return sizeof(s32_t);
		} else {
			return 0;
		}
	default:
		return 0;
	}
}
static snmp_err_t sensor_table_set_value(struct snmp_node_instance *instance,
		u16_t len, void *value) {
	u32_t i = instance->reference.u32;
	s32_t *temperature = (s32_t*) value;
	s32_t in_data = *((s32_t*) value);
//	sensors[i].value = *temperature;
	switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
//		case 1: /* sensor value */
//
//			//*temperature = i;
//
//			return sizeof(s32_t);
//		case 2: /* sensor value */
//
//			//*temperature = FW_data.gpio.IN_PORT[i].sost_filtr;
//
//			return sizeof(s32_t);
	case 3: /* sensor value */

		if (((in_data) > (-2)) && ((in_data) < (2)) && (i < 2)) {
			if (in_data < 0) {
				if (FW_data.gpio.OUT_PORT[i].sost == 0) {
					FW_data.gpio.OUT_PORT[i].sost = 1;
					FW_data.gpio.OUT_PORT[i].event = OUT_SET;
					reple_to_save.type_event = OUT_SET;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				} else {
					FW_data.gpio.OUT_PORT[i].sost = 0;
					FW_data.gpio.OUT_PORT[i].event = OUT_RES;
					reple_to_save.type_event = OUT_RES;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				}
//					FW_data.gpio.OUT_PORT[i].type_logic=3;
//					FW_data.gpio.OUT_PORT[i].sost=1;

			} else {
				FW_data.gpio.OUT_PORT[i].sost = in_data;
				if (in_data == 0) {
					FW_data.gpio.OUT_PORT[i].event = OUT_RES;
					reple_to_save.type_event = OUT_RES;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				} else {
					FW_data.gpio.OUT_PORT[i].event = OUT_SET;
					reple_to_save.type_event = OUT_SET;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				}
			}
			FW_data.gpio.OUT_PORT[i].aflag = 1;

			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}
	case 9: /* sensor value */
		if (i < 4) {
			if (in_data == 0) {
				FW_data.gpio.OUT_PORT[i].count = 0;
			} else {
				return SNMP_ERR_GENERROR;
			}

			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}
	case 12: /* sensor value */

		if (i < 2) {
			FW_data.gpio.OUT_PORT[i].delay = 100 * in_data;
			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}
	case 13: /* sensor value */

		if ((i < 2) && (in_data == 1)) {
			FW_data.gpio.OUT_PORT[i].type_logic = 3;
			FW_data.gpio.OUT_PORT[i].sost = 1;
			FW_data.gpio.OUT_PORT[i].event = OUT_TOL;
			reple_to_save.type_event = OUT_TOL;
			reple_to_save.event_cfg.canal = i;
			reple_to_save.event_cfg.source = SNMP;
			reple_to_save.dicr = 1;
			FW_data.gpio.OUT_PORT[i].aflag = 1;
			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}

	default:
		return SNMP_ERR_GENERROR;
	}
//	  OUT_PORT0_SET,
//	  OUT_PORT0_RES,
//	  OUT_PORT1_SET,
//	  OUT_PORT1_RES,
//	  OUT_PORT0_TOL,
//	  OUT_PORT0_TOL
}
static snmp_err_t termo_table_set_value(struct snmp_node_instance *instance,
		u16_t len, void *value) {
	u32_t i = instance->reference.u32;
	s32_t *temperature = (s32_t*) value;
	s32_t in_data = *((s32_t*) value);
//	sensors[i].value = *temperature;
	switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
//		case 1: /* sensor value */
//
//			//*temperature = i;
//
//			return sizeof(s32_t);
//		case 2: /* sensor value */
//
//			//*temperature = FW_data.gpio.IN_PORT[i].sost_filtr;
//
//			return sizeof(s32_t);
	case 3: /* sensor value */

		if (((in_data) > (-2)) && ((in_data) < (2)) && (i < 2)) {
			if (in_data < 0) {
				if (FW_data.gpio.OUT_PORT[i].sost == 0) {
					FW_data.gpio.OUT_PORT[i].sost = 1;
					FW_data.gpio.OUT_PORT[i].event = OUT_SET;
					reple_to_save.type_event = OUT_SET;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				} else {
					FW_data.gpio.OUT_PORT[i].sost = 0;
					FW_data.gpio.OUT_PORT[i].event = OUT_RES;
					reple_to_save.type_event = OUT_RES;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				}
//					FW_data.gpio.OUT_PORT[i].type_logic=3;
//					FW_data.gpio.OUT_PORT[i].sost=1;

			} else {
				FW_data.gpio.OUT_PORT[i].sost = in_data;
				if (in_data == 0) {
					FW_data.gpio.OUT_PORT[i].event = OUT_RES;
					reple_to_save.type_event = OUT_RES;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				} else {
					FW_data.gpio.OUT_PORT[i].event = OUT_SET;
					reple_to_save.type_event = OUT_SET;
					reple_to_save.event_cfg.canal = i;
					reple_to_save.event_cfg.source = SNMP;
					reple_to_save.dicr = 1;
				}
			}
			FW_data.gpio.OUT_PORT[i].aflag = 1;

			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}
	case 9: /* sensor value */
		if (i < 4) {
			if (in_data == 0) {
				FW_data.gpio.OUT_PORT[i].count = 0;
			} else {
				return SNMP_ERR_GENERROR;
			}

			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}
	case 12: /* sensor value */

		if (i < 2) {
			FW_data.gpio.OUT_PORT[i].delay = 100 * in_data;
			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}
	case 13: /* sensor value */

		if ((i < 2) && (in_data == 1)) {
			FW_data.gpio.OUT_PORT[i].type_logic = 3;
			FW_data.gpio.OUT_PORT[i].sost = 1;
			FW_data.gpio.OUT_PORT[i].event = OUT_TOL;
			reple_to_save.type_event = OUT_TOL;
			reple_to_save.event_cfg.canal = i;
			reple_to_save.event_cfg.source = SNMP;
			reple_to_save.dicr = 1;
			FW_data.gpio.OUT_PORT[i].aflag = 1;
			return SNMP_ERR_NOERROR;
		} else {
			return SNMP_ERR_GENERROR;
		}

	default:
		return SNMP_ERR_GENERROR;
	}
//	  OUT_PORT0_SET,
//	  OUT_PORT0_RES,
//	  OUT_PORT1_SET,
//	  OUT_PORT1_RES,
//	  OUT_PORT0_TOL,
//	  OUT_PORT0_TOL
}
void send_mess_trap(s32_t *OID_TR, char *mess, uint16_t lens_mess,
		uint8_t canal) {
	struct snmp_obj_id OID;

	OID.id[0] = OID_TR[0];
	OID.id[1] = OID_TR[1];
	OID.id[2] = OID_TR[2];
	OID.id[3] = OID_TR[3];
	OID.id[4] = OID_TR[4];
	OID.id[5] = OID_TR[5];
	OID.id[6] = OID_TR[6];
	OID.id[7] = OID_TR[7];
	OID.id[8] = OID_TR[8];
	OID.id[9] = OID_TR[9];
	OID.id[10] = OID_TR[10];
	OID.id[11] = OID_TR[11];
	OID.len = 11;

	//struct snmp_varbind vbt_tr1,vbt_tr2,vbt_tr6,vbt_tr15,vbt_tr18,vbt_tr19;

//  vbt_tr2.value=mess;
//  vbt_tr2.value_len=lens_mess;
//  vbt_tr2.type=SNMP_ASN1_TYPE_OCTET_STRING;
//  vbt_tr2.oid.len=11;
//  vbt_tr2.oid.id[0]=1;
//  vbt_tr2.oid.id[1]=3;
//  vbt_tr2.oid.id[2]=6;
//  vbt_tr2.oid.id[3]=1;
//  vbt_tr2.oid.id[4]=4;
//  vbt_tr2.oid.id[5]=1;
//  vbt_tr2.oid.id[6]=25728;
//  vbt_tr2.oid.id[7]=5500;
//  vbt_tr2.oid.id[8]=3;
//  vbt_tr2.oid.id[9]=2;
//  vbt_tr2.oid.id[10]=1;
//  vbt_tr2.next=&vbt_tr6;
//  vbt_tr2.prev=&vbt_tr1;

	uint32_t data = canal;
	vbt_tr1.value = &data;
	vbt_tr1.value_len = sizeof(data);
	vbt_tr1.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr1.oid.len = 11;
	vbt_tr1.oid.id[0] = 1;
	vbt_tr1.oid.id[1] = 3;
	vbt_tr1.oid.id[2] = 6;
	vbt_tr1.oid.id[3] = 1;
	vbt_tr1.oid.id[4] = 4;
	vbt_tr1.oid.id[5] = 1;
	vbt_tr1.oid.id[6] = 25728;
	vbt_tr1.oid.id[7] = 5500;
	vbt_tr1.oid.id[8] = 3;
	vbt_tr1.oid.id[9] = 1;
	vbt_tr1.oid.id[10] = 0;
	vbt_tr1.next = &vbt_tr2;

	vbt_tr2.value = &(FW_data.gpio.OUT_PORT[canal]);
	vbt_tr2.value_len = 4;
	vbt_tr2.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr2.oid.len = 11;
	vbt_tr2.oid.id[0] = 1;
	vbt_tr2.oid.id[1] = 3;
	vbt_tr2.oid.id[2] = 6;
	vbt_tr2.oid.id[3] = 1;
	vbt_tr2.oid.id[4] = 4;
	vbt_tr2.oid.id[5] = 1;
	vbt_tr2.oid.id[6] = 25728;
	vbt_tr2.oid.id[7] = 5500;
	vbt_tr2.oid.id[8] = 3;
	vbt_tr2.oid.id[9] = 2;
	vbt_tr2.oid.id[10] = 0;
	vbt_tr2.next = &vbt_tr6;
	vbt_tr2.prev = &vbt_tr1;

	vbt_tr6.value = FW_data.gpio.name[canal];
	vbt_tr6.value_len = strlen(FW_data.gpio.name[canal]);
	vbt_tr6.type = SNMP_ASN1_TYPE_OCTET_STRING;
	vbt_tr6.oid.len = 11;
	vbt_tr6.oid.id[0] = 1;
	vbt_tr6.oid.id[1] = 3;
	vbt_tr6.oid.id[2] = 6;
	vbt_tr6.oid.id[3] = 1;
	vbt_tr6.oid.id[4] = 4;
	vbt_tr6.oid.id[5] = 1;
	vbt_tr6.oid.id[6] = 25728;
	vbt_tr6.oid.id[7] = 5500;
	vbt_tr6.oid.id[8] = 3;
	vbt_tr6.oid.id[9] = 6;
	vbt_tr6.oid.id[10] = 0;
	vbt_tr6.next = &vbt_tr15;
	vbt_tr6.prev = &vbt_tr2;

	int data3;
	data3 = FW_data.gpio.OUT_PORT[canal].realtime;
	vbt_tr15.value = &data3;
	vbt_tr15.value_len = 4;
	vbt_tr15.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr15.oid.len = 11;
	vbt_tr15.oid.id[0] = 1;
	vbt_tr15.oid.id[1] = 3;
	vbt_tr15.oid.id[2] = 6;
	vbt_tr15.oid.id[3] = 1;
	vbt_tr15.oid.id[4] = 4;
	vbt_tr15.oid.id[5] = 1;
	vbt_tr15.oid.id[6] = 25728;
	vbt_tr15.oid.id[7] = 5500;
	vbt_tr15.oid.id[8] = 3;
	vbt_tr15.oid.id[9] = 15;
	vbt_tr15.oid.id[10] = 0;
	vbt_tr15.next = &vbt_tr18;
	vbt_tr15.prev = &vbt_tr6;

	int data2;
	data2 = (-1);
	vbt_tr18.value = &data2;
	vbt_tr18.value_len = 4;
	vbt_tr18.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr18.oid.len = 11;
	vbt_tr18.oid.id[0] = 1;
	vbt_tr18.oid.id[1] = 3;
	vbt_tr18.oid.id[2] = 6;
	vbt_tr18.oid.id[3] = 1;
	vbt_tr18.oid.id[4] = 4;
	vbt_tr18.oid.id[5] = 1;
	vbt_tr18.oid.id[6] = 25728;
	vbt_tr18.oid.id[7] = 5500;
	vbt_tr18.oid.id[8] = 3;
	vbt_tr18.oid.id[9] = 18;
	vbt_tr18.oid.id[10] = 0;
	vbt_tr18.next = &vbt_tr19;
	vbt_tr18.prev = &vbt_tr15;

	char buf_list[64];
	int year, mons;
	year = timeinfo.tm_year + 1900;
	mons = (timeinfo.tm_mon + 1);
	sprintf(buf_list, "%dг. %dм. %dд. %dч. %dм. %dс.", year, mons,
			timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min,
			timeinfo.tm_sec);
	vbt_tr19.value = buf_list;
	vbt_tr19.value_len = strlen(buf_list);
	vbt_tr19.type = SNMP_ASN1_TYPE_OCTET_STRING;
	vbt_tr19.oid.len = 11;
	vbt_tr19.oid.id[0] = 1;
	vbt_tr19.oid.id[1] = 3;
	vbt_tr19.oid.id[2] = 6;
	vbt_tr19.oid.id[3] = 1;
	vbt_tr19.oid.id[4] = 4;
	vbt_tr19.oid.id[5] = 1;
	vbt_tr19.oid.id[6] = 25728;
	vbt_tr19.oid.id[7] = 5500;
	vbt_tr19.oid.id[8] = 3;
	vbt_tr19.oid.id[9] = 19;
	vbt_tr19.oid.id[10] = 0;
	vbt_tr19.prev = &vbt_tr18;

	snmp_send_trap((const struct snmp_obj_id*) &OID,
			SNMP_GENTRAP_ENTERPRISE_SPECIFIC, 1, &vbt_tr1);

//  snmp_send_trap_specific(SNMP_GENTRAP_ENTERPRISE_SPECIFIC,&vbt_tr);
//  snmp_send_trap_specific(SNMP_GENTRAP_ENTERPRISE_SPECIFIC,&vbt_tr);

}

void send_mess_trap_termo(s32_t *OID_TR, uint8_t canal) {
	struct snmp_obj_id OID;

	OID.id[0] = OID_TR[0];
	OID.id[1] = OID_TR[1];
	OID.id[2] = OID_TR[2];
	OID.id[3] = OID_TR[3];
	OID.id[4] = OID_TR[4];
	OID.id[5] = OID_TR[5];
	OID.id[6] = OID_TR[6];
	OID.id[7] = OID_TR[7];
	OID.id[8] = OID_TR[8];
	OID.id[9] = OID_TR[9];
	OID.id[10] = OID_TR[10];
	OID.id[11] = OID_TR[11];
	OID.len = 11;

	//struct snmp_varbind vbt_tr1,vbt_tr2,vbt_tr6,vbt_tr15,vbt_tr18,vbt_tr19;

//  vbt_tr2.value=mess;
//  vbt_tr2.value_len=lens_mess;
//  vbt_tr2.type=SNMP_ASN1_TYPE_OCTET_STRING;
//  vbt_tr2.oid.len=11;
//  vbt_tr2.oid.id[0]=1;
//  vbt_tr2.oid.id[1]=3;
//  vbt_tr2.oid.id[2]=6;
//  vbt_tr2.oid.id[3]=1;
//  vbt_tr2.oid.id[4]=4;
//  vbt_tr2.oid.id[5]=1;
//  vbt_tr2.oid.id[6]=25728;
//  vbt_tr2.oid.id[7]=5500;
//  vbt_tr2.oid.id[8]=3;
//  vbt_tr2.oid.id[9]=2;
//  vbt_tr2.oid.id[10]=1;
//  vbt_tr2.next=&vbt_tr6;
//  vbt_tr2.prev=&vbt_tr1;

	uint32_t data = canal;
	vbt_tr1.value = &data;
	vbt_tr1.value_len = sizeof(data);
	vbt_tr1.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr1.oid.len = 11;
	vbt_tr1.oid.id[0] = 1;
	vbt_tr1.oid.id[1] = 3;
	vbt_tr1.oid.id[2] = 6;
	vbt_tr1.oid.id[3] = 1;
	vbt_tr1.oid.id[4] = 4;
	vbt_tr1.oid.id[5] = 1;
	vbt_tr1.oid.id[6] = 25728;
	vbt_tr1.oid.id[7] = 8800;
	vbt_tr1.oid.id[8] = 2;
	vbt_tr1.oid.id[9] = 1;
	vbt_tr1.oid.id[10] = 0;
	vbt_tr1.next = &vbt_tr2;

	vbt_tr2.value = &(FW_data.termo[canal].temper);
	vbt_tr2.value_len = 4;
	vbt_tr2.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr2.oid.len = 11;
	vbt_tr2.oid.id[0] = 1;
	vbt_tr2.oid.id[1] = 3;
	vbt_tr2.oid.id[2] = 6;
	vbt_tr2.oid.id[3] = 1;
	vbt_tr2.oid.id[4] = 4;
	vbt_tr2.oid.id[5] = 1;
	vbt_tr2.oid.id[6] = 25728;
	vbt_tr2.oid.id[7] = 8800;
	vbt_tr2.oid.id[8] = 2;
	vbt_tr2.oid.id[9] = 2;
	vbt_tr2.oid.id[10] = 0;
	vbt_tr2.next = &vbt_tr6;
	vbt_tr2.prev = &vbt_tr1;

	vbt_tr6.value = &(FW_data.termo[canal].status);
	vbt_tr6.value_len = 4;
	vbt_tr6.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr6.oid.len = 11;
	vbt_tr6.oid.id[0] = 1;
	vbt_tr6.oid.id[1] = 3;
	vbt_tr6.oid.id[2] = 6;
	vbt_tr6.oid.id[3] = 1;
	vbt_tr6.oid.id[4] = 4;
	vbt_tr6.oid.id[5] = 1;
	vbt_tr6.oid.id[6] = 25728;
	vbt_tr6.oid.id[7] = 8800;
	vbt_tr6.oid.id[8] = 2;
	vbt_tr6.oid.id[9] = 3;
	vbt_tr6.oid.id[10] = 0;
	vbt_tr6.next = &vbt_tr15;
	vbt_tr6.prev = &vbt_tr2;

	uint32_t data4;
	data4 = FW_data.termo[canal].t_dw;
	vbt_tr15.value = &(data4);
	vbt_tr15.value_len = 4;
	vbt_tr15.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr15.oid.len = 11;
	vbt_tr15.oid.id[0] = 1;
	vbt_tr15.oid.id[1] = 3;
	vbt_tr15.oid.id[2] = 6;
	vbt_tr15.oid.id[3] = 1;
	vbt_tr15.oid.id[4] = 4;
	vbt_tr15.oid.id[5] = 1;
	vbt_tr15.oid.id[6] = 25728;
	vbt_tr15.oid.id[7] = 5500;
	vbt_tr15.oid.id[8] = 3;
	vbt_tr15.oid.id[9] = 15;
	vbt_tr15.oid.id[10] = 1;
	vbt_tr15.next = &vbt_tr18;
	vbt_tr15.prev = &vbt_tr6;

	uint32_t data5;
	data5 = FW_data.termo[canal].t_up;

	vbt_tr18.value = &(data5);
	vbt_tr18.value_len = 4;
	vbt_tr18.type = SNMP_ASN1_TYPE_INTEGER;
	vbt_tr18.oid.len = 11;
	vbt_tr18.oid.id[0] = 1;
	vbt_tr18.oid.id[1] = 3;
	vbt_tr18.oid.id[2] = 6;
	vbt_tr18.oid.id[3] = 1;
	vbt_tr18.oid.id[4] = 4;
	vbt_tr18.oid.id[5] = 1;
	vbt_tr18.oid.id[6] = 25728;
	vbt_tr18.oid.id[7] = 5500;
	vbt_tr18.oid.id[8] = 3;
	vbt_tr18.oid.id[9] = 18;
	vbt_tr18.oid.id[10] = 1;
	vbt_tr18.next = &vbt_tr19;
	vbt_tr18.prev = &vbt_tr15;

//  char buf_list[32];
//  sprintf(buf_list,"%dг. %dм. %dд. %dч. %dм. %dс.",time_run[0],time_run[1],time_run[2],time_run[3],time_run[4],time_run[5]);
	vbt_tr19.value = FW_data.termo[canal].name;
	vbt_tr19.value_len = strlen(FW_data.termo[canal].name);
	vbt_tr19.type = SNMP_ASN1_TYPE_OCTET_STRING;
	vbt_tr19.oid.len = 11;
	vbt_tr19.oid.id[0] = 1;
	vbt_tr19.oid.id[1] = 3;
	vbt_tr19.oid.id[2] = 6;
	vbt_tr19.oid.id[3] = 1;
	vbt_tr19.oid.id[4] = 4;
	vbt_tr19.oid.id[5] = 1;
	vbt_tr19.oid.id[6] = 25728;
	vbt_tr19.oid.id[7] = 5500;
	vbt_tr19.oid.id[8] = 3;
	vbt_tr19.oid.id[9] = 19;
	vbt_tr19.oid.id[10] = 1;
	vbt_tr19.prev = &vbt_tr15;

	snmp_send_trap((const struct snmp_obj_id*) &OID,
			SNMP_GENTRAP_ENTERPRISE_SPECIFIC, 1, &vbt_tr1);

//  snmp_send_trap_specific(SNMP_GENTRAP_ENTERPRISE_SPECIFIC,&vbt_tr);
//  snmp_send_trap_specific(SNMP_GENTRAP_ENTERPRISE_SPECIFIC,&vbt_tr);

}
