#include <string.h>
#include <stdlib.h>

#include "esp_timer.h"
#include "telemetry_fmt.h"

void telemetry_fmt_sensor_readings(char* buf, size_t buf_len, const dht22_reading_t* sensor_data)
{
    // Check for NULL ptrs
    if (buf == NULL || sensor_data == NULL) return;

    // Get timestamp
    int64_t t_start = esp_timer_get_time();

    snprintf(buf, buf_len, "{\"temp\":%d.%d,\"hum\":%d.%d,\"ts\":%lld}",
        sensor_data->temperature_x10 / 10,
        abs(sensor_data->temperature_x10 % 10),
        sensor_data->humidity_x10 / 10,
        sensor_data->humidity_x10 % 10,
        t_start);
}