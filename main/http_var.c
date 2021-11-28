/*
 * http_var.c
 *
 *  Created on: 18 сент. 2021 г.
 *      Author: ivanov
 */
#include "../main/includes_base.h"
#include "esp_tls_crypto.h"
//#include "html_data_simple_ru.h"
#include "update.h"
#include "mime.h"
#include "app.h"
#include "LOGS.h"
#include "smtp.h"
#include "http_var.h"
#define STORAGE_NAMESPACE "storage"
#define TAG_http "http mes"
#define HTTPD_401      "401 UNAUTHORIZED"           /*!< HTTP Response 401 */
#define HTTPD_302      "302 Found Location: /index.html"           /*!< HTTP Response 401 */
/* An HTTP GET handler */
const uint8_t mask_dec[33][4] = { { 255, 255, 255, 255 },
		{ 255, 255, 255, 254 }, { 255, 255, 255, 252 }, { 255, 255, 255, 248 },
		{ 255, 255, 255, 240 }, { 255, 255, 255, 224 }, { 255, 255, 255, 192 },
		{ 255, 255, 255, 128 }, { 255, 255, 255, 000 }, { 255, 255, 254, 000 },
		{ 255, 255, 252, 000 }, { 255, 255, 248, 000 }, { 255, 255, 240, 000 },
		{ 255, 255, 224, 000 }, { 255, 255, 192, 000 }, { 255, 255, 128, 000 },
		{ 255, 255, 000, 000 }, { 255, 254, 000, 000 }, { 255, 252, 000, 000 },
		{ 255, 248, 000, 000 }, { 255, 240, 000, 000 }, { 255, 224, 000, 000 },
		{ 255, 192, 000, 000 }, { 255, 128, 000, 000 }, { 255, 000, 000, 000 },
		{ 254, 000, 000, 000 }, { 252, 000, 000, 000 }, { 248, 000, 000, 000 },
		{ 240, 000, 000, 000 }, { 224, 000, 000, 000 }, { 192, 000, 000, 000 },
		{ 128, 000, 000, 000 }, { 000, 000, 000, 000 } };

uint8_t page_sost;

void char2_to_hex(char *in, uint8_t *out, uint32_t len) {
//	Bcd_To_Hex((unsigned char *)in, (unsigned char *)out, len);
	for (uint32_t ct = 0; ct < len; ct++) {
		if (in[ct * 2] > 0x46) {
			in[ct * 2] = in[ct * 2] - 0x57;
		} else if (in[ct * 2] > 0x40) {
			in[ct * 2] = in[ct * 2] - 0x37;
		} else if (in[ct * 2] > 0x2f) {
			in[ct * 2] = in[ct * 2] - 0x30;
		} else {
			break;
		}

		if (in[ct * 2 + 1] > 0x46) {
			in[ct * 2 + 1] = in[ct * 2 + 1] - 0x57;
		} else if (in[ct * 2 + 1] > 0x40) {
			in[ct * 2 + 1] = in[ct * 2 + 1] - 0x37;
		} else if (in[ct * 2 + 1] > 0x2f) {
			in[ct * 2 + 1] = in[ct * 2 + 1] - 0x30;
		} else {
			break;
		}
		out[ct] = (in[ct * 2] << 4) | in[ct * 2 + 1];
	}

}
uint8_t read_mess_smtp(char *in, uint8_t *out) {
	uint8_t len = 0;
	if (in[0] > 0x46) {
		in[0] = in[0] - 0x57;
	} else if (in[0] > 0x40) {
		in[0] = in[0] - 0x37;
	} else if (in[0] > 0x2f) {
		in[0] = in[0] - 0x30;
	} else {
		return 0;
	}
	if (in[1] > 0x46) {
		in[1] = in[1] - 0x57;
	} else if (in[1] > 0x40) {
		in[1] = in[1] - 0x37;
	} else if (in[1] > 0x2f) {
		in[1] = in[1] - 0x30;
	} else {
		return 0;
	}
	len = (in[0] << 4) | in[1];
	char2_to_hex((char*) (in + 2), (uint8_t*) out, len);
	return len;
}

static esp_err_t hello_get_handler(httpd_req_t *req) {
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");

	const esp_partition_t *running = esp_ota_get_running_partition();
	esp_app_desc_t app_desc;
	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
	if (ret != ESP_OK) {
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
				"Can't read FW version!");
		return ESP_FAIL;
	}
//   char buf[128];
//   sprintf(buf,
//   		"var fwver='v%.31s'\n"
//   		"var devname='Test Netping-on-ESP32';\n",
//   		 app_desc.version);
//	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t sse_get_cgi_handler(httpd_req_t *req) {
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");
	//const esp_partition_t *running = esp_ota_get_running_partition();
	char buf[128];
	char buf_temp[128]; // "retry: 2000\n\nevent: sse_ping.data: -\n\nevent: sse_ping.data: -\n\n"  unsigned n = sprintf(s,"event: io_state\n""data: %u\n\n", io_get_state_bitmap());
	sprintf(buf, "retry: 2000\n\n");
	//sprintf(buf_temp,"event: sse_ping\n""data: -\n\n");

	uint8_t s[2];
	if (FW_data.gpio.dir[0] == 0) {
		s[0] = FW_data.gpio.OUT_PORT[0].input_str.sost_filtr;
	} else {
		s[0] = FW_data.gpio.OUT_PORT[0].realtime;
	}
	if (FW_data.gpio.dir[1] == 0) {
		s[1] = FW_data.gpio.OUT_PORT[1].input_str.sost_filtr;
	} else {
		s[1] = FW_data.gpio.OUT_PORT[1].realtime;
	}

	sprintf(buf, "event: io_state\n" "data: %d%d%d%d\n\n",
			FW_data.gpio.IN_PORT[0].sost_filtr,
			FW_data.gpio.IN_PORT[1].sost_filtr, s[1], s[0]);

	strcat(buf, buf_temp);
	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

