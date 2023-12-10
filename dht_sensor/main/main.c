#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "gbaby_dht.h"

static const dht_sensor_type_t sensor_type_1 = DHT_TYPE_DHT11;
static const gpio_num_t dht_gpio_1 = 32;

static const dht_sensor_type_t sensor_type_2 = DHT_TYPE_DHT11;
static const gpio_num_t dht_gpio_2 = 21;

void dht_task_1(void *pvParameter)
{
    int16_t temperature = 0;
    int16_t humidity = 0;

    while (1)
    {
        if (dht_read_data(sensor_type_1, dht_gpio_1, &humidity, &temperature) == ESP_OK)
        {
            printf("DHT:1: Humidity: %d Temp: %d^C\n", humidity / 10, temperature / 10);
        }
        else
        {
            printf("DHT:1: Could not read data from sensor\n");
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void dht_task_2(void *pvParameter)
{
    int16_t temperature = 0;
    int16_t humidity = 0;

    while (1)
    {
        if (dht_read_data(sensor_type_2, dht_gpio_2, &humidity, &temperature) == ESP_OK)
        {
            printf("DHT:2: Humidity: %d Temp: %d^C\n", humidity / 10, temperature / 10);
        }
        else
        {
            printf("DHT:2: Could not read data from sensor\n");
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(
        &dht_task_1,
        "dht_task_1",
        2048,
        NULL,
        5,
        NULL);

    xTaskCreate(
        &dht_task_2,
        "dht_task_2",
        2048,
        NULL,
        5,
        NULL);
}