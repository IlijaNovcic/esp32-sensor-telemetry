#pragma once

#include "dht22.h"

class SensorManager
{
    public:

        // Constructor used to initialize dth22 sensor
        explicit SensorManager(gpio_num_t gpio_pin)
        :_gpio_pin(gpio_pin)
        {
            dht22_init(gpio_pin);
        }

        // Method used to read and return data from the sensor
        dht22_err_t read(dht22_reading_t* out)
        {
            dht22_err_t ret_val = DHT22_OK;

            // trigger driver layer
            ret_val = dht22_read(out);

            if(ret_val == DHT22_OK)
                last_sensor_reading = *out;

            return ret_val;
        }

        // Destructor - return internal/private fields to initial state
        ~SensorManager() {last_sensor_reading = {0, 0}; _gpio_pin = GPIO_NUM_NC;}
    private:
        // Keep last_sensor_reading in init state
        dht22_reading_t last_sensor_reading = {0, 0};
        // Keep private gpio_pin in undefined state before it is forwarded via constructor
        gpio_num_t _gpio_pin = GPIO_NUM_NC;
};