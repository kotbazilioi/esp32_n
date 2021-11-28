/*
 * update.c
 *
 *  Created on: 18 сент. 2021 г.
 *      Author: ivanov
 */

////////////////////////////////////////////////////////
#include "../main/includes_base.h"






//esp_err_t np_http_get_handler(httpd_req_t *req);





#include "mime.h"
#include "html_data_simple_ru.h"
#include "LOGS.h"
static const char *TAG = "update_app";
esp_app_desc_t app_desc;
esp_err_t np_http_get_handler(httpd_req_t *req)
{

#warning "******** where is no error processing !  *******"
	struct np_html_page_s *page = req->user_ctx;
	httpd_resp_set_hdr(req, "Cache-Control", "max-age=30, private");
	httpd_resp_set_type(req, page->mime);


	if(page->flags & HTML_FLG_COMPRESSED)
	  httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
	httpd_resp_send(req, page->addr, page->size);
	return ESP_OK;
}

const httpd_uri_t np_html_uri_fwupdate_cgi = {
	"/fwupdate.cgi",
	HTTP_GET,
	np_http_get_handler,
	(void*)&_html_page_update_html
};

static esp_ota_handle_t otah;
const esp_partition_t *new_part;

esp_err_t np_http_update_set(httpd_req_t *req)
{
	 struct {
	    unsigned short signature;
	    unsigned short op;
	    unsigned offset;
	    unsigned len;
	  } head;
	char buf[1024];

    int ret;

    ret = httpd_req_recv(req, (void*)&head, sizeof head);
    if(ret != sizeof head)
    	goto err;
    if(head.signature != 0xdeba)
    	goto err; // check signature
    if(sizeof head + head.len != req->content_len )
    {
       ESP_LOGE(TAG, "Wrong fwupdate POST size %d!", req->content_len);
       goto err;
    }


    if(head.op == 1)
    {

        if(head.offset == 0)
        {
			new_part = esp_ota_get_next_update_partition(NULL);
			if(new_part == NULL)
				goto err;
			esp_err_t ret = esp_ota_begin(new_part, OTA_SIZE_UNKNOWN, &otah);
			if(ret < 0)
			{
			  ESP_LOGE(TAG, "OTA starting API error %d", (int)ret);
			  goto err;
			}
			ESP_LOGI(TAG, "OTA started");
        }

		int remaining = head.len;
		while (remaining > 0)
		{
			ret = httpd_req_recv(req, buf, remaining);
			if(ret == HTTPD_SOCK_ERR_TIMEOUT)
				continue;
			if(ret < 0)
				return ESP_FAIL;
			remaining -= ret;
		}
		esp_err_t ret = esp_ota_write_with_offset(otah, buf, head.len, head.offset);
		if(ret < 0)
			goto err;
		ESP_LOGI(TAG, "FW RW, offset %08x", head.offset);
    }
    else
    if(head.op == 5)
    {
    	esp_err_t ret = esp_ota_end(otah);
     	ESP_LOGI(TAG, "OTA closed, code %d", ret);
     	if(ret == ESP_OK)
     	{
     		ret = esp_ota_set_boot_partition(new_part);
     		if(ret != ESP_OK)
     		{
     			ESP_LOGE(TAG, "Can't assign updated partition for boot, code %d", ret);
     			goto err;
     		}
     		ESP_LOGI(TAG, "Commanding reboot into %s", new_part->label);
     		httpd_resp_send(req, "", 0);
     		vTaskDelay(3 * configTICK_RATE_HZ);

     		esp_restart();
     	}
    }
    else
      goto err;



    // End response
    httpd_resp_send(req, "", 0);
    return ESP_OK;

err:

    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Update error in partition or OTA API!");
    return ESP_FAIL;
}

const httpd_uri_t np_html_uri_update_set = {
	"/update_set.cgi",
	HTTP_POST,
	np_http_update_set,
	0
};


esp_err_t np_http_devname_cgi(httpd_req_t *req)
{

#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control", "max-age=30, private");
	httpd_resp_set_type(req, mime_js);

    const esp_partition_t *running = esp_ota_get_running_partition();
    //esp_app_desc_t app_desc;
    esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
    if(ret != ESP_OK)
    {
    	httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Can't read FW version!");
    	return ESP_FAIL;
    }
    char buf[256];
    sprintf(buf,
    		"var fwver='v%.31s'\n var devname='%s';",
    		 app_desc.version,FW_data.sys.V_Name_dev);
	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
esp_err_t np_http_reboot_cgi(httpd_req_t *req)
{

#warning "******** where is no error processing !  *******"
	httpd_resp_set_hdr(req, "Cache-Control", "max-age=30, private");
	httpd_resp_set_type(req, mime_js);

    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t app_desc;
    esp_err_t ret = esp_ota_get_partition_description(running, &app_desc);
    if(ret != ESP_OK)
    {
    	httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Can't read FW version!");
    	return ESP_FAIL;
    }

	reple_to_save.type_event = RESETL;
	reple_to_save.event_cfg.canal = 0;
    reple_to_save.event_cfg.source=SNMP;
	reple_to_save.dicr = 1;


    esp_restart();
//    char buf[128];
//    sprintf(buf,
//    		"var fwver='v%.31s'\n"
//    		"var devname='Test Netping-APP-ESP32';\n",
//    		 app_desc.version);
///	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);

	return ESP_OK;
}

const httpd_uri_t np_html_uri_devname_cgi = {
	"/devname.cgi",
	HTTP_GET,
	np_http_devname_cgi,
	0
};

const httpd_uri_t np_html_uri_reboot_cgi = {
	"/reboot.cgi",
	HTTP_POST,
	np_http_reboot_cgi,
	0
};


const httpd_uri_t np_html_uri_update = {
	"/update.html",
	HTTP_GET,
	np_http_get_handler,
	(void*)&_html_page_update_html
};
const httpd_uri_t np_html_uri_main = {
	"/index.html",
	HTTP_GET,
	np_http_get_handler,
	(void*)&_html_page_index_html
};

const httpd_uri_t np_html_uri_wdog = {
	"/wdog.html",
	HTTP_GET,
	np_http_get_handler,
	(void*)&_html_page_wdog_html
};

const httpd_uri_t np_html_uri_termo = {
	"/termo.html",
	HTTP_GET,
	np_http_get_handler,
	(void*)&_html_page_termo_html
};


const httpd_uri_t np_html_uri_setings = {
	"/settings.html",
	HTTP_GET,
	np_http_get_handler,
	(void*)&_html_page_settings_html
};

const httpd_uri_t np_html_uri_sendmail = {
	"/sendmail.html",
	HTTP_GET,
	np_http_get_handler,
	(void*)&_html_page_sendmail_html
};

