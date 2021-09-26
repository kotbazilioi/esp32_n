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

#define STORAGE_NAMESPACE "storage"
#define TAG_http "http mes"
#define HTTPD_401      "401 UNAUTHORIZED"           /*!< HTTP Response 401 */
#define HTTPD_302      "302 Found Location: /index.html"           /*!< HTTP Response 401 */
/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control", "max-age=30, private");
	httpd_resp_set_type(req, mime_js);

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
	sprintf(buf, "event: io_state\n" "data: %d%d\n\n", IN_PORT[0].sost_filtr,
			IN_PORT[1].sost_filtr);

	strcat(buf, buf_temp);
	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
static esp_err_t log_get_cgi_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control",
			"no-store\, no-cache\, must-revalidate");
	httpd_resp_set_type(req, mime_text);

	char buf[128*50] = { 0 };
	char buf_temp[256];
	uint16_t i = 0;
	while ((logs_read(i, buf_temp) == 0)&&(i<max_log_mess)) {
		strcat(buf, buf_temp);
		i++;

	}
	printf("\n\rRead %d messege logs\n\r", i);

	//sprintf(buf,"11.05.21 Tu 07:38:15.040 Watchdog: reset of chan.1 \"Сигнал\"\. A (8.8.8.8) no reply, B (192.168.0.55) no reply, C (124.211.45.11) is ignored.\r\n");

	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
static esp_err_t io_get_cgi_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control", "max-age=30, private");
	httpd_resp_set_type(req, mime_js);

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
	sprintf(buf,
			"var packfmt={name:{offs:0,len:32},direction:{offs:42,len:1},delay:{offs:40,len:2},level_out:{offs:43,len:1},pulse_dur:{offs:45,len:1},__len:48};");
	sprintf(buf_temp, "var data_status=15;");
	strcat(buf, buf_temp);
	sprintf(buf_temp,
			" var data=[{name:\"IO_port1\",direction:0,delay:500,level_out:1,pulse_dur:10,level:1,nf_legend_high:\"\",nf_legend_low:\"\",colors:82},");
	strcat(buf, buf_temp);
	sprintf(buf_temp,
			"{name:\"IO_port2\",direction:0,delay:500,level_out:0,pulse_dur:10,level:1,nf_legend_high:\"\",nf_legend_low:\"\",colors:82}];");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var fwver='v110.0.0';");
	strcat(buf, buf_temp);
	sprintf(buf_temp, "var sys_location='Barnaul';");
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
	httpd_resp_set_hdr(req, "Cache-Control", "max-age=30, private");
	httpd_resp_set_type(req, mime_js);
	char buf[1024];
	int ret, remaining = req->content_len;
	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf))))
						<= 0) {
	//				if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
	//					/* Retry receiving if timeout occurred */
	//					continue;
	//				}
	//				return ESP_FAIL;
				}

//	const esp_partition_t *running = esp_ota_get_running_partition();
//	esp_app_desc_t app_desc;
//	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
//	if (ret != ESP_OK) {
//		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
//				"Can't read FW version!");
//		return ESP_FAIL;
//	}

	uint8_t sost = (IN_PORT[0].sost_filtr)|(IN_PORT[1].sost_filtr<<1);

	sprintf(buf,"io_result('ok', %d);",sost);
	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
