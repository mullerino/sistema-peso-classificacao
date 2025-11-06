#include "task_deteccao.h"
#include "spiffs_manager.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "TaskDeteccao";
extern SemaphoreHandle_t sem_item_detectado;

#define GPIO_BOTAO GPIO_NUM_0

void task_deteccao(void *pvParameters)
{
  ESP_LOGI(TAG, "TaskDeteccao iniciada (usando GPIO%d como botão)", GPIO_BOTAO);

  gpio_config_t io_conf = {
      .pin_bit_mask = 1ULL << GPIO_BOTAO,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  gpio_config(&io_conf);

  int estado_anterior = 1;

  while (1)
  {
    int estado = gpio_get_level(GPIO_BOTAO);

    if (estado == 0 && estado_anterior == 1)
    {
      ESP_LOGI(TAG, "Botão pressionado → simulando detecção");

      spiffs_manager_read_file("/spiffs/registros.csv");

      xSemaphoreGive(sem_item_detectado);
      vTaskDelay(pdMS_TO_TICKS(500));
    }

    estado_anterior = estado;
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
