/*
 * nvs_task.c
 *
 *  Created on: 16 сент. 2021 г.
 *      Author: ivanov
 */



#include <string.h>   //ƒл€ memcpy
#include "esp_flash.h"
#include "esp_flash_spi_init.h"
#include "nvs.h"
#include "LOGS.h"
#include "nvs_task.h"
#define get_name(x) #x
FW_data_t FW_data;
nvs_handle_t nvs_data_handle;
nvs_flags_t nvs_flags;

esp_err_t save_data_blok(void)
{

	 esp_err_t  err =nvs_open_from_partition("nvs","storage", NVS_READWRITE, &nvs_data_handle);



	err = nvs_set_blob(nvs_data_handle,get_name(V_IP_CONFIG),FW_data.net.V_IP_CONFIG,4);

	err=nvs_set_u8(nvs_data_handle,get_name(LOAD_DEF_FLAG),FW_data.sys.LOAD_DEF_FLAG);

	err =err|nvs_set_blob(nvs_data_handle,get_name(V_IP_MASK),FW_data.net.V_IP_MASK,4);


	err=err|nvs_set_blob(nvs_data_handle,get_name(V_IP_GET),FW_data.net.V_IP_GET,4);

	err=err|nvs_set_u8(nvs_data_handle,get_name(V_DHCP),FW_data.net.V_DHCP);
	err=err|nvs_set_blob(nvs_data_handle,get_name(V_IP_DNS),FW_data.net.V_IP_DNS,4);

	err=err|nvs_set_blob(nvs_data_handle,get_name(V_FW1_VER),FW_data.sys.V_FW1_VER,4);

	err=err|nvs_set_u32(nvs_data_handle,get_name(V_FW1_LEN),FW_data.sys.V_FW1_LEN);

	err=err|nvs_set_u32(nvs_data_handle,get_name(V_BOOT_VER),FW_data.sys.V_BOOT_VER);

	err=err|nvs_set_blob(nvs_data_handle,get_name(V_LOGIN),FW_data.http.V_LOGIN,16);

	err=err|nvs_set_blob(nvs_data_handle,get_name(V_PASSWORD),FW_data.http.V_PASSWORD,16);
	err=err|nvs_set_blob(nvs_data_handle,get_name(V_COMM),FW_data.snmp.V_COMMUNITY,16);
	err=err|nvs_set_blob(nvs_data_handle,get_name(V_COMMW),FW_data.snmp.V_COMMUNITY_WRITE,16);


	err=err|nvs_set_u16(nvs_data_handle,get_name(V_WEB_PORT),FW_data.http.V_WEB_PORT);

	err=err|nvs_set_u16(nvs_data_handle,get_name(V_PORT_SNMP),FW_data.snmp.V_PORT_SNMP);

	err=err|nvs_set_u16(nvs_data_handle,get_name(V_HTTP_IP),FW_data.net.V_HTTP_IP);

	err=err|nvs_set_blob(nvs_data_handle,get_name(V_ON_MESS),FW_data.http.V_ON_MESS,32);

	err=err|nvs_set_blob(nvs_data_handle,get_name(V_OFF_MESS),FW_data.http.V_OFF_MESS,32);

	err=err|nvs_set_u8(nvs_data_handle,get_name(V_FLAG_EN_MAN),FW_data.http.V_FLAG_EN_MAN);

    err=err|nvs_set_u8(nvs_data_handle,get_name(V_FLAG_EN_RASP),FW_data.rasp.V_FLAG_EN_RASP);

    err=err|nvs_set_u8(nvs_data_handle,get_name(V_FLAG_EN_WEB),FW_data.http.V_FLAG_EN_WEB);

    err=err|nvs_set_u8(nvs_data_handle,get_name(EN_WATCHMAN),FW_data.wdt[0].V_FLAG_EN_WATCHMAN);

    err=err|nvs_set_u8(nvs_data_handle,get_name(EN_WATCHMAN1),FW_data.wdt[1].V_FLAG_EN_WATCHMAN);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_set_u32(nvs_data_handle,get_name(V_IP_PING_TIME),FW_data.rasp.V_IP_PING_TIME);

    err=err|nvs_set_u32(nvs_data_handle,get_name(V_TIME_SEND),FW_data.rasp.V_TIME_SEND);

    err=err|nvs_set_u32(nvs_data_handle,get_name(V_TIME_READ),FW_data.rasp.V_TIME_READ);

    err=err|nvs_set_u32(nvs_data_handle,get_name(V_N_RERID),FW_data.rasp.V_N_RERID);

    err=err|nvs_set_u32(nvs_data_handle,get_name(V_TIME_RESET),FW_data.rasp.V_TIME_RESET);

    err=err|nvs_set_u32(nvs_data_handle,get_name(V_DELAY_PING),FW_data.rasp.V_DELAY_PING);

    err=err|nvs_set_u32(nvs_data_handle,get_name(V_SOST_RESET),FW_data.rasp.V_SOST_RESET);

    err=err|nvs_set_u16(nvs_data_handle,get_name(V_N_PING),FW_data.wdt[0].V_N_PING);

    err=err|nvs_set_u16(nvs_data_handle,get_name(V_N_PING1),FW_data.wdt[1].V_N_PING);


    err=err|nvs_set_blob(nvs_data_handle,get_name(V_IP_NTP1),FW_data.net.V_IP_NTP1,4);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_IP_NTP2),FW_data.net.V_IP_NTP2,4);

    err=err|nvs_set_u8(nvs_data_handle,get_name(V_PORT_NTP),FW_data.net.V_PORT_NTP);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_NAME_SMTP),FW_data.smtp.V_NAME_SMTP,32);

    err=err|nvs_set_u16(nvs_data_handle,get_name(V_PORT_SNMP),FW_data.snmp.V_PORT_SNMP);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_LOGIN_SMTP),FW_data.smtp.V_LOGIN_SMTP,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_PASSWORD_SMTP),FW_data.smtp.V_PASSWORD_SMTP,32);

    err=err|nvs_set_u16(nvs_data_handle,get_name(EMAIL_PORT),FW_data.smtp.V_FLAG_EMAIL_PORT);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_EMAIL_ADDR),FW_data.smtp.V_EMAIL_ADDR,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_EMAIL_FROM),FW_data.smtp.V_EMAIL_FROM,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_EMAIL_TO),FW_data.smtp.V_EMAIL_TO,32);

    err=err|nvs_set_u8(nvs_data_handle,get_name(DEF_EMAIL),FW_data.smtp.V_FLAG_DEF_EMAIL);

    err=err|nvs_set_u8(nvs_data_handle,get_name(EN_EMAIL),FW_data.smtp.V_FLAG_EN_EMAIL);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_GEOM_NAME),FW_data.sys.V_GEOM_NAME,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_ID_MAC), FW_data.sys.V_ID_MAC,8);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_Name_dev), FW_data.sys.V_Name_dev,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_CALL_DATA),FW_data.sys.V_CALL_DATA,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_Name_dev),FW_data.sys.V_Name_dev,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_CALL_DATA),FW_data.sys.V_CALL_DATA,16);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_IP_SYSL),FW_data.net.V_IP_SYSL,4);

    err=err|nvs_set_blob(nvs_data_handle,get_name(V_IP_SNMP),FW_data.snmp.V_IP_SNMP,4);

    err=err|nvs_set_u8(nvs_data_handle,get_name(V_TYPE_OUT),FW_data.sys.V_TYPE_OUT);


    err=err|nvs_set_i8(nvs_data_handle,get_name(V_NTP_CIRCL),FW_data.sys.V_NTP_CIRCL);




    err=err|nvs_set_u8(nvs_data_handle,get_name(V_TYPE_LOGIC),FW_data.wdt[0].V_TYPE_LOGIC);

    err=err|nvs_set_u8(nvs_data_handle,get_name(V_EN_WATCHDOG), FW_data.wdt[0].V_EN_WATCHDOG);

      err=err|nvs_set_u8(nvs_data_handle,get_name(WATCHDOG_CN_A),FW_data.wdt[0].V_EN_WATCHDOG_CN_A);

      err=err|nvs_set_u8(nvs_data_handle,get_name(WATCHDOG_CN_B),FW_data.wdt[0].V_EN_WATCHDOG_CN_B);

      err=err|nvs_set_u8(nvs_data_handle,get_name(WATCHDOG_CN_C),FW_data.wdt[0].V_EN_WATCHDOG_CN_C);

      err=err|nvs_set_blob(nvs_data_handle,get_name(WDT_ADDR_CN_A),FW_data.wdt[0].V_IP_WDT_ADDR_CN_A,4);

      err=err|nvs_set_blob(nvs_data_handle,get_name(WDT_ADDR_CN_B),FW_data.wdt[0].V_IP_WDT_ADDR_CN_B,4);

      err=err|nvs_set_blob(nvs_data_handle,get_name(WDT_ADDR_CN_C),FW_data.wdt[0].V_IP_WDT_ADDR_CN_C,4);

      err=err|nvs_set_u16(nvs_data_handle,get_name(CT_RES_ALLST),FW_data.wdt[0].V_CT_RES_ALLSTART);

      err=err|nvs_set_u16(nvs_data_handle,get_name(V_T_SEND_P),FW_data.wdt[0].V_T_SEND_PING);

      err=err|nvs_set_u16(nvs_data_handle,get_name(V_TIME_RSD_P),FW_data.wdt[0].V_TIME_RESEND_PING);

      err=err|nvs_set_u16(nvs_data_handle,get_name(V_MAX_REPID_P),FW_data.wdt[0].V_MAX_REPID_PING);

      err=err|nvs_set_u16(nvs_data_handle,get_name(V_TIME_R_PULSE),FW_data.wdt[0].V_TIME_RESET_PULSE);

      err=err|nvs_set_u16(nvs_data_handle,get_name(V_PAUSE_R_TO_R),FW_data.wdt[0].V_PAUSE_RESET_TO_REPID);

      err=err|nvs_set_u16(nvs_data_handle,get_name(V_MAX_R_PACET_R),FW_data.wdt[0].V_MAX_RESEND_PACET_RESET);



      err=err|nvs_set_u8(nvs_data_handle,get_name(V_TYPE_LOGIC1),FW_data.wdt[1].V_TYPE_LOGIC);

      err=err|nvs_set_u8(nvs_data_handle,get_name(V_EN_WATCHDOG1), FW_data.wdt[1].V_EN_WATCHDOG);

        err=err|nvs_set_u8(nvs_data_handle,get_name(WATCHDOG_CN_A1),FW_data.wdt[1].V_EN_WATCHDOG_CN_A);

        err=err|nvs_set_u8(nvs_data_handle,get_name(WATCHDOG_CN_B1),FW_data.wdt[1].V_EN_WATCHDOG_CN_B);

        err=err|nvs_set_u8(nvs_data_handle,get_name(WATCHDOG_CN_C1),FW_data.wdt[1].V_EN_WATCHDOG_CN_C);

        err=err|nvs_set_blob(nvs_data_handle,get_name(WDT_ADDR_CN_A1),FW_data.wdt[1].V_IP_WDT_ADDR_CN_A,4);

        err=err|nvs_set_blob(nvs_data_handle,get_name(WDT_ADDR_CN_B1),FW_data.wdt[1].V_IP_WDT_ADDR_CN_B,4);

        err=err|nvs_set_blob(nvs_data_handle,get_name(WDT_ADDR_CN_C1),FW_data.wdt[1].V_IP_WDT_ADDR_CN_C,4);

        err=err|nvs_set_u16(nvs_data_handle,get_name(CT_RES_ALLST1),FW_data.wdt[1].V_CT_RES_ALLSTART);

        err=err|nvs_set_u16(nvs_data_handle,get_name(V_T_SEND_P1),FW_data.wdt[1].V_T_SEND_PING);

        err=err|nvs_set_u16(nvs_data_handle,get_name(V_TIME_RSD_P1),FW_data.wdt[1].V_TIME_RESEND_PING);

        err=err|nvs_set_u16(nvs_data_handle,get_name(V_MAX_REPID_P1),FW_data.wdt[1].V_MAX_REPID_PING);

        err=err|nvs_set_u16(nvs_data_handle,get_name(V_TIME_R_PULSE1),FW_data.wdt[1].V_TIME_RESET_PULSE);

        err=err|nvs_set_u16(nvs_data_handle,get_name(V_PAUSE_R_TO_R1),FW_data.wdt[1].V_PAUSE_RESET_TO_REPID);

        err=err|nvs_set_u16(nvs_data_handle,get_name(V_MAX_R_PACET_R1),FW_data.wdt[1].V_MAX_RESEND_PACET_RESET);





  	err=err|nvs_set_blob(nvs_data_handle,get_name(gpio_name_0),&(FW_data.gpio.name[0]),16);
  	err=err|nvs_set_blob(nvs_data_handle,get_name(gpio_name_1),&(FW_data.gpio.name[1]),16);
  	err=err|nvs_set_blob(nvs_data_handle,get_name(gpio_name_2),&(FW_data.gpio.name[2]),16);
  	err=err|nvs_set_blob(nvs_data_handle,get_name(gpio_name_3),&(FW_data.gpio.name[3]),16);

  	err=err|nvs_set_blob(nvs_data_handle,get_name(V_NAME),FW_data.wdt[0].V_NAME,16);

  	err=err|nvs_set_blob(nvs_data_handle,get_name(V_NAME1),FW_data.wdt[1].V_NAME,16);

  	err=err|nvs_set_u8(nvs_data_handle,get_name(gpio_dir_0), FW_data.gpio.dir[0]);
  	err=err|nvs_set_u8(nvs_data_handle,get_name(gpio_dir_1), FW_data.gpio.dir[1]);
  	err=err|nvs_set_u8(nvs_data_handle,get_name(gpio_dir_2), FW_data.gpio.dir[2]);
  	err=err|nvs_set_u8(nvs_data_handle,get_name(gpio_dir_3), FW_data.gpio.dir[3]);

  	err=err|nvs_set_u16(nvs_data_handle,get_name(gpio_in_t0),FW_data.gpio.IN_PORT[0].filtr_time);
  	err=err|nvs_set_u16(nvs_data_handle,get_name(gpio_in_t1),FW_data.gpio.IN_PORT[1].filtr_time);
  	err=err|nvs_set_u16(nvs_data_handle,get_name(gpio_out_t0),FW_data.gpio.OUT_PORT[0].input_str.filtr_time);
  	err=err|nvs_set_u16(nvs_data_handle,get_name(gpio_out_t1),FW_data.gpio.OUT_PORT[1].input_str.filtr_time);

  	err=err|nvs_set_u16(nvs_data_handle,get_name(gpio_out_d0),FW_data.gpio.OUT_PORT[0].delay);
  	err=err|nvs_set_u16(nvs_data_handle,get_name(gpio_out_d1),FW_data.gpio.OUT_PORT[1].delay);

  	err=err|nvs_set_i16(nvs_data_handle,get_name(t0_dw), FW_data.termo[0].t_dw);
  	err=err|nvs_set_i16(nvs_data_handle,get_name(t0_up), FW_data.termo[0].t_up);
  	err=err|nvs_set_u8(nvs_data_handle,get_name(t0_st), FW_data.termo[0].status);
  	err=err|nvs_set_blob(nvs_data_handle,get_name(t0_name),&(FW_data.termo[0].name[0]),16);

  	err=err|nvs_set_i16(nvs_data_handle,get_name(t1_dw), FW_data.termo[1].t_dw);
  	err=err|nvs_set_i16(nvs_data_handle,get_name(t1_up), FW_data.termo[1].t_up);
  	err=err|nvs_set_u8(nvs_data_handle,get_name(t1_st), FW_data.termo[1].status);
  	err=err|nvs_set_blob(nvs_data_handle,get_name(t1_name),&(FW_data.termo[1].name[0]),16);

  	err=err|nvs_set_u8(nvs_data_handle,get_name(V_L_TIME), FW_data.sys.V_L_TIME);
  	err=err|nvs_set_blob(nvs_data_handle,get_name(V_IP_SO),FW_data.sys.V_IP_SOURCE,4);
  	err=err|nvs_set_u8(nvs_data_handle,get_name(V_MS_S), FW_data.sys.V_MASK_SOURCE);
  	err=err|nvs_set_u8(nvs_data_handle,get_name(V_REFR_TR), FW_data.snmp.V_REFR_TRAP);
  	err=err|nvs_set_u16(nvs_data_handle,get_name(V_N_O0),FW_data.wdt[0].V_N_OUT);
  	err=err|nvs_set_u16(nvs_data_handle,get_name(V_N_O1),FW_data.wdt[1].V_N_OUT);


      printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
      printf("Committing updates in NVS ... ");
      err = nvs_commit(nvs_data_handle);
      printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
      nvs_close(nvs_data_handle);


      return err;
}
esp_err_t load_data_blok(void)
{
    uint8_t lens = 4;
	 esp_err_t  err =nvs_open_from_partition("nvs","storage", NVS_READWRITE, &nvs_data_handle);


	err=nvs_get_u8(nvs_data_handle,get_name(LOAD_DEF_FLAG),&FW_data.sys.LOAD_DEF_FLAG);

	err =err| nvs_get_blob(nvs_data_handle,get_name(V_IP_CONFIG),FW_data.net.V_IP_CONFIG,&lens);

	err =err|nvs_get_blob(nvs_data_handle,get_name(V_IP_MASK),FW_data.net.V_IP_MASK,&lens);


	err=err|nvs_get_blob(nvs_data_handle,get_name(V_IP_GET),FW_data.net.V_IP_GET,&lens);

	err=err|nvs_get_u8(nvs_data_handle,get_name(V_DHCP),&FW_data.net.V_DHCP);

	err=err|nvs_get_blob(nvs_data_handle,get_name(V_IP_DNS),FW_data.net.V_IP_DNS,&lens);

	err=err|nvs_get_blob(nvs_data_handle,get_name(V_FW1_VER),FW_data.sys.V_FW1_VER,&lens);

	err=err|nvs_get_u32(nvs_data_handle,get_name(V_FW1_LEN),&FW_data.sys.V_FW1_LEN);

	err=err|nvs_get_u32(nvs_data_handle,get_name(V_BOOT_VER),&FW_data.sys.V_BOOT_VER);

	lens =16;
	err=err|nvs_get_blob(nvs_data_handle,get_name(V_LOGIN),FW_data.http.V_LOGIN,&lens);

	err=err|nvs_get_blob(nvs_data_handle,get_name(V_PASSWORD),FW_data.http.V_PASSWORD,&lens);
	err=err|nvs_get_blob(nvs_data_handle,get_name(V_COMM),FW_data.snmp.V_COMMUNITY,&lens);
	err=err|nvs_get_blob(nvs_data_handle,get_name(V_COMMW),FW_data.snmp.V_COMMUNITY_WRITE,&lens);
	err=err|nvs_get_blob(nvs_data_handle,get_name(V_NAME),FW_data.wdt[0].V_NAME,&lens);

	err=err|nvs_get_blob(nvs_data_handle,get_name(V_NAME1),FW_data.wdt[1].V_NAME,&lens);

	err=err|nvs_get_u16(nvs_data_handle,get_name(V_WEB_PORT),&FW_data.http.V_WEB_PORT);

	err=err|nvs_get_u16(nvs_data_handle,get_name(V_PORT_SNMP),&FW_data.snmp.V_PORT_SNMP);

	err=err|nvs_get_u16(nvs_data_handle,get_name(V_HTTP_IP),&FW_data.net.V_HTTP_IP);

	lens = 32;
	err=err|nvs_get_blob(nvs_data_handle,get_name(V_ON_MESS),FW_data.http.V_ON_MESS,&lens);

	err=err|nvs_get_blob(nvs_data_handle,get_name(V_OFF_MESS),FW_data.http.V_OFF_MESS,&lens);

	err=err|nvs_get_u8(nvs_data_handle,get_name(V_FLAG_EN_MAN),&FW_data.http.V_FLAG_EN_MAN);

    err=err|nvs_get_u8(nvs_data_handle,get_name(V_FLAG_EN_RASP),&FW_data.rasp.V_FLAG_EN_RASP);

    err=err|nvs_get_u8(nvs_data_handle,get_name(V_FLAG_EN_WEB),&FW_data.http.V_FLAG_EN_WEB);

    err=err|nvs_get_u8(nvs_data_handle,get_name(EN_WATCHMAN),&FW_data.wdt[0].V_FLAG_EN_WATCHMAN);

    err=err|nvs_get_u8(nvs_data_handle,get_name(EN_WATCHMAN1),&FW_data.wdt[1].V_FLAG_EN_WATCHMAN);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,&lens);

    err=err|nvs_get_u32(nvs_data_handle,get_name(V_IP_PING_TIME),&FW_data.rasp.V_IP_PING_TIME);

    err=err|nvs_get_u32(nvs_data_handle,get_name(V_TIME_SEND),&FW_data.rasp.V_TIME_SEND);

    err=err|nvs_get_u32(nvs_data_handle,get_name(V_TIME_READ),&FW_data.rasp.V_TIME_READ);

    err=err|nvs_get_u32(nvs_data_handle,get_name(V_N_RERID),&FW_data.rasp.V_N_RERID);

    err=err|nvs_get_u32(nvs_data_handle,get_name(V_TIME_RESET),&FW_data.rasp.V_TIME_RESET);

    err=err|nvs_get_u32(nvs_data_handle,get_name(V_DELAY_PING),&FW_data.rasp.V_DELAY_PING);

    err=err|nvs_get_u32(nvs_data_handle,get_name(V_SOST_RESET),&FW_data.rasp.V_SOST_RESET);

    err=err|nvs_get_u16(nvs_data_handle,get_name(V_N_PING),&FW_data.wdt[0].V_N_PING);

    err=err|nvs_get_u16(nvs_data_handle,get_name(V_N_PING1),&FW_data.wdt[1].V_N_PING);

    lens=4;
    err=err|nvs_get_blob(nvs_data_handle,get_name(V_IP_NTP1),FW_data.net.V_IP_NTP1,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_IP_NTP2),FW_data.net.V_IP_NTP2,&lens);

    err=err|nvs_get_u8(nvs_data_handle,get_name(V_PORT_NTP),&FW_data.net.V_PORT_NTP);
    lens=32;

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_NAME_SMTP),FW_data.smtp.V_NAME_SMTP,&lens);

    err=err|nvs_get_u16(nvs_data_handle,get_name(V_PORT_SNMP),&FW_data.snmp.V_PORT_SNMP);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_LOGIN_SMTP),FW_data.smtp.V_LOGIN_SMTP,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_PASSWORD_SMTP),FW_data.smtp.V_PASSWORD_SMTP,&lens);

    err=err|nvs_get_u16(nvs_data_handle,get_name(EMAIL_PORT),&FW_data.smtp.V_FLAG_EMAIL_PORT);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_EMAIL_ADDR),FW_data.smtp.V_EMAIL_ADDR,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_EMAIL_FROM),FW_data.smtp.V_EMAIL_FROM,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_EMAIL_TO),FW_data.smtp.V_EMAIL_TO,&lens);

    err=err|nvs_get_u8(nvs_data_handle,get_name(DEF_EMAIL),&FW_data.smtp.V_FLAG_DEF_EMAIL);

    err=err|nvs_get_u8(nvs_data_handle,get_name(EN_EMAIL),&FW_data.smtp.V_FLAG_EN_EMAIL);
    lens=88;
    err=err|nvs_get_blob(nvs_data_handle,get_name(V_GEOM_NAME),FW_data.sys.V_GEOM_NAME,&lens);
    lens=8;
    err=err|nvs_get_blob(nvs_data_handle,get_name(V_ID_MAC), FW_data.sys.V_ID_MAC,&lens);
    lens=88;
    err=err|nvs_get_blob(nvs_data_handle,get_name(V_Name_dev), FW_data.sys.V_Name_dev,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_CALL_DATA),FW_data.sys.V_CALL_DATA,&lens);

