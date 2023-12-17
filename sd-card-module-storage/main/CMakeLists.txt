idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES fatfs
                    PRIV_REQUIRES eco_bloom_storage)