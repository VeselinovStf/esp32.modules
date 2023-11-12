idf_component_register(SRCS "gbaby_fs.c" "${srcs}"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_netif driver esp_wifi vfs spiffs fatfs)