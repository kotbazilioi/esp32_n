                    
idf_component_register(SRCS
 "main.c" 
 "mdns_task.c"
 "nvs_task.c"
 "http_var.c"
 "update.c"
 "smtp.c"
 "LOGS.c"
 "app.c"
 "app_owb.c"
 "ping.c"
 "owb.c"
 "owb_gpio.c"
 "owb_rmt.c"
 "ds18b20.c"
   
                    INCLUDE_DIRS ".")



#LBS force placing of 'custom_app_desc' segment
target_link_libraries(${COMPONENT_TARGET} "-u updater_js")


