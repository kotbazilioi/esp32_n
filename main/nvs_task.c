/*
 * nvs_task.c
 *
 *  Created on: 16 сент. 2021 г.
 *      Author: ivanov
 */



#include <string.h>   //ƒл€ memcpy
#include "esp_flash.h"
#include "esp_flash_spi_init.h"

#include "nvs_task.h"

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
	  int32_t counter;




    while(1) {
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