//notify_get.cgi?nfid=0800
static esp_err_t notify_get_cgi_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store\, no-cache\, must-revalidate");
	httpd_resp_set_type(req, mime_js);

	char buf[1024] = { 0 };
	char buf_temp[256];
	uint8_t number_mess;

	uint8_t reset, suspend, report;
	uint8_t canal;
	if (page_sost == WDT) {
		canal = (*req).uri[24] - 0x30;

		reset = (FW_data.wdt[canal].V_RESET_L << 0)
				| (FW_data.wdt[canal].V_RESET_SL << 1)
				| (FW_data.wdt[canal].V_RESET_E << 2)
				| (FW_data.wdt[canal].V_RESET_S << 3)
				| (FW_data.wdt[canal].V_RESET_T << 4);
		suspend = (FW_data.wdt[canal].V_EVENT_L << 0)
				| (FW_data.wdt[canal].V_EVENT_SL << 1)
				| (FW_data.wdt[canal].V_EVENT_E << 2)
				| (FW_data.wdt[canal].V_EVENT_S << 3)
				| (FW_data.wdt[canal].V_EVENT_T << 4);
		report = FW_data.wdt[canal].V_RELOG_E << 2;
		sprintf(buf_temp, "({reset:%d,suspend:%d,report:%d})", reset, suspend,
				report);
		strcat(buf, buf_temp);
	}

	if (page_sost == TERMO) {
		canal = (*req).uri[24] - 0x30;
		uint8_t high, norm, low, fail, flags, repeat_alarm;
		high = (FW_data.termo[canal].TEMP_UP_L << 0)
				| (FW_data.termo[canal].TEMP_UP_SL << 1)
				| (FW_data.termo[canal].TEMP_UP_E << 2)
				| (FW_data.termo[canal].TEMP_UP_SM << 3)
				| (FW_data.termo[canal].TEMP_UP_SN << 4);
		norm = (FW_data.termo[canal].TEMP_UP_L << 0)
				| (FW_data.termo[canal].TEMP_OK_SL << 1)
				| (FW_data.termo[canal].TEMP_OK_E << 2)
				| (FW_data.termo[canal].TEMP_OK_SM << 3)
				| (FW_data.termo[canal].TEMP_OK_SN << 4);

		low = (FW_data.termo[canal].TEMP_DW_L << 0)
				| (FW_data.termo[canal].TEMP_DW_SL << 1)
				| (FW_data.termo[canal].TEMP_DW_E << 2)
				| (FW_data.termo[canal].TEMP_DW_SM << 3)
				| (FW_data.termo[canal].TEMP_DW_SN << 4);

		fail = (FW_data.termo[canal].TEMP_ERR_L << 0)
				| (FW_data.termo[canal].TEMP_ERR_SL << 1)
				| (FW_data.termo[canal].TEMP_ERR_E << 2)
				| (FW_data.termo[canal].TEMP_ERR_SM << 3)
				| (FW_data.termo[canal].TEMP_ERR_SN << 4);

		report = (FW_data.termo[canal].TEMP_CIKL_E & 0x01) << 2;

		repeat_alarm = FW_data.termo[canal].repit_3r;
		sprintf(buf_temp,
				"({high:%d,norm:%d,low:%d,fail:%d,report:%d,flags:0,repeat_alarm:%d})",
				high, norm, low, fail, report, repeat_alarm);
		strcat(buf, buf_temp);
		//({high:4,norm:4,low:4,fail:4,report:4,flags:0,repeat_alarm:1})
	}

	if (page_sost == IO) {
		uint8_t high, low, colors, rep_filter_mode, rep_filter_time;
		canal = (*req).uri[24] - 0x30;

		high = (FW_data.gpio.RISE_L[canal] << 0)
				| (FW_data.gpio.RISE_SL[canal] << 1)
				| (FW_data.gpio.RISE_E[canal] << 2)
				| (FW_data.gpio.RISE_SM[canal] << 3)
				| (FW_data.gpio.RISE_SN[canal] << 4);
		low = (FW_data.gpio.FALL_L[canal] << 0)
				| (FW_data.gpio.FALL_SL[canal] << 1)
				| (FW_data.gpio.FALL_E[canal] << 2)
				| (FW_data.gpio.FALL_SM[canal] << 3)
				| (FW_data.gpio.FALL_SN[canal] << 4);

		colors = (FW_data.gpio.SET_COLOR[canal] << 4)
				| FW_data.gpio.CLR_COLOR[canal];
		rep_filter_mode = FW_data.gpio.reactiv[canal];
		rep_filter_time = FW_data.gpio.cicle_t[canal];
		report = (FW_data.gpio.CIKL_E[canal] & 0x01) << 2;

		sprintf(buf_temp,
				"({high:%d,low:%d,report:%d,legend_high:\"%s\",legend_low:\"%s\",colors:%d,rep_filter_mode:%d,rep_filter_time:%d})",
				high, low, report, FW_data.gpio.mess_hi[canal],
				FW_data.gpio.mess_low[canal], colors, rep_filter_mode,
				rep_filter_time);
		strcat(buf, buf_temp);
	}
	///({high:19,low:19,report:0,legend_high:"",legend_low:"",colors:82,rep_filter_mode:0,rep_filter_time:0})

	//sprintf(buf,"11.05.21 Tu 07:38:15.040 Watchdog: reset of chan.1 \"Сигнал\"\. A (8.8.8.8) no reply, B (192.168.0.55) no reply, C (124.211.45.11) is ignored.\r\n");

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t log_get_cgi_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store\, no-cache\, must-revalidate");
	httpd_resp_set_type(req, mime_text);

	char buf[128 * 50] = { 0 };
	char buf_temp[256];
	uint8_t number_mess;

	esp_err_t err = nvs_open_from_partition("nvs", "storage", NVS_READWRITE,
			&nvs_data_handle);
	err = nvs_get_u16(nvs_data_handle, "number_mess", &number_mess);
	err = nvs_commit(nvs_data_handle);
	nvs_close(nvs_data_handle);

	//
	buf[0] = ' ';
	for (uint16_t i = number_mess; i > 0; i--) {
		logs_read(i, buf_temp);
		strcat(buf, buf_temp);
		printf("\n\rRead %d messege logs\n\r", i);
	}
	for (uint16_t i = max_log_mess; i < number_mess; i--) {
		logs_read(i, buf_temp);
		strcat(buf, buf_temp);
		printf("\n\rRead %d messege logs\n\r", i);
	}

	//sprintf(buf,"11.05.21 Tu 07:38:15.040 Watchdog: reset of chan.1 \"Сигнал\"\. A (8.8.8.8) no reply, B (192.168.0.55) no reply, C (124.211.45.11) is ignored.\r\n");

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
static esp_err_t io_get_cgi_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");

	const esp_partition_t *running = esp_ota_get_running_partition();
	esp_app_desc_t app_desc;
	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
	if (ret != ESP_OK) {
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
				"Can't read FW version!");
		return ESP_FAIL;
	}
	page_sost = IO;
	char buf[2048];
	char buf_temp[256];
	sprintf(buf,
			"var packfmt={name:{offs:0,len:32},direction:{offs:42,len:1},delay:{offs:40,len:2},level_out:{offs:43,len:1},pulse_dur:{offs:45,len:1},__len:48};");
	sprintf(buf_temp, "var data_status=15;");
	strcat(buf, buf_temp);
	sprintf(buf_temp,
			" var data=[{name:\"%s\",direction:%d,delay:%d,level_out:%d,pulse_dur:%d,level:1,nf_legend_high:\"\",nf_legend_low:\"\",colors:82},",
			FW_data.gpio.name[0], FW_data.gpio.dir[0],
			FW_data.gpio.OUT_PORT[0].input_str.filtr_time,
			FW_data.gpio.OUT_PORT[0].sost,
			FW_data.gpio.OUT_PORT[0].delay / 100);
	strcat(buf, buf_temp);
	sprintf(buf_temp,
			"{name:\"%s\",direction:%d,delay:%d,level_out:%d,pulse_dur:%d,level:1,nf_legend_high:\"\",nf_legend_low:\"\",colors:82},",
			FW_data.gpio.name[1], FW_data.gpio.dir[1],
			FW_data.gpio.OUT_PORT[1].input_str.filtr_time,
			FW_data.gpio.OUT_PORT[1].sost,
			FW_data.gpio.OUT_PORT[1].delay / 100);
	strcat(buf, buf_temp);
	sprintf(buf_temp,
			"{name:\"%s\",direction:%d,delay:%d,level_out:0,pulse_dur:10,level:1,nf_legend_high:\"\",nf_legend_low:\"\",colors:82},",
			FW_data.gpio.name[2], FW_data.gpio.dir[2],
			FW_data.gpio.IN_PORT[0].filtr_time);
	strcat(buf, buf_temp);
	sprintf(buf_temp,
			"{name:\"%s\",direction:%d,delay:%d,level_out:0,pulse_dur:10,level:1,nf_legend_high:\"\",nf_legend_low:\"\",colors:82}];",
			FW_data.gpio.name[3], FW_data.gpio.dir[3],
			FW_data.gpio.IN_PORT[1].filtr_time);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var fwver='v%.31s';", app_desc.version);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_location='%s';", FW_data.sys.V_GEOM_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwmodel=110;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwver=1;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_name='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t email_send_test_cgi_handler(httpd_req_t *req) {
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_status(req, HTTPD_200);
	httpd_resp_set_hdr(req, "Connection", "Close");
	char buf[256];
	my_smtp_test();

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t sendmail_get_cgi_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");

	const esp_partition_t *running = esp_ota_get_running_partition();
	esp_app_desc_t app_desc;
	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
	if (ret != ESP_OK) {
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
				"Can't read FW version!");
		return ESP_FAIL;
	}
	page_sost = SMTP;
	char buf[1024];
	char buf_temp[256];
	sprintf(buf,
			"var packfmt={fqdn:{offs:0,len:64},port:{offs:64,len:2},flags:{offs:66,len:1},user:{offs:68,len:48},passwd:{offs:116,len:32},from:{offs:148,len:48},to:{offs:196,len:48},cc_1:{offs:308,len:48},cc_2:{offs:356,len:48},cc_3:{offs:404,len:48},reports:{offs:244,len:64},__len:452};");
	sprintf(buf_temp, "var data={fqdn:\"%s\",", FW_data.smtp.V_NAME_SMTP);
	strcat(buf, buf_temp);
	uint8_t flags = 0x80 | (FW_data.smtp.V_FLAG_DEF_EMAIL)
			| (FW_data.smtp.V_FLAG_EN_EMAIL << 1);
	sprintf(buf_temp, "port:%d,flags:%d,", FW_data.smtp.V_FLAG_EMAIL_PORT,
			flags);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "user:\"%s\",", FW_data.smtp.V_LOGIN_SMTP);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "passwd:\"%s\",", FW_data.smtp.V_PASSWORD_SMTP);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "from:\"%s\",", FW_data.smtp.V_EMAIL_FROM);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "to:\"%s\",", FW_data.smtp.V_EMAIL_TO);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "cc_1:\"%s\",", FW_data.smtp.V_EMAIL_CC1);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "cc_2:\"%s\",", FW_data.smtp.V_EMAIL_CC2);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "cc_3:\"%s\",", FW_data.smtp.V_EMAIL_CC3);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reports:\"\",");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "default_from:\"%s\"};", FW_data.smtp.V_EMAIL_FROM);
	strcat(buf, buf_temp);

	sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var fwver='v%.32s';", app_desc.version);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_location='%s';", FW_data.sys.V_GEOM_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwmodel=110;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwver=1;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_name='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
static esp_err_t io_cgi_api_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");
	char *buf;
	int ret, remaining = httpd_req_get_url_query_len(req) + 1;
	buf = malloc(1024);
	if (httpd_req_get_url_query_str(req, buf, remaining) == ESP_OK) {
		ESP_LOGI(TAG_http, "Found header => Host: %s", buf);
	}

	if ((buf[0] == 'i') && (buf[1] == 'o')) {
		if (buf[2] == 0) {
			memset(buf, 0, 1024);
			uint8_t sost = (FW_data.gpio.IN_PORT[0].sost_filtr)
					| (FW_data.gpio.IN_PORT[1].sost_filtr << 1);
			sprintf(buf, "io_result('ok', %d);", sost);
		} else {
			if ((buf[2] == '0') && (buf[3] == 0)) {
				memset(buf, 0, 1024);
				sprintf(buf, "io_result('ok',-1,%d,0);",
						FW_data.gpio.IN_PORT[0].sost_filtr);
			} else if ((buf[2] == '1') && (buf[3] == 0)) {
				memset(buf, 0, 1024);
				sprintf(buf, "io_result('ok',-1,%d,0);",
						FW_data.gpio.IN_PORT[1].sost_filtr);
			} else {
				memset(buf, 0, 1024);
				sprintf(buf, "io_result('error');");
			}
		}

	} else {
		memset(buf, 0, 1024);
		sprintf(buf, "io_result('error');");
	}

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	free(buf);

	return ESP_OK;
}

static esp_err_t termo_get_cgi_api_handler(httpd_req_t *req) {
//#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");
	const esp_partition_t *running = esp_ota_get_running_partition();
	esp_app_desc_t app_desc;
	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
	if (ret != ESP_OK) {
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
				"Can't read FW version!");
		return ESP_FAIL;
	}
	page_sost = TERMO;
	char buf[1024];
	char buf_temp[256];
	sprintf(buf,
			"var packfmt={name:{offs:0,len:18},ow_addr:{offs:20,len:8},bottom:{offs:18,len:1},top:{offs:19,len:1},__len:34};");

	sprintf(buf_temp, "var data_double_hyst=2;");
	strcat(buf, buf_temp);

	sprintf(buf_temp, " var data=[{name:\"%s\",", FW_data.termo[0].name);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ow_addr:\"%02x%02x %02x%02x %02x%02x %02x%02x\",",
			FW_data.termo[0].id[0], FW_data.termo[0].id[1],
			FW_data.termo[0].id[2], FW_data.termo[0].id[3],
			FW_data.termo[0].id[4], FW_data.termo[0].id[5],
			FW_data.termo[0].id[6], FW_data.termo[0].id[7]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "bottom:%d,top:%d,", FW_data.termo[0].t_dw,
			FW_data.termo[0].t_up);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "value:%d,", FW_data.termo[0].temper);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "status:%d}", FW_data.termo[0].status);
	strcat(buf, buf_temp);
	sprintf(buf_temp, ",");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "{name:\"%s\",", FW_data.termo[1].name);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ow_addr:\"%02x%02x %02x%02x %02x%02x %02x%02x\",",
			FW_data.termo[1].id[0], FW_data.termo[1].id[1],
			FW_data.termo[1].id[2], FW_data.termo[1].id[3],
			FW_data.termo[1].id[4], FW_data.termo[1].id[5],
			FW_data.termo[1].id[6], FW_data.termo[1].id[7]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "bottom:%d,", FW_data.termo[1].t_dw);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "top:%d,", FW_data.termo[1].t_up);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "value:%d,", FW_data.termo[1].temper);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "status:%d}", FW_data.termo[1].status);
	strcat(buf, buf_temp);
