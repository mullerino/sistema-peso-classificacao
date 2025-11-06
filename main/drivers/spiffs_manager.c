#include "spiffs_manager.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include <stdio.h>

static const char *TAG = "SPIFFS_MANAGER";

esp_err_t spiffs_manager_init(void)
{
  ESP_LOGI(TAG, "Montando SPIFFS...");

  esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true};

  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK)
  {
    ESP_LOGE(TAG, "Falha ao montar SPIFFS (%s)", esp_err_to_name(ret));
    return ret;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(NULL, &total, &used);

  if (ret == ESP_OK)
  {
    ESP_LOGI(TAG, "SPIFFS montado com sucesso: %.1f KB livres de %.1f KB",
             (float)(total - used) / 1024, (float)total / 1024);
  }
  else
  {
    ESP_LOGW(TAG, "Não foi possível obter informações da partição SPIFFS (%s)", esp_err_to_name(ret));
  }

  return ESP_OK;
}

void spiffs_manager_save_event(const char *timestamp, float peso, const char *status)
{
  FILE *f = fopen("/spiffs/registros.csv", "a");

  if (!f)
  {
    ESP_LOGE(TAG, "Erro ao abrir arquivo para escrita");
    return;
  }

  fprintf(f, "%s,%.2f,%s\n", timestamp, peso, status);
  fclose(f);

  ESP_LOGI(TAG, "Evento salvo no SPIFFS: [%s] %.2f g (%s)", timestamp, peso, status);
}

void spiffs_manager_read_file(const char *path)
{
  FILE *file = fopen(path, "r");

  if (file == NULL)
  {
    ESP_LOGE(TAG, "Falha ao abrir arquivo!");
    return;
  }
  else
  {
    ESP_LOGI(TAG, "Arquivo aberto com sucesso! Lendo conteúdo...");
  }

  char readed[128];
  while (fgets(readed, sizeof(readed), file) != NULL)
  {
    ESP_LOGI(TAG, "%s", readed);
  }
  fclose(file);
}
