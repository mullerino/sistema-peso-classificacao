#include "task_deteccao.h"
#include "ir_sensor.h"
#include "esp_log.h"

static const char *TAG = "TaskDeteccao";
QueueHandle_t queue_deteccao = NULL;

void task_deteccao(void *pvParameters)
{
  ir_sensor_init();
  ESP_LOGI(TAG, "TaskDeteccao iniciada");

  bool estadoEntradaAnt = false;
  bool estadoSaidaAnt = false;

  while (1)
  {
    bool entrada = ir_sensor_is_triggered(IR_SENSOR_ENTRADA);
    bool saida = ir_sensor_is_triggered(IR_SENSOR_SAIDA);

    if (entrada && !estadoEntradaAnt)
    {
      EventoDeteccao ev = OBJETO_ENTROU;
      xQueueSend(queue_deteccao, &ev, 0);
      ESP_LOGI(TAG, "Objeto detectado na ENTRADA");
    }

    if (saida && !estadoSaidaAnt)
    {
      EventoDeteccao ev = OBJETO_SAIU;
      xQueueSend(queue_deteccao, &ev, 0);
      ESP_LOGI(TAG, "Objeto detectado na SAÍDA");
    }

    estadoEntradaAnt = entrada;
    estadoSaidaAnt = saida;

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
