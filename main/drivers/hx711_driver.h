#pragma once
#include "hx711_driver.h"
#include "esp_err.h"

esp_err_t hx711_driver_init(void);

float hx711_driver_read(void);

void hx711_driver_set_calibration(float scale, int offset);
