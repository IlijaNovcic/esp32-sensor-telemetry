#include "dht22.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#define DHT22_MAX_WAIT_US   2000
#define DHT22_BIT_ONE_US    50

static gpio_num_t s_gpio_pin = GPIO_NUM_NC;

dht22_err_t dht22_init(gpio_num_t gpio_pin)
{
    if (gpio_pin < GPIO_NUM_0 || gpio_pin >= GPIO_NUM_MAX) {
        return DHT22_ERR_ARG;
    }

    s_gpio_pin = gpio_pin;

    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << gpio_pin),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };

    gpio_config(&cfg);
    gpio_set_level(s_gpio_pin, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));

    return DHT22_OK;
}

dht22_err_t dht22_read(dht22_reading_t *out)
{
    if (out == NULL || s_gpio_pin == GPIO_NUM_NC) {
        return DHT22_ERR_ARG;
    }

    uint8_t data[5] = {0};

    /* Start signal */
    gpio_set_direction(s_gpio_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(s_gpio_pin, 0);
    ets_delay_us(1000);
    gpio_set_level(s_gpio_pin, 1);
    gpio_set_direction(s_gpio_pin, GPIO_MODE_INPUT);
    // Enable internal pull-up resistor
    gpio_set_pull_mode(s_gpio_pin, GPIO_PULLUP_ONLY); 
    
    /* Wait for sensor response LOW */
    int64_t t_start = esp_timer_get_time();

    while (gpio_get_level(s_gpio_pin) == 1) {
        if ((esp_timer_get_time() - t_start) > DHT22_MAX_WAIT_US) {
            return DHT22_ERR_TIMEOUT;
        }
    }

    t_start = esp_timer_get_time();

    while (gpio_get_level(s_gpio_pin) == 0) {
        if ((esp_timer_get_time() - t_start) > DHT22_MAX_WAIT_US) {
            return DHT22_ERR_TIMEOUT;
        }
    }

    t_start = esp_timer_get_time();
    while (gpio_get_level(s_gpio_pin) == 1) {
        if ((esp_timer_get_time() - t_start) > DHT22_MAX_WAIT_US) {
            return DHT22_ERR_TIMEOUT;
        }
    }

    /* Read 40 bits */
    for (int i = 0; i < 40; i++) {

        /* Wait for LOW to finish */
        t_start = esp_timer_get_time();
        while (gpio_get_level(s_gpio_pin) == 0) {
            if ((esp_timer_get_time() - t_start) > DHT22_MAX_WAIT_US) {
                return DHT22_ERR_TIMEOUT;
            }
        }

        /* Measure HIGH duration */
        int64_t t_high = esp_timer_get_time();
        while (gpio_get_level(s_gpio_pin) == 1) {
            if ((esp_timer_get_time() - t_high) > DHT22_MAX_WAIT_US) {
                return DHT22_ERR_TIMEOUT;
            }
        }

        int64_t high_us = esp_timer_get_time() - t_high;

        data[i / 8] |= (high_us > DHT22_BIT_ONE_US) << (7 - i % 8);
    }

    /* Checksum */
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        return DHT22_ERR_CHECKSUM;
    }

    /* Parse humidity */
    out->humidity_x10 = (data[0] << 8) | data[1];

    /* Parse temperature */
    out->temperature_x10 = ((data[2] & 0x7F) << 8) | data[3];
    if (data[2] & 0x80) {
        out->temperature_x10 = -out->temperature_x10;
    }

    return DHT22_OK;
}