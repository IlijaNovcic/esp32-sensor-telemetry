#pragma once 
#include <stddef.h>

#include "dht22.h"

/** @brief API used to transfer sensor_data to the string used from USER (AWS format).
 *  @param buf - buffer used to write formated string
 *  @param buf_len - string length
 *  @param sensor_data - readings from dht22 sensor (temperature and humidity) */ 
void telemetry_fmt_sensor_readings(char* buf, size_t buf_len, const dht22_reading_t* sensor_data);