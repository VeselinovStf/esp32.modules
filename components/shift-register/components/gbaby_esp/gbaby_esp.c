/**
 * @file gbaby_esp.c
 * @author Stefan Veselinov (veselinovStf@gmail.com)
 * @brief Wrapper functions over esp-idf
 * @version 0.1
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "gbaby_esp.h"

esp_err_t digitalWrite(uint8_t pin, uint8_t val)
{
    if (gpio_set_level((gpio_num_t)pin, val) != ESP_OK)
    {
        return ESP_FAIL;
    };

    return ESP_OK;
}

int digitalRead(uint8_t pin)
{
    return gpio_get_level((gpio_num_t)pin);
}

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

esp_err_t shiftIn(uint8_t dp, uint8_t clcp, uint8_t bo)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        // digitalWrite(clockPin, HIGH);
        if (bo == 0)
            value |= digitalRead(dp) << i;
        else
            value |= digitalRead(dp) << (7 - i);
        digitalWrite(clcp, HIGH);
        digitalWrite(clcp, LOW);
    }

    return value;
}

void shiftOut(uint8_t dp, uint8_t clcp, uint8_t bo, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (bo == LSBFIRST)
            digitalWrite(dp, !!(val & (1 << i)));
        else
            digitalWrite(dp, !!(val & (1 << (7 - i))));

        digitalWrite(clcp, HIGH);
        digitalWrite(clcp, LOW);
    }
}