//	sprintf(buf_temp, ",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "{name:\"3333333333333333\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "ow_addr:\"\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "bottom:15,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "top:30,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "value:0,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "status:0},");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "{name:\"4444444444444444\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "ow_addr:\"\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "bottom:10,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "top:60,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "value:0,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "status:0}");
//			strcat(buf, buf_temp);
	sprintf(buf_temp, "];");
	strcat(buf, buf_temp);

	sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var fwver='v%.31s';", app_desc.version);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_location='%s';", FW_data.sys.V_GEOM_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwmodel=110;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwver=1;");
	strcat(buf, buf_temp);

	sprintf(buf_temp, "var sys_name='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t termo_data_cgi_api_handler(httpd_req_t *req) {
//#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");
	const esp_partition_t *running = esp_ota_get_running_partition();
	esp_app_desc_t app_desc;
	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
	if (ret != ESP_OK) {
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
				"Can't read FW version!");
		return ESP_FAIL;
	}
	char buf[1024];
	char buf_temp[256];

	sprintf(buf, " [{name:\"%s\",", FW_data.termo[0].name);
	sprintf(buf_temp, "ow_addr:\"%02x%02x %02x%02x %02x%02x %02x%02x\",",
			FW_data.termo[0].id[0], FW_data.termo[0].id[1],
			FW_data.termo[0].id[2], FW_data.termo[0].id[3],
			FW_data.termo[0].id[4], FW_data.termo[0].id[5],
			FW_data.termo[0].id[6], FW_data.termo[0].id[7]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "bottom:%d,top:%d,", FW_data.termo[0].t_dw,
			FW_data.termo[0].t_up);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "value:%d,", FW_data.termo[0].temper);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "status:%d}", FW_data.termo[0].status);
	strcat(buf, buf_temp);
	sprintf(buf_temp, ",");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "{name:\"%s\",", FW_data.termo[1].name);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ow_addr:\"%02x%02x %02x%02x %02x%02x %02x%02x\",",
			FW_data.termo[1].id[0], FW_data.termo[1].id[1],
			FW_data.termo[1].id[2], FW_data.termo[1].id[3],
			FW_data.termo[1].id[4], FW_data.termo[1].id[5],
			FW_data.termo[1].id[6], FW_data.termo[1].id[7]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "bottom:%d,", FW_data.termo[1].t_dw);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "top:%d,", FW_data.termo[1].t_up);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "value:%d,", FW_data.termo[1].temper);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "status:%d}", FW_data.termo[1].status);
	strcat(buf, buf_temp);
//	sprintf(buf_temp, ",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "{name:\"3333333333333333\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "ow_addr:\"\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "bottom:15,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "top:30,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "value:0,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "status:0},");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "{name:\"4444444444444444\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "ow_addr:\"\",");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "bottom:10,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "top:60,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "value:0,");
//			strcat(buf, buf_temp);
//	sprintf(buf_temp, "status:0}");
//			strcat(buf, buf_temp);

	sprintf(buf_temp, "];");
	strcat(buf, buf_temp);

	sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var fwver='v%.31s';", app_desc.version);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_location='%s';", FW_data.sys.V_GEOM_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwmodel=110;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwver=1;");
	strcat(buf, buf_temp);

	sprintf(buf_temp, "var sys_name='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
static esp_err_t wdog_get_cgi_handler(httpd_req_t *req) {
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");

	esp_err_t err;
	nvs_handle_t my_handle;

	char buf[9048];
	char buf_temp[256];
	page_sost = WDT;
//	const esp_partition_t *running = esp_ota_get_running_partition();
//	esp_app_desc_t app_desc;
//	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
//	if (ret != ESP_OK) {
//		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
//				"Can't read FW version!");
//		return ESP_FAIL;
//	}

	int mac = 5566223;
	vTaskDelay(300 / portTICK_PERIOD_MS);
	sprintf(buf, "var packfmt={"
			"signature:{offs:0,len:4},"
			"name:{offs:4,len:32},"
			"output:{offs:62,len:2},"
			"ip0:{offs:36,len:4},"
			"ip1:{offs:40,len:4},"
			"ip2:{offs:44,len:4},"
			"poll_period:{offs:48,len:2},"
			"ping_timeout:{offs:50,len:2},"
			"reset_time:{offs:52,len:2},"
			"reboot_pause:{offs:54,len:2},"
			"max_retry:{offs:56,len:1},"
			"doubling_pause_resets:{offs:57,len:1},"
			"reset_mode:{offs:58,len:1},"
			"active:{offs:59,len:1},"
			"logic_mode:{offs:60,len:1},"
			"fqdn0:{offs:64,len:64},"
			"fqdn1:{offs:128,len:64},"
			"fqdn2:{offs:192,len:64},"
			"__len:256};\n");
	uint32_t activ = 0;

	sprintf(buf_temp, "var data=[");
	strcat(buf, buf_temp);

	sprintf(buf_temp, "{signature:2893295841,");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "name:\"%s\",", FW_data.wdt[0].V_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "output:%d,", FW_data.wdt[0].V_N_OUT);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ip0:'8.8.8.8',");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ip1:'192.168.0.99',");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ip2:'98.98.8.8',");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "poll_period:%d,", FW_data.wdt[0].V_T_SEND_PING);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ping_timeout:%d,", FW_data.wdt[0].V_TIME_RESEND_PING);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reset_time:%d,", FW_data.wdt[0].V_TIME_RESET_PULSE);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reboot_pause:%d,",
			FW_data.wdt[0].V_PAUSE_RESET_TO_REPID);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "max_retry:%d,", FW_data.wdt[0].V_MAX_REPID_PING);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "doubling_pause_resets:%d,",
			FW_data.wdt[0].V_MAX_RESEND_PACET_RESET);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reset_mode:%d,", FW_data.sys.V_TYPE_OUT);
	strcat(buf, buf_temp);
	activ = 0;
	activ = ((FW_data.wdt[0].V_EN_WATCHDOG << 7)
			| (FW_data.wdt[0].V_EN_WATCHDOG_CN_C << 2)
			| (FW_data.wdt[0].V_EN_WATCHDOG_CN_B << 1)
			| (FW_data.wdt[0].V_EN_WATCHDOG_CN_A));
	sprintf(buf_temp, "active:%d,", activ);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "logic_mode:%d,", FW_data.wdt[0].V_TYPE_LOGIC);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "fqdn0:\"%d.%d.%d.%d\",",
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[0],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[1],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[2],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "fqdn1:\"%d.%d.%d.%d\",",
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[0],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[1],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[2],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "fqdn2:\"%d.%d.%d.%d\",",
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[0],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[1],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[2],
			FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reset_count:%d}", FW_data.wdt[0].V_CT_RES_ALLSTART);
	strcat(buf, buf_temp);

	sprintf(buf_temp, ",");
	strcat(buf, buf_temp);

	sprintf(buf_temp, "{signature:2893295841,");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "name:\"%s\",", FW_data.wdt[1].V_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "output:%d,", FW_data.wdt[1].V_N_OUT);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ip0:'8.8.8.8',");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ip1:'192.168.0.99',");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ip2:'98.98.8.8',");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "poll_period:%d,", FW_data.wdt[1].V_T_SEND_PING);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ping_timeout:%d,", FW_data.wdt[1].V_TIME_RESEND_PING);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reset_time:%d,", FW_data.wdt[1].V_TIME_RESET_PULSE);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reboot_pause:%d,",
			FW_data.wdt[1].V_PAUSE_RESET_TO_REPID);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "max_retry:%d,", FW_data.wdt[1].V_MAX_REPID_PING);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "doubling_pause_resets:%d,",
			FW_data.wdt[1].V_MAX_RESEND_PACET_RESET);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reset_mode:%d,", FW_data.sys.V_TYPE_OUT);
	strcat(buf, buf_temp);
	activ = 0;
	activ = ((FW_data.wdt[1].V_EN_WATCHDOG << 7)
			| (FW_data.wdt[1].V_EN_WATCHDOG_CN_C << 2)
			| (FW_data.wdt[1].V_EN_WATCHDOG_CN_B << 1)
			| (FW_data.wdt[1].V_EN_WATCHDOG_CN_A));
	sprintf(buf_temp, "active:%d,", activ);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "logic_mode:%d,", FW_data.wdt[1].V_TYPE_LOGIC);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "fqdn0:\"%d.%d.%d.%d\",",
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[0],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[1],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[2],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "fqdn1:\"%d.%d.%d.%d\",",
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[0],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[1],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[2],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "fqdn2:\"%d.%d.%d.%d\",",
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[0],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[1],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[2],
			FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "reset_count:%d}", FW_data.wdt[1].V_CT_RES_ALLSTART);
	strcat(buf, buf_temp);

	sprintf(buf_temp, "];");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var fwver='v%.31s';", app_desc.version);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwmodel=110;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwver=1;");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_name='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_location='%s';", FW_data.sys.V_GEOM_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwmodel=110;");
	strcat(buf, buf_temp);
	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t setup_get_cgi_handler(httpd_req_t *req) {
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_js);
	httpd_resp_set_hdr(req, "Connection", "Close");

	esp_err_t err;
	nvs_handle_t my_handle;

	char buf[14096];
	char buf_temp[256];
	page_sost = SETT;

	int mac = 5566223;

	sprintf(buf,
			"var packfmt={mac:{offs:0,len:6},ip:{offs:6,len:4},gate:{offs:10,len:4},mask:{offs:14,len:1},dst:{offs:15,len:1},http_port:{offs:16,len:2},uname:{offs:18,len:18},passwd:{offs:36,len:18},community_r:{offs:54,len:18},community_w:{offs:72,len:18},filt_ip1:{offs:90,len:4},filt_mask1:{offs:94,len:1},powersaving:{offs:96,len:1},trap_refresh:{offs:97,len:1},trap_ip1:{offs:105,len:4},trap_ip2:{offs:109,len:4},ntp_ip1:{offs:113,len:4},ntp_ip2:{offs:117,len:4},timezone:{offs:121,len:1},syslog_ip1:{offs:122,len:4},facility:{offs:130,len:1},severity:{offs:131,len:1},snmp_port:{offs:132,len:2},notification_email:{offs:134,len:48},hostname:{offs:184,len:64},location:{offs:312,len:64},contact:{offs:248,len:64},dns_ip1:{offs:376,len:4},trap_hostname1:{offs:384,len:64},trap_hostname2:{offs:448,len:64},ntp_hostname1:{offs:512,len:64},ntp_hostname2:{offs:576,len:64},syslog_hostname1:{offs:640,len:64},__len:768};\n");
	sprintf(buf_temp, "var data={serial:\"SN: %6d\"", serial_id);
	strcat(buf, buf_temp);
	sprintf(buf_temp, ",serialnum:%d,", serial_id);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "mac:'00:a2:40:cd:2d:1c',");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ip:'%d.%d.%d.%d',", FW_data.net.V_IP_CONFIG[0],
			FW_data.net.V_IP_CONFIG[1], FW_data.net.V_IP_CONFIG[2],
			FW_data.net.V_IP_CONFIG[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "gate:'%d.%d.%d.%d',", FW_data.net.V_IP_GET[0],
			FW_data.net.V_IP_GET[1], FW_data.net.V_IP_GET[2],
			FW_data.net.V_IP_GET[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "mask:'%d.%d.%d.%d',", FW_data.net.V_IP_MASK[0],
			FW_data.net.V_IP_MASK[1], FW_data.net.V_IP_MASK[2],
			FW_data.net.V_IP_MASK[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "dst:0,");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "http_port:%d,", FW_data.http.V_WEB_PORT);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "uname:\"%s\",", FW_data.http.V_LOGIN);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "passwd:\"%s\",", FW_data.http.V_PASSWORD);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "community_r:\"%s\"", FW_data.snmp.V_COMMUNITY);
	strcat(buf, buf_temp);
	sprintf(buf_temp, ",community_w:\"%s\",", FW_data.snmp.V_COMMUNITY_WRITE);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "filt_ip1:'%d.%d.%d.%d',", FW_data.sys.V_IP_SOURCE[0],
			FW_data.sys.V_IP_SOURCE[1], FW_data.sys.V_IP_SOURCE[2],
			FW_data.sys.V_IP_SOURCE[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "filt_mask1:'%d.%d.%d.%d',",
			mask_dec[32 - FW_data.sys.V_MASK_SOURCE][0],
			mask_dec[32 - FW_data.sys.V_MASK_SOURCE][1],
			mask_dec[32 - FW_data.sys.V_MASK_SOURCE][2],
			mask_dec[32 - FW_data.sys.V_MASK_SOURCE][3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "powersaving:0,");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "trap_refresh:%d,", FW_data.snmp.V_REFR_TRAP);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "trap_ip1:'%d.%d.%d.%d',", FW_data.snmp.V_IP_SNMP[0],
			FW_data.snmp.V_IP_SNMP[1], FW_data.snmp.V_IP_SNMP[2],
			FW_data.snmp.V_IP_SNMP[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "trap_ip2:'%d.%d.%d.%d',", FW_data.snmp.V_IP_SNMP_S[0],
			FW_data.snmp.V_IP_SNMP_S[1], FW_data.snmp.V_IP_SNMP_S[2],
			FW_data.snmp.V_IP_SNMP_S[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ntp_ip1:'%d.%d.%d.%d',", FW_data.net.V_IP_NTP1[0],
			FW_data.net.V_IP_NTP1[1], FW_data.net.V_IP_NTP1[2],
			FW_data.net.V_IP_NTP1[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "ntp_ip2:'%d.%d.%d.%d',", FW_data.net.V_IP_NTP2[0],
			FW_data.net.V_IP_NTP2[1], FW_data.net.V_IP_NTP2[2],
			FW_data.net.V_IP_NTP2[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "timezone:%d,", FW_data.sys.V_NTP_CIRCL);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "syslog_ip1:'10%d.%d.%d.%d',", FW_data.net.V_IP_SYSL[0],
			FW_data.net.V_IP_SYSL[1], FW_data.net.V_IP_SYSL[2],
			FW_data.net.V_IP_SYSL[3]);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "facility:16,severity:6,");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "snmp_port:%d,", FW_data.snmp.V_PORT_SNMP);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "notification_email:\"\"");
	strcat(buf, buf_temp);
	sprintf(buf_temp, ",hostname:\"%s\",", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "location:\"%s\",", FW_data.sys.V_GEOM_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "contact:\"%s\",", FW_data.sys.V_CALL_DATA);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "dns_ip1:'%d.%d.%d.%d',", FW_data.net.V_IP_DNS[0],
			FW_data.net.V_IP_DNS[1], FW_data.net.V_IP_DNS[2],
			FW_data.net.V_IP_DNS[3]);
	strcat(buf, buf_temp);
