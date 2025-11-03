#include "ir_sensor.h"
#include "driver/gpio.h"
#include "config.h"

esp_err_t ir_sensor_init(void)
{
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << IR_ENTRADA) | (1ULL << IR_SAIDA),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};
  return gpio_config(&io_conf);
}

bool ir_sensor_is_triggered(ir_sensor_id_t sensor)
{
  gpio_num_t pin = (sensor == IR_SENSOR_ENTRADA) ? IR_ENTRADA : IR_SAIDA;
  return gpio_get_level(pin) == 0;
}
