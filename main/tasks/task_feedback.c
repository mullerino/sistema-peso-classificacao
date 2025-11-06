#include "task_feedback.h"
#include "drivers/led_buzzer.h"
#include "esp_log.h"

static const char *TAG = "TaskFeedback";
QueueHandle_t queue_feedback = NULL;

void task_feedback(void *pvParameters)
{
  led_buzzer_init();
  feedback_event_t evento;

  ESP_LOGI(TAG, "TaskFeedback iniciada");

  while (1)
  {
    if (xQueueReceive(queue_feedback, &evento, portMAX_DELAY))
    {
      switch (evento)
      {
      case FEEDBACK_APROVADO:
        led_set_color(LED_BLUE);
        buzzer_beep(2000, 100);
        led_set_color(LED_OFF);
        break;

      case FEEDBACK_REPROVADO:
        led_set_color(LED_RED);
        buzzer_beep(1000, 400);
        led_set_color(LED_OFF);
        break;

      case FEEDBACK_IDLE:
      default:
        led_set_color(LED_OFF);
        break;
      }
    }
  }
}
