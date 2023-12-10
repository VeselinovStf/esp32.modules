#ifndef GBABY_DHT_SENSOR_DATA_H
#define GBABY_DHT_SENSOR_DATA_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// Declare the semaphore as extern
extern SemaphoreHandle_t sensorDataSemaphore;

#endif  // SENSOR_DATA_H