//    err=err|nvs_get_blob(nvs_data_handle,get_name(V_Name_dev),FW_data.sys.V_Name_dev,&lens);
    lens=16;

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_CALL_DATA),FW_data.sys.V_CALL_DATA,&lens);
    lens=4;

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_IP_SYSL),FW_data.net.V_IP_SYSL,&lens);

    err=err|nvs_get_blob(nvs_data_handle,get_name(V_IP_SNMP),FW_data.snmp.V_IP_SNMP,&lens);

    err=err|nvs_get_u8(nvs_data_handle,get_name(V_TYPE_OUT),&FW_data.sys.V_TYPE_OUT);

    err=err|nvs_get_i8(nvs_data_handle,get_name(V_NTP_CIRCL),&FW_data.sys.V_NTP_CIRCL);





    err=err|nvs_get_u8(nvs_data_handle,get_name(V_TYPE_LOGIC),&FW_data.wdt[0].V_TYPE_LOGIC);

    err=err|nvs_get_u8(nvs_data_handle,get_name(V_EN_WATCHDOG),&FW_data.wdt[0].V_EN_WATCHDOG);

      err=err|nvs_get_u8(nvs_data_handle,get_name(WATCHDOG_CN_A),&FW_data.wdt[0].V_EN_WATCHDOG_CN_A);

      err=err|nvs_get_u8(nvs_data_handle,get_name(WATCHDOG_CN_B),&FW_data.wdt[0].V_EN_WATCHDOG_CN_B);

      err=err|nvs_get_u8(nvs_data_handle,get_name(WATCHDOG_CN_C),&FW_data.wdt[0].V_EN_WATCHDOG_CN_C);

      err=err|nvs_get_blob(nvs_data_handle,get_name(WDT_ADDR_CN_A),FW_data.wdt[0].V_IP_WDT_ADDR_CN_A,&lens);

      err=err|nvs_get_blob(nvs_data_handle,get_name(WDT_ADDR_CN_B),FW_data.wdt[0].V_IP_WDT_ADDR_CN_B,&lens);

      err=err|nvs_get_blob(nvs_data_handle,get_name(WDT_ADDR_CN_C),FW_data.wdt[0].V_IP_WDT_ADDR_CN_C,&lens);

      err=err|nvs_get_u16(nvs_data_handle,get_name(CT_RES_ALLST),&FW_data.wdt[0].V_CT_RES_ALLSTART);

      err=err|nvs_get_u16(nvs_data_handle,get_name(V_T_SEND_P),&FW_data.wdt[0].V_T_SEND_PING);

      err=err|nvs_get_u16(nvs_data_handle,get_name(V_TIME_RSD_P),&FW_data.wdt[0].V_TIME_RESEND_PING);

      err=err|nvs_get_u16(nvs_data_handle,get_name(V_MAX_REPID_P),&FW_data.wdt[0].V_MAX_REPID_PING);

      err=err|nvs_get_u16(nvs_data_handle,get_name(V_TIME_R_PULSE),&FW_data.wdt[0].V_TIME_RESET_PULSE);

      err=err|nvs_get_u16(nvs_data_handle,get_name(V_PAUSE_R_TO_R),&FW_data.wdt[0].V_PAUSE_RESET_TO_REPID);

      err=err|nvs_get_u16(nvs_data_handle,get_name(V_MAX_R_PACET_R),&FW_data.wdt[0].V_MAX_RESEND_PACET_RESET);



      err=err|nvs_get_u8(nvs_data_handle,get_name(V_TYPE_LOGIC1),&FW_data.wdt[1].V_TYPE_LOGIC);

      err=err|nvs_get_u8(nvs_data_handle,get_name(V_EN_WATCHDOG1),&FW_data.wdt[1].V_EN_WATCHDOG);

        err=err|nvs_get_u8(nvs_data_handle,get_name(WATCHDOG_CN_A1),&FW_data.wdt[1].V_EN_WATCHDOG_CN_A);

        err=err|nvs_get_u8(nvs_data_handle,get_name(WATCHDOG_CN_B1),&FW_data.wdt[1].V_EN_WATCHDOG_CN_B);

        err=err|nvs_get_u8(nvs_data_handle,get_name(WATCHDOG_CN_C1),&FW_data.wdt[1].V_EN_WATCHDOG_CN_C);

        err=err|nvs_get_blob(nvs_data_handle,get_name(WDT_ADDR_CN_A1),FW_data.wdt[1].V_IP_WDT_ADDR_CN_A,&lens);

        err=err|nvs_get_blob(nvs_data_handle,get_name(WDT_ADDR_CN_B1),FW_data.wdt[1].V_IP_WDT_ADDR_CN_B,&lens);

        err=err|nvs_get_blob(nvs_data_handle,get_name(WDT_ADDR_CN_C1),FW_data.wdt[1].V_IP_WDT_ADDR_CN_C,&lens);

        err=err|nvs_get_u16(nvs_data_handle,get_name(CT_RES_ALLST1),&FW_data.wdt[1].V_CT_RES_ALLSTART);

        err=err|nvs_get_u16(nvs_data_handle,get_name(V_T_SEND_P1),&FW_data.wdt[1].V_T_SEND_PING);

        err=err|nvs_get_u16(nvs_data_handle,get_name(V_TIME_RSD_P1),&FW_data.wdt[1].V_TIME_RESEND_PING);

        err=err|nvs_get_u16(nvs_data_handle,get_name(V_MAX_REPID_P1),&FW_data.wdt[1].V_MAX_REPID_PING);

        err=err|nvs_get_u16(nvs_data_handle,get_name(V_TIME_R_PULSE1),&FW_data.wdt[1].V_TIME_RESET_PULSE);

        err=err|nvs_get_u16(nvs_data_handle,get_name(V_PAUSE_R_TO_R1),&FW_data.wdt[1].V_PAUSE_RESET_TO_REPID);

        err=err|nvs_get_u16(nvs_data_handle,get_name(V_MAX_R_PACET_R1),&FW_data.wdt[1].V_MAX_RESEND_PACET_RESET);






      lens=16;
    	err=err|nvs_get_blob(nvs_data_handle,get_name(gpio_name_0),&(FW_data.gpio.name[0]),&lens);
    	err=err|nvs_get_blob(nvs_data_handle,get_name(gpio_name_1),&(FW_data.gpio.name[1]),&lens);
    	err=err|nvs_get_blob(nvs_data_handle,get_name(gpio_name_2),&(FW_data.gpio.name[2]),&lens);
    	err=err|nvs_get_blob(nvs_data_handle,get_name(gpio_name_3),&(FW_data.gpio.name[3]),&lens);

    	err=err|nvs_get_u8(nvs_data_handle,get_name(gpio_dir_0), &(FW_data.gpio.dir[0]));
    	err=err|nvs_get_u8(nvs_data_handle,get_name(gpio_dir_1), &(FW_data.gpio.dir[1]));
    	err=err|nvs_get_u8(nvs_data_handle,get_name(gpio_dir_2), &(FW_data.gpio.dir[2]));
    	err=err|nvs_get_u8(nvs_data_handle,get_name(gpio_dir_3), &(FW_data.gpio.dir[3]));

    	err=err|nvs_get_u16(nvs_data_handle,get_name(gpio_in_t0),&FW_data.gpio.IN_PORT[0].filtr_time);
    	err=err|nvs_get_u16(nvs_data_handle,get_name(gpio_in_t1),&FW_data.gpio.IN_PORT[1].filtr_time);
    	err=err|nvs_get_u16(nvs_data_handle,get_name(gpio_out_t0),&FW_data.gpio.OUT_PORT[0].input_str.filtr_time);
    	err=err|nvs_get_u16(nvs_data_handle,get_name(gpio_out_t1),&FW_data.gpio.OUT_PORT[1].input_str.filtr_time);

    	err=err|nvs_get_u16(nvs_data_handle,get_name(gpio_out_d0),&FW_data.gpio.OUT_PORT[0].delay);
    	err=err|nvs_get_u16(nvs_data_handle,get_name(gpio_out_d1),&FW_data.gpio.OUT_PORT[1].delay);


    	  lens=16;

      	err=err|nvs_get_i16(nvs_data_handle,get_name(t0_dw), &FW_data.termo[0].t_dw);
      	err=err|nvs_get_i16(nvs_data_handle,get_name(t0_up), &FW_data.termo[0].t_up);
      	err=err|nvs_get_u8(nvs_data_handle,get_name(t0_st), &FW_data.termo[0].status);
      	err=err|nvs_get_blob(nvs_data_handle,get_name(t0_name),&(FW_data.termo[0].name[0]),&lens);

      	err=err|nvs_get_i16(nvs_data_handle,get_name(t1_dw), &FW_data.termo[1].t_dw);
      	err=err|nvs_get_i16(nvs_data_handle,get_name(t1_up), &FW_data.termo[1].t_up);
      	err=err|nvs_get_u8(nvs_data_handle,get_name(t1_st), &FW_data.termo[1].status);
      	err=err|nvs_get_blob(nvs_data_handle,get_name(t1_name),&(FW_data.termo[1].name[0]),&lens);


      	err=err|nvs_get_u8(nvs_data_handle,get_name(V_L_TIME), &FW_data.sys.V_L_TIME);
        lens=4;

      	  	err=err|nvs_get_blob(nvs_data_handle,get_name(V_IP_SO),FW_data.sys.V_IP_SOURCE,&lens);
      	  	err=err|nvs_get_u8(nvs_data_handle,get_name(V_MS_S), &FW_data.sys.V_MASK_SOURCE);
      	  	err=err|nvs_get_u8(nvs_data_handle,get_name(V_REFR_TR), &FW_data.snmp.V_REFR_TRAP);
      	  	err=err|nvs_get_u16(nvs_data_handle,get_name(V_N_O0),&FW_data.wdt[0].V_N_OUT);
      	  	err=err|nvs_get_u16(nvs_data_handle,get_name(V_N_O1),&FW_data.wdt[1].V_N_OUT);


