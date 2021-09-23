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

#include "nvs_task.h"
#define get_name(x) #x
FW_data_t FW_data;
nvs_handle_t nvs_data_handle;

esp_err_t save_data_blok(void)
{
	esp_err_t err=0;
	err = nvs_set_blob(nvs_data_handle,get_name(FW_data.net.V_IP_CONFIG),FW_data.net.V_IP_CONFIG,4);


	err =err|nvs_set_blob(nvs_data_handle,get_name(FW_data.net.V_IP_MASK),FW_data.net.V_IP_MASK,4);


	err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.net.V_IP_GET),FW_data.net.V_IP_GET,4);

	err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.net.V_DHCP),FW_data.net.V_DHCP);
	err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.net.V_IP_DNS),FW_data.net.V_IP_DNS,4);

	err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.sys.V_FW1_VER),FW_data.sys.V_FW1_VER,4);

	err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.sys.V_FW1_LEN),FW_data.sys.V_FW1_LEN);

	err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.sys.V_BOOT_VER),FW_data.sys.V_BOOT_VER);

	err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.http.V_LOGIN),FW_data.http.V_LOGIN,16);

	err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.http.V_PASSWORD),FW_data.http.V_PASSWORD,16);

	err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.http.V_WEB_PORT),FW_data.http.V_WEB_PORT);

	err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.snmp.V_PORT_SNMP),FW_data.snmp.V_PORT_SNMP);

	err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.net.V_HTTP_IP),FW_data.net.V_HTTP_IP);

	err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.http.V_ON_MESS),FW_data.http.V_ON_MESS,32);

	err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.http.V_OFF_MESS),FW_data.http.V_OFF_MESS,32);

	err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.http.V_FLAG_EN_MAN),FW_data.http.V_FLAG_EN_MAN);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.rasp.V_FLAG_EN_RASP),FW_data.rasp.V_FLAG_EN_RASP);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.http.V_FLAG_EN_WEB),FW_data.http.V_FLAG_EN_WEB);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.wdt.V_FLAG_EN_WATCHMAN),FW_data.wdt.V_FLAG_EN_WATCHMAN);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.rasp.V_IP_PING_TIME),FW_data.rasp.V_IP_PING_TIME);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.rasp.V_TIME_SEND),FW_data.rasp.V_TIME_SEND);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.rasp.V_TIME_READ),FW_data.rasp.V_TIME_READ);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.rasp.V_N_RERID),FW_data.rasp.V_N_RERID);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.rasp.V_TIME_RESET),FW_data.rasp.V_TIME_RESET);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.rasp.V_DELAY_PING),FW_data.rasp.V_DELAY_PING);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.rasp.V_SOST_RESET),FW_data.rasp.V_SOST_RESET);

    err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_N_PING),FW_data.wdt.V_N_PING);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.net.V_IP_NTP1),FW_data.net.V_IP_NTP1,4);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.net.V_IP_NTP2),FW_data.net.V_IP_NTP2,4);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.net.V_PORT_NTP),FW_data.net.V_PORT_NTP);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_NAME_SMTP),FW_data.smtp.V_NAME_SMTP,32);

    err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.snmp.V_PORT_SNMP),FW_data.snmp.V_PORT_SNMP);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_LOGIN_SMTP),FW_data.smtp.V_LOGIN_SMTP,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_PASSWORD_SMTP),FW_data.smtp.V_PASSWORD_SMTP,32);

    err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.smtp.V_FLAG_EMAIL_PORT),FW_data.smtp.V_FLAG_EMAIL_PORT);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ADDR),FW_data.smtp.V_EMAIL_ADDR,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_FROM),FW_data.smtp.V_EMAIL_FROM,32);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_TO),FW_data.smtp.V_EMAIL_TO,32);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.smtp.V_FLAG_DEF_EMAIL),FW_data.smtp.V_FLAG_DEF_EMAIL);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.smtp.V_FLAG_EN_EMAIL),FW_data.smtp.V_FLAG_EN_EMAIL);

    err=err|nvs_set_blob(nvs_data_handle,get_name(FW_data.sys.V_GEOM_NAME),FW_data.sys.V_GEOM_NAME,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.sys.V_ID_MAC), FW_data.sys.V_ID_MAC,8);

    err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.sys.V_Name_dev), FW_data.sys.V_Name_dev,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.sys.V_CALL_DATA),FW_data.sys.V_CALL_DATA,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.sys.V_Name_dev),FW_data.sys.V_Name_dev,88);

    err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.sys.V_CALL_DATA),FW_data.sys.V_CALL_DATA,16);

    err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.net.V_IP_SYSL),FW_data.net.V_IP_SYSL,4);

    err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.net.V_IP_SNMP),FW_data.net.V_IP_SNMP,4);

    err=err|nvs_set_u32(nvs_data_handle,get_name(FW_data.sys.V_TYPE_OUT),FW_data.sys.V_TYPE_OUT);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.sys.V_NTP_CIRCL),FW_data.sys.V_NTP_CIRCL);

    err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.sys.V_TYPE_LOGIC),FW_data.sys.V_TYPE_LOGIC);

    err=err|nvs_set_u8(nvs_data_handle,get_name( FW_data.wdt.V_EN_WATCHDOG), FW_data.wdt.V_EN_WATCHDOG);

      err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.wdt.V_EN_WATCHDOG_CN_A),FW_data.wdt.V_EN_WATCHDOG_CN_A);

      err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.wdt.V_EN_WATCHDOG_CN_B),FW_data.wdt.V_EN_WATCHDOG_CN_B);

      err=err|nvs_set_u8(nvs_data_handle,get_name(FW_data.wdt.V_EN_WATCHDOG_CN_C),FW_data.wdt.V_EN_WATCHDOG_CN_C);

      err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.wdt.V_IP_WDT_ADDR_CN_A),FW_data.wdt.V_IP_WDT_ADDR_CN_A,4);

      err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.wdt.V_IP_WDT_ADDR_CN_B),FW_data.wdt.V_IP_WDT_ADDR_CN_B,4);

      err=err|nvs_set_blob(nvs_data_handle,get_name( FW_data.wdt.V_IP_WDT_ADDR_CN_C),FW_data.wdt.V_IP_WDT_ADDR_CN_C,4);

      err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_CT_RES_ALLSTART),FW_data.wdt.V_CT_RES_ALLSTART);

      err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_T_SEND_PING),FW_data.wdt.V_T_SEND_PING);

      err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_TIME_RESEND_PING),FW_data.wdt.V_TIME_RESEND_PING);

      err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_MAX_REPID_PING),FW_data.wdt.V_MAX_REPID_PING);

      err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_TIME_RESET_PULSE),FW_data.wdt.V_TIME_RESET_PULSE);

      err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_PAUSE_RESET_TO_REPID),FW_data.wdt.V_PAUSE_RESET_TO_REPID);

      err=err|nvs_set_u16(nvs_data_handle,get_name(FW_data.wdt.V_MAX_RESEND_PACET_RESET),FW_data.wdt.V_MAX_RESEND_PACET_RESET);

      return err;
}
esp_err_t load_data_blok(void)
{
	esp_err_t err=0;
	err = nvs_get_blob(nvs_data_handle,get_name(FW_data.net.V_IP_CONFIG),FW_data.net.V_IP_CONFIG,4);


	err =err|nvs_get_blob(nvs_data_handle,get_name(FW_data.net.V_IP_MASK),FW_data.net.V_IP_MASK,4);


	err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.net.V_IP_GET),FW_data.net.V_IP_GET,4);

	err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.net.V_DHCP),FW_data.net.V_DHCP);

	err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.net.V_IP_DNS),FW_data.net.V_IP_DNS,4);

	err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.sys.V_FW1_VER),FW_data.sys.V_FW1_VER,4);

	err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.sys.V_FW1_LEN),FW_data.sys.V_FW1_LEN);

	err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.sys.V_BOOT_VER),FW_data.sys.V_BOOT_VER);

	err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.http.V_LOGIN),FW_data.http.V_LOGIN,16);

	err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.http.V_PASSWORD),FW_data.http.V_PASSWORD,16);

	err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.http.V_WEB_PORT),FW_data.http.V_WEB_PORT);

	err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.snmp.V_PORT_SNMP),FW_data.snmp.V_PORT_SNMP);

	err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.net.V_HTTP_IP),FW_data.net.V_HTTP_IP);

	err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.http.V_ON_MESS),FW_data.http.V_ON_MESS,32);

	err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.http.V_OFF_MESS),FW_data.http.V_OFF_MESS,32);

	err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.http.V_FLAG_EN_MAN),FW_data.http.V_FLAG_EN_MAN);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.rasp.V_FLAG_EN_RASP),FW_data.rasp.V_FLAG_EN_RASP);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.http.V_FLAG_EN_WEB),FW_data.http.V_FLAG_EN_WEB);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.wdt.V_FLAG_EN_WATCHMAN),FW_data.wdt.V_FLAG_EN_WATCHMAN);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ERR),FW_data.smtp.V_EMAIL_ERR,32);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.rasp.V_IP_PING_TIME),FW_data.rasp.V_IP_PING_TIME);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.rasp.V_TIME_SEND),FW_data.rasp.V_TIME_SEND);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.rasp.V_TIME_READ),FW_data.rasp.V_TIME_READ);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.rasp.V_N_RERID),FW_data.rasp.V_N_RERID);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.rasp.V_TIME_RESET),FW_data.rasp.V_TIME_RESET);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.rasp.V_DELAY_PING),FW_data.rasp.V_DELAY_PING);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.rasp.V_SOST_RESET),FW_data.rasp.V_SOST_RESET);

    err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_N_PING),FW_data.wdt.V_N_PING);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.net.V_IP_NTP1),FW_data.net.V_IP_NTP1,4);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.net.V_IP_NTP2),FW_data.net.V_IP_NTP2,4);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.net.V_PORT_NTP),FW_data.net.V_PORT_NTP);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_NAME_SMTP),FW_data.smtp.V_NAME_SMTP,32);

    err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.snmp.V_PORT_SNMP),FW_data.snmp.V_PORT_SNMP);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_LOGIN_SMTP),FW_data.smtp.V_LOGIN_SMTP,32);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_PASSWORD_SMTP),FW_data.smtp.V_PASSWORD_SMTP,32);

    err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.smtp.V_FLAG_EMAIL_PORT),FW_data.smtp.V_FLAG_EMAIL_PORT);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_ADDR),FW_data.smtp.V_EMAIL_ADDR,32);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_FROM),FW_data.smtp.V_EMAIL_FROM,32);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.smtp.V_EMAIL_TO),FW_data.smtp.V_EMAIL_TO,32);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.smtp.V_FLAG_DEF_EMAIL),FW_data.smtp.V_FLAG_DEF_EMAIL);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.smtp.V_FLAG_EN_EMAIL),FW_data.smtp.V_FLAG_EN_EMAIL);

    err=err|nvs_get_blob(nvs_data_handle,get_name(FW_data.sys.V_GEOM_NAME),FW_data.sys.V_GEOM_NAME,88);

    err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.sys.V_ID_MAC), FW_data.sys.V_ID_MAC,8);

    err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.sys.V_Name_dev), FW_data.sys.V_Name_dev,88);

    err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.sys.V_CALL_DATA),FW_data.sys.V_CALL_DATA,88);

    err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.sys.V_Name_dev),FW_data.sys.V_Name_dev,88);

    err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.sys.V_CALL_DATA),FW_data.sys.V_CALL_DATA,16);

    err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.net.V_IP_SYSL),FW_data.net.V_IP_SYSL,4);

    err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.net.V_IP_SNMP),FW_data.net.V_IP_SNMP,4);

    err=err|nvs_get_u32(nvs_data_handle,get_name(FW_data.sys.V_TYPE_OUT),FW_data.sys.V_TYPE_OUT);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.sys.V_NTP_CIRCL),FW_data.sys.V_NTP_CIRCL);

    err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.sys.V_TYPE_LOGIC),FW_data.sys.V_TYPE_LOGIC);

    err=err|nvs_get_u8(nvs_data_handle,get_name( FW_data.wdt.V_EN_WATCHDOG), FW_data.wdt.V_EN_WATCHDOG);

      err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.wdt.V_EN_WATCHDOG_CN_A),FW_data.wdt.V_EN_WATCHDOG_CN_A);

      err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.wdt.V_EN_WATCHDOG_CN_B),FW_data.wdt.V_EN_WATCHDOG_CN_B);

      err=err|nvs_get_u8(nvs_data_handle,get_name(FW_data.wdt.V_EN_WATCHDOG_CN_C),FW_data.wdt.V_EN_WATCHDOG_CN_C);

      err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.wdt.V_IP_WDT_ADDR_CN_A),FW_data.wdt.V_IP_WDT_ADDR_CN_A,4);

      err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.wdt.V_IP_WDT_ADDR_CN_B),FW_data.wdt.V_IP_WDT_ADDR_CN_B,4);

      err=err|nvs_get_blob(nvs_data_handle,get_name( FW_data.wdt.V_IP_WDT_ADDR_CN_C),FW_data.wdt.V_IP_WDT_ADDR_CN_C,4);

      err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_CT_RES_ALLSTART),FW_data.wdt.V_CT_RES_ALLSTART);

      err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_T_SEND_PING),FW_data.wdt.V_T_SEND_PING);

      err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_TIME_RESEND_PING),FW_data.wdt.V_TIME_RESEND_PING);

      err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_MAX_REPID_PING),FW_data.wdt.V_MAX_REPID_PING);

      err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_TIME_RESET_PULSE),FW_data.wdt.V_TIME_RESET_PULSE);

      err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_PAUSE_RESET_TO_REPID),FW_data.wdt.V_PAUSE_RESET_TO_REPID);

      err=err|nvs_get_u16(nvs_data_handle,get_name(FW_data.wdt.V_MAX_RESEND_PACET_RESET),FW_data.wdt.V_MAX_RESEND_PACET_RESET);



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
     FW_data.wdt.V_FLAG_EN_WATCHMAN =0;
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
     FW_data.wdt.V_N_PING = 255;
     FW_data.net.V_IP_NTP1[0]=85;//    85.21.78.23
     FW_data.net.V_IP_NTP1[1]=21;
     FW_data.net.V_IP_NTP1[2]=78;
     FW_data.net.V_IP_NTP1[3]=23;

     FW_data.net.V_IP_NTP2[0]=194;
     FW_data.net.V_IP_NTP2[1]=190;
     FW_data.net.V_IP_NTP2[2]=168;
     FW_data.net.V_IP_NTP2[3]=1;
     FW_data.net.V_PORT_NTP  = 123;





     memset((uint8_t*)&FW_data.smtp.V_NAME_SMTP,0,32);
     memcpy((uint8_t*)&FW_data.smtp.V_NAME_SMTP, (uint8_t *)"Name SMTP ",sizeof("Name SMTP"));
     FW_data.snmp.V_PORT_SNMP = 162;
     memset((uint8_t*)&FW_data.smtp.V_LOGIN_SMTP,0,32);
     memcpy((uint32_t*)&FW_data.smtp.V_LOGIN_SMTP, (uint32_t *)"admin", 5);
     memset((uint8_t*)&FW_data.smtp.V_PASSWORD_SMTP,0,32);
     memcpy((uint32_t*)&FW_data.smtp.V_PASSWORD_SMTP, (uint32_t *)"admin", 5);
     FW_data.smtp.V_FLAG_EMAIL_PORT=25;

     FW_data.smtp.V_FLAG_DEF_EMAIL=1;
     FW_data.smtp.V_FLAG_EN_EMAIL=1;


     memset((uint8_t*)&FW_data.smtp.V_EMAIL_ADDR,0,32);
     //memcpy((uint32_t*)&FW_data.smtp.V_EMAIL_ADDR, (uint32_t *)"", 5);

     memset((uint8_t*)&FW_data.smtp.V_EMAIL_FROM,0,32);
     memcpy((uint32_t*)&FW_data.smtp.V_EMAIL_FROM, (uint32_t *)"dkst59@smtp.netping.ru", sizeof("dkst59@smtp.netping.ru"));

     memset((uint8_t*)&FW_data.smtp.V_EMAIL_TO,0,32);
     memcpy((uint32_t*)&FW_data.smtp.V_EMAIL_TO, (uint32_t *)"kotbazilioi@ngs.ru", sizeof("kotbazilioi@ngs.ru"));





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
    memcpy((uint8_t*)&FW_data.sys.V_Name_dev,(uint8_t *)"DKSF 110",8);
    memcpy((uint8_t*)&FW_data.sys.V_CALL_DATA,(uint8_t *)"netping.ru",10);