static esp_err_t setup_get_cgi_handler(httpd_req_t *req) {
#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control", "max-age=30, private");
	httpd_resp_set_type(req, mime_js);

	esp_err_t err;
	nvs_handle_t my_handle;

	char buf[2048];
	char buf_temp[256];



//	const esp_partition_t *running = esp_ota_get_running_partition();
//	esp_app_desc_t app_desc;
//	esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
//	if (ret != ESP_OK) {
//		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR,
//				"Can't read FW version!");
//		return ESP_FAIL;
//	}

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
		sprintf(buf_temp, "http_port:80,");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "uname:\"%s\",", FW_data.http.V_LOGIN);
		strcat(buf, buf_temp);
		sprintf(buf_temp, "passwd:\"%s\",", FW_data.http.V_PASSWORD);
		strcat(buf, buf_temp);
		sprintf(buf_temp, "community_r:\"ping34\"");
		strcat(buf, buf_temp);
		sprintf(buf_temp, ",community_w:\"ping34\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "filt_ip1:'0.0.0.0',");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "filt_mask1:'0.0.0.0',");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "powersaving:0,");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "trap_refresh:0,");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "trap_ip1:'0.0.0.0',");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "trap_ip2:'0.0.0.0',");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "ntp_ip1:'125.227.188.173',");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "ntp_ip2:'0.0.0.0',");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "timezone:3,");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "syslog_ip1:'0.0.0.0',");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "facility:16,severity:6,");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "snmp_port:161,");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "notification_email:\"\"");
		strcat(buf, buf_temp);
		sprintf(buf_temp, ",hostname:\"Drivers\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "location:\"Barnaul\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "contact:\"admin@mail.ru\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "dns_ip1:'%d.%d.%d.%d',", FW_data.net.V_IP_DNS[0],
				FW_data.net.V_IP_DNS[1], FW_data.net.V_IP_DNS[2],
				FW_data.net.V_IP_DNS[3]);
		strcat(buf, buf_temp);
		sprintf(buf_temp, "trap_hostname1:\"\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "trap_hostname2:\"\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "ntp_hostname1:\"ntp.netping.ru\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "ntp_hostname2:\"\",");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "syslog_hostname1:\"\"};");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "var data_rtc=1632334692;");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "var uptime_100ms=69374927;");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "var devname='%s';", FW_data.sys.V_Name_dev);
		strcat(buf, buf_temp);
		sprintf(buf_temp, "var sys_name='%s';", FW_data.sys.V_Name_dev);
		strcat(buf, buf_temp);
		sprintf(buf_temp, "var sys_location='Barnaul';");
		strcat(buf, buf_temp);
		sprintf(buf_temp, "var hwmodel=110;");
		strcat(buf, buf_temp);

		httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
		return ESP_OK;
	}

	static esp_err_t rtcset_post_handler(httpd_req_t *req) {
		esp_err_t err;
		nvs_handle_t my_handle;

		char buf[1000];
		int ret, remaining = req->content_len;

		while (remaining > 0) {
			/* Read the data for the request */
			if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf))))
					<= 0) {
				if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
					/* Retry receiving if timeout occurred */
					continue;
				}
				return ESP_FAIL;
			}

			/* Send back the same data */
			httpd_resp_send_chunk(req, buf, ret);
			remaining -= ret;

