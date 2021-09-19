#!python
# -*- coding: utf-8 -*-

from html_5_5 import *

residents = [
    "favicon.ico",
    "index.html",
    "index.css",
    "menu.js",
	"settings.html",
    "update.html",
    "log.html"
    ]
## residents - menu.js is included in update.html, used if not available via http

res_dir          = "./html_ru"
output_file      = "../html_data_simple_ru.h"

pack_html_pages(res_dir, output_file, residents)

"""
res_dir          = "./html_en"
output_file      = "../../html_data_dksf561_en.h"

pack_html_pages(res_dir, output_file, residents)
"""