//	sprintf(buf_temp, "trap_hostname1:\"\",");
//	strcat(buf, buf_temp);
//	sprintf(buf_temp, "trap_hostname2:\"\",");
//	strcat(buf, buf_temp);
//	sprintf(buf_temp, "ntp_hostname1:\"ntp.netping.ru\",");
//	strcat(buf, buf_temp);
//	sprintf(buf_temp, "ntp_hostname2:\"\",");
//	strcat(buf, buf_temp);
	sprintf(buf_temp, "syslog_hostname1:\"%d.%d.%d.%d\"};",
			FW_data.net.V_IP_SYSL[0], FW_data.net.V_IP_SYSL[1],
			FW_data.net.V_IP_SYSL[2], FW_data.net.V_IP_SYSL[3]);
	strcat(buf, buf_temp);
	uint32_t rtc_time;
	rtc_time = timeinfo.tm_sec + timeinfo.tm_min * 60 + timeinfo.tm_hour * 3600
			+ timeinfo.tm_yday * 86400 + (timeinfo.tm_year - 70) * 31557600;
	sprintf(buf_temp, "var data_rtc=%d;", rtc_time);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var uptime_100ms=%d;", rtc_time - timeup);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_name='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_location='%s';", FW_data.sys.V_GEOM_NAME);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var hwmodel=110;");
	strcat(buf, buf_temp);

	sprintf(buf_temp, "var fwver='v%.31s';", app_desc.version);
	strcat(buf, buf_temp);

	sprintf(buf_temp, "var hwver=1;");
	strcat(buf, buf_temp);

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t rtcset_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\settings.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");

	char buf[1000] = { 0 };
	char buf_temp[256] = { 0 };
	int ret, remaining = req->content_len;

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
//				if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//					/* Retry receiving if timeout occurred */
//					continue;
//				}
//				return ESP_FAIL;
	}
	char2_to_hex((char*) (buf + 9), (uint8_t*) buf_temp, 8);
	struct timeval tv;
	struct timezone tz;
	struct timezone tz_utc = { 0, 0 };

	// Set a fixed time of 2020-09-26 00:00:00, UTC  //1601216683
	tv.tv_sec = ((buf_temp[0] << 24) | (buf_temp[1] << 16) | (buf_temp[2] << 8)
			| (buf_temp[3]));
	tz.tz_minuteswest = FW_data.sys.V_NTP_CIRCL * 60;
	tz.tz_dsttime = 0;
	settimeofday(&tv, &tz_utc);
	tzset();

	time(&now);
	localtime_r(&now, &timeinfo);
	timeup = timeinfo.tm_sec + timeinfo.tm_min * 60 + timeinfo.tm_hour * 3600
			+ timeinfo.tm_yday * 86400 + (timeinfo.tm_year - 70) * 31557600;

	//rtc_time=timeinfo.tm_sec+timeinfo.tm_min*60+timeinfo.tm_hour*3600+timeinfo.tm_yday*86400+(timeinfo.tm_year-70)*31557600;
//	memset(FW_data.http.V_LOGIN, 0, 16);
//	memcpy(FW_data.http.V_LOGIN, (char*) (buf + 19), buf[18]);
//	memset(FW_data.http.V_PASSWORD, 0, 16);
//	memcpy(FW_data.http.V_PASSWORD, (char*) (buf + 37), buf[36]);
//
//
//
//	memset(FW_data.snmp.V_COMMUNITY, 0, 16);
//	memcpy(FW_data.snmp.V_COMMUNITY, (char*) (buf + 55), buf[54]);
//	memset(FW_data.snmp.V_COMMUNITY_WRITE, 0, 16);
//    memcpy(FW_data.snmp.V_COMMUNITY_WRITE, (char*) (buf + 73), buf[72]);
//
//	FW_data.sys.V_IP_SOURCE[0] = buf[90];
//	FW_data.sys.V_IP_SOURCE[1] = buf[91];
//	FW_data.sys.V_IP_SOURCE[2] = buf[92];
//	FW_data.sys.V_IP_SOURCE[3] = buf[93];
//
//	FW_data.sys.V_MASK_SOURCE = buf[94];
//
//	FW_data.snmp.V_IP_SNMP[0]= buf[105];
//	FW_data.snmp.V_IP_SNMP[1]= buf[106];
//	FW_data.snmp.V_IP_SNMP[2]= buf[107];
//	FW_data.snmp.V_IP_SNMP[3]= buf[108];
//
//	FW_data.snmp.V_IP_SNMP_S[0]= buf[109];
//	FW_data.snmp.V_IP_SNMP_S[1]= buf[110];
//	FW_data.snmp.V_IP_SNMP_S[2]= buf[111];
//	FW_data.snmp.V_IP_SNMP_S[3]= buf[112];
//
//	FW_data.net.V_IP_NTP1[0]= buf[113];
//	FW_data.net.V_IP_NTP1[1]= buf[114];
//	FW_data.net.V_IP_NTP1[2]= buf[115];
//	FW_data.net.V_IP_NTP1[3]= buf[116];
//
//	FW_data.net.V_IP_NTP2[0]= buf[117];
//	FW_data.net.V_IP_NTP2[1]= buf[118];
//	FW_data.net.V_IP_NTP2[2]= buf[119];
//	FW_data.net.V_IP_NTP2[3]= buf[120];
//
//
//	FW_data.sys.V_NTP_CIRCL = buf[121];
//
//	FW_data.net.V_IP_SYSL[0] = buf[122];
//	FW_data.net.V_IP_SYSL[1] = buf[123];
//	FW_data.net.V_IP_SYSL[2] = buf[124];
//	FW_data.net.V_IP_SYSL[3] = buf[125];
//	FW_data.sys.V_L_TIME = buf[15];

	nvs_flags.data_param = 1;

	//	}

	// End response
	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}
static esp_err_t ip_set_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\settings.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");
	char buf[1000];

	int ret, remaining = req->content_len;

	//	while (remaining > 0) {
	/* Read the data for the request */
	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
//				if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//					/* Retry receiving if timeout occurred */
//					continue;
//				}
//				return ESP_FAIL;
	}

	FW_data.net.V_IP_CONFIG[0] = buf[6];
	FW_data.net.V_IP_CONFIG[1] = buf[7];
	FW_data.net.V_IP_CONFIG[2] = buf[8];
	FW_data.net.V_IP_CONFIG[3] = buf[9];

	FW_data.net.V_IP_GET[0] = buf[10];
	FW_data.net.V_IP_GET[1] = buf[11];
	FW_data.net.V_IP_GET[2] = buf[12];
	FW_data.net.V_IP_GET[3] = buf[13];

	uint32_t mask_temp = 0xffffffff << (32 - buf[14]);

	FW_data.net.V_IP_MASK[3] = mask_temp & 0x000000ff;
	FW_data.net.V_IP_MASK[2] = 0x000000ff & (mask_temp >> 8);
	FW_data.net.V_IP_MASK[1] = 0x000000ff & (mask_temp >> 16);
	FW_data.net.V_IP_MASK[0] = 0x000000ff & (mask_temp >> 24);
	FW_data.smtp.V_FLAG_DEF_EMAIL = 1;
	FW_data.smtp.V_FLAG_EN_EMAIL = 1;
	FW_data.http.V_WEB_PORT = buf[16];
	FW_data.snmp.V_PORT_SNMP = buf[132];
//	save_data_blok();
	nvs_flags.data_param = 1;

