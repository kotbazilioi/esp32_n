//#include "main.h"
#include <string.h>
#include "LOGS.h"
#include "nvs_task.h";

//#include "flash_if.h"
//#include "syslog.h"
log_reple_t reple_to_save;

// RTC_DateTypeDef dates;
// RTC_TimeTypeDef times;
struct tm timeinfo;
SemaphoreHandle_t flag_global_save_log = NULL;

void swich_mess_event (uint8_t event,char* mess)
{
 switch(event) {
          case NO_RUN:
            {

            }
          break;

          case RESETL:
            {
              sprintf(mess,"Произведен принудительный сброс устройства\n");
            }
          break;
          case UPDATE_FW:
            {
              sprintf(mess,"Произведен переход в режим загрузки \n");
            }
          break;
          case SWICH_ON_WEB:
            {
              sprintf(mess,"Включена нагрузка через веб интерфейс \n");
            }
          break;
          case SWICH_OFF_WEB:
            {
              sprintf(mess,"Выключена нагрузка через веб интерфейс \n");
            }
          break;
          case SWICH_TOLG_WEB:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки через веб интерфейс \n");
            }
          break;
          case SWICH_ON_WEB_N:
            {
              sprintf(mess,"Включена нагрузка через веб интерфейс (неэффективна)\n");
            }
          break;
          case SWICH_OFF_WEB_N:
            {
              sprintf(mess,"Выключена нагрузка через веб интерфейс (неэффективна)\n");
            }
          break;
          case SWICH_TOLG_WEB_N:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки через веб интерфейс (неэффективна)\n");
            }
          break;
           case SWICH_ON_SNMP:
            {
              sprintf(mess,"Включена нагрузка по SNMP интерфейсу \n");
            }
          break;
          case SWICH_OFF_SNMP:
            {
              sprintf(mess,"Выключена нагрузка по SNMP интерфейсу \n");
            }
          break;
          case SWICH_TOLG_SNMP:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки по SNMP интерфейсу \n");
            }
          break;
           case SWICH_ON_SNMP_N:
            {
              sprintf(mess,"Включена нагрузка по SNMP интерфейсу (неэффективна)\n");
            }
          break;
          case SWICH_OFF_SNMP_N:
            {
              sprintf(mess,"Выключена нагрузка по SNMP интерфейсу (неэффективна)\n");
            }
          break;
          case SWICH_TOLG_SNMP_N:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки по SNMP интерфейсу (неэффективна)\n");
            }
          break;

          case SWICH_ON_RASP:
            {
              sprintf(mess,"Включена нагрузка по расписанию \n");
            }
          break;
          case SWICH_OFF_RASP:
            {
              sprintf(mess,"Выключена нагрузка по расписанию \n");
            }
          break;
          case SWICH_TOLG_RASP:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузка по расписанию \n");
            }
          break;
           case SWICH_ON_RASP_N:
            {
              sprintf(mess,"Включена нагрузка по расписанию (неэффективна)\n");
            }
          break;
          case SWICH_OFF_RASP_N:
            {
              sprintf(mess,"Выключена нагрузка по расписанию (неэффективна)\n");
            }
          break;
          case SWICH_TOLG_RASP_N:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузка по расписанию (неэффективна)\n");
            }
          break;
          case SWICH_ON_WATCH:
            {
              sprintf(mess,"Включена нагрузка по сторожу \n");
            }
          break;
          case SWICH_OFF_WATCH:
            {
              sprintf(mess,"Выключена нагрузка по сторожу \n");
            }
          break;
          case SWICH_TOLG_WATCH:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки по сторожу \n");
            }
          break;
          case SWICH_ON_HTTP:
            {
              sprintf(mess,"Включена нагрузка по HTTP API \n");
            }
          break;
          case SWICH_OFF_HTTP:
            {
              sprintf(mess,"Выключена нагрузка по HTTP API \n");
            }
          break;
          case SWICH_TOLG_HTTP:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки по HTTP API \n");
            }
          break;
           case SWICH_ON_HTTP_N:
            {
              sprintf(mess,"Включена нагрузка по HTTP API (неэффективна)\n");
            }
          break;
          case SWICH_OFF_HTTP_N:
            {
              sprintf(mess,"Выключена нагрузка по HTTP API (неэффективна)\n");
            }
          break;
          case SWICH_TOLG_HTTP_N:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки по HTTP API (неэффективна)\n");
            }
          break;
          case POWER_ON:
            {
              sprintf(mess,"Включение питания устройства \n");
            }
          break;
          case LOAD_DEF_DATA:
            {
              sprintf(mess,"Загружены настройки по умолчанию \n");
            }
          break;
          case SAVE_DATA_SETT:
            {
              sprintf(mess,"Сохранение настроек устройства \n");
            }
          break;
          case SEND_EMAIL:
            {
              sprintf(mess,"Отправлено E-mail сообщение\n");
            }
          break;
        }
}


