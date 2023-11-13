idf_build_get_property(target IDF_TARGET)

if(${target} STREQUAL "linux")
    # Header only library for linux

    idf_component_register(INCLUDE_DIRS include
                           SRCS gbaby_wifi.c)
    return()
endif()

set(srcs "gbaby_wifi.c"
         "connect.c")



idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_netif driver esp_wifi vfs)

