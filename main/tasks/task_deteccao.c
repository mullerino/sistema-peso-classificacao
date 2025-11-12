// #include "task_deteccao.h"
// #include "driver/gpio.h"
// #include "esp_log.h"

// static const char *TAG = "TaskDeteccao";

// extern SemaphoreHandle_t sem_item_detectado;

// #define GPIO_BOTAO GPIO_NUM_0

// void task_deteccao(void *pvParameters)

// {
//   ESP_LOGI(TAG, "TaskDeteccao iniciada (usando GPIO%d como botão)", GPIO_BOTAO);

//   gpio_config_t io_conf = {
//       .pin_bit_mask = 1ULL << GPIO_BOTAO,
//       .mode = GPIO_MODE_INPUT,
//       .pull_up_en = GPIO_PULLUP_ENABLE,
//       .pull_down_en = GPIO_PULLDOWN_DISABLE,
//       .intr_type = GPIO_INTR_DISABLE,
//   };

//   gpio_config(&io_conf);

//   int estado_anterior = 1;

//   while (1)
//   {
//     int estado = gpio_get_level(GPIO_BOTAO);

//     if (estado == 0 && estado_anterior == 1)
//     {
//       xSemaphoreGive(sem_item_detectado);
//       vTaskDelay(pdMS_TO_TICKS(500));
//     }

//     estado_anterior = estado;

//     vTaskDelay(pdMS_TO_TICKS(50));
//   }
// }

#include "task_deteccao.h"
#include "ir_sensor.h"
#include "esp_log.h"

static const char *TAG = "TaskDeteccao";
extern SemaphoreHandle_t sem_item_detectado;

void task_deteccao(void *pvParameters)
{
  ESP_LOGI(TAG, "TaskDeteccao iniciada (usando sensor IR)");

  // Inicializa o sensor infravermelho
  if (ir_sensor_init() != ESP_OK)
  {
    ESP_LOGE(TAG, "Falha ao inicializar sensor IR");
    vTaskDelete(NULL);
  }

  bool estado_anterior = false;

  while (1)
  {
    bool detectado = ir_sensor_is_triggered();

    if (detectado && !estado_anterior)
    {
      ESP_LOGI(TAG, "Feixe interrompido → item detectado");
      xSemaphoreGive(sem_item_detectado);
    }

    estado_anterior = detectado;
    vTaskDelay(pdMS_TO_TICKS(50)); // amostragem a cada 50ms
  }
}
