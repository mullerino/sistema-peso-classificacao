#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef enum
{
  OBJETO_NENHUM = 0,
  OBJETO_ENTROU,
  OBJETO_SAIU
} EventoDeteccao;

extern QueueHandle_t queue_deteccao;

void task_deteccao(void *pvParameters);
