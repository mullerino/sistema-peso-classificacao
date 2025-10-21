#include "task_registro.h"
#include "esp_log.h"

static const char *TAG = "TaskRegistro";
QueueHandle_t queue_pedido_hora = NULL;

void task_registro(void *pvParameters)
{
  rtc_ds3231_init();
  ESP_LOGI(TAG, "RTC DS3231 inicializado com sucesso");

  PedidoHora pedido;
  RespostaHora resposta;

  while (1)
  {
    if (xQueueReceive(queue_pedido_hora, &pedido, portMAX_DELAY))
    {
      if (rtc_ds3231_get_time(&resposta.hora) == ESP_OK)
      {
        xQueueSend(pedido.respostaQueue, &resposta, portMAX_DELAY);

        char buffer[64];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &resposta.hora);
        ESP_LOGI(TAG, "Timestamp enviado: %s", buffer);
      }
      else
      {
        ESP_LOGE(TAG, "Falha ao obter hora do RTC");
      }
    }
  }
}