//
//      	  uint8_t ALL_EVENT;
//      	  	uint8_t RISE_L[out_port_n+in_port_n];
//      	  	uint8_t RISE_SL[out_port_n+in_port_n];
//      	  	uint8_t RISE_E[out_port_n+in_port_n];
//      	  	uint8_t RISE_SM[out_port_n+in_port_n];
//      	  	uint8_t RISE_SN[out_port_n+in_port_n];
//
//      	  	uint8_t FALL_L[out_port_n+in_port_n];
//      	  	uint8_t FALL_SL[out_port_n+in_port_n];
//      	  	uint8_t FALL_E[out_port_n+in_port_n];
//      	  	uint8_t FALL_SM[out_port_n+in_port_n];
//      	      uint8_t FALL_SN[out_port_n+in_port_n];
//
//      	      uint8_t SET_COLOR;
//      	      uint8_t CLR_COLOR;
//      	    char mess_low[16];
//      	    char mess_hi[16];
//
//      	      uint8_t reactiv;
//      	      uint8_t cicle_t;

//      	  uint8_t TEMP_UP_L;
//      	  	uint8_t TEMP_UP_SL;
//      	  	uint8_t TEMP_UP_E;
//      	  	uint8_t TEMP_UP_SM;
//      	  	uint8_t TEMP_UP_SN;
//
//      	  	uint8_t TEMP_DW_L;
//      	  	uint8_t TEMP_DW_SL;
//      	  	uint8_t TEMP_DW_E;
//      	  	uint8_t TEMP_DW_SM;
//      	  	uint8_t TEMP_DW_SN;
//
//      	  	uint8_t TEMP_OK_L;
//      	  	uint8_t TEMP_OK_SL;
//      	  	uint8_t TEMP_OK_E;
//      	  	uint8_t TEMP_OK_SM;
//      	  	uint8_t TEMP_OK_SN;
//
//      	  	uint8_t TEMP_ERR_L;
//      	  	uint8_t TEMP_ERR_SL;
//      	  	uint8_t TEMP_ERR_E;
//      	  	uint8_t TEMP_ERR_SM;
//      	  	uint8_t TEMP_ERR_SN;
//
//      	  	uint8_t TEMP_CIKL_E;
//      	  	uint8_t TEMP_CIKL_SM;
//      	  	uint8_t ALL_EVENT;
//      	  	uint8_t repit_3r;



