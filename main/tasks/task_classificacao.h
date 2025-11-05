#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "task_leitura_peso.h"
#include "task_feedback.h"

extern QueueHandle_t queue_peso;
extern QueueHandle_t queue_feedback;

// Configuração de limites
typedef struct
{
  float limite_inferior;
  float limite_superior;
} ClassificacaoLimites;

void task_classificacao(void *pvParameters);
