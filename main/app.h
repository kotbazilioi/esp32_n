/*
 * app.h
 *
 *  Created on: 22 сент. 2021 г.
 *      Author: ivanov
 */

#ifndef MAIN_APP_H_
#define MAIN_APP_H_

typedef struct
{
 uint8_t sost_raw;
 uint8_t sost_filtr_old;
 uint8_t sost_filtr;
 uint8_t sost_rise;
 uint8_t sost_fall;
 uint32_t filtr_time;
 uint32_t filtr_count;
 uint32_t semple_count;
 uint8_t event;

}input_port_t;


extern uint8_t chipid[6];
extern uint32_t  serial_id;

extern input_port_t IN_PORT[2];

 void start_task(void *pvParameters);


#endif /* MAIN_APP_H_ */
