//#include "main.h"
#include <string.h>
#include "LOGS.h"
#include "nvs_task.h";
#include "app.h";
#include "smtp.h"
#include "config_pj.h"
#include "ping.h"
//#include "flash_if.h"
//#include "syslog.h"
log_reple_t reple_to_save;
log_reple_t reple_to_email;
// RTC_DateTypeDef dates;
// RTC_TimeTypeDef times;
struct tm timeinfo;
time_t now;
uint32_t timeup;
SemaphoreHandle_t flag_global_save_log = NULL;

void swich_mess_event(uint8_t event, char *mess) {
	switch (event) {
	case NO_RUN: {

	}
		break;

	case RESETL: {
		sprintf(mess, "Произведен принудительный сброс устройства\n");
	}
		break;
	case UPDATE_FW: {
		sprintf(mess, "Произведен переход в режим загрузки \n");
	}
		break;
	case SWICH_ON_WEB: {
		sprintf(mess, "Включена нагрузка через веб интерфейс \n");
	}
		break;
	case SWICH_OFF_WEB: {
		sprintf(mess, "Выключена нагрузка через веб интерфейс \n");
	}
		break;
	case SWICH_TOLG_WEB: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки через веб интерфейс \n");
	}
		break;
	case SWICH_ON_WEB_N: {
		sprintf(mess, "Включена нагрузка через веб интерфейс (неэффективна)\n");
	}
		break;
	case SWICH_OFF_WEB_N: {
		sprintf(mess,
				"Выключена нагрузка через веб интерфейс (неэффективна)\n");
	}
		break;
	case SWICH_TOLG_WEB_N: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки через веб интерфейс (неэффективна)\n");
	}
		break;
	case SWICH_ON_SNMP: {
		sprintf(mess, "Включена нагрузка по SNMP интерфейсу \n");
	}
		break;
	case SWICH_OFF_SNMP: {
		sprintf(mess, "Выключена нагрузка по SNMP интерфейсу \n");
	}
		break;
	case SWICH_TOLG_SNMP: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки по SNMP интерфейсу \n");
	}
		break;
	case SWICH_ON_SNMP_N: {
		sprintf(mess, "Включена нагрузка по SNMP интерфейсу (неэффективна)\n");
	}
		break;
	case SWICH_OFF_SNMP_N: {
		sprintf(mess, "Выключена нагрузка по SNMP интерфейсу (неэффективна)\n");
	}
		break;
	case SWICH_TOLG_SNMP_N: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки по SNMP интерфейсу (неэффективна)\n");
	}
		break;

	case SWICH_ON_RASP: {
		sprintf(mess, "Включена нагрузка по расписанию \n");
	}
		break;
	case SWICH_OFF_RASP: {
		sprintf(mess, "Выключена нагрузка по расписанию \n");
	}
		break;
	case SWICH_TOLG_RASP: {
		sprintf(mess, "Произведен импульсный сброс нагрузка по расписанию \n");
	}
		break;
	case SWICH_ON_RASP_N: {
		sprintf(mess, "Включена нагрузка по расписанию (неэффективна)\n");
	}
		break;
	case SWICH_OFF_RASP_N: {
		sprintf(mess, "Выключена нагрузка по расписанию (неэффективна)\n");
	}
		break;
	case SWICH_TOLG_RASP_N: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузка по расписанию (неэффективна)\n");
	}
		break;
	case SWICH_ON_WATCH: {
		sprintf(mess, "Включена нагрузка по сторожу \n");
	}
		break;
	case SWICH_OFF_WATCH: {
		sprintf(mess, "Выключена нагрузка по сторожу \n");
	}
		break;
	case SWICH_TOLG_WATCH: {
		sprintf(mess, "Произведен импульсный сброс нагрузки по сторожу \n");
	}
		break;
	case SWICH_ON_HTTP: {
		sprintf(mess, "Включена нагрузка по HTTP API \n");
	}
		break;
	case SWICH_OFF_HTTP: {
		sprintf(mess, "Выключена нагрузка по HTTP API \n");
	}
		break;
	case SWICH_TOLG_HTTP: {
		sprintf(mess, "Произведен импульсный сброс нагрузки по HTTP API \n");
	}
		break;
	case SWICH_ON_HTTP_N: {
		sprintf(mess, "Включена нагрузка по HTTP API (неэффективна)\n");
	}
		break;
	case SWICH_OFF_HTTP_N: {
		sprintf(mess, "Выключена нагрузка по HTTP API (неэффективна)\n");
	}
		break;
	case SWICH_TOLG_HTTP_N: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки по HTTP API (неэффективна)\n");
	}
		break;
	case POWER_ON: {
		sprintf(mess, "Включение питания устройства \n");
	}
		break;
	case LOAD_DEF_DATA: {
		sprintf(mess, "Загружены настройки по умолчанию \n");
	}
		break;
	case SAVE_DATA_SETT: {
		sprintf(mess, "Сохранение настроек устройства \n");
	}
		break;
	case SEND_EMAIL: {
		sprintf(mess, "Отправлено E-mail сообщение\n");
	}
		break;
	case IN_PORT0_RISE: {
		sprintf(mess, "На вход №0 подан высокий уровень\n");
	}
		break;
	case IN_PORT1_RISE: {
		sprintf(mess, "На вход №1 подан высокий уровень\n");
	}
		break;
	case IN_PORT2_RISE: {
		sprintf(mess, "На вход №2 подан высокий уровень\n");
	}
		break;
	case IN_PORT3_RISE: {
		sprintf(mess, "На вход №3 подан высокий уровень\n");
	}
		break;
	case IN_PORT0_FALL: {
		sprintf(mess, "На вход №0 подан низкий уровень\n");
	}
		break;
	case IN_PORT1_FALL: {
		sprintf(mess, "На вход №1 подан низкий уровень\n");
	}
		break;
	case IN_PORT2_FALL: {
		sprintf(mess, "На вход №2 подан низкий уровень\n");
	}
		break;
	case IN_PORT3_FALL: {
		sprintf(mess, "На вход №3 подан низкий уровень\n");
	}
		break;
	case SNMP_OUT_PORT0_SET: {
		sprintf(mess, "Включена нагрузка по SNMP интерфейсу PORT0\n");
	}
		break;
	case SNMP_OUT_PORT0_RES: {
		sprintf(mess, "Выключена нагрузка по SNMP интерфейсу PORT0\n");
	}
		break;
	case SNMP_OUT_PORT0_TOL: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки по SNMP интерфейсу PORT0\n");
	}
		break;
	case SNMP_OUT_PORT1_SET: {
		sprintf(mess, "Включена нагрузка по SNMP интерфейсу PORT1\n");
	}
		break;
	case SNMP_OUT_PORT1_RES: {
		sprintf(mess, "Выключена нагрузка по SNMP интерфейсу PORT1\n");
	}
		break;
	case SNMP_OUT_PORT1_TOL: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки по SNMP интерфейсу PORT1\n");
	}
		break;
	case WEB_OUT_PORT0_SET: {
		sprintf(mess, "Включена нагрузка по WEB интерфейсу PORT0\n");
	}
		break;
	case WEB_OUT_PORT0_RES: {
		sprintf(mess, "Выключена нагрузка по WEB интерфейсу PORT0\n");
	}
		break;
	case WEB_OUT_PORT0_TOL: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки по WEB интерфейсу PORT0\n");
	}
		break;
	case WEB_OUT_PORT1_SET: {
		sprintf(mess, "Включена нагрузка по WEB интерфейсу PORT1\n");
	}
		break;
	case WEB_OUT_PORT1_RES: {
		sprintf(mess, "Выключена нагрузка по WEB интерфейсу PORT1\n");
	}
		break;
	case WEB_OUT_PORT1_TOL: {
		sprintf(mess,
				"Произведен импульсный сброс нагрузки по WEB интерфейсу PORT1\n");
	}

	}
}

