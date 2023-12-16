# EcoBloomStorage - EBS

EcoBloomStorage is a lightweight and efficient storage library designed for ESP32 with ESP-IDF. Inspired by an eco-friendly, plant-themed approach, this library seamlessly integrates with SD Card modules (up to 32 GB). It supports simultaneous read and write operations, making it fast and reliable for managing data. Thread-safe by design, EcoBloomStorage is perfect for storing sensor data, web server responses, and internal states.

## Features

- **Thread Safety:** EcoBloomStorage ensures secure data handling, making it suitable for concurrent read and write operations.

- **OTA Refresh:** The library includes a built-in mechanism for Over-The-Air (OTA) type refresh of the SD card, simplifying the process of updating and managing data.

- **Efficient File Management:** EcoBloomStorage optimizes file management for speed and simplicity, providing an easy-to-use file structure.

- **ESP-IDF Integration:** Fully compatible with the ESP-IDF framework, EcoBloomStorage leverages the capabilities of ESP32 for seamless integration and optimal performance.

## Usage

```c
#include "ecobloom_storage.h"

void app_main() {
    // Initialize EcoBloomStorage
    ecobloom_storage_init();

    // Save sensor data to a file
    ecobloom_storage_write_file("sensor_data.txt", sensor_data);

    // Read data from the web server response file
    char web_server_response[512];
    ecobloom_storage_read_file("web_server_response.txt", web_server_response, sizeof(web_server_response));

    // Perform OTA-type refresh of the SD card
    ecobloom_storage_refresh_ota();

    // Deinitialize EcoBloomStorage when done
    ecobloom_storage_deinit();
}