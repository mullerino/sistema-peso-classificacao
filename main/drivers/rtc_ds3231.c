#include "rtc_ds3231.h"
#include "esp_log.h"
#include "config.h"

static const char *TAG = "RTC";

static esp_err_t i2c_master_init(void)
{
  i2c_config_t conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = I2C_MASTER_SDA_IO,
      .scl_io_num = I2C_MASTER_SCL_IO,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = I2C_MASTER_FREQ_HZ,
  };
  i2c_param_config(I2C_MASTER_NUM, &conf);
  return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

static uint8_t bcd_to_dec(uint8_t val) { return (val >> 4) * 10 + (val & 0x0F); }

esp_err_t rtc_ds3231_init(void)
{
  esp_err_t ret = i2c_master_init();
  if (ret == ESP_OK)
    ESP_LOGI(TAG, "RTC DS3231 inicializado com sucesso");
  else
    ESP_LOGE(TAG, "Falha ao inicializar I2C");
  return ret;
}

esp_err_t rtc_ds3231_get_time(struct tm *timeinfo)
{
  uint8_t data[7];

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (DS3231_ADDR << 1) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, 0x00, true);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (DS3231_ADDR << 1) | I2C_MASTER_READ, true);
  i2c_master_read(cmd, data, 7, I2C_MASTER_LAST_NACK);
  i2c_master_stop(cmd);
  esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
  i2c_cmd_link_delete(cmd);

  if (ret != ESP_OK)
    return ret;

  timeinfo->tm_sec = bcd_to_dec(data[0]);
  timeinfo->tm_min = bcd_to_dec(data[1]);
  timeinfo->tm_hour = bcd_to_dec(data[2]);
  timeinfo->tm_mday = bcd_to_dec(data[4]);
  timeinfo->tm_mon = bcd_to_dec(data[5]) - 1;
  timeinfo->tm_year = bcd_to_dec(data[6]) + 100;

  return ESP_OK;
}