//      		uint8_t V_EVENT_L;
//      		uint8_t V_EVENT_SL;
//      		uint8_t V_EVENT_E;
//      		uint8_t V_EVENT_S;
//      		uint8_t V_EVENT_T;
//
//      		uint8_t V_RESET_L;
//      		uint8_t V_RESET_SL;
//      		uint8_t V_RESET_E;
//      		uint8_t V_RESET_S;
//      		uint8_t V_RESET_T;
//      		uint8_t V_RELOG_E;


      printf((err != ESP_OK) ? "Failed!\n" : "Done\n");


      printf("Committing 2 updates in NVS ... ");
      err = nvs_commit(nvs_data_handle);
      printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
      nvs_close(nvs_data_handle);

      return err;
}

uint8_t load_def_data(void)
    {

	FW_data.sys.LOAD_DEF_FLAG = 1;

     FW_data.net.V_IP_CONFIG[0]=192;
     FW_data.net.V_IP_CONFIG[1]=168;
     FW_data.net.V_IP_CONFIG[2]=3;
     FW_data.net.V_IP_CONFIG[3]=100;

     FW_data.net.V_IP_MASK[0]=255;
     FW_data.net.V_IP_MASK[1]=255;
     FW_data.net.V_IP_MASK[2]=255;
     FW_data.net.V_IP_MASK[3]=0;

     FW_data.net.V_IP_GET[0]=192;
     FW_data.net.V_IP_GET[1]=168;
     FW_data.net.V_IP_GET[2]=3;
     FW_data.net.V_IP_GET[3]=1;

     FW_data.net.V_DHCP = 1;
     FW_data.net.V_IP_DNS[0]=10;
     FW_data.net.V_IP_DNS[1]=0;
     FW_data.net.V_IP_DNS[2]=0;
     FW_data.net.V_IP_DNS[3]=18;


     FW_data.sys.V_FW1_VER[0]=Platform;
     FW_data.sys.V_FW1_VER[1]=rev;
     FW_data.sys.V_FW1_VER[2]=Assembly;
     FW_data.sys.V_FW1_VER[3]=Bild;

     FW_data.sys.V_FW1_LEN=1280*1024;//*((uint32_t *)A_FW_LEN);
     FW_data.sys.V_BOOT_VER =0;// *((uint32_t *)A_BOOT_VER);
     FW_data.sys.V_CRC_DATA = 0;

      memset((uint8_t*)&FW_data.http.V_LOGIN,0,16);
     memcpy((uint32_t*)&FW_data.http.V_LOGIN, (uint32_t *)"visor", 5);
      memset((uint8_t*)&FW_data.http.V_PASSWORD,0,16);
     memcpy((uint32_t*)&FW_data.http.V_PASSWORD, (uint32_t *)"ping", 4);

       FW_data.http.V_WEB_PORT = 80;
//     HTTPD_SERVER_PORT =FW_data.V_WEB_PORT;
       FW_data.snmp.V_PORT_SNMP = 162;
       FW_data.net.V_HTTP_IP = 80;
     memset((uint8_t*)&FW_data.http.V_ON_MESS,0,32);
     memcpy((uint8_t*)&FW_data.http.V_ON_MESS, (uint8_t *)"Power swich ON",14);
     memset((uint8_t*)&FW_data.http.V_OFF_MESS,0,32);
     memcpy((uint8_t*)&FW_data.http.V_OFF_MESS, (uint8_t *)"Power swich OFF", 15);
     FW_data.http.V_FLAG_EN_MAN =1;
     FW_data.rasp.V_FLAG_EN_RASP =0;
     FW_data.http.V_FLAG_EN_WEB =1;
     FW_data.wdt[0].V_FLAG_EN_WATCHMAN =0;
     FW_data.wdt[1].V_FLAG_EN_WATCHMAN =0;
     memset((uint8_t*)&FW_data.smtp.V_EMAIL_ERR,0,32);
     memset((uint8_t*)&FW_data.rasp.V_D_TIME,61,sizeof(FW_data.rasp.V_D_TIME));
     memset((uint8_t*)&FW_data.rasp.V_RD_DATA,61,sizeof(FW_data.rasp.V_RD_DATA));


     FW_data.rasp.V_IP_PING_TIME = 9999;
     FW_data.rasp.V_TIME_SEND = 9999;
     FW_data.rasp.V_TIME_READ = 9999;
     FW_data.rasp.V_N_RERID = 9999;
     FW_data.rasp.V_TIME_RESET = 9999;
     FW_data.rasp.V_DELAY_PING = 9999;
     FW_data.rasp.V_SOST_RESET = 0;
     FW_data.wdt[0].V_N_PING = 255;
     FW_data.wdt[1].V_N_PING = 255;
     FW_data.net.V_IP_NTP1[0]=85;//    85.21.78.23
     FW_data.net.V_IP_NTP1[1]=21;
     FW_data.net.V_IP_NTP1[2]=78;
     FW_data.net.V_IP_NTP1[3]=23;

     FW_data.net.V_IP_NTP2[0]=194;
     FW_data.net.V_IP_NTP2[1]=190;
     FW_data.net.V_IP_NTP2[2]=168;
     FW_data.net.V_IP_NTP2[3]=1;
     FW_data.net.V_PORT_NTP  = 123;


     memcpy((uint8_t*)&FW_data.snmp.V_COMMUNITY,(uint8_t *)"SWICH",5);
     memcpy((uint8_t*)&FW_data.snmp.V_COMMUNITY_WRITE,(uint8_t *)"SWICH",5);


     memset((uint8_t*)&FW_data.smtp.V_NAME_SMTP,0,32);
     memcpy((uint8_t*)&FW_data.smtp.V_NAME_SMTP, (uint8_t *)"smtpforcustomers.netping.ru",sizeof("smtpforcustomers.netping.ru"));
     FW_data.snmp.V_PORT_SNMP = 162;
     memset((uint8_t*)&FW_data.smtp.V_LOGIN_SMTP,0,32);
     memcpy((uint32_t*)&FW_data.smtp.V_LOGIN_SMTP, (uint32_t *)"users_auto_mail@netping.ru", sizeof("users_auto_mail@netping.ru"));
     memset((uint8_t*)&FW_data.smtp.V_PASSWORD_SMTP,0,32);
     memcpy((uint32_t*)&FW_data.smtp.V_PASSWORD_SMTP, (uint32_t *)"dTRvdjc5cXN3eWEw",sizeof("dTRvdjc5cXN3eWEw"));
     FW_data.smtp.V_FLAG_EMAIL_PORT=2525;

     FW_data.smtp.V_FLAG_DEF_EMAIL=1;
     FW_data.smtp.V_FLAG_EN_EMAIL=1;


     memset((uint8_t*)&FW_data.smtp.V_EMAIL_ADDR,0,32);


     memset((uint8_t*)&FW_data.smtp.V_EMAIL_FROM,0,32);
     memcpy((uint32_t*)&FW_data.smtp.V_EMAIL_FROM, (uint32_t *)"dkst110@smtp.netping.ru", sizeof("dkst110@smtp.netping.ru"));

     memset((uint8_t*)&FW_data.smtp.V_EMAIL_TO,0,32);






      memset((uint8_t*)&FW_data.sys.V_GEOM_NAME,0,85);
     memcpy((uint32_t*)&FW_data.sys.V_GEOM_NAME, (uint32_t *)"Moscow office", 13);

    FW_data.sys.V_ID_MAC[0] =   00;//(uint16_t)idBase0[0];
    FW_data.sys.V_ID_MAC[1] =   00;//((uint16_t)idBase0[0])>>8;
    FW_data.sys.V_ID_MAC[2] =   00;///(uint16_t)idBase1[0];
    FW_data.sys.V_ID_MAC[3] =   00;//((uint16_t)idBase1[0])>>8;
    FW_data.sys.V_ID_MAC[4] =   00;//(uint16_t)idBase2[0];
    FW_data.sys.V_ID_MAC[5] =   00;//((uint16_t)idBase2[0])>>8;
    FW_data.sys.V_ID_MAC[6] =    0x59;
    FW_data.sys.V_ID_MAC[7] =    0xDC;
    memset((uint8_t*)&FW_data.sys.V_Name_dev,0,85);
    memset((uint8_t*)&FW_data.sys.V_CALL_DATA,0,85);
    memcpy((uint8_t*)&FW_data.sys.V_Name_dev,(uint8_t *)"netping110",10);
    memcpy((uint8_t*)&FW_data.sys.V_CALL_DATA,(uint8_t *)"netping.ru",10);


//    FW_data.V_logs_struct.CRC16 = 0;
//    memset((uint8_t*)&FW_data.V_logs_struct.log_reple,0,2000);

     FW_data.net.V_IP_SYSL[0]=0;//     62.117.76.142
     FW_data.net.V_IP_SYSL[1]=0;
     FW_data.net.V_IP_SYSL[2]=0;
     FW_data.net.V_IP_SYSL[3]=0;


     FW_data.snmp.V_IP_SNMP[0]=192;//     62.117.76.142
     FW_data.snmp.V_IP_SNMP[1]=168;
     FW_data.snmp.V_IP_SNMP[2]=0;
     FW_data.snmp.V_IP_SNMP[3]=152;
     FW_data.sys.V_TYPE_OUT=0;

     FW_data.sys.V_NTP_CIRCL = 4;

     //  FW_data.V_CRC_BOOT=*((uint16_t *)A_CRC_DATA_BOOT);
   //    FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
     //  FW_data.V_CRC_DATA=crc16_ccitt((uint8_t*)&(FW_data.V_DHCP),2018);

       FW_data.wdt[0].V_TYPE_LOGIC=0;
       FW_data.wdt[0].V_EN_WATCHDOG=0;
       FW_data.wdt[0].V_EN_WATCHDOG_CN_A=1;
       FW_data.wdt[0].V_EN_WATCHDOG_CN_B=1;
       FW_data.wdt[0].V_EN_WATCHDOG_CN_C=1;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[0]=192;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[1]=168;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[2]=0;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[3]=151;

       FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[0]=192;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[1]=168;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[2]=0;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[3]=152;

       FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[0]=192;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[1]=168;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[2]=0;
       FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[3]=153;
       FW_data.wdt[0].V_CT_RES_ALLSTART=0;
       FW_data.wdt[0].V_T_SEND_PING=15;
       FW_data.wdt[0].V_TIME_RESEND_PING=1000;
       FW_data.wdt[0].V_MAX_REPID_PING=8;
       FW_data.wdt[0].V_TIME_RESET_PULSE=12;
       FW_data.wdt[0].V_PAUSE_RESET_TO_REPID=15;
       FW_data.wdt[0].V_MAX_RESEND_PACET_RESET=0;


       FW_data.wdt[1].V_TYPE_LOGIC=0;
       FW_data.wdt[1].V_EN_WATCHDOG=0;
       FW_data.wdt[1].V_EN_WATCHDOG_CN_A=1;
       FW_data.wdt[1].V_EN_WATCHDOG_CN_B=1;
       FW_data.wdt[1].V_EN_WATCHDOG_CN_C=1;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[0]=192;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[1]=168;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[2]=0;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[3]=151;

       FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[0]=192;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[1]=168;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[2]=0;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[3]=152;

       FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[0]=192;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[1]=168;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[2]=0;
       FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[3]=153;
       FW_data.wdt[1].V_CT_RES_ALLSTART=0;
       FW_data.wdt[1].V_T_SEND_PING=15;
       FW_data.wdt[1].V_TIME_RESEND_PING=1000;
       FW_data.wdt[1].V_MAX_REPID_PING=8;
       FW_data.wdt[1].V_TIME_RESET_PULSE=12;
       FW_data.wdt[1].V_PAUSE_RESET_TO_REPID=15;
       FW_data.wdt[1].V_MAX_RESEND_PACET_RESET=0;




       memset((uint8_t*)&FW_data.gpio.name[0],0,32);
       memcpy((uint8_t*)&FW_data.gpio.name[0], (uint8_t *)"IO_PORT0", sizeof("IO_PORT0"));

       memset((uint8_t*)&FW_data.gpio.name[1],0,32);
       memcpy((uint8_t*)&FW_data.gpio.name[1], (uint8_t *)"IO_PORT1", sizeof("IO_PORT1"));

       memset((uint8_t*)&FW_data.gpio.name[2],0,32);
       memcpy((uint8_t*)&FW_data.gpio.name[2], (uint8_t *)"I_PORT2", sizeof("I_PORT2"));

       memset((uint8_t*)&FW_data.gpio.name[3],0,32);
       memcpy((uint8_t*)&FW_data.gpio.name[3], (uint8_t *)"I_PORT3", sizeof("I_PORT3"));

       memset((uint8_t*)FW_data.wdt[0].V_NAME,0,16);
       memcpy((uint8_t*)FW_data.wdt[0].V_NAME, (uint8_t *)"Canal1", sizeof("Canal1"));

       memset((uint8_t*)FW_data.wdt[1].V_NAME,0,16);
       memcpy((uint8_t*)FW_data.wdt[1].V_NAME, (uint8_t *)"Canal2", sizeof("Canal2"));

       FW_data.gpio.dir[0]=1;
       FW_data.gpio.dir[1]=1;
       FW_data.gpio.dir[2]=0;
       FW_data.gpio.dir[3]=0;

       FW_data.gpio.IN_PORT[0].filtr_time=500;
       FW_data.gpio.IN_PORT[1].filtr_time=500;
       FW_data.gpio.OUT_PORT[0].input_str.filtr_time=500;
       FW_data.gpio.OUT_PORT[1].input_str.filtr_time=500;

       FW_data.gpio.OUT_PORT[0].delay=1000;
       FW_data.gpio.OUT_PORT[1].delay=1000;

       FW_data.termo[0].t_up=30;
       FW_data.termo[0].t_dw=10;
       FW_data.termo[0].status=2;

       memset((uint8_t*)FW_data.termo[0].name,0,16);
       memcpy((uint8_t*)FW_data.termo[0].name, (uint8_t *)"Termo1", sizeof("Termo1"));

       FW_data.termo[1].t_up=30;
       FW_data.termo[1].t_dw=10;
       FW_data.termo[1].status=2;

       memset((uint8_t*)FW_data.termo[1].name,0,16);
       memcpy((uint8_t*)FW_data.termo[1].name, (uint8_t *)"Termo2", sizeof("Termo2"));

      save_data_blok();



        return 1;
      }

