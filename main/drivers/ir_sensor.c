#include "ir_sensor.h"
#include "driver/gpio.h"
#include "config.h"

esp_err_t ir_sensor_init(void)
{
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << IR_SENSOR_PIN),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE, // usa pull-up interno
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};
  return gpio_config(&io_conf);
}

bool ir_sensor_is_triggered(void)
{
  // Quando o feixe é recebido → nível 0
  // Quando o feixe é interrompido → nível 1
  int level = gpio_get_level(IR_SENSOR_PIN);
  return level == 1; // true = obstáculo detectado
}