//    FW_data.V_logs_struct.CRC16 = 0;
//    memset((uint8_t*)&FW_data.V_logs_struct.log_reple,0,2000);

     FW_data.net.V_IP_SYSL[0]=0;//     62.117.76.142
     FW_data.net.V_IP_SYSL[1]=0;
     FW_data.net.V_IP_SYSL[2]=0;
     FW_data.net.V_IP_SYSL[3]=0;


     FW_data.net.V_IP_SNMP[0]=192;//     62.117.76.142
     FW_data.net.V_IP_SNMP[1]=168;
     FW_data.net.V_IP_SNMP[2]=0;
     FW_data.net.V_IP_SNMP[3]=152;
     FW_data.sys.V_TYPE_OUT=0;

     FW_data.sys.V_NTP_CIRCL = 4;

     //  FW_data.V_CRC_BOOT=*((uint16_t *)A_CRC_DATA_BOOT);
   //    FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
     //  FW_data.V_CRC_DATA=crc16_ccitt((uint8_t*)&(FW_data.V_DHCP),2018);

       FW_data.sys.V_TYPE_LOGIC=0;
       FW_data.wdt.V_EN_WATCHDOG=0;
       FW_data.wdt.V_EN_WATCHDOG_CN_A=1;
       FW_data.wdt.V_EN_WATCHDOG_CN_B=1;
       FW_data.wdt.V_EN_WATCHDOG_CN_C=1;
       FW_data.wdt.V_IP_WDT_ADDR_CN_A[0]=192;
       FW_data.wdt.V_IP_WDT_ADDR_CN_A[1]=168;
       FW_data.wdt.V_IP_WDT_ADDR_CN_A[2]=0;
       FW_data.wdt.V_IP_WDT_ADDR_CN_A[3]=151;

       FW_data.wdt.V_IP_WDT_ADDR_CN_B[0]=192;
       FW_data.wdt.V_IP_WDT_ADDR_CN_B[1]=168;
       FW_data.wdt.V_IP_WDT_ADDR_CN_B[2]=0;
       FW_data.wdt.V_IP_WDT_ADDR_CN_B[3]=152;

       FW_data.wdt.V_IP_WDT_ADDR_CN_C[0]=192;
       FW_data.wdt.V_IP_WDT_ADDR_CN_C[1]=168;
       FW_data.wdt.V_IP_WDT_ADDR_CN_C[2]=0;
       FW_data.wdt.V_IP_WDT_ADDR_CN_C[3]=153;
       FW_data.wdt.V_CT_RES_ALLSTART=0;
       FW_data.wdt.V_T_SEND_PING=15;
       FW_data.wdt.V_TIME_RESEND_PING=1000;
       FW_data.wdt.V_MAX_REPID_PING=8;
       FW_data.wdt.V_TIME_RESET_PULSE=12;
       FW_data.wdt.V_PAUSE_RESET_TO_REPID=15;
       FW_data.wdt.V_MAX_RESEND_PACET_RESET=0;

      save_data_blok();



        return 1;
      }