//		/* Send back the same data */
//		httpd_resp_send_chunk(req, buf, ret);
//		remaining -= ret;
//
////        /* Log data received */
////        ESP_LOGI(TAG_http, "=========== RECEIVED DATA ==========");
////        ESP_LOGI(TAG_http, "%.*s", ret, buf);
////        ESP_LOGI(TAG_http, "====================================");
//
//		err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
//		err = nvs_set_blob(my_handle, "ip_set", buf, req->content_len);
//		if (err != ESP_OK)
//			printf("Error (%s) saving restart counter to NVS!\n",
//					esp_err_to_name(err));
//		err = nvs_commit(my_handle);
//		if (err != ESP_OK)
//			printf("Error (%s) saving restart counter to NVS!\n",
//					esp_err_to_name(err));
//		nvs_close(my_handle);

	//	}
	// End response
	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}
//sprintf(buf_temp, "{signature:2893295841,");
//sprintf(buf_temp, "name:\"%s\",",FW_data.wdt[0].V_NAME);
//sprintf(buf_temp, "output:11009,");
//sprintf(buf_temp, "ip0:'8.8.8.8',");
//sprintf(buf_temp, "ip1:'192.168.0.99',");
//sprintf(buf_temp, "ip2:'98.98.8.8',");
//sprintf(buf_temp, "poll_period:%d,",FW_data.wdt[0].V_T_SEND_PING);
//sprintf(buf_temp, "ping_timeout:%d,",FW_data.wdt[0].V_TIME_RESEND_PING);
//sprintf(buf_temp, "reset_time:%d,",FW_data.wdt[0].V_TIME_RESET_PULSE);
//sprintf(buf_temp, "reboot_pause:%d,",FW_data.wdt[0].V_PAUSE_RESET_TO_REPID);
//sprintf(buf_temp, "max_retry:%d,",FW_data.wdt[0].V_MAX_REPID_PING);
//sprintf(buf_temp, "doubling_pause_resets:%d,",FW_data.wdt[0].V_MAX_RESEND_PACET_RESET);
//sprintf(buf_temp, "reset_mode:%d,",FW_data.sys.V_TYPE_OUT);
//activ=((FW_data.wdt[0].V_EN_WATCHDOG<<8)|(FW_data.wdt[0].V_EN_WATCHDOG_CN_A<<2)|(FW_data.wdt[0].V_EN_WATCHDOG_CN_B<<1)|(FW_data.wdt[0].V_EN_WATCHDOG_CN_C));
//sprintf(buf_temp, "active:%d,",activ);
//sprintf(buf_temp, "logic_mode:%d,",FW_data.wdt[0].V_TYPE_LOGIC);
//sprintf(buf_temp, "fqdn0:\"%d.%d.%d.%d\",",FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[0],FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[1],FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[2],FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[3]);
//sprintf(buf_temp, "fqdn1:\"%d.%d.%d.%d\",",FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[0],FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[1],FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[2],FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[3]);
//sprintf(buf_temp, "fqdn2:\"%d.%d.%d.%d\",",FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[0],FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[1],FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[2],FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[3]);
//sprintf(buf_temp, "reset_count:%d}",FW_data.wdt[0].V_CT_RES_ALLSTART);

static esp_err_t wdog_set_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	uint8_t len;
	char buf[2048];
	char buf_temp[1024] = { 0 };
	io_set_t data;
	int ret, remaining = req->content_len;
	uint8_t ct;

	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\wdog.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {

	}

	len = read_mess_smtp((char*) (buf + 13), (uint8_t*) buf_temp);
	memset(FW_data.wdt[0].V_NAME, 0, 16);
	memcpy(FW_data.wdt[0].V_NAME, (char*) (buf_temp), len);
	char2_to_hex((char*) (buf + 13 + 64), (uint8_t*) buf_temp, 219);
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[0] = buf_temp[0];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[1] = buf_temp[1];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[2] = buf_temp[2];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_A[3] = buf_temp[3];

	FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[0] = buf_temp[4];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[1] = buf_temp[5];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[2] = buf_temp[6];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_B[3] = buf_temp[7];

	FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[0] = buf_temp[8];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[1] = buf_temp[9];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[2] = buf_temp[10];
	FW_data.wdt[0].V_IP_WDT_ADDR_CN_C[3] = buf_temp[11];

	FW_data.wdt[0].V_T_SEND_PING = (buf_temp[13] << 8) | buf_temp[12];
	FW_data.wdt[0].V_TIME_RESEND_PING = (buf_temp[15] << 8) | buf_temp[14];
	FW_data.wdt[0].V_TIME_RESET_PULSE = (buf_temp[17] << 8) | buf_temp[16];
	FW_data.wdt[0].V_PAUSE_RESET_TO_REPID = (buf_temp[19] << 8) | buf_temp[18];
	FW_data.wdt[0].V_MAX_REPID_PING = buf_temp[20];
	FW_data.wdt[0].V_MAX_RESEND_PACET_RESET = buf_temp[21];
	FW_data.wdt[0].V_EN_WATCHDOG_CN_A = (buf_temp[23]) & 0x01;
	FW_data.wdt[0].V_EN_WATCHDOG_CN_B = (buf_temp[23] >> 1) & 0x01;
	FW_data.wdt[0].V_EN_WATCHDOG_CN_C = (buf_temp[23] >> 2) & 0x01;
	FW_data.wdt[0].V_EN_WATCHDOG = (buf_temp[23] >> 7) & 0x01;
	FW_data.wdt[0].V_N_OUT = (buf_temp[27] << 8) | buf_temp[26];
	FW_data.wdt[0].V_TYPE_LOGIC = buf_temp[24];

	len = read_mess_smtp((char*) (buf + 525), (uint8_t*) buf_temp);
	memset(FW_data.wdt[1].V_NAME, 0, 16);
	memcpy(FW_data.wdt[1].V_NAME, (char*) (buf_temp), len);
	char2_to_hex((char*) (buf + 589), (uint8_t*) buf_temp, 219);
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[0] = buf_temp[0];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[1] = buf_temp[1];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[2] = buf_temp[2];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_A[3] = buf_temp[3];

	FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[0] = buf_temp[4];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[1] = buf_temp[5];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[2] = buf_temp[6];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_B[3] = buf_temp[7];

	FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[0] = buf_temp[8];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[1] = buf_temp[9];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[2] = buf_temp[10];
	FW_data.wdt[1].V_IP_WDT_ADDR_CN_C[3] = buf_temp[11];

	FW_data.wdt[1].V_T_SEND_PING = (buf_temp[13] << 8) | buf_temp[12];
	FW_data.wdt[1].V_TIME_RESEND_PING = (buf_temp[15] << 8) | buf_temp[14];
	FW_data.wdt[1].V_TIME_RESET_PULSE = (buf_temp[17] << 8) | buf_temp[16];
	FW_data.wdt[1].V_PAUSE_RESET_TO_REPID = (buf_temp[19] << 8) | buf_temp[18];
	FW_data.wdt[1].V_MAX_REPID_PING = buf_temp[20];
	FW_data.wdt[1].V_MAX_RESEND_PACET_RESET = buf_temp[21];
	FW_data.wdt[1].V_EN_WATCHDOG_CN_A = (buf_temp[23]) & 0x01;
	FW_data.wdt[1].V_EN_WATCHDOG_CN_B = (buf_temp[23] >> 1) & 0x01;
	FW_data.wdt[1].V_EN_WATCHDOG_CN_C = (buf_temp[23] >> 2) & 0x01;
	FW_data.wdt[1].V_EN_WATCHDOG = (buf_temp[23] >> 7) & 0x01;
	FW_data.wdt[1].V_N_OUT = (buf_temp[27] << 8) | buf_temp[26];
	FW_data.wdt[1].V_TYPE_LOGIC = buf_temp[24];

	nvs_flags.data_param = 1;

	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

static esp_err_t termo_set_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	uint8_t len;
	char buf[2048];
	char buf_temp[1024] = { 0 };
	io_set_t data;
	int ret, remaining = req->content_len;
	uint8_t ct;

	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\termo.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {

	}
	len = read_mess_smtp((char*) (buf + 5), (uint8_t*) buf_temp);
	memset(FW_data.termo[0].name, 0, 16);
	memcpy(FW_data.termo[0].name, (char*) (buf_temp), len);

	len = read_mess_smtp((char*) (buf + 73), (uint8_t*) buf_temp);
	memset(FW_data.termo[1].name, 0, 16);
	memcpy(FW_data.termo[1].name, (char*) (buf_temp), len);

	char2_to_hex((char*) (buf + 40), (uint8_t*) buf_temp, 35);

	FW_data.termo[0].t_dw = buf[41] << 4 | buf[42];
	FW_data.termo[0].t_up = buf[43] << 4 | buf[44];

	char2_to_hex((char*) (buf + 100), (uint8_t*) buf_temp, 70);
	FW_data.termo[1].t_dw = buf[109] << 4 | buf[110];
	FW_data.termo[1].t_up = buf[111] << 4 | buf[112];

	nvs_flags.data_param = 1;

	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

