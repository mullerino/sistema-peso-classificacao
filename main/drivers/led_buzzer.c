#include "led_buzzer.h"
#include "config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "LED_BUZZER";

#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_DUTY_RES LEDC_TIMER_8_BIT // 0–255
#define LEDC_FREQUENCY 5000            // 5 kHz base

esp_err_t led_buzzer_init(void)
{
  ledc_timer_config_t led_timer = {
      .speed_mode = LEDC_MODE,
      .timer_num = LEDC_TIMER,
      .duty_resolution = LEDC_DUTY_RES,
      .freq_hz = LEDC_FREQUENCY,
      .clk_cfg = LEDC_AUTO_CLK,
  };
  ESP_ERROR_CHECK(ledc_timer_config(&led_timer));

  ledc_channel_config_t channels[] = {
      {.gpio_num = GPIO_LED_R, .speed_mode = LEDC_MODE, .channel = LEDC_CHANNEL_0, .timer_sel = LEDC_TIMER, .duty = 0},
      {.gpio_num = GPIO_LED_G, .speed_mode = LEDC_MODE, .channel = LEDC_CHANNEL_1, .timer_sel = LEDC_TIMER, .duty = 0},
      {.gpio_num = GPIO_LED_B, .speed_mode = LEDC_MODE, .channel = LEDC_CHANNEL_2, .timer_sel = LEDC_TIMER, .duty = 0},
      {.gpio_num = GPIO_BUZZER, .speed_mode = LEDC_MODE, .channel = LEDC_CHANNEL_3, .timer_sel = LEDC_TIMER, .duty = 0},
  };

  for (int i = 0; i < 4; i++)
    ESP_ERROR_CHECK(ledc_channel_config(&channels[i]));

  ESP_LOGI(TAG, "LED RGB e Buzzer inicializados");
  return ESP_OK;
}

static void led_apply_duty(uint32_t r, uint32_t g, uint32_t b)
{
  ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, r);
  ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
  ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, g);
  ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);
  ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_2, b);
  ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_2);
}

void led_set_color(led_color_t color)
{
  switch (color)
  {
  case LED_RED:
    led_apply_duty(255, 0, 0);
    break;
  case LED_GREEN:
    led_apply_duty(0, 255, 0);
    break;
  case LED_BLUE:
    led_apply_duty(0, 0, 255);
    break;
  case LED_YELLOW:
    led_apply_duty(255, 255, 0);
    break;
  case LED_PURPLE:
    led_apply_duty(255, 0, 255);
    break;
  case LED_CYAN:
    led_apply_duty(0, 255, 255);
    break;
  case LED_WHITE:
    led_apply_duty(255, 255, 255);
    break;
  case LED_OFF:
  default:
    led_apply_duty(0, 0, 0);
    break;
  }
}

void buzzer_beep(uint16_t freq_hz, uint16_t duration_ms)
{
  ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq_hz);
  ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_3, 180);
  ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_3);

  vTaskDelay(pdMS_TO_TICKS(duration_ms));

  ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_3, 0);
  ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_3);
}