uint8_t load_struct_flash_data (void)
{
char * name =get_name(FW_data.sys.LOAD_DEF_FLAG);
   esp_err_t err=nvs_get_u8(nvs_data_handle,name, &FW_data.sys.LOAD_DEF_FLAG);
   err = nvs_open("storage", NVS_READWRITE, &nvs_data_handle);
   if ((FW_data.sys.LOAD_DEF_FLAG==0)||(err!=ESP_OK))
    {
     return  load_def_data();
    }
   else
    {

       return load_data_blok();
    }

   printf("Committing updates in NVS ... ");
   err = nvs_commit(nvs_data_handle);
   printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
   nvs_close(nvs_data_handle);
}




void nvs_task(void *pvParameters)
{

	esp_err_t err;// = nvs_flash_init();
//if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//    // NVS partition was truncated and needs to be erased
//    // Retry nvs_flash_init
//    ESP_ERROR_CHECK(nvs_flash_erase());
//    err = nvs_flash_init();
//}
//ESP_ERROR_CHECK( err );

// Open
	  int32_t restart_counter=0; // value will default to 0, if not set yet in NVS
	//  int32_t counter;
	  char* name_count =  get_name(FW_data.net.V_IP_CONFIG);




    while(1) {

//   	 printf("\n\r DATA - %s \n\r",name_count);

    	 //memcpy((char*)data_flash,point_nvs,8192);

//   	 gpio_set_level(5, 1);
//    	printf("\n");
//    	printf("Opening Non-Volatile Storage (NVS) handle... ");
//    	nvs_handle_t my_handle;
//    	err = nvs_open("storage", NVS_READWRITE, &my_handle);
//
//
//    	  if (err != ESP_OK) {
//    	        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
//    	    } else {
//    	        printf("Done\n");
//
//    	        // Read
//    	        printf("Reading restart counter from NVS ... ");
//
//    	        err = nvs_get_i32(my_handle, "restart_counter", &counter);
//    	        printf("\n\r READ DATA - %d \n\r",counter);
//    	        if (counter != restart_counter)
//    	        {
//    	        	 printf("\n\r ERROR DATA - %d \n\r",counter);
//    	        	 vTaskDelay(1000 / portTICK_PERIOD_MS);
//    	        }
//
//    	        	restart_counter=counter;
//
//
//    	        printf("Updating restart counter in NVS ... ");
//    	        restart_counter++;
//    	               err = nvs_set_i32(my_handle, "restart_counter", restart_counter);
//    	               printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
//    	               // Commit written value.
//    	                      // After setting any values, nvs_commit() must be called to ensure changes are written
//    	                      // to flash storage. Implementations may write to storage at other times,
//    	                      // but this is not guaranteed.
//    	                      printf("Committing updates in NVS ... ");
//    	                      err = nvs_commit(my_handle);
//    	                      printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
//
//    	                      // Close
//    	                      nvs_close(my_handle);
//    	    }
//    	  gpio_set_level(5, 0);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