void swich_mess_event_en(uint8_t event, char *mess) {
	switch (event) {
	case NO_RUN: {

	}
		break;

	case RESETL: {
		sprintf(mess, "A forced device reset has been performed  \n\r");
	}
		break;
	case UPDATE_FW: {
		sprintf(mess, "Transferred to download mode  \n\r");
	}
		break;
	case SWICH_ON_WEB_N: {
		sprintf(mess,
				"Load from the web interface is enabled  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_OFF_WEB_N: {
		sprintf(mess,
				"The load from the web interface is turned off  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_TOLG_WEB_N: {
		sprintf(mess,
				"Impulse load dump was performed from the web interface  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_ON_SNMP_N: {
		sprintf(mess,
				"Load from SNMP interface is enabled  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_OFF_SNMP_N: {
		sprintf(mess, "Disabled load from SNMP interface  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_TOLG_SNMP_N: {
		sprintf(mess,
				"Pulse load shedding from SNMP interface has been performed  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_ON_WEB: {
		sprintf(mess, "Load from the web interface is enabled  \n\r");
	}
		break;
	case SWICH_OFF_WEB: {
		sprintf(mess, "The load from the web interface is turned off  \n\r");
	}
		break;
	case SWICH_TOLG_WEB: {
		sprintf(mess,
				"Impulse load dump was performed from the web interface  \n\r");
	}
		break;
	case SWICH_ON_SNMP: {
		sprintf(mess, "Load from SNMP interface is enabled  \n\r");
	}
		break;
	case SWICH_OFF_SNMP: {
		sprintf(mess, "Disabled load from SNMP interface  \n\r");
	}
		break;
	case SWICH_TOLG_SNMP: {
		sprintf(mess,
				"Pulse load shedding from SNMP interface has been performed  \n\r");
	}
		break;
	case SWICH_ON_RASP: {
		sprintf(mess, "Scheduled load is enabled  \n\r");
	}
		break;
	case SWICH_OFF_RASP: {
		sprintf(mess, "Disabled load on schedule  \n\r");
	}
		break;
	case SWICH_TOLG_RASP: {
		sprintf(mess,
				"Impulse reset of the load according to the schedule \n\r");
	}
		break;
	case SWICH_ON_RASP_N: {
		sprintf(mess, "Scheduled load is enabled  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_OFF_RASP_N: {
		sprintf(mess, "Disabled load on schedule  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_TOLG_RASP_N: {
		sprintf(mess,
				"Impulse reset of the load according to the schedule 'uneffecrtive'\n\r");
	}
		break;
	case SWICH_ON_WATCH: {
		sprintf(mess, "Watchdog load included  \n\r");
	}
		break;
	case SWICH_OFF_WATCH: {
		sprintf(mess, "The watchdog load is turned off  \n\r");
	}
		break;
	case SWICH_TOLG_WATCH: {
		sprintf(mess, "Impulse load shedding by the watchman  \n\r");
	}
		break;
	case SWICH_ON_HTTP: {
		sprintf(mess, "HTTP API loading enabled  \n\r");
	}
		break;
	case SWICH_OFF_HTTP: {
		sprintf(mess, "Disabled loading via HTTP API  \n\r");
	}
		break;
	case SWICH_TOLG_HTTP: {
		sprintf(mess,
				"Pulse load shedding via HTTP API has been performed  \n\r");
	}
		break;
	case SWICH_ON_HTTP_N: {
		sprintf(mess, "HTTP API loading enabled  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_OFF_HTTP_N: {
		sprintf(mess, "Disabled loading via HTTP API  'uneffecrtive'\n\r");
	}
		break;
	case SWICH_TOLG_HTTP_N: {
		sprintf(mess,
				"Pulse load shedding via HTTP API has been performed  'uneffecrtive'\n\r");
	}
		break;
	case POWER_ON: {
		sprintf(mess, "Power on the device  \n\r");
	}
		break;
	case LOAD_DEF_DATA: {
		sprintf(mess, "Loaded default settings  \n\r");
	}
		break;
	case SAVE_DATA_SETT: {
		sprintf(mess, "Saving device settings  \n\r");
	}
		break;
	case SNMP_OUT_PORT0_SET: {
		sprintf(mess, "Load from SNMP interface is enabled PORT0  \n\r");
	}
		break;
	case SNMP_OUT_PORT0_RES: {
		sprintf(mess, "Disabled load from SNMP interface PORT0  \n\r");
	}
		break;
	case SNMP_OUT_PORT0_TOL: {
		sprintf(mess,
				"Pulse load shedding from SNMP interface has been performed PORT0  \n\r");
	}
		break;
	case SNMP_OUT_PORT1_SET: {
		sprintf(mess, "Load from SNMP interface is enabled PORT1  \n\r");
	}
		break;
	case SNMP_OUT_PORT1_RES: {
		sprintf(mess, "Disabled load from SNMP interface PORT1  \n\r");
	}
		break;
	case SNMP_OUT_PORT1_TOL: {
		sprintf(mess,
				"Pulse load shedding from SNMP interface has been performed PORT1  \n\r");
	}
		break;
	case WEB_OUT_PORT0_SET: {
		sprintf(mess, "Load from WEB interface is enabled PORT0  \n\r");
	}
		break;
	case WEB_OUT_PORT0_RES: {
		sprintf(mess, "Disabled load from WEB interface PORT0  \n\r");
	}
		break;
	case WEB_OUT_PORT0_TOL: {
		sprintf(mess,
				"Pulse load shedding from WEB interface has been performed PORT0  \n\r");
	}
		break;
	case WEB_OUT_PORT1_SET: {
		sprintf(mess, "Load from WEB interface is enabled PORT1  \n\r");
	}
		break;
	case WEB_OUT_PORT1_RES: {
		sprintf(mess, "Disabled load from WEB interface PORT1  \n\r");
	}
		break;
	case WEB_OUT_PORT1_TOL: {
		sprintf(mess,
				"Pulse load shedding from WEB interface has been performed PORT1  \n\r");
	}

	}
}

void GET_reple(log_reple_t *reple) {
	time_t now;
//  HAL_RTC_GetDate (&hrtc,&dates,RTC_FORMAT_BIN);
//  HAL_RTC_GetTime (&hrtc,&times,RTC_FORMAT_BIN);
	time(&now);
	localtime_r(&now, &timeinfo);
	//reple->type_event = event;
	reple->reple_hours = timeinfo.tm_hour;
	reple->reple_minuts = timeinfo.tm_min;
	reple->reple_seconds = timeinfo.tm_sec;
	reple->dweek = timeinfo.tm_wday;
	reple->day = timeinfo.tm_mday;
	reple->month = timeinfo.tm_mon;
	reple->year = 2000 + timeinfo.tm_year - 100;
	reple->dicr = 0x0;
	if (reple->year == 2000) {
		reple->year = 2001;
	}
}
void decode_reple(char *out, log_reple_t *reple) {
	char out_small[200] = { 0 };
//  memset()
	sprintf(out_small, "%02d.%02d.%d  %02d:%02d:%02d    ", reple->day,
			reple->month, reple->year, reple->reple_hours, reple->reple_minuts,
			reple->reple_seconds);
	strcat(out, out_small);
	memset(out_small, 0, 200);
	swich_mess_event(reple->type_event, out_small);
	strcat(out, out_small);
}
void decode_reple_en(char *out, log_reple_t *reple) {
	char out_small[200] = { 0 };
//  memset()
	sprintf(out_small, "%02d.%02d.%d  %02d:%02d:%02d    ", reple->day,
			reple->month, reple->year, reple->reple_hours, reple->reple_minuts,
			reple->reple_seconds);
	strcat(out, out_small);
	memset(out_small, 0, 200);
	swich_mess_event_en(reple->type_event, out_small);
	strcat(out, out_small);
}

void form_reple_to_save(event_struct_t cfg) {
	char mess_syslog[200] = { 0 };

	GET_reple(&reple_to_save);
	decode_reple_en(mess_syslog, &reple_to_save);
	//syslog_printf(mess_syslog);
///////  xSemaphoreTake (flag_global_save_log, (TickType_t) 100);
}

void form_reple_to_smtp(event_struct_t cfg) {
	char mess_syslog[200] = { 0 };

	GET_reple(&reple_to_save);
	decode_reple_en(mess_syslog, &reple_to_save);

	////////////// xSemaphoreTake (flag_global_save_log, (TickType_t) 100);
}

uint8_t logs_read(uint16_t n_mess, char *mess) {
	char name_str[6] = { 0 };

	uint16_t size = 128;
	sprintf(name_str, "%d", n_mess);
	esp_err_t err = nvs_open_from_partition("nvs", "storage", NVS_READWRITE,
			&nvs_data_handle);
	err = nvs_get_str(nvs_data_handle, name_str, mess, &size);
	if ((err == ESP_ERR_NVS_PART_NOT_FOUND) || (err == ESP_ERR_NVS_NOT_FOUND)) {
		err = ESP_OK;
		mess = NULL;
		printf("zero mess");
		return 1;
	}
	err = nvs_commit(nvs_data_handle);
//	printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
	nvs_close(nvs_data_handle);
	return 0;
}
void save_reple_log(log_reple_t reple2) {
	char name_str[6];
	char mess[256];
	char event_mess[100];
	uint16_t size = 128;
	uint16_t number_mess;

	reple_to_email.type_event = reple2.type_event;
	reple_to_email.dicr = 1;

	esp_err_t err = nvs_open_from_partition("nvs", "storage", NVS_READWRITE,
			&nvs_data_handle);
	swich_mess_event(reple2.type_event, event_mess);
	err = nvs_get_u16(nvs_data_handle, "number_mess", &number_mess);
	if ((err == ESP_ERR_NVS_PART_NOT_FOUND) || (err == ESP_ERR_NVS_NOT_FOUND)) {
		err = ESP_OK;
		number_mess = 0;
	}
	printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
	sprintf(mess, " %02d.%02d.%02d  %02d:%02d:%02d   %s ", reple2.day,
			reple2.month, reple2.year, reple2.reple_hours, reple2.reple_minuts,
			reple2.reple_seconds, event_mess);

	if ((err == ESP_OK) && (number_mess < max_log_mess))
		number_mess++;
	if ((number_mess == max_log_mess) || (number_mess > max_log_mess))
		number_mess = 0;

	err = nvs_commit(nvs_data_handle);
	printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

	sprintf(name_str, "%d", number_mess);

//	send_smtp_mess(mess);

	err = nvs_set_str(nvs_data_handle, name_str, mess);
	err = nvs_set_u16(nvs_data_handle, "number_mess", number_mess);
	printf("save %d log mess committing updates in NVS ... ", number_mess);
	err = nvs_commit(nvs_data_handle);
	printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
	nvs_close(nvs_data_handle);
}
void log_task(void *pvParameters) {
	uint8_t ct;

	while (1) {
		for (ct = 0; ct < in_port_n; ct++) {

			if (FW_data.gpio.IN_PORT[ct].event == 1) {
				if (FW_data.gpio.IN_PORT[ct].sost_rise == 1) {
					reple_to_save.type_event = IN_PORT0_RISE + ct + out_port_n;
					reple_to_save.dicr = 1;
					FW_data.gpio.IN_PORT[ct].sost_rise = 0;
					FW_data.gpio.IN_PORT[ct].event = 0;

				}
				if (FW_data.gpio.IN_PORT[ct].sost_fall == 1) {

					reple_to_save.type_event = IN_PORT0_FALL + ct + out_port_n;
					reple_to_save.dicr = 1;
					FW_data.gpio.IN_PORT[ct].sost_fall = 0;
					FW_data.gpio.IN_PORT[ct].event = 0;
				}
			}
		}
		for (ct = 0; ct < out_port_n; ct++) {
			if (FW_data.gpio.OUT_PORT[ct].event != 0) {
				reple_to_save.type_event = FW_data.gpio.OUT_PORT[ct].event;
				reple_to_save.dicr = 1;
				FW_data.gpio.OUT_PORT[ct].event = 0;
				reple_to_save.dicr = 1;

			}

			if (FW_data.gpio.OUT_PORT[ct].input_str.event == 1) {
				if (FW_data.gpio.OUT_PORT[ct].input_str.sost_rise == 1) {
					reple_to_save.type_event = IN_PORT0_RISE + ct;
					reple_to_save.dicr = 1;
					FW_data.gpio.OUT_PORT[ct].input_str.sost_rise = 0;
					FW_data.gpio.OUT_PORT[ct].input_str.event = 0;

				}
				if (FW_data.gpio.OUT_PORT[ct].input_str.sost_fall == 1) {

					reple_to_save.type_event = IN_PORT0_FALL + ct;
					reple_to_save.dicr = 1;
					FW_data.gpio.OUT_PORT[ct].input_str.sost_fall = 0;
					FW_data.gpio.OUT_PORT[ct].input_str.event = 0;
				}
			}

		}
//		if (FW_data.gpio.IN_PORT[1].event == 1) {
//			if (FW_data.gpio.IN_PORT[1].sost_rise == 1) {
//				reple_to_save.type_event=IN_PORT1_RISE;
//				reple_to_save.dicr=1;
//				FW_data.gpio.IN_PORT[1].sost_rise = 0;
//				FW_data.gpio.IN_PORT[1].event=0;
//
//
//			}
//			if (FW_data.gpio.IN_PORT[1].sost_fall == 1) {
//				reple_to_save.type_event=IN_PORT1_FALL;
//				reple_to_save.dicr=1;
//				FW_data.gpio.IN_PORT[1].sost_fall = 0;
//				FW_data.gpio.IN_PORT[1].event=0;
//
//			}
//	}



//
//
//		if ((FW_data.wdt[0].V_EN_WATCHDOG == 1) && (flag_delay_ping == 0)) {
//			if (FW_data.wdt[0].V_TYPE_LOGIC == 0) {
//				if ((ping_data.flag_err[0] != 0) || (ping_data.flag_err[1] != 0)
//						|| (ping_data.flag_err[2] != 0)) {
//
//					if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//						flag_delay_ping = 1;
//						form_reple_to_save(SWICH_TOLG_WATCH);
//						//flag_global_swich_out = SWICH_TOLG_WATCH;
//						if (FW_data.wdt[0].V_N_OUT == 1) {
//							FW_data.gpio.OUT_PORT[0].old_sost =
//									FW_data.gpio.OUT_PORT[0].sost;
//							FW_data.gpio.OUT_PORT[0].sost = 1;
//							FW_data.gpio.OUT_PORT[0].type_logic = 3;
//							FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//							FW_data.gpio.OUT_PORT[0].aflag = 1;
//						} else if (FW_data.wdt[0].V_N_OUT == 2){
//							FW_data.gpio.OUT_PORT[1].old_sost =
//									FW_data.gpio.OUT_PORT[1].sost;
//							FW_data.gpio.OUT_PORT[1].sost = 1;
//							FW_data.gpio.OUT_PORT[1].type_logic = 3;
//							FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//							FW_data.gpio.OUT_PORT[1].aflag = 1;
//						}
//						//  HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//
//						ct_res_wdt();
//						ct_cn_a = 0;
//						ct_cn_b = 0;
//						ct_cn_c = 0;
//					} else {
//						if (ct_max_res
//								< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//							flag_delay_ping = 1;
//							form_reple_to_save(SWICH_TOLG_WATCH);
//							//flag_global_swich_out = SWICH_TOLG_WATCH;
//							if (FW_data.wdt[0].V_N_OUT == 1) {
//								FW_data.gpio.OUT_PORT[0].old_sost =
//										FW_data.gpio.OUT_PORT[0].sost;
//								FW_data.gpio.OUT_PORT[0].sost = 1;
//								FW_data.gpio.OUT_PORT[0].type_logic = 3;
//								FW_data.gpio.OUT_PORT[0].event =
//										WEB_OUT_PORT0_TOL;
//								FW_data.gpio.OUT_PORT[0].aflag = 1;
//							} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//								FW_data.gpio.OUT_PORT[1].old_sost =
//										FW_data.gpio.OUT_PORT[1].sost;
//								FW_data.gpio.OUT_PORT[1].sost = 1;
//								FW_data.gpio.OUT_PORT[1].type_logic = 3;
//								FW_data.gpio.OUT_PORT[1].event =
//										WEB_OUT_PORT1_TOL;
//								FW_data.gpio.OUT_PORT[1].aflag = 1;
//							}
//							///    HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//							ct_max_res++;
//
//							ct_res_wdt();
//							ct_cn_a = 0;
//							ct_cn_b = 0;
//							ct_cn_c = 0;
//
//						}
//						//         else
//						//          {
//						//
//						//           en_ping_a=0;
//						//           en_ping_b=0;
//						//           en_ping_c=0;
//						//           ping_data.flag_err[0]=0;
//						//           ping_data.flag_err[1]=0;
//						//           ping_data.flag_err[2]=0;
//						//
//						//
//						//          }
//					}
//				} else {
//					ct_max_res = 0;
//				}
//			}
//			if (FW_data.wdt[0].V_TYPE_LOGIC == 1) {
//				if ((ping_data.flag_err[0] != 0) && (ping_data.flag_err[1] != 0)
//						&& (ping_data.flag_err[2] != 0)) {
//
//					if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//						form_reple_to_save(SWICH_TOLG_WATCH);
//						//flag_global_swich_out=SWICH_TOLG_WATCH;/
//						if (FW_data.wdt[0].V_N_OUT == 1) {
//							FW_data.gpio.OUT_PORT[0].old_sost =
//									FW_data.gpio.OUT_PORT[0].sost;
//							FW_data.gpio.OUT_PORT[0].sost = 1;
//							FW_data.gpio.OUT_PORT[0].type_logic = 3;
//							FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//							FW_data.gpio.OUT_PORT[0].aflag = 1;
//						} else  if (FW_data.wdt[0].V_N_OUT == 2){
//							FW_data.gpio.OUT_PORT[1].old_sost =
//									FW_data.gpio.OUT_PORT[1].sost;
//							FW_data.gpio.OUT_PORT[1].sost = 1;
//							FW_data.gpio.OUT_PORT[1].type_logic = 3;
//							FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//							FW_data.gpio.OUT_PORT[1].aflag = 1;
//						}
//						//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//						flag_delay_ping = 1;
//						ct_res_wdt();
//						ct_cn_a = 0;
//						ct_cn_b = 0;
//						ct_cn_c = 0;
//					} else {
//						if (ct_max_res
//								< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//							form_reple_to_save(SWICH_TOLG_WATCH);
//							//flag_global_swich_out=SWICH_TOLG_WATCH;
//							if (FW_data.wdt[0].V_N_OUT == 1) {
//								FW_data.gpio.OUT_PORT[0].old_sost =
//										FW_data.gpio.OUT_PORT[0].sost;
//								FW_data.gpio.OUT_PORT[0].sost = 1;
//								FW_data.gpio.OUT_PORT[0].type_logic = 3;
//								FW_data.gpio.OUT_PORT[0].event =
//										WEB_OUT_PORT0_TOL;
//								FW_data.gpio.OUT_PORT[0].aflag = 1;
//							} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//								FW_data.gpio.OUT_PORT[1].old_sost =
//										FW_data.gpio.OUT_PORT[1].sost;
//								FW_data.gpio.OUT_PORT[1].sost = 1;
//								FW_data.gpio.OUT_PORT[1].type_logic = 3;
//								FW_data.gpio.OUT_PORT[1].event =
//										WEB_OUT_PORT1_TOL;
//								FW_data.gpio.OUT_PORT[1].aflag = 1;
//							}
//							//        HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//							ct_max_res++;
//							flag_delay_ping = 1;
//							ct_res_wdt();
//							ct_cn_a = 0;
//							ct_cn_b = 0;
//							ct_cn_c = 0;
//						}
//						//         else
//						//          {
//						//           en_ping_a=0;
//						//           en_ping_b=0;
//						//           en_ping_c=0;
//						//           ping_data.flag_err[0]=0;
//						//           ping_data.flag_err[1]=0;
//						//           ping_data.flag_err[2]=0;
//						//          }
//					}
//				} else {
//					ct_max_res = 0;
//				}
//			}
//			if (FW_data.wdt[0].V_TYPE_LOGIC == 2) {
//				if (((ping_data.flag_err[0] != 0)
//						&& (ping_data.flag_err[1] != 0))
//						|| ((ping_data.flag_err[0] != 0)
//								&& (ping_data.flag_err[2] != 0))) {
//
//					if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//						form_reple_to_save(SWICH_TOLG_WATCH);
//						//    flag_global_swich_out=SWICH_TOLG_WATCH;
//						if (FW_data.wdt[0].V_N_OUT == 1) {
//							FW_data.gpio.OUT_PORT[0].old_sost =
//									FW_data.gpio.OUT_PORT[0].sost;
//							FW_data.gpio.OUT_PORT[0].sost = 1;
//							FW_data.gpio.OUT_PORT[0].type_logic = 3;
//							FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//							FW_data.gpio.OUT_PORT[0].aflag = 1;
//						} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//							FW_data.gpio.OUT_PORT[1].old_sost =
//									FW_data.gpio.OUT_PORT[1].sost;
//							FW_data.gpio.OUT_PORT[1].sost = 1;
//							FW_data.gpio.OUT_PORT[1].type_logic = 3;
//							FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//							FW_data.gpio.OUT_PORT[1].aflag = 1;
//						}
//						//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//						flag_delay_ping = 1;
//						ct_res_wdt();
//						ct_cn_a = 0;
//						ct_cn_b = 0;
//						ct_cn_c = 0;
//					} else {
//						if (ct_max_res
//								< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//							form_reple_to_save(SWICH_TOLG_WATCH);
//							// flag_global_swich_out=SWICH_TOLG_WATCH;
//							//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//							if (FW_data.wdt[0].V_N_OUT == 1) {
//								FW_data.gpio.OUT_PORT[0].old_sost =
//										FW_data.gpio.OUT_PORT[0].sost;
//								FW_data.gpio.OUT_PORT[0].sost = 1;
//								FW_data.gpio.OUT_PORT[0].type_logic = 3;
//								FW_data.gpio.OUT_PORT[0].event =
//										WEB_OUT_PORT0_TOL;
//								FW_data.gpio.OUT_PORT[0].aflag = 1;
//							} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//								FW_data.gpio.OUT_PORT[1].old_sost =
//										FW_data.gpio.OUT_PORT[1].sost;
//								FW_data.gpio.OUT_PORT[1].sost = 1;
//								FW_data.gpio.OUT_PORT[1].type_logic = 3;
//								FW_data.gpio.OUT_PORT[1].event =
//										WEB_OUT_PORT1_TOL;
//								FW_data.gpio.OUT_PORT[1].aflag = 1;
//							}
//							ct_max_res++;
//							flag_delay_ping = 1;
//							ct_res_wdt();
//							ct_cn_a = 0;
//							ct_cn_b = 0;
//							ct_cn_c = 0;
//						}
//						//          else
//						//          {
//						//           en_ping_a=0;
//						//           en_ping_b=0;
//						//           en_ping_c=0;
//						//           ping_data.flag_err[0]=0;
//						//           ping_data.flag_err[1]=0;
//						//           ping_data.flag_err[2]=0;
//						//          }
//					}
//				} else {
//					ct_max_res = 0;
//				}
//			}
//			if (FW_data.wdt[0].V_TYPE_LOGIC == 3) {
//				if (((ping_data.flag_err[0] != 0)
//						&& (ping_data.flag_err[1] != 1))
//						&& ((ping_data.flag_err[0] != 0)
//								&& (ping_data.flag_err[2] != 1))) {
//					if (ct_max_res < FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//						form_reple_to_save(SWICH_TOLG_WATCH);
//						//  flag_global_swich_out=SWICH_TOLG_WATCH;
//						if (FW_data.wdt[0].V_N_OUT == 1) {
//							FW_data.gpio.OUT_PORT[0].old_sost =
//									FW_data.gpio.OUT_PORT[0].sost;
//							FW_data.gpio.OUT_PORT[0].sost = 1;
//							FW_data.gpio.OUT_PORT[0].type_logic = 3;
//							FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//							FW_data.gpio.OUT_PORT[0].aflag = 1;
//						} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//							FW_data.gpio.OUT_PORT[1].old_sost =
//									FW_data.gpio.OUT_PORT[1].sost;
//							FW_data.gpio.OUT_PORT[1].sost = 1;
//							FW_data.gpio.OUT_PORT[1].type_logic = 3;
//							FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//							FW_data.gpio.OUT_PORT[1].aflag = 1;
//						}
//						//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//						ct_max_res++;
//						flag_delay_ping = 1;
//						ct_res_wdt();
//						ct_cn_a = 0;
//						ct_cn_b = 0;
//						ct_cn_c = 0;
//					}
//					if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//						form_reple_to_save(SWICH_TOLG_WATCH);
//						//  flag_global_swich_out=SWICH_TOLG_WATCH;
//						if (FW_data.wdt[0].V_N_OUT == 1) {
//							FW_data.gpio.OUT_PORT[0].old_sost =
//									FW_data.gpio.OUT_PORT[0].sost;
//							FW_data.gpio.OUT_PORT[0].sost = 1;
//							FW_data.gpio.OUT_PORT[0].type_logic = 3;
//							FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//							FW_data.gpio.OUT_PORT[0].aflag = 1;
//						} else  if (FW_data.wdt[0].V_N_OUT == 2){
//							FW_data.gpio.OUT_PORT[1].old_sost =
//									FW_data.gpio.OUT_PORT[1].sost;
//							FW_data.gpio.OUT_PORT[1].sost = 1;
//							FW_data.gpio.OUT_PORT[1].type_logic = 3;
//							FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//							FW_data.gpio.OUT_PORT[1].aflag = 1;
//						}
//						//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//						flag_delay_ping = 1;
//						ct_res_wdt();
//						ct_cn_a = 0;
//						ct_cn_b = 0;
//						ct_cn_c = 0;
//					}
//				} else {
//					ct_max_res = 0;
//				}
//			}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//			if ((FW_data.wdt[0].V_EN_WATCHDOG == 1) && (flag_delay_ping == 0)) {
//						if (FW_data.wdt[0].V_TYPE_LOGIC == 0) {
//							if ((ping_data.flag_err[0] != 0) || (ping_data.flag_err[1] != 0)
//									|| (ping_data.flag_err[2] != 0)) {
//
//								if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//									flag_delay_ping = 1;
//									form_reple_to_save(SWICH_TOLG_WATCH);
//									//flag_global_swich_out = SWICH_TOLG_WATCH;
//									if (FW_data.wdt[0].V_N_OUT == 1) {
//										FW_data.gpio.OUT_PORT[0].old_sost =
//												FW_data.gpio.OUT_PORT[0].sost;
//										FW_data.gpio.OUT_PORT[0].sost = 1;
//										FW_data.gpio.OUT_PORT[0].type_logic = 3;
//										FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//										FW_data.gpio.OUT_PORT[0].aflag = 1;
//									} else if (FW_data.wdt[0].V_N_OUT == 2){
//										FW_data.gpio.OUT_PORT[1].old_sost =
//												FW_data.gpio.OUT_PORT[1].sost;
//										FW_data.gpio.OUT_PORT[1].sost = 1;
//										FW_data.gpio.OUT_PORT[1].type_logic = 3;
//										FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//										FW_data.gpio.OUT_PORT[1].aflag = 1;
//									}
//									//  HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//
//									ct_res_wdt();
//									ct_cn_a = 0;
//									ct_cn_b = 0;
//									ct_cn_c = 0;
//								} else {
//									if (ct_max_res
//											< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//										flag_delay_ping = 1;
//										form_reple_to_save(SWICH_TOLG_WATCH);
//										//flag_global_swich_out = SWICH_TOLG_WATCH;
//										if (FW_data.wdt[0].V_N_OUT == 1) {
//											FW_data.gpio.OUT_PORT[0].old_sost =
//													FW_data.gpio.OUT_PORT[0].sost;
//											FW_data.gpio.OUT_PORT[0].sost = 1;
//											FW_data.gpio.OUT_PORT[0].type_logic = 3;
//											FW_data.gpio.OUT_PORT[0].event =
//													WEB_OUT_PORT0_TOL;
//											FW_data.gpio.OUT_PORT[0].aflag = 1;
//										} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//											FW_data.gpio.OUT_PORT[1].old_sost =
//													FW_data.gpio.OUT_PORT[1].sost;
//											FW_data.gpio.OUT_PORT[1].sost = 1;
//											FW_data.gpio.OUT_PORT[1].type_logic = 3;
//											FW_data.gpio.OUT_PORT[1].event =
//													WEB_OUT_PORT1_TOL;
//											FW_data.gpio.OUT_PORT[1].aflag = 1;
//										}
//										///    HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//										ct_max_res++;
//
//										ct_res_wdt();
//										ct_cn_a = 0;
//										ct_cn_b = 0;
//										ct_cn_c = 0;
//
//									}
//									//         else
//									//          {
//									//
//									//           en_ping_a=0;
//									//           en_ping_b=0;
//									//           en_ping_c=0;
//									//           ping_data.flag_err[0]=0;
//									//           ping_data.flag_err[1]=0;
//									//           ping_data.flag_err[2]=0;
//									//
//									//
//									//          }
//								}
//							} else {
//								ct_max_res = 0;
//							}
//						}
//						if (FW_data.wdt[0].V_TYPE_LOGIC == 1) {
//							if ((ping_data.flag_err[0] != 0) && (ping_data.flag_err[1] != 0)
//									&& (ping_data.flag_err[2] != 0)) {
//
//								if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//									form_reple_to_save(SWICH_TOLG_WATCH);
//									//flag_global_swich_out=SWICH_TOLG_WATCH;/
//									if (FW_data.wdt[0].V_N_OUT == 1) {
//										FW_data.gpio.OUT_PORT[0].old_sost =
//												FW_data.gpio.OUT_PORT[0].sost;
//										FW_data.gpio.OUT_PORT[0].sost = 1;
//										FW_data.gpio.OUT_PORT[0].type_logic = 3;
//										FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//										FW_data.gpio.OUT_PORT[0].aflag = 1;
//									} else  if (FW_data.wdt[0].V_N_OUT == 2){
//										FW_data.gpio.OUT_PORT[1].old_sost =
//												FW_data.gpio.OUT_PORT[1].sost;
//										FW_data.gpio.OUT_PORT[1].sost = 1;
//										FW_data.gpio.OUT_PORT[1].type_logic = 3;
//										FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//										FW_data.gpio.OUT_PORT[1].aflag = 1;
//									}
//									//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//									flag_delay_ping = 1;
//									ct_res_wdt();
//									ct_cn_a = 0;
//									ct_cn_b = 0;
//									ct_cn_c = 0;
//								} else {
//									if (ct_max_res
//											< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//										form_reple_to_save(SWICH_TOLG_WATCH);
//										//flag_global_swich_out=SWICH_TOLG_WATCH;
//										if (FW_data.wdt[0].V_N_OUT == 1) {
//											FW_data.gpio.OUT_PORT[0].old_sost =
//													FW_data.gpio.OUT_PORT[0].sost;
//											FW_data.gpio.OUT_PORT[0].sost = 1;
//											FW_data.gpio.OUT_PORT[0].type_logic = 3;
//											FW_data.gpio.OUT_PORT[0].event =
//													WEB_OUT_PORT0_TOL;
//											FW_data.gpio.OUT_PORT[0].aflag = 1;
//										} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//											FW_data.gpio.OUT_PORT[1].old_sost =
//													FW_data.gpio.OUT_PORT[1].sost;
//											FW_data.gpio.OUT_PORT[1].sost = 1;
//											FW_data.gpio.OUT_PORT[1].type_logic = 3;
//											FW_data.gpio.OUT_PORT[1].event =
//													WEB_OUT_PORT1_TOL;
//											FW_data.gpio.OUT_PORT[1].aflag = 1;
//										}
//										//        HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//										ct_max_res++;
//										flag_delay_ping = 1;
//										ct_res_wdt();
//										ct_cn_a = 0;
//										ct_cn_b = 0;
//										ct_cn_c = 0;
//									}
//									//         else
//									//          {
//									//           en_ping_a=0;
//									//           en_ping_b=0;
//									//           en_ping_c=0;
//									//           ping_data.flag_err[0]=0;
//									//           ping_data.flag_err[1]=0;
//									//           ping_data.flag_err[2]=0;
//									//          }
//								}
//							} else {
//								ct_max_res = 0;
//							}
//						}
//						if (FW_data.wdt[0].V_TYPE_LOGIC == 2) {
//							if (((ping_data.flag_err[0] != 0)
//									&& (ping_data.flag_err[1] != 0))
//									|| ((ping_data.flag_err[0] != 0)
//											&& (ping_data.flag_err[2] != 0))) {
//
//								if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//									form_reple_to_save(SWICH_TOLG_WATCH);
//									//    flag_global_swich_out=SWICH_TOLG_WATCH;
//									if (FW_data.wdt[0].V_N_OUT == 1) {
//										FW_data.gpio.OUT_PORT[0].old_sost =
//												FW_data.gpio.OUT_PORT[0].sost;
//										FW_data.gpio.OUT_PORT[0].sost = 1;
//										FW_data.gpio.OUT_PORT[0].type_logic = 3;
//										FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//										FW_data.gpio.OUT_PORT[0].aflag = 1;
//									} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//										FW_data.gpio.OUT_PORT[1].old_sost =
//												FW_data.gpio.OUT_PORT[1].sost;
//										FW_data.gpio.OUT_PORT[1].sost = 1;
//										FW_data.gpio.OUT_PORT[1].type_logic = 3;
//										FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//										FW_data.gpio.OUT_PORT[1].aflag = 1;
//									}
//									//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//									flag_delay_ping = 1;
//									ct_res_wdt();
//									ct_cn_a = 0;
//									ct_cn_b = 0;
//									ct_cn_c = 0;
//								} else {
//									if (ct_max_res
//											< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//										form_reple_to_save(SWICH_TOLG_WATCH);
//										// flag_global_swich_out=SWICH_TOLG_WATCH;
//										//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//										if (FW_data.wdt[0].V_N_OUT == 1) {
//											FW_data.gpio.OUT_PORT[0].old_sost =
//													FW_data.gpio.OUT_PORT[0].sost;
//											FW_data.gpio.OUT_PORT[0].sost = 1;
//											FW_data.gpio.OUT_PORT[0].type_logic = 3;
//											FW_data.gpio.OUT_PORT[0].event =
//													WEB_OUT_PORT0_TOL;
//											FW_data.gpio.OUT_PORT[0].aflag = 1;
//										} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//											FW_data.gpio.OUT_PORT[1].old_sost =
//													FW_data.gpio.OUT_PORT[1].sost;
//											FW_data.gpio.OUT_PORT[1].sost = 1;
//											FW_data.gpio.OUT_PORT[1].type_logic = 3;
//											FW_data.gpio.OUT_PORT[1].event =
//													WEB_OUT_PORT1_TOL;
//											FW_data.gpio.OUT_PORT[1].aflag = 1;
//										}
//										ct_max_res++;
//										flag_delay_ping = 1;
//										ct_res_wdt();
//										ct_cn_a = 0;
//										ct_cn_b = 0;
//										ct_cn_c = 0;
//									}
//									//          else
//									//          {
//									//           en_ping_a=0;
//									//           en_ping_b=0;
//									//           en_ping_c=0;
//									//           ping_data.flag_err[0]=0;
//									//           ping_data.flag_err[1]=0;
//									//           ping_data.flag_err[2]=0;
//									//          }
//								}
//							} else {
//								ct_max_res = 0;
//							}
//						}
//						if (FW_data.wdt[0].V_TYPE_LOGIC == 3) {
//							if (((ping_data.flag_err[0] != 0)
//									&& (ping_data.flag_err[1] != 1))
//									&& ((ping_data.flag_err[0] != 0)
//											&& (ping_data.flag_err[2] != 1))) {
//								if (ct_max_res < FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
//									form_reple_to_save(SWICH_TOLG_WATCH);
//									//  flag_global_swich_out=SWICH_TOLG_WATCH;
//									if (FW_data.wdt[0].V_N_OUT == 1) {
//										FW_data.gpio.OUT_PORT[0].old_sost =
//												FW_data.gpio.OUT_PORT[0].sost;
//										FW_data.gpio.OUT_PORT[0].sost = 1;
//										FW_data.gpio.OUT_PORT[0].type_logic = 3;
//										FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//										FW_data.gpio.OUT_PORT[0].aflag = 1;
//									} else  if (FW_data.wdt[0].V_N_OUT == 2) {
//										FW_data.gpio.OUT_PORT[1].old_sost =
//												FW_data.gpio.OUT_PORT[1].sost;
//										FW_data.gpio.OUT_PORT[1].sost = 1;
//										FW_data.gpio.OUT_PORT[1].type_logic = 3;
//										FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//										FW_data.gpio.OUT_PORT[1].aflag = 1;
//									}
//									//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//									ct_max_res++;
//									flag_delay_ping = 1;
//									ct_res_wdt();
//									ct_cn_a = 0;
//									ct_cn_b = 0;
//									ct_cn_c = 0;
//								}
//								if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
//									form_reple_to_save(SWICH_TOLG_WATCH);
//									//  flag_global_swich_out=SWICH_TOLG_WATCH;
//									if (FW_data.wdt[0].V_N_OUT == 1) {
//										FW_data.gpio.OUT_PORT[0].old_sost =
//												FW_data.gpio.OUT_PORT[0].sost;
//										FW_data.gpio.OUT_PORT[0].sost = 1;
//										FW_data.gpio.OUT_PORT[0].type_logic = 3;
//										FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
//										FW_data.gpio.OUT_PORT[0].aflag = 1;
//									} else  if (FW_data.wdt[0].V_N_OUT == 2){
//										FW_data.gpio.OUT_PORT[1].old_sost =
//												FW_data.gpio.OUT_PORT[1].sost;
//										FW_data.gpio.OUT_PORT[1].sost = 1;
//										FW_data.gpio.OUT_PORT[1].type_logic = 3;
//										FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
//										FW_data.gpio.OUT_PORT[1].aflag = 1;
//									}
//									//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//									flag_delay_ping = 1;
//									ct_res_wdt();
//									ct_cn_a = 0;
//									ct_cn_b = 0;
//									ct_cn_c = 0;
//								}
//							} else {
//								ct_max_res = 0;
//							}
//						}
//		}
//




			if ((FW_data.wdt[0].V_EN_WATCHDOG == 1) && (flag_delay_ping == 0)) {
				if (FW_data.wdt[0].V_TYPE_LOGIC == 0) {
					if ((ping_data.flag_err[0] != 0) || (ping_data.flag_err[1] != 0)
							|| (ping_data.flag_err[2] != 0)) {

						if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
							flag_delay_ping = 1;

							form_reple_to_save(event_main);
							//form_reple_to_save(SWICH_TOLG_WATCH);
							//flag_global_swich_out = SWICH_TOLG_WATCH;
							if (FW_data.wdt[0].V_N_OUT == 11008) {
								FW_data.gpio.OUT_PORT[0].old_sost =
										FW_data.gpio.OUT_PORT[0].sost;
								FW_data.gpio.OUT_PORT[0].sost = 1;
								FW_data.gpio.OUT_PORT[0].type_logic = 3;
								FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
								FW_data.gpio.OUT_PORT[0].aflag = 1;
							} else if (FW_data.wdt[0].V_N_OUT == 11009){
								FW_data.gpio.OUT_PORT[1].old_sost =
										FW_data.gpio.OUT_PORT[1].sost;
								FW_data.gpio.OUT_PORT[1].sost = 1;
								FW_data.gpio.OUT_PORT[1].type_logic = 3;
								FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
								FW_data.gpio.OUT_PORT[1].aflag = 1;
							}
							//  HAL_RTCEx_BKUPWrite(&hrtc,1,2);

							ct_res_wdt();
							ct_cn_a = 0;
							ct_cn_b = 0;
							ct_cn_c = 0;
						} else {
							if (ct_max_res
									< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
								flag_delay_ping = 1;
								form_reple_to_save(event_main);
								//form_reple_to_save(SWICH_TOLG_WATCH);
								//flag_global_swich_out = SWICH_TOLG_WATCH;
								if (FW_data.wdt[0].V_N_OUT == 11008) {
									FW_data.gpio.OUT_PORT[0].old_sost =
											FW_data.gpio.OUT_PORT[0].sost;
									FW_data.gpio.OUT_PORT[0].sost = 1;
									FW_data.gpio.OUT_PORT[0].type_logic = 3;
									FW_data.gpio.OUT_PORT[0].event =
											WEB_OUT_PORT0_TOL;
									FW_data.gpio.OUT_PORT[0].aflag = 1;
								} else  if (FW_data.wdt[0].V_N_OUT == 11009) {
									FW_data.gpio.OUT_PORT[1].old_sost =
											FW_data.gpio.OUT_PORT[1].sost;
									FW_data.gpio.OUT_PORT[1].sost = 1;
									FW_data.gpio.OUT_PORT[1].type_logic = 3;
									FW_data.gpio.OUT_PORT[1].event =
											WEB_OUT_PORT1_TOL;
									FW_data.gpio.OUT_PORT[1].aflag = 1;
								}
								///    HAL_RTCEx_BKUPWrite(&hrtc,1,2);
								ct_max_res++;

								ct_res_wdt();
								ct_cn_a = 0;
								ct_cn_b = 0;
								ct_cn_c = 0;

							}
							//         else
							//          {
							//
							//           en_ping_a=0;
							//           en_ping_b=0;
							//           en_ping_c=0;
							//           ping_data.flag_err[0]=0;
							//           ping_data.flag_err[1]=0;
							//           ping_data.flag_err[2]=0;
							//
							//
							//          }
						}
					} else {
						ct_max_res = 0;
					}
				}
				if (FW_data.wdt[0].V_TYPE_LOGIC == 1) {
					if ((ping_data.flag_err[0] != 0) && (ping_data.flag_err[1] != 0)
							&& (ping_data.flag_err[2] != 0)) {

						if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
							form_reple_to_save(event_main);
							//form_reple_to_save(SWICH_TOLG_WATCH);
							//flag_global_swich_out=SWICH_TOLG_WATCH;/
							if (FW_data.wdt[0].V_N_OUT == 11008) {
								FW_data.gpio.OUT_PORT[0].old_sost =
										FW_data.gpio.OUT_PORT[0].sost;
								FW_data.gpio.OUT_PORT[0].sost = 1;
								FW_data.gpio.OUT_PORT[0].type_logic = 3;
								FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
								FW_data.gpio.OUT_PORT[0].aflag = 1;
							} else  if (FW_data.wdt[0].V_N_OUT == 11009){
								FW_data.gpio.OUT_PORT[1].old_sost =
										FW_data.gpio.OUT_PORT[1].sost;
								FW_data.gpio.OUT_PORT[1].sost = 1;
								FW_data.gpio.OUT_PORT[1].type_logic = 3;
								FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
								FW_data.gpio.OUT_PORT[1].aflag = 1;
							}
							//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
							flag_delay_ping = 1;
							ct_res_wdt();
							ct_cn_a = 0;
							ct_cn_b = 0;
							ct_cn_c = 0;
						} else {
							if (ct_max_res
									< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
								form_reple_to_save(event_main);
								//form_reple_to_save(SWICH_TOLG_WATCH);
								//flag_global_swich_out=SWICH_TOLG_WATCH;
								if (FW_data.wdt[0].V_N_OUT == 11008) {
									FW_data.gpio.OUT_PORT[0].old_sost =
											FW_data.gpio.OUT_PORT[0].sost;
									FW_data.gpio.OUT_PORT[0].sost = 1;
									FW_data.gpio.OUT_PORT[0].type_logic = 3;
									FW_data.gpio.OUT_PORT[0].event =
											WEB_OUT_PORT0_TOL;
									FW_data.gpio.OUT_PORT[0].aflag = 1;
								} else  if (FW_data.wdt[0].V_N_OUT == 11009) {
									FW_data.gpio.OUT_PORT[1].old_sost =
											FW_data.gpio.OUT_PORT[1].sost;
									FW_data.gpio.OUT_PORT[1].sost = 1;
									FW_data.gpio.OUT_PORT[1].type_logic = 3;
									FW_data.gpio.OUT_PORT[1].event =
											WEB_OUT_PORT1_TOL;
									FW_data.gpio.OUT_PORT[1].aflag = 1;
								}
								//        HAL_RTCEx_BKUPWrite(&hrtc,1,2);
								ct_max_res++;
								flag_delay_ping = 1;
								ct_res_wdt();
								ct_cn_a = 0;
								ct_cn_b = 0;
								ct_cn_c = 0;
							}
							//         else
							//          {
							//           en_ping_a=0;
							//           en_ping_b=0;
							//           en_ping_c=0;
							//           ping_data.flag_err[0]=0;
							//           ping_data.flag_err[1]=0;
							//           ping_data.flag_err[2]=0;
							//          }
						}
					} else {
						ct_max_res = 0;
					}
				}
				if (FW_data.wdt[0].V_TYPE_LOGIC == 2) {
					if (((ping_data.flag_err[0] != 0)
							&& (ping_data.flag_err[1] != 0))
							|| ((ping_data.flag_err[0] != 0)
									&& (ping_data.flag_err[2] != 0))) {

						if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
							form_reple_to_save(event_main);
							//form_reple_to_save(SWICH_TOLG_WATCH);
							//    flag_global_swich_out=SWICH_TOLG_WATCH;
							if (FW_data.wdt[0].V_N_OUT == 11008) {
								FW_data.gpio.OUT_PORT[0].old_sost =
										FW_data.gpio.OUT_PORT[0].sost;
								FW_data.gpio.OUT_PORT[0].sost = 1;
								FW_data.gpio.OUT_PORT[0].type_logic = 3;
								FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
								FW_data.gpio.OUT_PORT[0].aflag = 1;
							} else  if (FW_data.wdt[0].V_N_OUT == 11009) {
								FW_data.gpio.OUT_PORT[1].old_sost =
										FW_data.gpio.OUT_PORT[1].sost;
								FW_data.gpio.OUT_PORT[1].sost = 1;
								FW_data.gpio.OUT_PORT[1].type_logic = 3;
								FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
								FW_data.gpio.OUT_PORT[1].aflag = 1;
							}
							//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
							flag_delay_ping = 1;
							ct_res_wdt();
							ct_cn_a = 0;
							ct_cn_b = 0;
							ct_cn_c = 0;
						} else {
							if (ct_max_res
									< FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
								form_reple_to_save(event_main);
								//form_reple_to_save(SWICH_TOLG_WATCH);
								// flag_global_swich_out=SWICH_TOLG_WATCH;
								//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
								if (FW_data.wdt[0].V_N_OUT == 11008) {
									FW_data.gpio.OUT_PORT[0].old_sost =
											FW_data.gpio.OUT_PORT[0].sost;
									FW_data.gpio.OUT_PORT[0].sost = 1;
									FW_data.gpio.OUT_PORT[0].type_logic = 3;
									FW_data.gpio.OUT_PORT[0].event =
											WEB_OUT_PORT0_TOL;
									FW_data.gpio.OUT_PORT[0].aflag = 1;
								} else  if (FW_data.wdt[0].V_N_OUT == 11009) {
									FW_data.gpio.OUT_PORT[1].old_sost =
											FW_data.gpio.OUT_PORT[1].sost;
									FW_data.gpio.OUT_PORT[1].sost = 1;
									FW_data.gpio.OUT_PORT[1].type_logic = 3;
									FW_data.gpio.OUT_PORT[1].event =
											WEB_OUT_PORT1_TOL;
									FW_data.gpio.OUT_PORT[1].aflag = 1;
								}
								ct_max_res++;
								flag_delay_ping = 1;
								ct_res_wdt();
								ct_cn_a = 0;
								ct_cn_b = 0;
								ct_cn_c = 0;
							}
							//          else
							//          {
							//           en_ping_a=0;
							//           en_ping_b=0;
							//           en_ping_c=0;
							//           ping_data.flag_err[0]=0;
							//           ping_data.flag_err[1]=0;
							//           ping_data.flag_err[2]=0;
							//          }
						}
					} else {
						ct_max_res = 0;
					}
				}
				if (FW_data.wdt[0].V_TYPE_LOGIC == 3) {
					if (((ping_data.flag_err[0] != 0)
							&& (ping_data.flag_err[1] != 1))
							&& ((ping_data.flag_err[0] != 0)
									&& (ping_data.flag_err[2] != 1))) {
						if (ct_max_res < FW_data.wdt[0].V_MAX_RESEND_PACET_RESET) {
							form_reple_to_save(event_main);
							//form_reple_to_save(SWICH_TOLG_WATCH);
							//  flag_global_swich_out=SWICH_TOLG_WATCH;
							if (FW_data.wdt[0].V_N_OUT == 11008) {
								FW_data.gpio.OUT_PORT[0].old_sost =
										FW_data.gpio.OUT_PORT[0].sost;
								FW_data.gpio.OUT_PORT[0].sost = 1;
								FW_data.gpio.OUT_PORT[0].type_logic = 3;
								FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
								FW_data.gpio.OUT_PORT[0].aflag = 1;
							} else  if (FW_data.wdt[0].V_N_OUT == 11009) {
								FW_data.gpio.OUT_PORT[1].old_sost =
										FW_data.gpio.OUT_PORT[1].sost;
								FW_data.gpio.OUT_PORT[1].sost = 1;
								FW_data.gpio.OUT_PORT[1].type_logic = 3;
								FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
								FW_data.gpio.OUT_PORT[1].aflag = 1;
							}
							//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
							ct_max_res++;
							flag_delay_ping = 1;
							ct_res_wdt();
							ct_cn_a = 0;
							ct_cn_b = 0;
							ct_cn_c = 0;
						}
						if (FW_data.wdt[0].V_MAX_RESEND_PACET_RESET == 0) {
							form_reple_to_save(event_main);
							//form_reple_to_save(SWICH_TOLG_WATCH);
							//  flag_global_swich_out=SWICH_TOLG_WATCH;
							if (FW_data.wdt[0].V_N_OUT == 11008) {
								FW_data.gpio.OUT_PORT[0].old_sost =
										FW_data.gpio.OUT_PORT[0].sost;
								FW_data.gpio.OUT_PORT[0].sost = 1;
								FW_data.gpio.OUT_PORT[0].type_logic = 3;
								FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
								FW_data.gpio.OUT_PORT[0].aflag = 1;
							} else  if (FW_data.wdt[0].V_N_OUT == 11009){
								FW_data.gpio.OUT_PORT[1].old_sost =
										FW_data.gpio.OUT_PORT[1].sost;
								FW_data.gpio.OUT_PORT[1].sost = 1;
								FW_data.gpio.OUT_PORT[1].type_logic = 3;
								FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
								FW_data.gpio.OUT_PORT[1].aflag = 1;
							}
							//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
							flag_delay_ping = 1;
							ct_res_wdt();
							ct_cn_a = 0;
							ct_cn_b = 0;
							ct_cn_c = 0;
						}
					} else {
						ct_max_res = 0;
					}
				}



			}













				if ((FW_data.wdt[1].V_EN_WATCHDOG == 1) && (flag_delay_ping1 == 0)) {
							if (FW_data.wdt[1].V_TYPE_LOGIC == 0) {
								if ((ping_data1.flag_err[0] != 0) || (ping_data1.flag_err[1] != 0)
										|| (ping_data1.flag_err[2] != 0)) {

									if (FW_data.wdt[1].V_MAX_RESEND_PACET_RESET == 0) {
										flag_delay_ping1 = 1;
										form_reple_to_save(event_main);
										//	form_reple_to_save(SWICH_TOLG_WATCH);

										if (FW_data.wdt[1].V_N_OUT == 11008) {
											FW_data.gpio.OUT_PORT[0].old_sost =
													FW_data.gpio.OUT_PORT[0].sost;
											FW_data.gpio.OUT_PORT[0].sost = 1;
											FW_data.gpio.OUT_PORT[0].type_logic = 3;
											FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
											FW_data.gpio.OUT_PORT[0].aflag = 1;
										} else if (FW_data.wdt[1].V_N_OUT == 11009){
											FW_data.gpio.OUT_PORT[1].old_sost =
													FW_data.gpio.OUT_PORT[1].sost;
											FW_data.gpio.OUT_PORT[1].sost = 1;
											FW_data.gpio.OUT_PORT[1].type_logic = 3;
											FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
											FW_data.gpio.OUT_PORT[1].aflag = 1;
										}
										//  HAL_RTCEx_BKUPWrite(&hrtc,1,2);

										ct_res_wdt1();
										ct_cn_a1 = 0;
										ct_cn_b1 = 0;
										ct_cn_c1 = 0;
									} else {
										if (ct_max_res1
												< FW_data.wdt[1].V_MAX_RESEND_PACET_RESET) {
											flag_delay_ping1 = 1;
											form_reple_to_save(event_main);
											//form_reple_to_save(SWICH_TOLG_WATCH);

											if (FW_data.wdt[1].V_N_OUT == 11008) {
												FW_data.gpio.OUT_PORT[0].old_sost =
														FW_data.gpio.OUT_PORT[0].sost;
												FW_data.gpio.OUT_PORT[0].sost = 1;
												FW_data.gpio.OUT_PORT[0].type_logic = 3;
												FW_data.gpio.OUT_PORT[0].event =
														WEB_OUT_PORT0_TOL;
												FW_data.gpio.OUT_PORT[0].aflag = 1;
											} else  if (FW_data.wdt[1].V_N_OUT == 11009) {
												FW_data.gpio.OUT_PORT[1].old_sost =
														FW_data.gpio.OUT_PORT[1].sost;
												FW_data.gpio.OUT_PORT[1].sost = 1;
												FW_data.gpio.OUT_PORT[1].type_logic = 3;
												FW_data.gpio.OUT_PORT[1].event =
														WEB_OUT_PORT1_TOL;
												FW_data.gpio.OUT_PORT[1].aflag = 1;
											}
											///    HAL_RTCEx_BKUPWrite(&hrtc,1,2);
											ct_max_res1++;

											ct_res_wdt1();
											ct_cn_a1 = 0;
											ct_cn_b1 = 0;
											ct_cn_c1 = 0;

										}
										//         else
										//          {
										//
										//           en_ping_a=0;
										//           en_ping_b=0;
										//           en_ping_c=0;
										//           ping_data.flag_err[0]=0;
										//           ping_data.flag_err[1]=0;
										//           ping_data.flag_err[2]=0;
										//
										//
										//          }
									}
								} else {
									ct_max_res1 = 0;
								}
							}
							if (FW_data.wdt[1].V_TYPE_LOGIC == 1) {
								if ((ping_data1.flag_err[0] != 0) && (ping_data1.flag_err[1] != 0)
										&& (ping_data1.flag_err[2] != 0)) {

									if (FW_data.wdt[1].V_MAX_RESEND_PACET_RESET == 11008) {
										form_reple_to_save(event_main);
										//form_reple_to_save(SWICH_TOLG_WATCH);

										if (FW_data.wdt[1].V_N_OUT == 1) {
											FW_data.gpio.OUT_PORT[0].old_sost =
													FW_data.gpio.OUT_PORT[0].sost;
											FW_data.gpio.OUT_PORT[0].sost = 1;
											FW_data.gpio.OUT_PORT[0].type_logic = 3;
											FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
											FW_data.gpio.OUT_PORT[0].aflag = 1;
										} else  if (FW_data.wdt[1].V_N_OUT == 11009){
											FW_data.gpio.OUT_PORT[1].old_sost =
													FW_data.gpio.OUT_PORT[1].sost;
											FW_data.gpio.OUT_PORT[1].sost = 1;
											FW_data.gpio.OUT_PORT[1].type_logic = 3;
											FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
											FW_data.gpio.OUT_PORT[1].aflag = 1;
										}
										//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
										flag_delay_ping1 = 1;
										ct_res_wdt1();
										ct_cn_a1 = 0;
										ct_cn_b1 = 0;
										ct_cn_c1 = 0;
									} else {
										if (ct_max_res1
												< FW_data.wdt[1].V_MAX_RESEND_PACET_RESET) {
											form_reple_to_save(event_main);
											//form_reple_to_save(SWICH_TOLG_WATCH);

											if (FW_data.wdt[1].V_N_OUT == 11008) {
												FW_data.gpio.OUT_PORT[0].old_sost =
														FW_data.gpio.OUT_PORT[0].sost;
												FW_data.gpio.OUT_PORT[0].sost = 1;
												FW_data.gpio.OUT_PORT[0].type_logic = 3;
												FW_data.gpio.OUT_PORT[0].event =
														WEB_OUT_PORT0_TOL;
												FW_data.gpio.OUT_PORT[0].aflag = 1;
											} else  if (FW_data.wdt[1].V_N_OUT == 11009) {
												FW_data.gpio.OUT_PORT[1].old_sost =
														FW_data.gpio.OUT_PORT[1].sost;
												FW_data.gpio.OUT_PORT[1].sost = 1;
												FW_data.gpio.OUT_PORT[1].type_logic = 3;
												FW_data.gpio.OUT_PORT[1].event =
														WEB_OUT_PORT1_TOL;
												FW_data.gpio.OUT_PORT[1].aflag = 1;
											}
											//        HAL_RTCEx_BKUPWrite(&hrtc,1,2);
											ct_max_res1++;
											flag_delay_ping1 = 1;
											ct_res_wdt1();
											ct_cn_a1 = 0;
											ct_cn_b1 = 0;
											ct_cn_c1 = 0;
										}
										//         else
										//          {
										//           en_ping_a=0;
										//           en_ping_b=0;
										//           en_ping_c=0;
										//           ping_data.flag_err[0]=0;
										//           ping_data.flag_err[1]=0;
										//           ping_data.flag_err[2]=0;
										//          }
									}
								} else {
									ct_max_res1 = 0;
								}
							}
							if (FW_data.wdt[1].V_TYPE_LOGIC == 2) {
								if (((ping_data1.flag_err[0] != 0)
										&& (ping_data1.flag_err[1] != 0))
										|| ((ping_data1.flag_err[0] != 0)
												&& (ping_data1.flag_err[2] != 0))) {

									if (FW_data.wdt[1].V_MAX_RESEND_PACET_RESET == 0) {

										form_reple_to_save(event_main);
//										form_reple_to_save(SWICH_TOLG_WATCH);

										//    flag_global_swich_out=SWICH_TOLG_WATCH;
										if (FW_data.wdt[1].V_N_OUT == 11008) {
											FW_data.gpio.OUT_PORT[0].old_sost =
													FW_data.gpio.OUT_PORT[0].sost;
											FW_data.gpio.OUT_PORT[0].sost = 1;
											FW_data.gpio.OUT_PORT[0].type_logic = 3;
											FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
											FW_data.gpio.OUT_PORT[0].aflag = 1;
										} else  if (FW_data.wdt[1].V_N_OUT == 11009) {
											FW_data.gpio.OUT_PORT[1].old_sost =
													FW_data.gpio.OUT_PORT[1].sost;
											FW_data.gpio.OUT_PORT[1].sost = 1;
											FW_data.gpio.OUT_PORT[1].type_logic = 3;
											FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
											FW_data.gpio.OUT_PORT[1].aflag = 1;
										}
										//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
										flag_delay_ping1 = 1;
										ct_res_wdt1();
										ct_cn_a1 = 0;
										ct_cn_b1 = 0;
										ct_cn_c1 = 0;
									} else {
										if (ct_max_res1
												< FW_data.wdt[1].V_MAX_RESEND_PACET_RESET) {
											form_reple_to_save(event_main);
											//form_reple_to_save(SWICH_TOLG_WATCH);
											// flag_global_swich_out=SWICH_TOLG_WATCH;
											//      HAL_RTCEx_BKUPWrite(&hrtc,1,2);
											if (FW_data.wdt[1].V_N_OUT == 11008) {
												FW_data.gpio.OUT_PORT[0].old_sost =
														FW_data.gpio.OUT_PORT[0].sost;
												FW_data.gpio.OUT_PORT[0].sost = 1;
												FW_data.gpio.OUT_PORT[0].type_logic = 3;
												FW_data.gpio.OUT_PORT[0].event =
														WEB_OUT_PORT0_TOL;
												FW_data.gpio.OUT_PORT[0].aflag = 1;
											} else  if (FW_data.wdt[1].V_N_OUT == 11009) {
												FW_data.gpio.OUT_PORT[1].old_sost =
														FW_data.gpio.OUT_PORT[1].sost;
												FW_data.gpio.OUT_PORT[1].sost = 1;
												FW_data.gpio.OUT_PORT[1].type_logic = 3;
												FW_data.gpio.OUT_PORT[1].event =
														WEB_OUT_PORT1_TOL;
												FW_data.gpio.OUT_PORT[1].aflag = 1;
											}
											ct_max_res1++;
											flag_delay_ping1 = 1;
											ct_res_wdt1();
											ct_cn_a1 = 0;
											ct_cn_b1 = 0;
											ct_cn_c1 = 0;
										}
										//          else
										//          {
										//           en_ping_a=0;
										//           en_ping_b=0;
										//           en_ping_c=0;
										//           ping_data.flag_err[0]=0;
										//           ping_data.flag_err[1]=0;
										//           ping_data.flag_err[2]=0;
										//          }
									}
								} else {
									ct_max_res1 = 0;
								}
							}
							if (FW_data.wdt[1].V_TYPE_LOGIC == 3) {
								if (((ping_data1.flag_err[0] != 0)
										&& (ping_data1.flag_err[1] != 1))
										&& ((ping_data1.flag_err[0] != 0)
												&& (ping_data1.flag_err[2] != 1))) {
									if (ct_max_res1 < FW_data.wdt[1].V_MAX_RESEND_PACET_RESET) {
										form_reple_to_save(event_main);
										//form_reple_to_save(SWICH_TOLG_WATCH);
										//  flag_global_swich_out=SWICH_TOLG_WATCH;
										if (FW_data.wdt[1].V_N_OUT == 11008) {
											FW_data.gpio.OUT_PORT[0].old_sost =
													FW_data.gpio.OUT_PORT[0].sost;
											FW_data.gpio.OUT_PORT[0].sost = 1;
											FW_data.gpio.OUT_PORT[0].type_logic = 3;
											FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
											FW_data.gpio.OUT_PORT[0].aflag = 1;
										} else  if (FW_data.wdt[1].V_N_OUT == 11009) {
											FW_data.gpio.OUT_PORT[1].old_sost =
													FW_data.gpio.OUT_PORT[1].sost;
											FW_data.gpio.OUT_PORT[1].sost = 1;
											FW_data.gpio.OUT_PORT[1].type_logic = 3;
											FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
											FW_data.gpio.OUT_PORT[1].aflag = 1;
										}
										//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
										ct_max_res1++;
										flag_delay_ping1 = 1;
										ct_res_wdt1();
										ct_cn_a1 = 0;
										ct_cn_b1 = 0;
										ct_cn_c1 = 0;
									}
									if (FW_data.wdt[1].V_MAX_RESEND_PACET_RESET == 0) {
										form_reple_to_save(event_main);
										//form_reple_to_save(SWICH_TOLG_WATCH);
										//  flag_global_swich_out=SWICH_TOLG_WATCH;
										if (FW_data.wdt[1].V_N_OUT == 11008) {
											FW_data.gpio.OUT_PORT[0].old_sost =
													FW_data.gpio.OUT_PORT[0].sost;
											FW_data.gpio.OUT_PORT[0].sost = 1;
											FW_data.gpio.OUT_PORT[0].type_logic = 3;
											FW_data.gpio.OUT_PORT[0].event = WEB_OUT_PORT0_TOL;
											FW_data.gpio.OUT_PORT[0].aflag = 1;
										} else  if (FW_data.wdt[1].V_N_OUT == 11009){
											FW_data.gpio.OUT_PORT[1].old_sost =
													FW_data.gpio.OUT_PORT[1].sost;
											FW_data.gpio.OUT_PORT[1].sost = 1;
											FW_data.gpio.OUT_PORT[1].type_logic = 3;
											FW_data.gpio.OUT_PORT[1].event = WEB_OUT_PORT1_TOL;
											FW_data.gpio.OUT_PORT[1].aflag = 1;
										}
										//       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
										flag_delay_ping1 = 1;
										ct_res_wdt1();
										ct_cn_a1 = 0;
										ct_cn_b1 = 0;
										ct_cn_c1 = 0;
									}
								} else {
									ct_max_res1 = 0;
								}
							}
			}



		vTaskDelay(300 / portTICK_PERIOD_MS);

	}
}
