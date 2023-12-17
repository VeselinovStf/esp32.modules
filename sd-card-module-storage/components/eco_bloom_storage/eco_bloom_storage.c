#include <stdio.h>
#include "eco_bloom_storage.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "esp_log.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

// Static variable declaration
static EBS ebs;
static sdmmc_host_t host = SDSPI_HOST_DEFAULT();
static int slot = 0;
static const char* EBS_TAB = "[EBS]";
#define EBS_MAX_CHAR_SIZE 64
EBS* GetInstance(){
    return &ebs;
}

esp_err_t  Initialize()
{
    if (ebs.card != NULL)
    {
        ESP_LOGW(EBS_TAB, "SD card is already initialized.");
        return ESP_OK;
    }

    esp_err_t ret;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 10,
        .allocation_unit_size = 16 * 1024};

    const char mount_point[] = "/sdcard";
    ESP_LOGI(EBS_TAB, "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.
    ESP_LOGI(EBS_TAB, "Using SPI peripheral");

    host.max_freq_khz = 400;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = 23,
        .miso_io_num = 19,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000};
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK)
    {
        ESP_LOGE(EBS_TAB, "Failed to initialize bus.");
        return ret;
    }

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = 5;

    slot_config.host_id = host.slot;

    ESP_LOGI(EBS_TAB, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &ebs.card);

    if (ret != ESP_OK)
    {
        ESP_LOGE(EBS_TAB, "SD Card Error Code: %d", ret);
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(EBS_TAB, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        }
        else
        {
            ESP_LOGE(EBS_TAB, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     esp_err_to_name(ret));
        }
        return ret;
    }
    ESP_LOGI(EBS_TAB, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, ebs.card);

    return ESP_OK;
}

void deinitialize()
{
    if (ebs.card != NULL)
    {
        // Unmount partition and free resources
        esp_vfs_fat_sdcard_unmount("/sdcard", ebs.card);
        spi_bus_free(slot);
        ebs.card = NULL;
    }
}

static esp_err_t s_example_write_file(const char *path, char *data)
{
    ESP_LOGI(EBS_TAB, "Opening file %s", path);
    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        ESP_LOGE(EBS_TAB, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(EBS_TAB, "File written");

    return ESP_OK;
}

static esp_err_t s_example_read_file(const char *path)
{
    ESP_LOGI(EBS_TAB, "Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        ESP_LOGE(EBS_TAB, "Failed to open file for reading");
        return ESP_FAIL;
    }
    char line[EBS_MAX_CHAR_SIZE];
    fgets(line, sizeof(line), f);
    fclose(f);

    // strip newline
    char *pos = strchr(line, '\n');
    if (pos)
    {
        *pos = '\0';
    }
    ESP_LOGI(EBS_TAB, "Read from file: '%s'", line);

    return ESP_OK;
}

esp_err_t writeFile(const char *path, const char *data)
{
    if (ebs.card == NULL)
    {
        ESP_LOGE(EBS_TAB, "SD card not initialized.");
        return ESP_FAIL;
    }

    return s_example_write_file(path, data);
}

esp_err_t readFile(const char *path)
{
    if (ebs.card == NULL)
    {
        ESP_LOGE(EBS_TAB, "SD card not initialized.");
        return ESP_FAIL;
    }

    return s_example_read_file(path);
}
