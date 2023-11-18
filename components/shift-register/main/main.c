#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

//#include "ic74hc595.h"
#include "root_esp.h"

#define latchPin 4 // SH/LD PIN
#define clockPin 2 // CLK PIN
#define dataPin 15 // QH PIN

// #define PIN_SHLD GPIO_OUTPUT_IO_25
// #define PIN_CLK GPIO_OUTPUT_IO_26
// #define PIN_QH GPIO_INPUT_IO_27

// #define GPIO_OUTPUT_PIN_SEL ((1 << PIN_SHLD) | (1 << PIN_CLK))
// // #define GPIO_INPUT_PIN_SEL ((1 << PIN_QH))
// shift_reg_config_t shiftConfig;



void app_main()
{
    uint8_t movingBytes = 0b00000001;

    pinMode(latchPin, GPIO_MODE_OUTPUT);
    pinMode(clockPin, GPIO_MODE_OUTPUT);
    pinMode(dataPin, GPIO_MODE_OUTPUT);

    while(1){
        for (uint8_t i = 0; i < 8; i++)
        {
            printf("INDEX: %d\n", i);
            /* code */
            if (digitalWrite(latchPin, LOW) != ESP_OK)
            {
                printf("Can't set pin LOW\n");
                return;
            };

            shiftOut(dataPin, clockPin, LSBFIRST, movingBytes);

            if (digitalWrite(latchPin, HIGH) != ESP_OK)
            {
                printf("Can't set pin HIGH\n");
                return;
            };

            movingBytes = movingBytes << 1;
            vTaskDelay(2500 / portTICK_PERIOD_MS);
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);

        movingBytes = 0b10000000;

        for (uint8_t i = 0; i < 8; i++)
        {
            printf("INDEX: %d\n", i);
            /* code */
            if (digitalWrite(latchPin, LOW) != ESP_OK)
            {
                printf("Can't set pin LOW\n");
                return;
            };

            shiftOut(dataPin, clockPin, LSBFIRST, movingBytes);

            if (digitalWrite(latchPin, HIGH) != ESP_OK)
            {
                printf("Can't set pin HIGH\n");
                return;
            };

            movingBytes = movingBytes >> 1;
            vTaskDelay(2500 / portTICK_PERIOD_MS);
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    

    // shiftConfig.num_reg = 1;

    // shiftConfig.pin.clk = 2;
    // shiftConfig.pin.latch = 4;
    // shiftConfig.pin.signal = 15;

    // // Initialize the library
    // if (ic74hc595_init(&shiftConfig) < 0)
    // {
    //     printf("Error initiating! \n");
    //     return;
    // }

    // uint8_t dataToSend[1] = {0xFF}; // Example data, adjust as needed

    // // Send data to the shift register
    // if (ic74hc595_send(dataToSend, sizeof(dataToSend), &shiftConfig) < 0)
    // {
    //     printf("Error sending! \n");
    //     // Handle error
    // }

    // // Deinitialize the library when done
    // printf("Kill! \n");

    // ic74hc595_deinit(&shiftConfig);

    // io_conf.intr_type = GPIO_INTR_DISABLE;
    // io_conf.mode = GPIO_MODE_OUTPUT;
    // io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    // io_conf.pull_down_en = 0;
    // io_conf.pull_up_en = 0;
    // gpio_config(&io_conf);

    // io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    // io_conf.mode = GPIO_MODE_INPUT;
    // gpio_config(&io_conf);

    // gpio_set_level(PIN_SHLD, 1);
    // gpio_set_level(PIN_CLK, 0);

    // int i;
    // int value;

    // while (1)
    // {

    //     i = 0;
    //     value = 0;

    //     // SHLD must be low to load from Parallel Inputs (A-H)
    //     gpio_set_level(PIN_SHLD, 0);

    //     // SHLD must be high to realize the shift function
    //     gpio_set_level(PIN_SHLD, 1);

    //     printf("Switch Status:\n");

    //     // Reading from QH
    //     for (i = 0; i < 8; i++)
    //     {
    //         printf("RUN: %d\n", i);

    //         if (gpio_get_level(PIN_QH))
    //         {
    //             value = value | (1 << i);
    //             printf("%d:On\n", abs(i - 8));
    //         }

    //         // CLK INH PIN must be low. Toggle CLK to shift data into QH
    //         gpio_set_level(PIN_CLK, 1);
    //         gpio_set_level(PIN_CLK, 0);
    //     }

    //     printf("Value in decimal %d\n", value);

    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
}