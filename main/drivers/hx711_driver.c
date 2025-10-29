#include "hx711_driver.h"
#include "config.h"
#include "esp_log.h"

static const char *TAG = "HX711_DRIVER";
static hx711_t hx;
static float scale_factor = 1.0f;
static int offset = 0;

esp_err_t hx711_driver_init(void)
{
  hx711_init(&hx, HX711_DOUT, HX711_SCK);
  hx711_power_up(&hx);

  ESP_LOGI(TAG, "HX711 iniciado (DOUT=%d, SCK=%d)", HX711_DOUT, HX711_SCK);
  return ESP_OK;
}

void hx711_driver_set_calibration(float scale, int offs)
{
  scale_factor = scale;
  offset = offs;
}

float hx711_driver_read(void)
{
  int32_t raw;
  if (hx711_wait(&hx, 1000) != ESP_OK)
  {
    ESP_LOGW(TAG, "Timeout aguardando dado HX711");
    return 0;
  }

  hx711_read(&hx, &raw);
  float weight = ((float)(raw - offset)) / scale_factor;
  return weight;
}