uint8_t load_struct_flash_data (void)
{
		esp_err_t err= nvs_flash_init_partition("nvs");
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
	    // NVS partition was truncated and needs to be erased
	    // Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase_partition("nvs"));
	    err = nvs_flash_init_partition("nvs");
	}
	ESP_ERROR_CHECK( err );

   err =nvs_open_from_partition("nvs","storage", NVS_READWRITE, &nvs_data_handle);

   err=nvs_get_u8(nvs_data_handle,get_name(LOAD_DEF_FLAG), (uint8_t*)&(FW_data.sys.LOAD_DEF_FLAG));

   printf("Committing1 updates in NVS ... ");
   err = nvs_commit(nvs_data_handle);

   printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
   nvs_close(nvs_data_handle);

   if ((FW_data.sys.LOAD_DEF_FLAG==0)||(err!=ESP_OK))
    {
	 printf("Run load_def_data() - settings default load \n\r");
	 nvs_erase_all(nvs_data_handle);
     return  load_def_data();
    }
   else
    {

       return load_data_blok();
    }


}




void nvs_task(void *pvParameters)
{
	esp_err_t err= nvs_flash_init_partition("nvs");
			if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
			    // NVS partition was truncated and needs to be erased
			    // Retry nvs_flash_init
			    ESP_ERROR_CHECK(nvs_flash_erase_partition("nvs"));
			    err = nvs_flash_init_partition("nvs");
			}
			ESP_ERROR_CHECK( err );

    while(1) {

    	if (reple_to_save.dicr!=0)
    	{
    		GET_reple(&reple_to_save);
    		save_reple_log(reple_to_save);
    	}
    	if (nvs_flags.data_param!=0)
    	{
    		// gpio_set_level(BLINK_GPIO, 1);
    		save_data_blok();
//    		load_data_blok();
    	//	 gpio_set_level(BLINK_GPIO, 0);
    	//	esp_restart();
    		nvs_flags.data_param=0;
    	}
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
