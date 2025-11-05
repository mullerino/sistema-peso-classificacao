#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

typedef struct
{
  float peso;
} PesoMedido;

extern QueueHandle_t queue_peso;
extern SemaphoreHandle_t sem_item_detectado;

void task_leitura_peso(void *pvParameters);
