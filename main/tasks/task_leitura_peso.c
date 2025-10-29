#include "task_leitura_peso.h"
#include "hx711_driver.h"
#include "esp_log.h"

static const char *TAG = "TaskLeituraPeso";
QueueHandle_t queue_peso = NULL;

void task_leitura_peso(void *pvParameters)
{
  hx711_driver_init();
  hx711_driver_set_calibration(210.5f, 0); // valores a ajustar

  ESP_LOGI(TAG, "TaskLeituraPeso iniciada");

  while (1)
  {
    PesoMedido medida;
    medida.peso = hx711_driver_read();

    if (medida.peso != 0)
    {
      xQueueSend(queue_peso, &medida, 0);
      ESP_LOGI(TAG, "Peso: %.2f g", medida.peso);
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
