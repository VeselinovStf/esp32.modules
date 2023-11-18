#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gbaby_esp.h"
#include "shift_reley_module.h"

#define LP 4 // SH/LD PIN
#define CP 2 // CLK PIN
#define DP 15 // QH PIN

void app_main()
{
    uint8_t movingBytes = strtol("01000001", NULL, 2);

    shift_reg_conf_t shift_reg_conf;

    shift_reg_conf.latchPin = LP;
    shift_reg_conf.clockPin = CP;
    shift_reg_conf.dataPin = DP;

    initShiftRelay(&shift_reg_conf);
    sendToShiftRelay(movingBytes, &shift_reg_conf);
}