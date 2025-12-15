#include "http_server.h"
#include "esp_spiffs.h"
#include <stdio.h>
#include <string.h>

#include "esp_http_server.h"
#include "esp_log.h"

static const char TAG[] = "http_server";

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

extern const uint8_t app_css_start[] asm("_binary_app_css_start");
extern const uint8_t app_css_end[] asm("_binary_app_css_end");

extern const uint8_t app_js_start[] asm("_binary_app_js_start");
extern const uint8_t app_js_end[] asm("_binary_app_js_end");

static httpd_handle_t server = NULL;

static esp_err_t root_get_handler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/html");
  httpd_resp_send(
      req,
      (const char *)index_html_start,
      index_html_end - index_html_start);
  return ESP_OK;
}

static esp_err_t app_css_get_handler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/css");
  httpd_resp_send(
      req,
      (const char *)app_css_start,
      app_css_end - app_css_start);
  return ESP_OK;
}

static esp_err_t app_js_get_handler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "application/javascript");
  httpd_resp_send(
      req,
      (const char *)app_js_start,
      app_js_end - app_js_start);
  return ESP_OK;
}

static esp_err_t status_get_handler(httpd_req_t *req)
{
  const char *json =
      "{"
      "\"status\": \"ok\","
      "\"device\": \"esp32\""
      "}";

  httpd_resp_set_type(req, "application/json");
  httpd_resp_send(req, json, HTTPD_RESP_USE_STRLEN);

  return ESP_OK;
}

static esp_err_t registros_get_handler(httpd_req_t *req)
{
  FILE *f = fopen("/spiffs/registros.csv", "r");
  if (!f)
  {
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Arquivo não encontrado");
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "application/json");
  httpd_resp_sendstr_chunk(req, "[");

  char line[128];
  bool first = true;

  fgets(line, sizeof(line), f);

  while (fgets(line, sizeof(line), f))
  {
    char *timestamp = strtok(line, ",");
    char *peso = strtok(NULL, ",");
    char *status = strtok(NULL, "\n");

    if (!timestamp || !peso || !status)
      continue;

    if (!first)
      httpd_resp_sendstr_chunk(req, ",");

    first = false;

    char json[160];
    snprintf(json, sizeof(json),
             "{\"timestamp\":\"%s\",\"peso\":%.2f,\"status\":\"%s\"}",
             timestamp,
             atof(peso),
             status);

    httpd_resp_sendstr_chunk(req, json);
  }

  fclose(f);
  httpd_resp_sendstr_chunk(req, "]");
  httpd_resp_sendstr_chunk(req, NULL);

  return ESP_OK;
}

static void http_server_register_uri_handlers(httpd_handle_t server)
{
  httpd_uri_t root = {
      .uri = "/",
      .method = HTTP_GET,
      .handler = root_get_handler,
  };

  httpd_uri_t css = {
      .uri = "/app.css",
      .method = HTTP_GET,
      .handler = app_css_get_handler,
  };

  httpd_uri_t js = {
      .uri = "/app.js",
      .method = HTTP_GET,
      .handler = app_js_get_handler,
  };

  httpd_uri_t status = {
      .uri = "/status",
      .method = HTTP_GET,
      .handler = status_get_handler,
  };

  httpd_uri_t registros = {
      .uri = "/registros",
      .method = HTTP_GET,
      .handler = registros_get_handler,
  };

  httpd_register_uri_handler(server, &registros);
  httpd_register_uri_handler(server, &root);
  httpd_register_uri_handler(server, &css);
  httpd_register_uri_handler(server, &js);
  httpd_register_uri_handler(server, &status);
}

esp_err_t http_server_start(void)
{
  if (server != NULL)
  {
    ESP_LOGW(TAG, "Servidor HTTP já está rodando");
    return ESP_OK;
  }

  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.stack_size = 4096;
  config.server_port = 80;

  ESP_LOGI(TAG, "Iniciando servidor HTTP na porta %d", config.server_port);

  esp_err_t err = httpd_start(&server, &config);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Erro ao iniciar servidor HTTP (%s)", esp_err_to_name(err));
    return err;
  }

  http_server_register_uri_handlers(server);

  return ESP_OK;
}

esp_err_t http_server_stop(void)
{
  if (server == NULL)
  {
    ESP_LOGW(TAG, "Servidor HTTP não está rodando");
    return ESP_OK;
  }

  ESP_LOGI(TAG, "Parando servidor HTTP");

  esp_err_t err = httpd_stop(server);
  server = NULL;

  return err;
}
