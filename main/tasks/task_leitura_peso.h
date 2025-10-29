#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef struct
{
  float peso;
} PesoMedido;

extern QueueHandle_t queue_peso;

void task_leitura_peso(void *pvParameters);
