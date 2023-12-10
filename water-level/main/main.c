/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars_s1;
static const adc_channel_t channel_s1 = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_unit_t unit_s1 = ADC_UNIT_1;

static const adc_unit_t unit_s2 = ADC_UNIT_2;
static esp_adc_cal_characteristics_t *adc_chars_s2;
static const adc_channel_t channel_s2 = ADC_CHANNEL_7;     //GPIO34 if ADC1, GPIO14 if ADC2

static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_0;



static void check_efuse(void)
{

    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}


static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

#define HIGH 0x1
#define LOW 0x0

esp_err_t pinMode(uint8_t pin, gpio_mode_t mode)
{
    gpio_config_t conf = {
        .pin_bit_mask = (1ULL << pin),         /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
        .pull_up_en = GPIO_PULLUP_DISABLE,     /*!< GPIO pull-up                                         */
        .pull_down_en = GPIO_PULLDOWN_DISABLE, /*!< GPIO pull-down                                       */
        .mode = mode                           /*!< GPIO mode: set input/output mode                     */
    };

    return gpio_config(&conf);
}

esp_err_t digitalWrite(uint8_t pin, uint8_t val)
{
    if (gpio_set_level((gpio_num_t)pin, val) != ESP_OK)
    {
        return ESP_FAIL;
    };

    return ESP_OK;
}

void app_main(void)
{
    pinMode(2, GPIO_MODE_OUTPUT);

    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (unit_s1 == ADC_UNIT_1) {
        adc1_config_width(width);
        adc1_config_channel_atten(channel_s1, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_s1, atten);
    }

    if (unit_s2 == ADC_UNIT_2) {
        adc1_config_width(width);
        adc1_config_channel_atten(channel_s2, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_s2, atten);
    }

    //Characterize ADC
    adc_chars_s1 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type_s1 = esp_adc_cal_characterize(unit_s1, atten, width, DEFAULT_VREF, adc_chars_s1);
    print_char_val_type(val_type_s1);

    adc_chars_s2 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type_s2 = esp_adc_cal_characterize(unit_s2, atten, width, DEFAULT_VREF, adc_chars_s2);
    print_char_val_type(val_type_s2);

    digitalWrite(2,HIGH);
    while (1) {
        int adc_reading_s1 = 0;
        int adc_reading_s2 = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit_s1 == ADC_UNIT_1) {
                adc_reading_s1 += adc1_get_raw((adc1_channel_t)channel_s1);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel_s1, width, &raw);
                adc_reading_s1 += raw;
            }

            if (unit_s2 == ADC_UNIT_2) {
                adc_reading_s2 += adc1_get_raw((adc1_channel_t)channel_s2);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel_s2, width, &raw);
                adc_reading_s2 += raw;
            }
        }
        adc_reading_s1 /= NO_OF_SAMPLES;
        adc_reading_s2 /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        int voltage_s1 = esp_adc_cal_raw_to_voltage(adc_reading_s1, adc_chars_s1);
        int voltage_s2 = esp_adc_cal_raw_to_voltage(adc_reading_s2, adc_chars_s2);
        printf("S1: Raw: %d\\tVoltage: %dmV\n", adc_reading_s1, voltage_s1);
        printf("S2: Raw: %d\\tVoltage: %dmV\n", adc_reading_s2, voltage_s2);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}