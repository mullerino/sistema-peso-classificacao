#include "hx711_driver.h"
#include "hx711.h"
#include "config.h"
#include "esp_log.h"

static const char *TAG = "HX711_DRIVER";
static hx711_t hx;
static float scale_factor = 1.0f;
static int offset = 0;

esp_err_t hx711_driver_init(void)
{
  // Configura estrutura de pinos e ganho
  hx.dout = HX711_DOUT;
  hx.pd_sck = HX711_SCK;
  hx.gain = HX711_GAIN_A_128;

  esp_err_t ret = hx711_init(&hx);

  if (ret != ESP_OK)
  {
    ESP_LOGE(TAG, "Falha ao inicializar HX711");
    return ret;
  }

  // Liga o módulo (down = false)
  hx711_power_down(&hx, false);

  ESP_LOGI(TAG, "HX711 inicializado (DOUT=%d, SCK=%d)", HX711_DOUT, HX711_SCK);
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

  // Aguarda dado disponível
  if (hx711_wait(&hx, 1000) != ESP_OK)
  {
    ESP_LOGW(TAG, "Timeout aguardando dado HX711");
    return 0;
  }

  if (hx711_read_average(&hx, 10, &raw) != ESP_OK)
  {
    ESP_LOGW(TAG, "Erro ao ler HX711");
    return 0;
  }

  float weight = ((float)(raw - offset)) / scale_factor;

  return weight;
}
