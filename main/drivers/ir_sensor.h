#pragma once
#include "esp_err.h"
#include <stdbool.h>

typedef enum
{
  IR_SENSOR_ENTRADA = 0,
  IR_SENSOR_SAIDA
} ir_sensor_id_t;

esp_err_t ir_sensor_init(void);
bool ir_sensor_is_triggered(ir_sensor_id_t sensor);
