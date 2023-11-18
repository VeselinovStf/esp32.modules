/**
 * @file root_esp.h
 * @author Stefan Veselinov (veselinovStf@gmail.com)
 * @brief Wrapper functions over esp-idf
 * @version 0.1
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#if !defined(_ROOT_ESP_H_)
#define _ROOT_ESP_H_

#define HIGH 0x1
#define LOW 0x0
#define LSBFIRST 0

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

esp_err_t digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
esp_err_t pinMode(uint8_t pin, gpio_mode_t mode);
esp_err_t shiftIn(uint8_t dp, uint8_t clcp, uint8_t bo);
void shiftOut(uint8_t dp, uint8_t clcp, uint8_t bo, uint8_t val);

#endif // _ROOT_ESP_H_