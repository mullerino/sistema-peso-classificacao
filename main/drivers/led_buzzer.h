#pragma once
#include "driver/ledc.h"
#include "esp_err.h"

// Enum para as cores básicas
typedef enum
{
  LED_OFF,
  LED_RED,
  LED_GREEN,
  LED_BLUE,
  LED_YELLOW,
  LED_PURPLE,
  LED_CYAN,
  LED_WHITE
} led_color_t;

esp_err_t led_buzzer_init(void);

void led_set_color(led_color_t color);

void buzzer_beep(uint16_t freq_hz, uint16_t duration_ms);
