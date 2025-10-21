#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "tasks/task_registro.h"
#include "esp_log.h"

void app_main(void)
{
  queue_pedido_hora = xQueueCreate(5, sizeof(PedidoHora));

  xTaskCreate(task_registro, "TaskRegistro", 4096, NULL, 5, NULL);

  while (1)
  {
    PedidoHora pedido = {
        .id_mensagem = 1,
        .respostaQueue = xQueueCreate(1, sizeof(RespostaHora))};

    xQueueSend(queue_pedido_hora, &pedido, portMAX_DELAY);

    RespostaHora resposta;
    if (xQueueReceive(pedido.respostaQueue, &resposta, pdMS_TO_TICKS(2000)))
    {
      char buffer[64];
      strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &resposta.hora);
      ESP_LOGI("APP_MAIN", "Hora recebida: %s", buffer);
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