void swich_mess_event_en (uint8_t event,char* mess)
{
 switch(event) {
          case NO_RUN:
            {

            }
          break;

          case RESETL:
            {
              sprintf(mess,"A forced device reset has been performed  \n\r");
            }
          break;
          case UPDATE_FW:
            {
              sprintf(mess,"Transferred to download mode  \n\r");
            }
          break;
          case SWICH_ON_WEB_N:
            {
              sprintf(mess,"Load from the web interface is enabled  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_OFF_WEB_N:
            {
              sprintf(mess,"The load from the web interface is turned off  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_TOLG_WEB_N:
            {
              sprintf(mess,"Impulse load dump was performed from the web interface  'uneffecrtive'\n\r");
            }
          break;
           case SWICH_ON_SNMP_N:
            {
              sprintf(mess,"Load from SNMP interface is enabled  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_OFF_SNMP_N:
            {
              sprintf(mess,"Disabled load from SNMP interface  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_TOLG_SNMP_N:
            {
              sprintf(mess,"Pulse load shedding from SNMP interface has been performed  'uneffecrtive'\n\r");
            }
          break;
           case SWICH_ON_WEB:
            {
              sprintf(mess,"Load from the web interface is enabled  \n\r");
            }
          break;
          case SWICH_OFF_WEB:
            {
              sprintf(mess,"The load from the web interface is turned off  \n\r");
            }
          break;
          case SWICH_TOLG_WEB:
            {
              sprintf(mess,"Impulse load dump was performed from the web interface  \n\r");
            }
          break;
           case SWICH_ON_SNMP:
            {
              sprintf(mess,"Load from SNMP interface is enabled  \n\r");
            }
          break;
          case SWICH_OFF_SNMP:
            {
              sprintf(mess,"Disabled load from SNMP interface  \n\r");
            }
          break;
          case SWICH_TOLG_SNMP:
            {
              sprintf(mess,"Pulse load shedding from SNMP interface has been performed  \n\r");
            }
          break;
          case SWICH_ON_RASP:
            {
              sprintf(mess,"Scheduled load is enabled  \n\r");
            }
          break;
          case SWICH_OFF_RASP:
            {
              sprintf(mess,"Disabled load on schedule  \n\r");
            }
          break;
          case SWICH_TOLG_RASP:
            {
              sprintf(mess,"Impulse reset of the load according to the schedule \n\r");
            }
          break;
           case SWICH_ON_RASP_N:
            {
              sprintf(mess,"Scheduled load is enabled  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_OFF_RASP_N:
            {
              sprintf(mess,"Disabled load on schedule  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_TOLG_RASP_N:
            {
              sprintf(mess,"Impulse reset of the load according to the schedule 'uneffecrtive'\n\r");
            }
          break;
          case SWICH_ON_WATCH:
            {
              sprintf(mess,"Watchdog load included  \n\r");
            }
          break;
          case SWICH_OFF_WATCH:
            {
              sprintf(mess,"The watchdog load is turned off  \n\r");
            }
          break;
          case SWICH_TOLG_WATCH:
            {
              sprintf(mess,"Impulse load shedding by the watchman  \n\r");
            }
          break;
          case SWICH_ON_HTTP:
            {
              sprintf(mess,"HTTP API loading enabled  \n\r");
            }
          break;
          case SWICH_OFF_HTTP:
            {
              sprintf(mess,"Disabled loading via HTTP API  \n\r");
            }
          break;
          case SWICH_TOLG_HTTP:
            {
              sprintf(mess,"Pulse load shedding via HTTP API has been performed  \n\r");
            }
          break;
           case SWICH_ON_HTTP_N:
            {
              sprintf(mess,"HTTP API loading enabled  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_OFF_HTTP_N:
            {
              sprintf(mess,"Disabled loading via HTTP API  'uneffecrtive'\n\r");
            }
          break;
          case SWICH_TOLG_HTTP_N:
            {
              sprintf(mess,"Pulse load shedding via HTTP API has been performed  'uneffecrtive'\n\r");
            }
          break;
          case POWER_ON:
            {
              sprintf(mess,"Power on the device  \n\r");
            }
          break;
          case LOAD_DEF_DATA:
            {
              sprintf(mess,"Loaded default settings  \n\r");
            }
          break;
          case SAVE_DATA_SETT:
            {
              sprintf(mess,"Saving device settings  \n\r");
            }
          break;
        }
}

void logs_read (void)
  {
//     memcpy((void *)(&FW_data.V_logs_struct.log_reple[0]), (char *)A_LOG, 2000);

  }
void save_reple_log (log_reple_t reple2)
  {
//    log_reple_t reple_temp;
//    uint8_t ct_repl;
// //   logs_read();
//    reple2.dicr= 0x7a;
//    if (FW_data.V_logs_struct.log_reple[199].dicr==0x7a)
//      {
//         memcpy((uint8_t *)(&FW_data.V_logs_struct.log_reple[0]), (uint8_t *)(&FW_data.V_logs_struct.log_reple[1]), 1990);
//         memcpy((uint8_t *)(&FW_data.V_logs_struct.log_reple[199]), (uint8_t *)(&reple2), 10);
//         FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
//
//      }
//    else
//      {
//         for (ct_repl=0;ct_repl<200;ct_repl++)
//          {
//            memcpy((uint8_t *)(&reple_temp),(uint8_t *)(&FW_data.V_logs_struct.log_reple[ct_repl]), 10);
//            if (reple_temp.dicr!= 0x7a)
//              {
//                memcpy((uint8_t *)(&FW_data.V_logs_struct.log_reple[ct_repl]),(uint8_t *)(&reple2), 10);
//                FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
//                ct_repl=200;
//              }
//          }
//      }
//    save_data_blok(1,(uint32_t*)&FW_data.V_logs_struct.CRC16);
//     logs_read();
  }

void GET_reple (uint8_t event,log_reple_t* reple)
{
	time_t now;
//  HAL_RTC_GetDate (&hrtc,&dates,RTC_FORMAT_BIN);
//  HAL_RTC_GetTime (&hrtc,&times,RTC_FORMAT_BIN);
	 time(&now);
	 localtime_r(&now, &timeinfo);
  reple->type_event = event;
  reple->reple_hours =timeinfo.tm_hour;
  reple->reple_minuts = timeinfo.tm_min;
  reple->reple_seconds =timeinfo.tm_sec;
  reple->dweek =timeinfo.tm_wday;
  reple->day =timeinfo.tm_mday;
  reple->month = timeinfo.tm_mon;
  reple->year = 2000+timeinfo.tm_year-100;
  reple->dicr = 0x7a;  
  if (reple->year==2000)
    {
    reple->year = 2001;
    }
}
void decode_reple (char* out,log_reple_t* reple)
{
  char out_small[200]={0};
//  memset()
  sprintf(out_small,"%02d.%02d.%d  %02d:%02d:%02d    ",reple->day,reple->month,reple->year,reple->reple_hours,reple->reple_minuts,reple->reple_seconds);    
  strcat(out,out_small);
   memset(out_small,0,200);
  swich_mess_event(  reple->type_event,out_small);
  strcat(out,out_small);
}
void decode_reple_en (char* out,log_reple_t* reple)
{
  char out_small[200]={0};
//  memset()
  sprintf(out_small,"%02d.%02d.%d  %02d:%02d:%02d    ",reple->day,reple->month,reple->year,reple->reple_hours,reple->reple_minuts,reple->reple_seconds);    
  strcat(out,out_small);
   memset(out_small,0,200);
  swich_mess_event_en(  reple->type_event,out_small);
  strcat(out,out_small);
}


void form_reple_to_save (uint8_t event)
{
  char mess_syslog[200]={0};

  GET_reple (event,&reple_to_save);
  decode_reple_en(mess_syslog, &reple_to_save);  
  //syslog_printf(mess_syslog);
///////  xSemaphoreTake (flag_global_save_log, (TickType_t) 100);
}


void form_reple_to_smtp (uint8_t event)
{
  char mess_syslog[200]={0};

  GET_reple (event,&reple_to_save);
  decode_reple_en(mess_syslog, &reple_to_save);  

 ////////////// xSemaphoreTake (flag_global_save_log, (TickType_t) 100);
}
