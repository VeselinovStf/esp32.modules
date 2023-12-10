idf_component_register(SRCS "gbaby_dht.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES driver
                    REQUIRES esp_idf_lib_helpers)
