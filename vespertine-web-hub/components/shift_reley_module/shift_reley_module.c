/**
 * @file shift_reley_module.c
 * @author Stefan Veselinov (veselinovStf@gmail.com)
 * @brief 8-Relay Module Controlled by Shift Register
 * @version 0.1
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "shift_reley_module.h"
#include "gbaby_esp.h"

esp_err_t initShiftRelay(shift_reg_conf_t *conf)
{
    // pinMode(conf->latchPin, GPIO_MODE_OUTPUT);
    // pinMode(conf->clockPin, GPIO_MODE_OUTPUT);
    // pinMode(conf->dataPin, GPIO_MODE_OUTPUT);

    return ESP_OK;
};

esp_err_t sendToShiftRelay(uint8_t movingBytes, shift_reg_conf_t *conf)
{
    // if (digitalWrite(conf->latchPin, LOW) != ESP_OK)
    // {
    //     printf("Can't set pin LOW\n");
    //     return ESP_FAIL;
    // };

    // shiftOut(conf->dataPin, conf->clockPin, LSBFIRST, movingBytes);

    // if (digitalWrite(conf->latchPin, HIGH) != ESP_OK)
    // {
    //     printf("Can't set pin HIGH\n");
    //     return ESP_FAIL;
    // };

    return ESP_OK;
};
