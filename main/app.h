/*
 * app.h
 *
 *  Created on: 22 сент. 2021 г.
 *      Author: ivanov
 */

#ifndef MAIN_APP_H_
#define MAIN_APP_H_




extern uint8_t chipid[6];
extern uint32_t  serial_id;

extern char SNMP_COMMUNITY[32];
extern char SNMP_COMMUNITY_WRITE[32];

 void start_task(void *pvParameters);


#endif /* MAIN_APP_H_ */
