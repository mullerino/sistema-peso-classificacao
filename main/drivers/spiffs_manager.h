#pragma once
#include "esp_err.h"

esp_err_t spiffs_manager_init(void);

void spiffs_manager_save_event(const char *timestamp, float peso, const char *status);

void spiffs_manager_read_file(const char *path);
