idf_component_register(SRCS "main.cpp"
                        INCLUDE_DIRS ""
                        REQUIRES dht22_driver cmd_parser telemetry_fmt sensor_manager)