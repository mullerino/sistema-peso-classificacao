#include <stdio.h>
#include <time.h>
#include "nvs_flash.h"
#include "drivers/spiffs_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tasks/task_deteccao.h"
#include "tasks/task_classificacao.h"
#include "freertos/queue.h"
#include "tasks/task_registro.h"
#include "tasks/task_leitura_peso.h"
#include "tasks/task_feedback.h"
#include "esp_log.h"
#include "wifi_app.h"
#include "esp_event.h"
#include "esp_netif.h"

static const char *TAG = "MAIN";
SemaphoreHandle_t sem_item_detectado;

TaskHandle_t handle_deteccao;
TaskHandle_t handle_leitura_peso;
TaskHandle_t handle_classificacao;
TaskHandle_t handle_registro;
TaskHandle_t handle_feedback;

void app_main(void)
{
  ESP_LOGI(TAG, "Inicializando sistema de verificação e classificação de peso...");

  esp_err_t ret_nvs = nvs_flash_init();
  if (ret_nvs == ESP_ERR_NVS_NO_FREE_PAGES || ret_nvs == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_flash_init());
  }

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_err_t ret = spiffs_manager_init();
  if (ret != ESP_OK)
  {
    ESP_LOGE(TAG, "Falha ao montar SPIFFS. Continuando sem armazenamento local.");
  }

  wifi_app_start();

  queue_peso = xQueueCreate(5, sizeof(PesoMedido));
  queue_pedido_hora = xQueueCreate(5, sizeof(PedidoHora));
  queue_feedback = xQueueCreate(5, sizeof(feedback_event_t));

  sem_item_detectado = xSemaphoreCreateBinary();

  xTaskCreate(task_deteccao, "TaskDeteccao", 4096, NULL, 5, &handle_deteccao);
  xTaskCreate(task_leitura_peso, "TaskLeituraPeso", 4096, NULL, 5, &handle_leitura_peso);
  xTaskCreate(task_classificacao, "TaskClassificacao", 4096, NULL, 6, &handle_classificacao);
  xTaskCreate(task_registro, "TaskRegistro", 4096, NULL, 4, &handle_registro);
  xTaskCreate(task_feedback, "TaskFeedback", 4096, NULL, 3, &handle_feedback);
}
