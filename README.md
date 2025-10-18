# Sistema de Verificação e Classificação de Peso em Tempo Real

Este projeto implementa um sistema embarcado modular baseado em **ESP32** e **ESP-IDF**, capaz de detectar a presença de objetos, medir seu peso e classificá-los conforme limites pré-definidos, fornecendo feedback visual e sonoro ao operador.

O foco principal é demonstrar a integração de sensores, atuadores e multitarefa com FreeRTOS, formando um protótipo rastreável e escalável.

---

## Funcionalidades Principais

- Detecção de presença via sensores infravermelhos (entrada e saída)
- Leitura precisa de peso utilizando célula de carga + módulo HX711
- Registro temporal de medições com módulo RTC (DS3231)
- Classificação automática do item (aprovado / reprovado)
- Feedback visual e sonoro com LED RGB e buzzer
- Execução multitarefa com FreeRTOS (tarefas independentes e sincronizadas via filas e semáforos)

---

## Arquitetura do Sistema

```
┌──────────────────────┐
│      Sensores IR     │
│ (Entrada / Saída)    │
└─────────┬────────────┘
          │
          ▼
┌──────────────────────┐
│   TaskDeteccao       │
│ Detecta presença e   │
│ aciona leitura       │
└─────────┬────────────┘
          │ Queue
          ▼
┌──────────────────────┐
│   TaskLeituraPeso    │
│ Lê HX711 e converte │
│ valor em gramas      │
└─────────┬────────────┘
          │ Queue
          ▼
┌──────────────────────┐
│   TaskClassificacao  │
│ Compara peso e       │
│ determina status      │
└─────────┬────────────┘
          │ Queue
          ▼
┌──────────────────────┐
│   TaskRegistro       │
│ Usa RTC (DS3231) p/ │
│ timestamp e log      │
└─────────┬────────────┘
          │ Queue
          ▼
┌──────────────────────┐
│   TaskFeedback       │
│ Controla LED e buzzer│
└──────────────────────┘
```

---

## Estrutura de Pastas

```
SISTEMA-PESO-CLASSIFICACAO/
├── CMakeLists.txt
├── sdkconfig.defaults
├── README.md
├── .gitignore
│
├── main/
│   ├── CMakeLists.txt
│   ├── app_main.c
│   ├── config.h
│   │
│   ├── tasks/
│   │   ├── task_deteccao.c
│   │   ├── task_leitura_peso.c
│   │   ├── task_classificacao.c
│   │   ├── task_registro.c
│   │   ├── task_feedback.c
│   │   └── tasks.h
│   │
│   ├── drivers/
│   │   ├── hx711.c / hx711.h
│   │   ├── rtc_ds3231.c / rtc_ds3231.h
│   │   ├── ir_sensor.c / ir_sensor.h
│   │   └── led_buzzer.c / led_buzzer.h
│   │
│   └── utils/
│       ├── logger.c / logger.h
│
├── components/
│   └── hx711/ (opcional como componente separado)
│
└── docs/
    ├── diagrama_blocos.png
    ├── tabela_pinos.xlsx
    └── arquitetura.mmd
```

---

## Tecnologias Utilizadas

| Categoria        | Ferramenta                                |
| ---------------- | ----------------------------------------- |
| Microcontrolador | ESP32                                     |
| Framework        | ESP-IDF                                   |
| RTOS             | FreeRTOS                                  |
| Sensores         | HX711 (peso), IR (presença), DS3231 (RTC) |
| Atuadores        | LED RGB, buzzer                           |
| Linguagem        | C (padrão ESP-IDF)                        |
| IDE / Editor     | VSCode com ESP-IDF Extension              |

---

## Principais Bibliotecas

| Biblioteca            | Função                                          |
| --------------------- | ----------------------------------------------- |
| `driver/gpio.h`       | Controle digital de sensores e atuadores        |
| `driver/i2c.h`        | Comunicação I²C com RTC                         |
| `driver/ledc.h`       | Geração de PWM para LED e buzzer                |
| `esp_timer.h`         | Temporização precisa em microssegundos          |
| `freertos/FreeRTOS.h` | Gerenciamento de multitarefa                    |
| `hx711.h`             | Leitura da célula de carga (biblioteca externa) |

---

## Compilação e Upload

### 1. Configuração do ambiente

Certifique-se de ter o ESP-IDF instalado e configurado:

```bash
idf.py --version
```

### 2. Compilação do projeto

```bash
idf.py build
```

### 3. Gravação no ESP32

```bash
idf.py -p /dev/ttyUSB0 flash
```

### 4. Monitoramento serial

```bash
idf.py monitor
```

Para sair do monitor, pressione `Ctrl + ]`.

---

## Ajustes de Configuração

As definições de pinos e limites de peso estão no arquivo:

```
main/config.h
```

Exemplo:

```c
#define HX711_DOUT  4
#define HX711_SCK   5
#define GPIO_IR_ENTRADA 25
#define GPIO_IR_SAIDA   26
#define PESO_MINIMO 50.0f
#define PESO_MAXIMO 200.0f
```

---

## Licença

Este projeto é distribuído sob a licença **MIT**.
Sinta-se à vontade para modificar e expandir conforme suas necessidades.

---

## Autores

**Leandro Müller**
**Camila Raquel**

Desenvolvimento conjunto do sistema embarcado com ESP32 e FreeRTOS.
