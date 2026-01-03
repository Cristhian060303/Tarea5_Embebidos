#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#define LED_PIN GPIO_NUM_2

static int sensor_value = 0;

// TAREA 1: Sensor virtual
void task_sensor(void *pvParameters) {
    while (1) {
        sensor_value++;
        printf("Sensor leído: %d\n", sensor_value);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 segundo
    }
}

// TAREA 2: LED intermitente
void task_led(void *pvParameters) {
    int led_state = 0;

    while (1) {
        led_state = !led_state;
        gpio_set_level(LED_PIN, led_state);
        printf("LED: %s\n", led_state ? "ON" : "OFF");
        vTaskDelay(pdMS_TO_TICKS(500)); // 500 ms
    }
}

// TAREA 3: Monitor serial
void task_monitor(void *pvParameters) {
    while (1) {
        printf("Estado del sistema | Sensor=%d\n", sensor_value);
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 segundos
    }
}


void app_main(void) {

    // Configuración del LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    // Creación de tareas
    xTaskCreate(
        task_sensor,
        "Tarea Sensor",
        2048,
        NULL,
        2,      // Prioridad media
        NULL
    );

    xTaskCreate(
        task_led,
        "Tarea LED",
        2048,
        NULL,
        1,      // Prioridad baja
        NULL
    );

    xTaskCreate(
        task_monitor,
        "Tarea Monitor",
        2048,
        NULL,
        3,      // Prioridad alta
        NULL
    );
}
