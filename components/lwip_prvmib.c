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

#if LWIP_SNMP

#include <string.h>
#include <stdio.h>

#include "lwip/apps/snmp_table.h"
#include "lwip/apps/snmp_scalar.h"
#include "../main/app.h"
#define SENSOR_COUNT 2

#define SENSOR_MAX      10
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
		SNMP_ASN1_TYPE_COUNTER32, SNMP_NODE_INSTANCE_READ_WRITE }, { 12,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_WRITE }, { 13,
		SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_WRITE } };

/* sensortable .1.3.6.1.4.1.26381.1.1 */
static const struct snmp_table_node sensor_table = SNMP_TABLE_CREATE(
		1, sensor_table_columns,
		sensor_table_get_cell_instance, sensor_table_get_next_cell_instance,
		sensor_table_get_value, snmp_set_test_ok, sensor_table_set_value);

/* sensorcount .1.3.6.1.4.1.26381.1.2 */
static const struct snmp_scalar_node sensor_count =
		SNMP_SCALAR_CREATE_NODE_READONLY(2, SNMP_ASN1_TYPE_INTEGER,
				sensor_count_get_value);

/* example .1.3.6.1.4.1.26381.1 */
static const struct snmp_node *const io_nodes[] = { &sensor_table.node.node,
		&sensor_count.node.node };

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

static const struct snmp_scalar_node sres_node =
		SNMP_SCALAR_CREATE_NODE(1, SNMP_NODE_INSTANCE_READ_WRITE,
				SNMP_ASN1_TYPE_INTEGER, get_sres_value, NULL, set_sres_value); //read-and-write example

static const struct snmp_scalar_node tres_node =
		SNMP_SCALAR_CREATE_NODE(2, SNMP_NODE_INSTANCE_READ_WRITE,
				SNMP_ASN1_TYPE_INTEGER, get_tres_value, NULL, set_tres_value);

static const struct snmp_scalar_node hres_node =
		SNMP_SCALAR_CREATE_NODE(3, SNMP_NODE_INSTANCE_READ_WRITE,
				SNMP_ASN1_TYPE_INTEGER, get_hres_value, NULL, set_hres_value);

static const struct snmp_node *const np_nodes[] = { &sres_node.node.node,
		&tres_node.node.node, &hres_node.node.node };

static const struct snmp_tree_node io_node = SNMP_CREATE_TREE_NODE(1, io_nodes);
static const struct snmp_tree_node np_node = SNMP_CREATE_TREE_NODE(1, np_nodes);
///{ 1,3,6,1,4,1,26381,1 }
static const u32_t prvmib_base_oid[] = { 1, 3, 6, 1, 4, 1, 25728, 8900 };
static const u32_t prvmib_np_base_oid[] = { 1, 3, 6, 1, 4, 1, 25728, 911 };
const struct snmp_mib mib_private =
		SNMP_MIB_CREATE(prvmib_base_oid, &io_node.node);
const struct snmp_mib mib_np_private =
		SNMP_MIB_CREATE(prvmib_np_base_oid, &np_node.node);

/**
 * Initialises this private MIB before use.
 * @see main.c
 */
void lwip_privmib_init(void) {

	u8_t i;

	memset(sensors, 0, sizeof(sensors));

	printf("SNMP private MIB start, detecting sensors.\n");

	for (i = 0; i < SENSOR_COUNT; i++) {
		sensors[i].num = (u8_t) (i + 1);
		snprintf(sensors[i].file, sizeof(sensors[i].file), "%d.txt", i);
		/* initialize sensor value to != zero */
		sensors[i].value = 11 * (i + 1);
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

/* sensortable .1.3.6.1.4.1.26381.1.1 */
/* list of allowed value ranges for incoming OID */
static const struct snmp_oid_range sensor_table_oid_ranges[] = { { 1, SENSOR_MAX
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

static s16_t sensor_table_get_value(struct snmp_node_instance *instance,
		void *value) {
	char buf[32];
	u32_t i = instance->reference.u32;
	s32_t *temperature = (s32_t*) value;

	switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
	case 1: /* sensor value */

		*temperature = i;

		return sizeof(s32_t);
	case 2: /* sensor value */

		*temperature = IN_PORT[i].sost_filtr;

		return sizeof(s32_t);
	case 3: /* sensor value */

		*temperature = 3; //IN_PORT[i].sost_filtr;

		return sizeof(s32_t);
	case 6: /* file name */
		sprintf(buf, "INPUT #%d ", i);
		MEMCPY(value, buf, strlen(buf));
		return (s16_t) strlen(buf);
	default:
		return 0;
	}
}

static snmp_err_t sensor_table_set_value(struct snmp_node_instance *instance,
		u16_t len, void *value) {
	u32_t i = instance->reference.u32;
	s32_t *temperature = (s32_t*) value;
	sensors[i].value = *temperature;
	LWIP_UNUSED_ARG(len);
	return SNMP_ERR_NOERROR;
}

#endif /* LWIP_SNMP */
