#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "drivers/rtc_ds3231.h"

void app_main(void)
{
  rtc_ds3231_init();

  while (1)
  {
    struct tm timeinfo;

    if (rtc_ds3231_get_time(&timeinfo) == ESP_OK)
    {
      char buffer[64];
      strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
      ESP_LOGI("APP_MAIN", "Hora atual: %s", buffer);
    }
    else
    {
      ESP_LOGE("APP_MAIN", "Falha ao ler RTC");
    }

    vTaskDelay(pdMS_TO_TICKS(3000));
  }
}
