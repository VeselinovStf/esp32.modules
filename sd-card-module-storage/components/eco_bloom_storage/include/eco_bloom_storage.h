#ifndef ECO_BLOOM_STORAGE_H
#define ECO_BLOOM_STORAGE_H

#include <pthread.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

typedef struct {
    sdmmc_card_t *card;
    // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
    // For setting a specific frequency, use host.max_freq_khz (range 400kHz - 20MHz for SDSPI)
    // Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    pthread_mutex_t mutex;
} EBS;


EBS* GetInstance();
esp_err_t  Initialize();
void deinitialize();
esp_err_t writeFile(const char *path, const char *data);
esp_err_t readFile(const char *path);

#endif // ECO_BLOOM_STORAGE_H