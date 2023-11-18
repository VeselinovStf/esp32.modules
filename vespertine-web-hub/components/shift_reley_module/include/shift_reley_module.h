/**
 * @file shift_reley_module.h
 * @author Stefan Veselinov (veselinovStf@gmail.com)
 * @brief 8-Relay Module Controlled by Shift Register
 * @version 0.1
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#if !defined(_SHIFT_RELEY_MODULE_H_)
#define _SHIFT_RELEY_MODULE_H_

#include "freertos/FreeRTOS.h"

typedef struct
{
    uint8_t latchPin;
    uint8_t clockPin;
    uint8_t dataPin;
} shift_reg_conf_t;

esp_err_t initShiftRelay(shift_reg_conf_t *conf);
esp_err_t sendToShiftRelay(uint8_t movingBytes, shift_reg_conf_t *conf);

#endif // _SHIFT_RELEY_MODULE_H_