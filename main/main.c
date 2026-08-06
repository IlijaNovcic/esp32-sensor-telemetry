#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht22.h"

void dht22_task(void *pvParameters)
{
    dht22_init(GPIO_NUM_26);

    while (1) {
        dht22_reading_t reading;
        dht22_err_t err = dht22_read(&reading);

        if (err == DHT22_OK) {
            printf("Temp: %d.%d C  Hum: %d.%d %%\n",
                reading.temperature_x10 / 10,
                reading.temperature_x10 % 10,
                reading.humidity_x10 / 10,
                reading.humidity_x10 % 10);
        } else if (err == DHT22_ERR_TIMEOUT) {
            printf("Error: TIMEOUT\n");
        } else if (err == DHT22_ERR_CHECKSUM) {
            printf("Error: CHECKSUM\n");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    printf("esp32-sensor-telemetry: system start\n");
    xTaskCreatePinnedToCore(dht22_task, "dht22_task", 2048, NULL, 5, NULL, 1);
}