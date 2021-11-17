/*
 * update.h
 *
 *  Created on: 18 сент. 2021 г.
 *      Author: ivanov
 */

#ifndef COMPONENTS_UPDATE_H_
#define COMPONENTS_UPDATE_H_

esp_err_t np_http_get_handler(httpd_req_t *req);
esp_err_t np_http_update_set(httpd_req_t *req);
esp_err_t np_http_devname_cgi(httpd_req_t *req);
extern const httpd_uri_t np_html_uri_main;
extern const httpd_uri_t np_html_uri_update_set;
extern const httpd_uri_t np_html_uri_fwupdate_cgi;
extern const httpd_uri_t np_html_uri_devname_cgi;
extern const httpd_uri_t np_html_uri_reboot_cgi;
extern const httpd_uri_t np_html_uri_update;
extern const httpd_uri_t np_html_uri_setings;
extern const httpd_uri_t np_html_uri_sendmail;
extern const httpd_uri_t np_html_uri_wdog;
extern const httpd_uri_t np_html_uri_termo;


extern esp_app_desc_t app_desc;

extern const httpd_uri_t np_html_uri[];
#define NP_HTML_HEADERS_NUMBER    7//12

#endif /* COMPONENTS_UPDATE_H_ */
