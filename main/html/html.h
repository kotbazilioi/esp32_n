
///Сигнатура наличия таблицы ресурсов http
#define HTTP_ID_SIGN  0x55AA
///Сигнатура наличия таблицы страниц http
#define HTTP_PAGE_SIGN 0x5AA5


// mime-типы, сюда ссылается поле mime структуры page_s
/****
extern const char mime_text[];
extern const char mime_html[];
extern const char mime_css[];
extern const char mime_js[];
extern const char mime_png[];
extern const char mime_sse[];
extern const char mime_ico[];
extern const char mime_bin[];
*****/

// структура заголовка страницы, автокомпонуются в сегмент HTML_HEADERS скриптом упаковки страниц и макросом HOOK_CGI
struct np_html_page_s {
  ////const char*    name;
  const char*    mime;
  const void*    addr;
  unsigned short size;
  unsigned short flags;
};

// биты поля flags стуктуры page_s
#define HTML_FLG_GET         0x01
#define HTML_FLG_POST        0x02
#define HTML_FLG_COMPRESSED  0x04
#define HTML_FLG_NOCACHE     0x08
#define HTML_FLG_CGI         0x40
#define HTML_FLG_SS_EVENT    0x80

static esp_err_t np_http_get_handler(httpd_req_t *req);


