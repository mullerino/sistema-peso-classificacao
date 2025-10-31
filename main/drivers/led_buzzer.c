#include "led_buzzer.h"
#include "config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "LED_BUZZER";

#define LEDC_MODE_LED LEDC_LOW_SPEED_MODE
#define LEDC_TIMER_LED LEDC_TIMER_0
#define LEDC_DUTY_RES_LED LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY_LED 5000

esp_err_t led_buzzer_init(void)
{
  ledc_timer_config_t led_timer = {
      .speed_mode = LEDC_MODE_LED,
      .timer_num = LEDC_TIMER_LED,
      .duty_resolution = LEDC_DUTY_RES_LED,
      .freq_hz = LEDC_FREQUENCY_LED,
      .clk_cfg = LEDC_AUTO_CLK,
  };
  ESP_ERROR_CHECK(ledc_timer_config(&led_timer));

  gpio_config_t buzzer_gpio = {
      .pin_bit_mask = (1ULL << GPIO_BUZZER),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};
  gpio_config(&buzzer_gpio);

  ledc_channel_config_t channels[] = {
      {.gpio_num = GPIO_LED_R, .speed_mode = LEDC_MODE_LED, .channel = LEDC_CHANNEL_0, .timer_sel = LEDC_TIMER_LED, .duty = 0},
      {.gpio_num = GPIO_LED_G, .speed_mode = LEDC_MODE_LED, .channel = LEDC_CHANNEL_1, .timer_sel = LEDC_TIMER_LED, .duty = 0},
      {.gpio_num = GPIO_LED_B, .speed_mode = LEDC_MODE_LED, .channel = LEDC_CHANNEL_2, .timer_sel = LEDC_TIMER_LED, .duty = 0},
  };

  for (int i = 0; i < 3; i++)
    ESP_ERROR_CHECK(ledc_channel_config(&channels[i]));

  ESP_LOGI(TAG, "LED RGB e Buzzer inicializados");
  return ESP_OK;
}

void led_apply_duty(uint32_t r, uint32_t g, uint32_t b)
{
  ledc_set_duty(LEDC_MODE_LED, LEDC_CHANNEL_0, r);
  ledc_update_duty(LEDC_MODE_LED, LEDC_CHANNEL_0);

  ledc_set_duty(LEDC_MODE_LED, LEDC_CHANNEL_1, g);
  ledc_update_duty(LEDC_MODE_LED, LEDC_CHANNEL_1);

  ledc_set_duty(LEDC_MODE_LED, LEDC_CHANNEL_2, b);
  ledc_update_duty(LEDC_MODE_LED, LEDC_CHANNEL_2);
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
  gpio_set_level(GPIO_BUZZER, 1);
  vTaskDelay(pdMS_TO_TICKS(duration_ms));
  gpio_set_level(GPIO_BUZZER, 0);
}
