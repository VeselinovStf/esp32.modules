/* SD card and FAT filesystem example.
   This example uses SPI peripheral to communicate with SD card.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <eco_bloom_storage.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

EBS *storage;
static const char *TAG = "[MAIN]: ";
#define EXAMPLE_MAX_CHAR_SIZE 64

void vReadTask(void *params) {
    const char *file_path = "/sdcard/pt_i_p1.db";
    while (1) {
        esp_err_t ret = readFile(file_path);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read file (%s).", esp_err_to_name(ret));
        }
        vTaskDelay(1000);
    }
}

void vWriteTask(void *params) {
    const char *file_path = "/sdcard/pt_i_p1.db";
    while (1) {
        char data[EXAMPLE_MAX_CHAR_SIZE];
        snprintf(data, EXAMPLE_MAX_CHAR_SIZE, "%s %s!\n", "Write: ", "WELCOME");
        esp_err_t ret = writeFile(file_path, data);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write file (%s).", esp_err_to_name(ret));
        }
        vTaskDelay(1000);
    }
}
void app_main(void)
{
    esp_err_t ret = Initialize();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SD card (%s).", esp_err_to_name(ret));
        return;
    }

    // Create tasks
    xTaskCreate(vReadTask, "vReadTask", 4096, NULL, 1, NULL);
    xTaskCreate(vReadTask, "vWriteBTask", 4096, NULL, 1, NULL);
    xTaskCreate(vReadTask, "vReadBTask", 4096, NULL, 1, NULL);
    xTaskCreate(vReadTask, "vReadCTask", 4096, NULL, 1, NULL);
    xTaskCreate(vWriteTask, "vWriteBTask", 4096, NULL, 1, NULL);
    xTaskCreate(vWriteTask, "vWriteCTask", 4096, NULL, 1, NULL);
    xTaskCreate(vWriteTask, "vWriteDTask", 4096, NULL, 1, NULL);
    xTaskCreate(vReadTask, "vReadDTask", 4096, NULL, 1, NULL);
    xTaskCreate(vReadTask, "vReadETask", 4096, NULL, 1, NULL);
    xTaskCreate(vWriteTask, "vWriteRTask", 4096, NULL, 1, NULL);

    // Wait indefinitely
    vTaskDelay(portMAX_DELAY);
}
