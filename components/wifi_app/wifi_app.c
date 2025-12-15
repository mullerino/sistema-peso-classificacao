#include "wifi_app.h"

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "lwip/inet.h"
#include "config.h"

#include "http_server.h"

static const char TAG[] = "wifi_app";

static QueueHandle_t wifi_app_queue_handle;

esp_netif_t *esp_netif_sta = NULL;
esp_netif_t *esp_netif_ap = NULL;

static void wifi_app_event_handler(void *arg,
                                   esp_event_base_t event_base,
                                   int32_t event_id,
                                   void *event_data)
{
  if (event_base == WIFI_EVENT)
  {
    switch (event_id)
    {
    case WIFI_EVENT_AP_START:
      ESP_LOGI(TAG, "AP iniciado");
      break;

    case WIFI_EVENT_STA_CONNECTED:
      ESP_LOGI(TAG, "STA conectada ao AP externo");
      break;

    case WIFI_EVENT_STA_DISCONNECTED:
      ESP_LOGW(TAG, "STA desconectada");
      wifi_app_send_message(WIFI_APP_MSG_STA_DISCONNECTED);
      break;

    default:
      break;
    }
  }
  else if (event_base == IP_EVENT)
  {
    if (event_id == IP_EVENT_STA_GOT_IP)
    {
      ESP_LOGI(TAG, "STA obteve IP");
      wifi_app_send_message(WIFI_APP_MSG_STA_CONNECTED);
    }
  }
}

static void wifi_app_event_handler_init(void)
{
  ESP_ERROR_CHECK(
      esp_event_handler_register(WIFI_EVENT,
                                 ESP_EVENT_ANY_ID,
                                 &wifi_app_event_handler,
                                 NULL));

  ESP_ERROR_CHECK(
      esp_event_handler_register(IP_EVENT,
                                 ESP_EVENT_ANY_ID,
                                 &wifi_app_event_handler,
                                 NULL));
}

static void wifi_app_default_init(void)
{
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  esp_netif_sta = esp_netif_create_default_wifi_sta();
  esp_netif_ap = esp_netif_create_default_wifi_ap();
}

static void wifi_app_softap_config(void)
{
  wifi_config_t ap_config = {
      .ap = {
          .ssid = WIFI_AP_SSID,
          .ssid_len = strlen(WIFI_AP_SSID),
          .password = WIFI_AP_PASSWORD,
          .channel = WIFI_AP_CHANNEL,
          .max_connection = WIFI_AP_MAX_CONNECTIONS,
          .authmode = WIFI_AUTH_WPA2_PSK,
      },
  };

  esp_netif_ip_info_t ip_info = {0};

  esp_netif_dhcps_stop(esp_netif_ap);
  inet_pton(AF_INET, WIFI_AP_IP, &ip_info.ip);
  inet_pton(AF_INET, WIFI_AP_GATEWAY, &ip_info.gw);
  inet_pton(AF_INET, WIFI_AP_NETMASK, &ip_info.netmask);
  ESP_ERROR_CHECK(esp_netif_set_ip_info(esp_netif_ap, &ip_info));
  ESP_ERROR_CHECK(esp_netif_dhcps_start(esp_netif_ap));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
}

static void wifi_app_task(void *pvParameters)
{
  wifi_app_queue_message_t msg;

  wifi_app_event_handler_init();
  wifi_app_default_init();
  wifi_app_softap_config();

  ESP_ERROR_CHECK(esp_wifi_start());

  wifi_app_send_message(WIFI_APP_MSG_START_HTTP_SERVER);

  for (;;)
  {
    if (xQueueReceive(wifi_app_queue_handle, &msg, portMAX_DELAY))
    {
      switch (msg.msgID)
      {
      case WIFI_APP_MSG_START_HTTP_SERVER:
        ESP_LOGI(TAG, "Iniciando servidor HTTP");
        http_server_start();
        break;

      case WIFI_APP_MSG_STA_CONNECTED:
        ESP_LOGI(TAG, "Wi-Fi conectado");
        break;

      case WIFI_APP_MSG_STA_DISCONNECTED:
        ESP_LOGW(TAG, "Wi-Fi desconectado");
        break;

      default:
        break;
      }
    }
  }
}

BaseType_t wifi_app_send_message(wifi_app_message_e msgID)
{
  wifi_app_queue_message_t msg = {
      .msgID = msgID};
  return xQueueSend(wifi_app_queue_handle, &msg, portMAX_DELAY);
}

void wifi_app_start(void)
{
  ESP_LOGI(TAG, "Inicializando Wi-Fi App");

  esp_log_level_set("wifi", ESP_LOG_NONE);

  wifi_app_queue_handle = xQueueCreate(5, sizeof(wifi_app_queue_message_t));

  xTaskCreatePinnedToCore(
      wifi_app_task,
      "wifi_app_task",
      WIFI_APP_TASK_STACK_SIZE,
      NULL,
      WIFI_APP_TASK_PRIORITY,
      NULL,
      WIFI_APP_TASK_CORE_ID);
}
