#include <stdint.h>
//#include "flash_if.h"
#include "nvs_task.h"
#define max_log_mess 50
void save_reple_log (log_reple_t reple2);
uint8_t logs_read (uint16_t n_mess,char* mess);
void GET_reple (log_reple_t* reple);
void form_reple_to_save (uint8_t event);
void decode_reple (char* out,log_reple_t* reple);
void form_reple_to_smtp (uint8_t event);
void log_task(void *pvParameters);
void swich_mess_event(uint8_t event, char *mess);
extern log_reple_t reple_to_save;
extern log_reple_t reple_to_email;
//extern RTC_DateTypeDef dates;
//extern RTC_TimeTypeDef times;
extern struct tm timeinfo;
extern SemaphoreHandle_t flag_global_save_log;

