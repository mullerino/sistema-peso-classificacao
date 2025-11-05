#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tasks/task_deteccao.h"
#include "tasks/task_classificacao.h"
#include "freertos/queue.h"
#include "tasks/task_registro.h"
#include "tasks/task_leitura_peso.h"
#include "tasks/task_feedback.h"
#include "esp_log.h"

static const char *TAG = "MAIN";
SemaphoreHandle_t sem_item_detectado;

void app_main(void)
{
  ESP_LOGI(TAG, "Inicializando sistema de verificação e classificação de peso...");

  queue_peso = xQueueCreate(5, sizeof(PesoMedido));
  queue_pedido_hora = xQueueCreate(5, sizeof(PedidoHora));
  queue_feedback = xQueueCreate(5, sizeof(feedback_event_t));

  sem_item_detectado = xSemaphoreCreateBinary();

  xTaskCreate(task_deteccao, "TaskDeteccao", 4096, NULL, 5, NULL);
  xTaskCreate(task_leitura_peso, "TaskLeituraPeso", 4096, NULL, 5, NULL);
  xTaskCreate(task_registro, "TaskRegistro", 4096, NULL, 4, NULL);
  xTaskCreate(task_feedback, "TaskFeedback", 4096, NULL, 3, NULL);
  xTaskCreate(task_classificacao, "TaskClassificacao", 4096, NULL, 6, NULL);
}