//        /* Log data received */
//        ESP_LOGI(TAG_http, "=========== RECEIVED DATA ==========");
//        ESP_LOGI(TAG_http, "%.*s", ret, buf);
//        ESP_LOGI(TAG_http, "====================================");

			err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
			if (err != ESP_OK)
				printf("Error (%s) saving restart counter to NVS!\n",
						esp_err_to_name(err));
			err = nvs_set_blob(my_handle, "rtcset", buf, req->content_len);
			if (err != ESP_OK)
				printf("Error (%s) saving restart counter to NVS!\n",
						esp_err_to_name(err));
			err = nvs_commit(my_handle);
			if (err != ESP_OK)
				printf("Error (%s) saving restart counter to NVS!\n",
						esp_err_to_name(err));
			nvs_close(my_handle);
		}

		// End response
		httpd_resp_send_chunk(req, NULL, 0);
		return ESP_OK;
	}
	static esp_err_t ip_set_post_handler(httpd_req_t *req) {
		esp_err_t err;
		nvs_handle_t my_handle;

		char buf[1000];
		int ret, remaining = req->content_len;

	//	while (remaining > 0) {
			/* Read the data for the request */
			if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf))))
					<= 0) {
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
			save_data_blok();

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
	static esp_err_t setup_set_post_handler(httpd_req_t *req) {
		esp_err_t err;
		nvs_handle_t my_handle;

		char buf[1000];
		int ret, remaining = req->content_len;


			if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf))))
					<= 0) {
//				if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//					/* Retry receiving if timeout occurred */
//					continue;
//				}
//				return ESP_FAIL;
			}

			memset(FW_data.http.V_LOGIN,0,16);
			memcpy(FW_data.http.V_LOGIN,(char*)(buf+19),buf[18]);
			memset(FW_data.http.V_PASSWORD,0,16);
			memcpy(FW_data.http.V_PASSWORD,(char*)(buf+37),buf[36]);

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



				save_data_blok();

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
			esp_crypto_base64_encode((unsigned char*) digest + 6, n,
					(size_t*) &out, (const unsigned char*) user_info,
					strlen(user_info));
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
			httpd_resp_set_hdr(req, "WWW-Authenticate",
					"Basic realm=\"Hello\"");
			httpd_resp_send(req, NULL, 0);
		}

		return ESP_OK;
	}

	static httpd_uri_t basic_auth = { .uri = "/", .method = HTTP_GET, .handler =
			basic_auth_get_handler, };

	static void httpd_register_basic_auth(httpd_handle_t server) {
		basic_auth_info_t *basic_auth_info = calloc(1,
				sizeof(basic_auth_info_t));
		if (basic_auth_info) {
			basic_auth_info->username = FW_data.http.V_LOGIN;
			basic_auth_info->password = FW_data.http.V_PASSWORD;

			basic_auth.user_ctx = basic_auth_info;
			httpd_register_uri_handler(server, &basic_auth);
		}
	}

	static const httpd_uri_t setup_get_cgi =
			{ .uri = "/setup_get.cgi", .method = HTTP_GET, .handler =
					setup_get_cgi_handler, .user_ctx = 0 };
	static const httpd_uri_t io_get_cgi = { .uri = "/io_get.cgi", .method =
			HTTP_GET, .handler = io_get_cgi_handler, .user_ctx = 0 };
	static const httpd_uri_t io_get_api = { .uri = "/io.cgi", .method =
			HTTP_GET, .handler = io_cgi_api_handler, .user_ctx = 0 };
	static const httpd_uri_t log_get_cgi = { .uri = "/log.cgi", .method =
			HTTP_GET, .handler = log_get_cgi_handler, .user_ctx = 0 };
	static const httpd_uri_t sse_get_cgi = { .uri = "/sse.cgi", .method =
			HTTP_GET, .handler = sse_get_cgi_handler, .user_ctx = 0 };
	static const httpd_uri_t eeprom_clone_get = {
			.uri = "/eeprom_clone_get.cgi", .method = HTTP_GET, .handler =
					hello_get_handler,
			/* Let's pass response string in user
			 * context to demonstrate it's usage */
			.user_ctx = 0 //(char*)(data__IOv3_setup_bin+data__IOv3_setup_bin_shift)
			};

	static const httpd_uri_t setup_set = { .uri = "/setup_set.cgi", .method =
			HTTP_POST, .handler = setup_set_post_handler, .user_ctx = NULL };

	static const httpd_uri_t ip_set = { .uri = "/ip_set.cgi", .method =
			HTTP_POST, .handler = ip_set_post_handler, .user_ctx = NULL };

	static const httpd_uri_t rtcset = { .uri = "/rtcset.cgi", .method =
			HTTP_POST, .handler = rtcset_post_handler, .user_ctx = NULL };

	httpd_handle_t start_webserver(void) {
		httpd_handle_t server = NULL;
		const httpd_config_t config = { .task_priority = tskIDLE_PRIORITY + 5,
				.stack_size = 32*1024, .core_id = tskNO_AFFINITY,
				.server_port = 80, .ctrl_port = 32768, .max_open_sockets = 7,
				.max_uri_handlers = 30, /*12*/
				.max_resp_headers = 8, .backlog_conn = 5, .lru_purge_enable =
						true, /**/
				.recv_wait_timeout = 5, .send_wait_timeout = 5,
				.global_user_ctx = NULL, .global_user_ctx_free_fn = NULL,
				.global_transport_ctx = NULL, .global_transport_ctx_free_fn =
						NULL, .open_fn = NULL, .close_fn = NULL, .uri_match_fn =
						NULL };
		//config.lru_purge_enable = true;

		// Start the httpd server
		ESP_LOGI(TAG_http, "Starting server on port: '%d'", config.server_port);
		if (httpd_start(&server, &config) == ESP_OK) {
			// Set URI handlers
			ESP_LOGI(TAG_http, "Registering URI handlers");

			httpd_register_uri_handler(server, &ip_set);
			httpd_register_uri_handler(server, &rtcset);
			httpd_register_uri_handler(server, &setup_set);
			httpd_register_uri_handler(server, &eeprom_clone_get);
			httpd_register_uri_handler(server, &setup_get_cgi);
			httpd_register_uri_handler(server, &io_get_cgi);
			httpd_register_uri_handler(server, &log_get_cgi);
			httpd_register_uri_handler(server, &sse_get_cgi);///io.cgi
			httpd_register_uri_handler(server, &io_get_api);
			for (int i = 0; i < NP_HTML_HEADERS_NUMBER; ++i) {
				httpd_register_uri_handler(server, &np_html_uri[i]);
			}
			httpd_register_uri_handler(server, &np_html_uri_main);
			httpd_register_uri_handler(server, &np_html_uri_update);
			httpd_register_uri_handler(server, &np_html_uri_setings);
			httpd_register_uri_handler(server, &np_html_uri_update_set);
			httpd_register_uri_handler(server, &np_html_uri_devname_cgi);
			httpd_register_uri_handler(server, &np_html_uri_reboot_cgi);
			httpd_register_err_handler(server, HTTPD_404_NOT_FOUND,
					http_404_error_handler);
			//  #if CONFIG_EXAMPLE_BASIC_AUTH
			//   httpd_register_basic_auth(server);
			// #endif
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

