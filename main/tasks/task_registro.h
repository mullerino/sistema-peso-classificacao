#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "drivers/rtc_ds3231.h"
#include <time.h>

typedef struct
{
  uint32_t id_mensagem;
  QueueHandle_t respostaQueue;
} PedidoHora;

typedef struct
{
  struct tm hora;
} RespostaHora;

extern QueueHandle_t queue_pedido_hora;

void task_registro(void *pvParameters);
