#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_PORT      UART_NUM_0     // Cambiar a UART_NUM_0 en caso de que se quiera simular en wowki
#define UART_TX_PIN    GPIO_NUM_17
#define UART_RX_PIN    GPIO_NUM_16
#define BUF_SIZE       256

#define LED_PIN        GPIO_NUM_2

static int led_state = 0;
static int command_counter = 0;

void trim_newline(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

void uart_init(void) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, BUF_SIZE, BUF_SIZE, 0, NULL, 0);
}

void send_uart(const char *msg) {
    uart_write_bytes(UART_PORT, msg, strlen(msg));
}

// TAREA UART
void task_uart(void *pvParameters) {
    uint8_t data[BUF_SIZE];

    send_uart("Sistema integrado listo\r\n");

    while (1) {
        int len = uart_read_bytes(
            UART_PORT,
            data,
            BUF_SIZE - 1,
            50 / portTICK_PERIOD_MS
        );

        if (len > 0) {
            data[len] = '\0';
            char *cmd = strtok((char *)data, "\r\n");

            if (cmd != NULL) {
                trim_newline(cmd);
                command_counter++;

                if (strcmp(cmd, "status") == 0) {
                    send_uart("Estado: OK\r\n");
                }
                else if (strcmp(cmd, "led on") == 0) {
                    led_state = 1;
                    send_uart("LED encendido\r\n");
                }
                else if (strcmp(cmd, "led off") == 0) {
                    led_state = 0;
                    send_uart("LED apagado\r\n");
                }
                else if (strcmp(cmd, "info") == 0) {
                    char msg[100];
                    sprintf(msg,
                            "Comandos: %d | LED: %s\r\n",
                            command_counter,
                            led_state ? "ON" : "OFF");
                    send_uart(msg);
                }
                else {
                    send_uart("Comando no valido\r\n");
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// TAREA LED
void task_led(void *pvParameters) {
    while (1) {
        gpio_set_level(LED_PIN, led_state);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// TAREA MONITOR
void task_monitor(void *pvParameters) {
    while (1) {
        char msg[80];
        sprintf(msg,
                "[Monitor] LED=%s | Cmd=%d\r\n",
                led_state ? "ON" : "OFF",
                command_counter);
        send_uart(msg);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void app_main(void) {

    uart_init();

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    xTaskCreate(task_uart, "UART Task", 4096, NULL, 3, NULL);
    xTaskCreate(task_led, "LED Task", 2048, NULL, 1, NULL);
    xTaskCreate(task_monitor, "Monitor Task", 2048, NULL, 2, NULL);
}
