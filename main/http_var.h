/*
 * http_var.h
 *
 *  Created on: 18 сент. 2021 г.
 *      Author: ivanov
 */

#ifndef MAIN_HTML_HTTP_VAR_H_
#define MAIN_HTML_HTTP_VAR_H_

httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t server);
__attribute__((used)) void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data);
__attribute__((used)) void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data);


#endif /* MAIN_HTML_HTTP_VAR_H_ */
