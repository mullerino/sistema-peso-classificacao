#include "task_registro.h"
#include "esp_log.h"

static const char *TAG = "TaskRegistro";
QueueHandle_t queue_pedido_hora = NULL;

void task_registro(void *pvParameters)
{
  rtc_ds3231_init();
  ESP_LOGI(TAG, "RTC DS3231 inicializado com sucesso");

  // ⚠️ AJUSTE MANUAL - EXECUTE UMA ÚNICA VEZ ⚠️
  // Define data/hora: 21/10/2025 às 11:20:00
  // struct tm agora = {
  //     .tm_year = 2025 - 1900,
  //     .tm_mon  = 9,      // Outubro (0 = Janeiro)
  //     .tm_mday = 21,
  //     .tm_hour = 11,
  //     .tm_min  = 43,
  //     .tm_sec  = 0
  // };

  // rtc_ds3231_set_time(&agora);
  // ESP_LOGI("AJUSTE_RTC", "Hora ajustada manualmente para 21/10/2025 11:20:00");


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
