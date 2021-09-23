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
extern tcpip_adapter_ip_info_t ipInfo;


 void start_task(void *pvParameters);


#endif /* MAIN_APP_H_ */
