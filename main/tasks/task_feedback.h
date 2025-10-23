#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef enum
{
  FEEDBACK_APROVADO,
  FEEDBACK_REPROVADO,
  FEEDBACK_IDLE
} feedback_event_t;

extern QueueHandle_t queue_feedback;

void task_feedback(void *pvParameters);