static esp_err_t notify_set_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	uint8_t len;
	char buf[2048];
	char buf_temp[1024] = { 0 };
	io_set_t data;
	int ret, remaining = req->content_len;
	uint8_t ct;

	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\wdog.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {

	}
	uint8_t canal;

	if (page_sost == WDT) {
		char2_to_hex((char*) (buf + 7), (uint8_t*) buf_temp, 25);
		canal = buf_temp[0];
		FW_data.wdt[canal].V_EVENT_L = (buf_temp[3] >> 0) & 0x01;
		FW_data.wdt[canal].V_EVENT_SL = (buf_temp[3] >> 1) & 0x01;
		FW_data.wdt[canal].V_EVENT_E = (buf_temp[3] >> 2) & 0x01;
		FW_data.wdt[canal].V_EVENT_S = (buf_temp[3] >> 3) & 0x01;
		FW_data.wdt[canal].V_EVENT_T = (buf_temp[3] >> 4) & 0x01;

		FW_data.wdt[canal].V_RESET_L = (buf_temp[1] >> 0) & 0x01;
		FW_data.wdt[canal].V_RESET_SL = (buf_temp[1] >> 1) & 0x01;
		FW_data.wdt[canal].V_RESET_E = (buf_temp[1] >> 2) & 0x01;
		FW_data.wdt[canal].V_RESET_S = (buf_temp[1] >> 3) & 0x01;
		FW_data.wdt[canal].V_RESET_T = (buf_temp[1] >> 4) & 0x01;

		FW_data.wdt[canal].V_RELOG_E = (buf_temp[5] >> 2) & 0x01;
	}
	if (page_sost == TERMO) {
		char2_to_hex((char*) (buf + 5), (uint8_t*) buf_temp, 25);
		canal = buf_temp[1];
		FW_data.termo[canal].TEMP_UP_L = (buf_temp[2] >> 0) & 0x01;
		FW_data.termo[canal].TEMP_UP_SL = (buf_temp[2] >> 1) & 0x01;
		FW_data.termo[canal].TEMP_UP_E = (buf_temp[2] >> 2) & 0x01;
		FW_data.termo[canal].TEMP_UP_SM = (buf_temp[2] >> 3) & 0x01;
		FW_data.termo[canal].TEMP_UP_SN = (buf_temp[2] >> 4) & 0x01;

		FW_data.termo[canal].TEMP_OK_L = (buf_temp[4] >> 0) & 0x01;
		FW_data.termo[canal].TEMP_OK_SL = (buf_temp[4] >> 1) & 0x01;
		FW_data.termo[canal].TEMP_OK_E = (buf_temp[4] >> 2) & 0x01;
		FW_data.termo[canal].TEMP_OK_SM = (buf_temp[4] >> 3) & 0x01;
		FW_data.termo[canal].TEMP_OK_SN = (buf_temp[4] >> 4) & 0x01;

		FW_data.termo[canal].TEMP_DW_L = (buf_temp[6] >> 0) & 0x01;
		FW_data.termo[canal].TEMP_DW_SL = (buf_temp[6] >> 1) & 0x01;
		FW_data.termo[canal].TEMP_DW_E = (buf_temp[6] >> 2) & 0x01;
		FW_data.termo[canal].TEMP_DW_SM = (buf_temp[6] >> 3) & 0x01;
		FW_data.termo[canal].TEMP_DW_SN = (buf_temp[6] >> 4) & 0x01;

		FW_data.termo[canal].TEMP_UP_L = (buf_temp[8] >> 0) & 0x01;
		FW_data.termo[canal].TEMP_ERR_SL = (buf_temp[8] >> 1) & 0x01;
		FW_data.termo[canal].TEMP_ERR_E = (buf_temp[8] >> 2) & 0x01;
		FW_data.termo[canal].TEMP_ERR_SM = (buf_temp[8] >> 3) & 0x01;
		FW_data.termo[canal].TEMP_ERR_SN = (buf_temp[8] >> 4) & 0x01;

		FW_data.termo[canal].TEMP_CIKL_E = (buf_temp[10] >> 2) & 0x01;

		FW_data.termo[canal].repit_3r = buf_temp[15];

	}
	if (page_sost == IO) {

		char2_to_hex((char*) (buf + 5), (uint8_t*) buf_temp, 2);
		canal = buf_temp[1];
		len = read_mess_smtp((char*) (buf + 9), (uint8_t*) buf_temp);
		memset(FW_data.gpio.mess_hi[canal], 0, 16);
		memcpy(FW_data.gpio.mess_hi[canal], (char*) (buf_temp), len);
		len = read_mess_smtp((char*) (buf + 41), (uint8_t*) buf_temp);
		memset(FW_data.gpio.mess_low[canal], 0, 16);
		memcpy(FW_data.gpio.mess_low[canal], (char*) (buf_temp), len);
		char2_to_hex((char*) (buf + 73), (uint8_t*) buf_temp, 20);
		FW_data.gpio.RISE_L[canal] = (buf_temp[0] >> 0) & 0x01;
		FW_data.gpio.RISE_SL[canal] = (buf_temp[0] >> 1) & 0x01;
		FW_data.gpio.RISE_E[canal] = (buf_temp[0] >> 2) & 0x01;
		FW_data.gpio.RISE_SM[canal] = (buf_temp[0] >> 3) & 0x01;
		FW_data.gpio.RISE_SN[canal] = (buf_temp[0] >> 4) & 0x01;

		FW_data.gpio.FALL_L[canal] = (buf_temp[2] >> 0) & 0x01;
		FW_data.gpio.FALL_SL[canal] = (buf_temp[2] >> 1) & 0x01;
		FW_data.gpio.FALL_E[canal] = (buf_temp[2] >> 2) & 0x01;
		FW_data.gpio.FALL_SM[canal] = (buf_temp[2] >> 3) & 0x01;
		FW_data.gpio.FALL_SN[canal] = (buf_temp[2] >> 4) & 0x01;

		FW_data.gpio.CIKL_E[canal] = (buf_temp[2] >> 2) & 0x01;

		FW_data.gpio.SET_COLOR[canal] = (buf_temp[8] >> 4) & 0x0f;
		FW_data.gpio.CLR_COLOR[canal] = (buf_temp[8]) & 0x0f;
		FW_data.gpio.reactiv[canal] = buf_temp[9];
		FW_data.gpio.cicle_t[canal] = buf_temp[10];

	}

	nvs_flags.data_param = 1;

	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

static esp_err_t io_set_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	uint8_t len;
	char buf[1000];
	char buf_temp[256] = { 0 };
	io_set_t data;
	int ret, remaining = req->content_len;
	uint8_t ct;

	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\io.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {

	}

	for (ct = 0; ct < (in_port_n + out_port_n); ct++) {
		len = read_mess_smtp((char*) (buf + ct * 96 + 5), (uint8_t*) buf_temp);

		memset(&(FW_data.gpio.name[ct][0]), 0, 32);
		memcpy(&(FW_data.gpio.name[ct][0]), (char*) (buf_temp), len);
		memset(buf_temp, 0, 256);

		char2_to_hex((char*) (buf + 84 + ct * 96), (uint8_t*) buf_temp, 5);
		FW_data.gpio.dir[ct] = buf[90 + ct * 96];
		if (FW_data.gpio.dir[ct] == 0) {
			if (ct < out_port_n) {
				FW_data.gpio.OUT_PORT[ct].input_str.filtr_time = ((buf[ct * 96
						+ 88] << 8) | (buf[ct * 96 + 85] << 4)
						| (buf[ct * 96 + 86]));
			} else {
				FW_data.gpio.IN_PORT[ct - 2].filtr_time = ((buf[ct * 96 + 88]
						<< 8) | (buf[ct * 96 + 85] << 4) | (buf[ct * 96 + 86]));
			}

		} else {
			FW_data.gpio.OUT_PORT[ct].input_str.filtr_time = ((buf[ct * 96 + 88]
					<< 8) | (buf[ct * 96 + 85] << 4) | (buf[ct * 96 + 86]));
			FW_data.gpio.OUT_PORT[ct].sost = buf[92 + ct * 96];
			FW_data.gpio.OUT_PORT[ct].aflag = 1;
			char2_to_hex((char*) (buf + 94 + ct * 96), (uint8_t*) buf_temp, 2);
			FW_data.gpio.OUT_PORT[ct].delay = ((buf[ct * 96 + 95] << 4)
					| (buf[ct * 96 + 96])) * 100;
		}

		if (FW_data.gpio.OUT_PORT[ct].sost
				> FW_data.gpio.OUT_PORT[ct].old_sost) {
			FW_data.gpio.OUT_PORT[ct].old_sost = FW_data.gpio.OUT_PORT[ct].sost;
			//FW_data.gpio.OUT_PORT[ct].event = WEB_OUT_PORT0_SET + ct;
							reple_to_save.type_event = OUT_SET;
							reple_to_save.event_cfg.canal = ct;
							reple_to_save.event_cfg.source=WEB;
							reple_to_save.dicr = 1;


		} else if (FW_data.gpio.OUT_PORT[ct].sost
				< FW_data.gpio.OUT_PORT[ct].old_sost) {
			FW_data.gpio.OUT_PORT[ct].old_sost = FW_data.gpio.OUT_PORT[ct].sost;
		//	FW_data.gpio.OUT_PORT[ct].event = WEB_OUT_PORT0_RES + ct;
			reple_to_save.type_event = OUT_RES;
    		reple_to_save.event_cfg.canal = ct;
			reple_to_save.event_cfg.source=WEB;
			reple_to_save.dicr = 1;




		}

	}

	nvs_flags.data_param = 1;

	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

static esp_err_t io_set_pulse_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	uint8_t len;
	char buf[1000];
	char buf_temp[256] = { 0 };
	io_set_t data;
	int ret, remaining = req->content_len;
	uint8_t ct;

	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\io.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {

	}

	if (buf[6] == 0x31) {
		char2_to_hex((char*) (buf + 7), (uint8_t*) buf_temp, 2);
		FW_data.gpio.OUT_PORT[1].delay = ((buf[7] << 4) | (buf[8])) * 100;
		FW_data.gpio.OUT_PORT[1].event = OUT_TOL;
		reple_to_save.type_event = OUT_TOL;
					reple_to_save.event_cfg.canal = 1;
					reple_to_save.event_cfg.source = WEB;
					reple_to_save.dicr = 1;
		FW_data.gpio.OUT_PORT[1].sost = 1;
		FW_data.gpio.OUT_PORT[1].type_logic = 3;
		FW_data.gpio.OUT_PORT[1].aflag = 1;

	} else {
		char2_to_hex((char*) (buf + 7), (uint8_t*) buf_temp, 2);
		FW_data.gpio.OUT_PORT[0].delay = ((buf[7] << 4) | (buf[8])) * 100;
		FW_data.gpio.OUT_PORT[0].event = OUT_TOL;
		reple_to_save.type_event = OUT_TOL;
							reple_to_save.event_cfg.canal = 0;
							reple_to_save.event_cfg.source = WEB;
							reple_to_save.dicr = 1;
		FW_data.gpio.OUT_PORT[0].sost = 1;
		FW_data.gpio.OUT_PORT[0].type_logic = 3;
		FW_data.gpio.OUT_PORT[0].aflag = 1;
	}

	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

static esp_err_t setup_set_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_hdr(req, "Location", "\settings.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_sse);
	httpd_resp_set_hdr(req, "Connection", "Close");
	char buf[1000];
	int ret, remaining = req->content_len;

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
//				if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//					/* Retry receiving if timeout occurred */
//					continue;
//				}
//				return ESP_FAIL;
	}

	memset(FW_data.http.V_LOGIN, 0, 16);
	memcpy(FW_data.http.V_LOGIN, (char*) (buf + 19), buf[18]);
	memset(FW_data.http.V_PASSWORD, 0, 16);
	memcpy(FW_data.http.V_PASSWORD, (char*) (buf + 37), buf[36]);

	memset(FW_data.snmp.V_COMMUNITY, 0, 16);
	memcpy(FW_data.snmp.V_COMMUNITY, (char*) (buf + 55), buf[54]);
	memset(FW_data.snmp.V_COMMUNITY_WRITE, 0, 16);
	memcpy(FW_data.snmp.V_COMMUNITY_WRITE, (char*) (buf + 73), buf[72]);

	FW_data.sys.V_IP_SOURCE[0] = buf[90];
	FW_data.sys.V_IP_SOURCE[1] = buf[91];
	FW_data.sys.V_IP_SOURCE[2] = buf[92];
	FW_data.sys.V_IP_SOURCE[3] = buf[93];

	FW_data.sys.V_MASK_SOURCE = buf[94];

	FW_data.snmp.V_IP_SNMP[0] = buf[105];
	FW_data.snmp.V_IP_SNMP[1] = buf[106];
	FW_data.snmp.V_IP_SNMP[2] = buf[107];
	FW_data.snmp.V_IP_SNMP[3] = buf[108];

	FW_data.snmp.V_IP_SNMP_S[0] = buf[109];
	FW_data.snmp.V_IP_SNMP_S[1] = buf[110];
	FW_data.snmp.V_IP_SNMP_S[2] = buf[111];
	FW_data.snmp.V_IP_SNMP_S[3] = buf[112];

	FW_data.net.V_IP_NTP1[0] = buf[113];
	FW_data.net.V_IP_NTP1[1] = buf[114];
	FW_data.net.V_IP_NTP1[2] = buf[115];
	FW_data.net.V_IP_NTP1[3] = buf[116];

	FW_data.net.V_IP_NTP2[0] = buf[117];
	FW_data.net.V_IP_NTP2[1] = buf[118];
	FW_data.net.V_IP_NTP2[2] = buf[119];
	FW_data.net.V_IP_NTP2[3] = buf[120];

	FW_data.sys.V_NTP_CIRCL = buf[121];

	FW_data.net.V_IP_SYSL[0] = buf[122];
	FW_data.net.V_IP_SYSL[1] = buf[123];
	FW_data.net.V_IP_SYSL[2] = buf[124];
	FW_data.net.V_IP_SYSL[3] = buf[125];
	FW_data.sys.V_L_TIME = buf[15];

