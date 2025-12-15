#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_netif.h"

typedef enum
{
  WIFI_APP_MSG_START_HTTP_SERVER = 0,
  WIFI_APP_MSG_STA_CONNECTED,
  WIFI_APP_MSG_STA_DISCONNECTED,
} wifi_app_message_e;

typedef struct
{
  wifi_app_message_e msgID;
} wifi_app_queue_message_t;

void wifi_app_start(void);
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

extern esp_netif_t *esp_netif_sta;
extern esp_netif_t *esp_netif_ap;
