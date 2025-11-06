#include "task_classificacao.h"
#include "drivers/spiffs_manager.h"
#include "task_registro.h"
#include "esp_log.h"

static const char *TAG = "TaskClassificacao";

static const ClassificacaoLimites limites = {
    .limite_inferior = 300.0f,
    .limite_superior = 500.0f,
};

void task_classificacao(void *pvParameters)
{
  ESP_LOGI(TAG, "TaskClassificacao iniciada (limites %.2f - %.2f g)",
           limites.limite_inferior, limites.limite_superior);

  while (1)
  {
    PesoMedido medida;

    if (xQueueReceive(queue_peso, &medida, portMAX_DELAY))
    {
      ESP_LOGI(TAG, "Peso recebido: %.2f g", medida.peso);

      feedback_event_t feedback;

      if (medida.peso >= limites.limite_inferior && medida.peso <= limites.limite_superior)
      {
        ESP_LOGI(TAG, "Classificação: APROVADO");
        feedback = FEEDBACK_APROVADO;
      }
      else
      {
        ESP_LOGW(TAG, "Classificação: REPROVADO");
        feedback = FEEDBACK_REPROVADO;
      }

      xQueueSend(queue_feedback, &feedback, 0);

      PedidoHora pedido = {
          .id_mensagem = 1,
          .respostaQueue = xQueueCreate(1, sizeof(RespostaHora))};

      xQueueSend(queue_pedido_hora, &pedido, portMAX_DELAY);

      RespostaHora resposta;
      if (xQueueReceive(pedido.respostaQueue, &resposta, pdMS_TO_TICKS(2000)))
      {
        char buffer[32];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", &resposta.hora);

        spiffs_manager_save_event(
            buffer,
            medida.peso,
            feedback == FEEDBACK_APROVADO ? "APROVADO" : "REPROVADO");

        ESP_LOGI("TaskClassificacao", "Evento registrado às %s (%.2f g - %s)",
                 buffer,
                 medida.peso,
                 feedback == FEEDBACK_APROVADO ? "APROVADO" : "REPROVADO");
      }
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