//		        FW_data.net.V_IP_CONFIG[0] = buf[6];
//				FW_data.net.V_IP_CONFIG[1] = buf[7];
//				FW_data.net.V_IP_CONFIG[2] = buf[8];
//				FW_data.net.V_IP_CONFIG[3] = buf[9];
//
//				FW_data.net.V_IP_GET[0] = buf[10];
//				FW_data.net.V_IP_GET[1] = buf[11];
//				FW_data.net.V_IP_GET[2] = buf[12];
//				FW_data.net.V_IP_GET[3] = buf[13];
//
//				uint32_t mask_temp = 0xffffffff << (32 - buf[14]);
//
//				FW_data.net.V_IP_MASK[3] = mask_temp & 0x000000ff;
//				FW_data.net.V_IP_MASK[2] = 0x000000ff & (mask_temp >> 8);
//				FW_data.net.V_IP_MASK[1] = 0x000000ff & (mask_temp >> 16);
//				FW_data.net.V_IP_MASK[0] = 0x000000ff & (mask_temp >> 24);
//				FW_data.smtp.V_FLAG_DEF_EMAIL = 1;
//				FW_data.smtp.V_FLAG_EN_EMAIL = 1;

	nvs_flags.data_param = 1;

	//	}

	// End response
	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

//void Bcd_To_Hex(unsigned char *Input_Buff, unsigned char *Output_Buff,
//		unsigned int Len) { // by Neeraj Verma : neeraj.niet@gmail.com
//	unsigned int i, j;
//	for (i = 0, j = 0; i < Len; i++, j++) {
//		if ((i + 1) >= Len) {
//			if (*(Input_Buff + Len - 1 - i) > 0x40)
//				*(Output_Buff + j) = (*(Input_Buff + Len - 1 - i) - 0x37);
//			else
//				*(Output_Buff + j) = (*(Input_Buff + Len - 1 - i) - 0x30);
//		} else {
//			if (*(Input_Buff + Len - 1 - i) > 0x40) {
//				if (*(Input_Buff + Len - 2 - i) > 0x40) {
//					*(Output_Buff + j) = ((*(Input_Buff + Len - i - 2) - 0x37)
//							<< 4) | (*(Input_Buff + Len - 1 - i) - 0x37);
//				} else {
//					*(Output_Buff + j) = ((*(Input_Buff + Len - i - 2) - 0x30)
//							<< 4) | (*(Input_Buff + Len - 1 - i) - 0x37);
//				}
//			} else {
//				if (*(Input_Buff + Len - 2 - i) > 0x40) {
//					*(Output_Buff + j) = ((*(Input_Buff + Len - i - 2) - 0x37)
//							<< 4) | (*(Input_Buff + Len - 1 - i) - 0x30);
//				} else {
//					*(Output_Buff + j) = ((*(Input_Buff + Len - i - 2) - 0x30)
//							<< 4) | (*(Input_Buff + Len - 1 - i) - 0x30);
//				}
//			}
//		}
//		i++;
//	}
//}

static esp_err_t sendmail_set_post_handler(httpd_req_t *req) {
	esp_err_t err;
	nvs_handle_t my_handle;
	uint8_t len;
//	httpd_resp_set_status(req, HTTPD_200);
//	httpd_resp_set_status(req, "303 See Other");
	httpd_resp_set_status(req, "302 Temporary Redirect");
	httpd_resp_set_hdr(req, "Location", "\sendmail.html");
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store, no-cache, must-revalidate");
	httpd_resp_set_type(req, mime_html);
	httpd_resp_set_hdr(req, "Connection", "Close");
	char buf[1000] = { 0 };
	char buf_temp[256] = { 0 };
	memset(buf, 0, 1000);
	int ret, remaining = req->content_len;

	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {

	}

	len = read_mess_smtp((char*) (buf + 5), (uint8_t*) buf_temp);

	memset(FW_data.smtp.V_EMAIL_ADDR, 0, 32);
	memcpy(FW_data.smtp.V_EMAIL_ADDR, (char*) (buf_temp), len);
	memset(buf_temp, 0, 256);
	char2_to_hex((char*) (buf + 133), (uint8_t*) buf_temp, 2);
	FW_data.smtp.V_FLAG_EMAIL_PORT = ((buf_temp[1] << 8) | buf_temp[0]);
	memset(buf_temp, 0, 256);
	len = read_mess_smtp((char*) (buf + 141), (uint8_t*) buf_temp);
	memset(FW_data.smtp.V_LOGIN_SMTP, 0, 32);
	memcpy(FW_data.smtp.V_LOGIN_SMTP, (char*) (buf_temp), len);
	memset(buf_temp, 0, 256);
	len = read_mess_smtp((char*) (buf + 237), (uint8_t*) buf_temp);
	memset(FW_data.smtp.V_PASSWORD_SMTP, 0, 32);
	memcpy(FW_data.smtp.V_PASSWORD_SMTP, (char*) (buf_temp), len);
	memset(buf_temp, 0, 256);
	len = read_mess_smtp((char*) (buf + 301), (uint8_t*) buf_temp);
	memset(FW_data.smtp.V_EMAIL_FROM, 0, 32);
	memcpy(FW_data.smtp.V_EMAIL_FROM, (char*) (buf_temp), len);
	memset(buf_temp, 0, 256);
	len = read_mess_smtp((char*) (buf + 397), (uint8_t*) buf_temp);
	memset(FW_data.smtp.V_EMAIL_TO, 0, 32);
	memcpy(FW_data.smtp.V_EMAIL_TO, (char*) (buf_temp), len);
	memset(buf_temp, 0, 256);
	len = read_mess_smtp((char*) (buf + 621), (uint8_t*) buf_temp);
	memset(FW_data.smtp.V_EMAIL_CC1, 0, 32);
	memcpy(FW_data.smtp.V_EMAIL_CC1, (char*) (buf_temp), len);
	memset(buf_temp, 0, 256);
	len = read_mess_smtp((char*) (buf + 717), (uint8_t*) buf_temp);
	memset(FW_data.smtp.V_EMAIL_CC2, 0, 32);
	memcpy(FW_data.smtp.V_EMAIL_CC2, (char*) (buf_temp), len);
	memset(buf_temp, 0, 256);
	len = read_mess_smtp((char*) (buf + 813), (uint8_t*) buf_temp);
	memset(FW_data.smtp.V_EMAIL_CC3, 0, 32);
	memcpy(FW_data.smtp.V_EMAIL_CC3, (char*) (buf_temp), len);
	uint8_t flag = buf[138] - 0x30;
	if ((flag & 0x01) != 0) {
		FW_data.smtp.V_FLAG_DEF_EMAIL = 1;
	} else {
		FW_data.smtp.V_FLAG_DEF_EMAIL = 0;
	}
	if ((flag & 0x02) != 0) {
		FW_data.smtp.V_FLAG_EN_EMAIL = 1;
	} else {
		FW_data.smtp.V_FLAG_EN_EMAIL = 0;
	}
	nvs_flags.data_param = 1;
//	save_data_blok();

	//	}

	// End response
	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

// HTTP Error (404) Handler - Redirects all requests to the root page
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err) {
	// Set status
	httpd_resp_set_status(req, "302 Temporary Redirect");
	// Redirect to the "/" root directory
	httpd_resp_set_hdr(req, "Location", "/");
	// iOS requires content in the response to detect a captive portal, simply redirecting is not sufficient.
	httpd_resp_send(req, "Redirect to the captive portal",
			HTTPD_RESP_USE_STRLEN);

	ESP_LOGI(TAG_http, "Redirecting to root");
	return ESP_OK;
}

typedef struct {
	char *username;
	char *password;
} basic_auth_info_t;

static char* http_auth_basic(const char *username, const char *password) {
	int out;
	char *user_info = NULL;
	char *digest = NULL;
	size_t n = 0;
	asprintf(&user_info, "%s:%s", username, password);
	if (!user_info) {
		ESP_LOGE(TAG_http, "No enough memory for user information");
		return NULL;
	}
	esp_crypto_base64_encode(NULL, 0, &n, (const unsigned char*) user_info,
			strlen(user_info));

	/* 6: The length of the "Basic " string
	 * n: Number of bytes for a base64 encode format
	 * 1: Number of bytes for a reserved which be used to fill zero
	 */
	digest = calloc(1, 6 + n + 1);
	if (digest) {
		strcpy(digest, "Basic ");
		esp_crypto_base64_encode((unsigned char*) digest + 6, n, (size_t*) &out,
				(const unsigned char*) user_info, strlen(user_info));
	}
	free(user_info);
	return digest;
}

