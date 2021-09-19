/*
 * http_var.c
 *
 *  Created on: 18 сент. 2021 г.
 *      Author: ivanov
 */
#include "../main/includes_base.h"

//#include "html_data_simple_ru.h"
#include "update.h"
#include "mime.h"

#define STORAGE_NAMESPACE "storage"
#define TAG_http "http mes"
/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req)
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
   char buf[128];
//   sprintf(buf,
//   		"var fwver='v%.31s'\n"
//   		"var devname='Test Netping-on-ESP32';\n",
//   		 app_desc.version);
	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

//    char*  buf;
//    size_t buf_len;
//
//    /* Get header value string length and allocate memory for length + 1,
//     * extra byte for null termination */
//    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
//    if (buf_len > 1) {
//        buf = malloc(buf_len);
//        /* Copy null terminated value string into buffer */
//        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
//            ESP_LOGI(TAG_http, "Found header => Host: %s", buf);
//        }
//        free(buf);
//    }
//
//    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-2") + 1;
//    if (buf_len > 1) {
//        buf = malloc(buf_len);
//        if (httpd_req_get_hdr_value_str(req, "Test-Header-2", buf, buf_len) == ESP_OK) {
//            ESP_LOGI(TAG_http, "Found header => Test-Header-2: %s", buf);
//        }
//        free(buf);
//    }
//
//    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
//    if (buf_len > 1) {
//        buf = malloc(buf_len);
//        if (httpd_req_get_hdr_value_str(req, "Test-Header-1", buf, buf_len) == ESP_OK) {
//            ESP_LOGI(TAG_http, "Found header => Test-Header-1: %s", buf);
//        }
//        free(buf);
//    }
//
//    /* Read URL query string length and allocate memory for length + 1,
//     * extra byte for null termination */
//    buf_len = httpd_req_get_url_query_len(req) + 1;
//    if (buf_len > 1) {
//        buf = malloc(buf_len);
//        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
//            ESP_LOGI(TAG_http, "Found URL query => %s", buf);
//            char param[32];
//            /* Get value of expected key from query string */
//            if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
//                ESP_LOGI(TAG_http, "Found URL query parameter => query1=%s", param);
//            }
//            if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
//                ESP_LOGI(TAG_http, "Found URL query parameter => query3=%s", param);
//            }
//            if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
//                ESP_LOGI(TAG_http, "Found URL query parameter => query2=%s", param);
//            }
//        }
//        free(buf);
//    }
//
//    /* Set some custom headers */
// //   httpd_resp_set_hdr(req,"Content-Encoding","gzip");
//    httpd_resp_set_hdr(req, "Connection", "Close");
//    httpd_resp_set_hdr(req, "Server","lwIP/1.3.1");
//    httpd_resp_set_type(req, "text/html");
//
//
//    /* Send response with custom headers and body set as the
//     * string passed in user context*/
//    const char* resp_str = (const char*) req->user_ctx;
//    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
//
//    /* After sending the HTTP response the old HTTP request
//     * headers are lost. Check if HTTP request headers can be read now. */
//    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
//        ESP_LOGI(TAG_http, "Request headers lost");
//    }
//    return ESP_OK;
//}

static esp_err_t cgi_get_handler(httpd_req_t *req)
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
    char buf[128];
//    sprintf(buf,
//    		"var fwver='v%.31s'\n"
//    		"var devname='Test Netping-on-ESP32';\n",
//    		 app_desc.version);
	httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

static esp_err_t rtcset_post_handler(httpd_req_t *req)
{
	esp_err_t err;
	nvs_handle_t my_handle;

    char buf[1000];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
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
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        err = nvs_set_blob(my_handle, "rtcset", buf, req->content_len);
        if (err != ESP_OK)
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        err = nvs_commit(my_handle);
        if (err != ESP_OK)
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        nvs_close(my_handle);
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
static esp_err_t ip_set_post_handler(httpd_req_t *req)
{
	esp_err_t err;
	nvs_handle_t my_handle;

    char buf[1000];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
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
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        err = nvs_set_blob(my_handle, "ip_set", buf, req->content_len);
        if (err != ESP_OK)
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        err = nvs_commit(my_handle);
        if (err != ESP_OK)
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        nvs_close(my_handle);
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
static esp_err_t setup_set_post_handler(httpd_req_t *req)
{
	esp_err_t err;
	nvs_handle_t my_handle;

    char buf[1000];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
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
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        err = nvs_set_blob(my_handle, "setup_set", buf, req->content_len);
        if (err != ESP_OK)
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        err = nvs_commit(my_handle);
        if (err != ESP_OK)
        	printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));
        nvs_close(my_handle);
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t setup_get_cgi = {
    .uri       = "/setup_get.cgi",
    .method    = HTTP_GET,
    .handler   = cgi_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = 0 //(char*)(data__setup_get_cgi+data__setup_get_cgi_shift)
};

static const httpd_uri_t eeprom_clone_get = {
       .uri       = "/eeprom_clone_get.cgi",
       .method    = HTTP_GET,
       .handler   = hello_get_handler,
       /* Let's pass response string in user
        * context to demonstrate it's usage */
       .user_ctx  = 0//(char*)(data__IOv3_setup_bin+data__IOv3_setup_bin_shift)
   };

static const httpd_uri_t setup_set = {
    .uri       = "/setup_set.cgi",
    .method    = HTTP_POST,
    .handler   = setup_set_post_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t ip_set = {
    .uri       = "/ip_set.cgi",
    .method    = HTTP_POST,
    .handler   = ip_set_post_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t rtcset = {
    .uri       = "/rtcset.cgi",
    .method    = HTTP_POST,
    .handler   = rtcset_post_handler,
    .user_ctx  = NULL
};


httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    const httpd_config_t config = {                        \
            .task_priority      = tskIDLE_PRIORITY+5,       \
            .stack_size         = 4096,                     \
            .core_id            = tskNO_AFFINITY,           \
            .server_port        = 80,                       \
            .ctrl_port          = 32768,                    \
            .max_open_sockets   = 7,                        \
            .max_uri_handlers   = 30,  /*12*/                     \
            .max_resp_headers   = 8,                        \
            .backlog_conn       = 5,                        \
            .lru_purge_enable   = true,   /**/                 \
            .recv_wait_timeout  = 5,                        \
            .send_wait_timeout  = 5,                        \
            .global_user_ctx = NULL,                        \
            .global_user_ctx_free_fn = NULL,                \
            .global_transport_ctx = NULL,                   \
            .global_transport_ctx_free_fn = NULL,           \
            .open_fn = NULL,                                \
            .close_fn = NULL,                               \
            .uri_match_fn = NULL                            \
    };
    //config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG_http, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG_http, "Registering URI handlers");

        httpd_register_uri_handler(server, &ip_set);
        httpd_register_uri_handler(server, &rtcset);
        httpd_register_uri_handler(server, &setup_set);
        httpd_register_uri_handler(server, &eeprom_clone_get);
        httpd_register_uri_handler(server, &setup_get_cgi);

        for(int i = 0; i < NP_HTML_HEADERS_NUMBER; ++i)
        httpd_register_uri_handler(server, &np_html_uri[i]);
        httpd_register_uri_handler(server, &np_html_uri_main);
        httpd_register_uri_handler(server, &np_html_uri_update_set);
        httpd_register_uri_handler(server, &np_html_uri_devname_cgi);
        return server;
    }

    ESP_LOGI(TAG_http, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}

__attribute__((used)) void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG_http, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

__attribute__((used)) void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG_http, "Starting webserver");
        *server = start_webserver();
    }
}

