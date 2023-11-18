idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver
                    REQUIRES gbaby_esp shift_reley_module)