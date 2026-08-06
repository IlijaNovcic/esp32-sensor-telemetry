#pragma once

#include <stdint.h>
#include "driver/gpio.h"

/* ------------------------------------------------------------------ */
/*  Public types                                                        */
/* ------------------------------------------------------------------ */

/**
 * @brief Return codes for all DHT22 operations.
 */
typedef enum {
    DHT22_OK            =  0,   /* Success                            */
    DHT22_ERR_TIMEOUT   = -1,   /* Line did not transition in time    */
    DHT22_ERR_CHECKSUM  = -2,   /* Received checksum does not match   */
    DHT22_ERR_ARG       = -3,   /* Invalid argument passed by caller  */
} dht22_err_t;

/**
 * @brief A single sensor reading.
 *
 * Temperature and humidity are stored as integers scaled by 10
 * to avoid floating point on the driver level.
 * Example: temperature_x10 = 235  means 23.5 °C
 *          humidity_x10    = 612  means 61.2 %RH
 */
typedef struct {
    int16_t  temperature_x10;   /* Temperature in 0.1 °C units       */
    uint16_t humidity_x10;      /* Humidity    in 0.1 %RH units      */
} dht22_reading_t;

/* ------------------------------------------------------------------ */
/*  Public API                                                          */
/* ------------------------------------------------------------------ */

/**
 * @brief  Initialize the DHT22 driver for a given GPIO pin.
 * @param  gpio_pin  GPIO number connected to the DHT22 data line.
 * @return DHT22_OK or DHT22_ERR_ARG if pin is invalid.
 */
dht22_err_t dht22_init(gpio_num_t gpio_pin);

/**
 * @brief  Read temperature and humidity from the sensor.
 * @param  out  Pointer to a dht22_reading_t struct to fill.
 * @return DHT22_OK, DHT22_ERR_TIMEOUT, or DHT22_ERR_CHECKSUM.
 */
dht22_err_t dht22_read(dht22_reading_t *out);