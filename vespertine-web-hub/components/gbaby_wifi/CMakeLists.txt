idf_component_register(SRCS "gbaby_wifi.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_netif driver esp_wifi vfs)