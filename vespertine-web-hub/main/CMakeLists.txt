idf_component_register(SRCS "main.c"
                            "rest_server.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_netif driver esp_wifi vfs nvs_flash fatfs spiffs esp_http_server json
                    REQUIRES gbaby_wifi mdns gbaby_fs)


set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../front/web-demo")
if(EXISTS ${WEB_SRC_DIR}/dist)
    spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
else()
    message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Please run 'npm run build' in ${WEB_SRC_DIR}")
endif()
