#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_PORT      UART_NUM_2     // Se cambia a UART_NUM_0 para poder realizar la simulacion
#define UART_TX_PIN    GPIO_NUM_17
#define UART_RX_PIN    GPIO_NUM_16
#define BUF_SIZE       256

#define LED_PIN        GPIO_NUM_2

static int command_counter = 0;
static int led_state = 0;

void trim_newline(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

void uart2_init(void) {
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

void send_response(const char *msg) {
    uart_write_bytes(UART_PORT, msg, strlen(msg));
}

void process_command(char *cmd) {
    command_counter++;

    if (strcmp(cmd, "status") == 0) {
        send_response("Estado: Sistema operativo\r\n");
    }
    else if (strcmp(cmd, "led on") == 0) {
        gpio_set_level(LED_PIN, 1);
        led_state = 1;
        send_response("LED encendido\r\n");
    }
    else if (strcmp(cmd, "led off") == 0) {
        gpio_set_level(LED_PIN, 0);
        led_state = 0;
        send_response("LED apagado\r\n");
    }
    else if (strcmp(cmd, "info") == 0) {
        char info[100];
        sprintf(info,
                "UART2 | Baud: 115200 | Comandos: %d | LED: %s\r\n",
                command_counter,
                led_state ? "ON" : "OFF");
        send_response(info);
    }
    else if (strcmp(cmd, "reset") == 0) {
        command_counter = 0;
        send_response("Variables reiniciadas\r\n");
    }
    else {
        send_response("Comando no reconocido\r\n");
    }
}

void app_main(void) {
    uint8_t data[BUF_SIZE];

    uart2_init();

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    send_response("Sistema listo. Esperando comandos...\r\n");

    while (1) {
        int len = uart_read_bytes(
            UART_PORT,
            data,
            BUF_SIZE - 1,
            20 / portTICK_PERIOD_MS
        );

        if (len > 0) {
            data[len] = '\0';

            char *cmd = strtok((char *)data, "\r\n");
            if (cmd != NULL) {
                trim_newline(cmd);
                process_command(cmd);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
