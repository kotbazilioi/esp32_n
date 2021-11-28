#include <stdint.h>
//#include "flash_if.h"
#include "nvs_task.h"
#define max_log_mess 50
void save_reple_log (log_reple_t reple2);
uint8_t logs_read (uint16_t n_mess,char* mess);
void GET_reple (log_reple_t* reple);
void form_reple_to_save (event_struct_t cfg);
void decode_reple (char* out,log_reple_t* reple);
void form_reple_to_smtp (event_struct_t cfg);
void log_task(void *pvParameters);
void swich_mess_event(log_reple_t* reply_sw, char *mess);
void swich_mess_event_en(log_reple_t* reply_sw, char *mess);
extern log_reple_t reple_to_save;
extern log_reple_t reple_to_email;
//extern RTC_DateTypeDef dates;
//extern RTC_TimeTypeDef times;
extern struct tm timeinfo;
extern SemaphoreHandle_t flag_global_save_log;
extern uint32_t timeup;
extern time_t now;