/* An HTTP GET handler */
static esp_err_t basic_auth_get_handler(httpd_req_t *req) {
	char *buf = NULL;
	size_t buf_len = 0;
	basic_auth_info_t *basic_auth_info = req->user_ctx;

	buf_len = httpd_req_get_hdr_value_len(req, "Authorization") + 1;
	if (buf_len > 1) {
		buf = calloc(1, buf_len);
		if (!buf) {
			ESP_LOGE(TAG_http, "No enough memory for basic authorization");
			return ESP_ERR_NO_MEM;
		}

		if (httpd_req_get_hdr_value_str(req, "Authorization", buf, buf_len)
				== ESP_OK) {
			ESP_LOGI(TAG_http, "Found header => Authorization: %s", buf);
		} else {
			ESP_LOGE(TAG_http, "No auth value received");
		}

		char *auth_credentials = http_auth_basic(basic_auth_info->username,
				basic_auth_info->password);
		if (!auth_credentials) {
			ESP_LOGE(TAG_http,
					"No enough memory for basic authorization credentials");
			free(buf);
			return ESP_ERR_NO_MEM;
		}

		if (strncmp(auth_credentials, buf, buf_len)) {
			ESP_LOGE(TAG_http, "Not authenticated");
			httpd_resp_set_status(req, HTTPD_401);
			httpd_resp_set_type(req, "application/json");
			httpd_resp_set_hdr(req, "Connection", "keep-alive");
			httpd_resp_set_hdr(req, "WWW-Authenticate",
					"Basic realm=\"Hello\"");
			httpd_resp_send(req, NULL, 0);
		} else {
			ESP_LOGI(TAG_http, "Authenticated!");
			char *basic_auth_resp = NULL;
//            httpd_resp_set_status(req, HTTPD_200);HTTPD_302
//            httpd_resp_set_type(req, "application/json");
//            httpd_resp_set_hdr(req, "Connection", "keep-alive");
			httpd_resp_set_status(req, "302 Temporary Redirect");
			// Redirect to the "/" root directory
			httpd_resp_set_hdr(req, "Location", "/index.html");
			// iOS requires content in the response to detect a captive portal, simply redirecting is not sufficient.
			httpd_resp_send(req, "Redirect to the captive portal",
					HTTPD_RESP_USE_STRLEN);

			//    asprintf(&basic_auth_resp, "{\"authenticated\": true,\"user\": \"%s\"}", basic_auth_info->username);
			if (!basic_auth_resp) {
				ESP_LOGE(TAG_http,
						"No enough memory for basic authorization response");
				free(auth_credentials);
				free(buf);
				return ESP_ERR_NO_MEM;
			}
			httpd_resp_send(req, basic_auth_resp, strlen(basic_auth_resp));
			free(basic_auth_resp);
		}
		free(auth_credentials);
		free(buf);
	} else {
		ESP_LOGE(TAG_http, "No auth header received");
		httpd_resp_set_status(req, HTTPD_401);
		httpd_resp_set_type(req, "application/json");
		httpd_resp_set_hdr(req, "Connection", "keep-alive");
		httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"Hello\"");
		httpd_resp_send(req, NULL, 0);
	}

	return ESP_OK;
}

static httpd_uri_t basic_auth = { .uri = "/", .method = HTTP_GET, .handler =
		basic_auth_get_handler, };

static void httpd_register_basic_auth(httpd_handle_t server) {
	basic_auth_info_t *basic_auth_info = calloc(1, sizeof(basic_auth_info_t));
	if (basic_auth_info) {
		basic_auth_info->username = FW_data.http.V_LOGIN;
		basic_auth_info->password = FW_data.http.V_PASSWORD;

		basic_auth.user_ctx = basic_auth_info;
		httpd_register_uri_handler(server, &basic_auth);
	}
}

static const httpd_uri_t setup_get_cgi = { .uri = "/setup_get.cgi", .method =
		HTTP_GET, .handler = setup_get_cgi_handler, .user_ctx = 0 };
static const httpd_uri_t wdog_get_cgi = { .uri = "/wdog_get.cgi", .method =
		HTTP_GET, .handler = wdog_get_cgi_handler, .user_ctx = 0 };
static const httpd_uri_t io_get_cgi = { .uri = "/io_get.cgi",
		.method = HTTP_GET, .handler = io_get_cgi_handler, .user_ctx = 0 };
static const httpd_uri_t email_send_test_cgi = { .uri = "/email_send_test.cgi",
		.method = HTTP_GET, .handler = email_send_test_cgi_handler, .user_ctx =
				0 };
static const httpd_uri_t sendmail_get_cgi = { .uri = "/sendmail_get.cgi",
		.method = HTTP_GET, .handler = sendmail_get_cgi_handler, .user_ctx = 0 };
static const httpd_uri_t io_get_api = { .uri = "/io.cgi", .method = HTTP_GET,
		.handler = io_cgi_api_handler, .user_ctx = 0 };

static const httpd_uri_t termo_get_api = { .uri = "/termo_get.cgi", .method =
		HTTP_GET, .handler = termo_get_cgi_api_handler, .user_ctx = 0 };

static const httpd_uri_t termo_data_api = { .uri = "/termo_data.cgi", .method =
		HTTP_GET, .handler = termo_data_cgi_api_handler, .user_ctx = 0 };

static const httpd_uri_t log_get_cgi = { .uri = "/log.cgi", .method = HTTP_GET,
		.handler = log_get_cgi_handler, .user_ctx = 0 };//({reset:19,suspend:19,report:0})

static const httpd_uri_t notify_get_cgi = { .uri = "/notify_get.cgi", .method =
		HTTP_GET, .handler = notify_get_cgi_handler, .user_ctx = 0 };

static const httpd_uri_t sse_get_cgi = { .uri = "/sse.cgi", .method = HTTP_GET,
		.handler = sse_get_cgi_handler, .user_ctx = 0 };
static const httpd_uri_t eeprom_clone_get = { .uri = "/eeprom_clone_get.cgi",
		.method = HTTP_GET, .handler = hello_get_handler,
		/* Let's pass response string in user
		 * context to demonstrate it's usage */
		.user_ctx = 0 //(char*)(data__IOv3_setup_bin+data__IOv3_setup_bin_shift)
		};
static const httpd_uri_t setup_set = { .uri = "/setup_set.cgi", .method =
		HTTP_POST, .handler = setup_set_post_handler, .user_ctx = NULL };

static const httpd_uri_t io_set = { .uri = "/io_set.cgi", .method = HTTP_POST,
		.handler = io_set_post_handler, .user_ctx = NULL };

static const httpd_uri_t wdog_set = { .uri = "/wdog_set.cgi", .method =
		HTTP_POST, .handler = wdog_set_post_handler, .user_ctx = NULL };

static const httpd_uri_t termo_set = { .uri = "/termo_set.cgi", .method =
		HTTP_POST, .handler = termo_set_post_handler, .user_ctx = NULL };

static const httpd_uri_t notify_set = { .uri = "/notify_set.cgi", .method =
		HTTP_POST, .handler = notify_set_post_handler, .user_ctx = NULL };

static const httpd_uri_t io_set_pulse = { .uri = "/io_set_pulse.cgi", .method =
		HTTP_POST, .handler = io_set_pulse_post_handler, .user_ctx = NULL };

static const httpd_uri_t sendmail_set = { .uri = "/sendmail_set.cgi", .method =
		HTTP_POST, .handler = sendmail_set_post_handler, .user_ctx = NULL };

static const httpd_uri_t ip_set = { .uri = "/ip_set.cgi", .method = HTTP_POST,
		.handler = ip_set_post_handler, .user_ctx = NULL };

static const httpd_uri_t rtcset = { .uri = "/rtcset.cgi", .method = HTTP_POST,
		.handler = rtcset_post_handler, .user_ctx = NULL };

httpd_handle_t start_webserver(void) {
	httpd_handle_t server = NULL;
	const httpd_config_t config = { .task_priority = tskIDLE_PRIORITY + 5,
			.stack_size = 32 * 1024, .core_id = tskNO_AFFINITY, .server_port =
					80, .ctrl_port = 32768, .max_open_sockets = 7,
			.max_uri_handlers = 50, /*12*/
			.max_resp_headers = 8, .backlog_conn = 5, .lru_purge_enable = true, /**/
			.recv_wait_timeout = 5, .send_wait_timeout = 5, .global_user_ctx =
					NULL, .global_user_ctx_free_fn = NULL,
			.global_transport_ctx = NULL, .global_transport_ctx_free_fn = NULL,
			.open_fn = NULL, .close_fn = NULL, .uri_match_fn = NULL };
	//config.lru_purge_enable = true;

	// Start the httpd server
	ESP_LOGI(TAG_http, "Starting server on port: '%d'", config.server_port);
	if (httpd_start(&server, &config) == ESP_OK) {
		// Set URI handlers
		ESP_LOGI(TAG_http, "Registering URI handlers");

		httpd_register_uri_handler(server, &ip_set);
		httpd_register_uri_handler(server, &rtcset);
		httpd_register_uri_handler(server, &setup_set); // kotbazilioi@ngs.ru
		httpd_register_uri_handler(server, &sendmail_set);
		httpd_register_uri_handler(server, &eeprom_clone_get);
		httpd_register_uri_handler(server, &wdog_get_cgi);
		httpd_register_uri_handler(server, &setup_get_cgi);

		httpd_register_uri_handler(server, &termo_get_api);
		httpd_register_uri_handler(server, &termo_data_api);
		httpd_register_uri_handler(server, &email_send_test_cgi);
		httpd_register_uri_handler(server, &notify_get_cgi);
		httpd_register_uri_handler(server, &log_get_cgi); //
		httpd_register_uri_handler(server, &sse_get_cgi); ///io.cgi
		httpd_register_uri_handler(server, &io_get_api);
		httpd_register_uri_handler(server, &io_set);
		httpd_register_uri_handler(server, &io_get_cgi);
		httpd_register_uri_handler(server, &wdog_set);
		httpd_register_uri_handler(server, &termo_set);
		httpd_register_uri_handler(server, &notify_set);
		httpd_register_uri_handler(server, &io_set_pulse);
		httpd_register_uri_handler(server, &sendmail_get_cgi);
		for (int i = 0; i < NP_HTML_HEADERS_NUMBER; ++i) {
			httpd_register_uri_handler(server, &np_html_uri[i]);
		}
		httpd_register_uri_handler(server, &np_html_uri_main);
		httpd_register_uri_handler(server, &np_html_uri_wdog);
		httpd_register_uri_handler(server, &np_html_uri_termo);
		httpd_register_uri_handler(server, &np_html_uri_update);
		httpd_register_uri_handler(server, &np_html_uri_setings);
		httpd_register_uri_handler(server, &np_html_uri_sendmail);
		httpd_register_uri_handler(server, &np_html_uri_update_set);
		httpd_register_uri_handler(server, &np_html_uri_devname_cgi);
		httpd_register_uri_handler(server, &np_html_uri_reboot_cgi);
		httpd_register_err_handler(server, HTTPD_404_NOT_FOUND,
				http_404_error_handler);
		//	  #if CONFIG_EXAMPLE_BASIC_AUTH
		httpd_register_basic_auth(server);
		//	 #endif
		return server;
	}

	ESP_LOGI(TAG_http, "Error starting server!");
	return NULL;
}

void stop_webserver(httpd_handle_t server) {
	httpd_stop(server);
}

__attribute__((used)) void disconnect_handler(void *arg,
		esp_event_base_t event_base, int32_t event_id, void *event_data) {
	httpd_handle_t *server = (httpd_handle_t*) arg;
	if (*server) {
		ESP_LOGI(TAG_http, "Stopping webserver");
		stop_webserver(*server);
		*server = NULL;
	}
}

__attribute__((used)) void connect_handler(void *arg,
		esp_event_base_t event_base, int32_t event_id, void *event_data) {
	httpd_handle_t *server = (httpd_handle_t*) arg;
	if (*server == NULL) {
		ESP_LOGI(TAG_http, "Starting webserver");
		*server = start_webserver();
	}
}

