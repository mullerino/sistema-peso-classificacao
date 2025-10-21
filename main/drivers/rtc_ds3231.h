#pragma once
#include "driver/i2c.h"
#include "esp_err.h"
#include <time.h>

esp_err_t rtc_ds3231_init(void);
esp_err_t rtc_ds3231_get_time(struct tm *timeinfo);
esp_err_t rtc_ds3231_set_time(const struct tm *timeinfo);

