#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_sleep.h"
#include "esp_system.h"

#define LED_PIN GPIO_NUM_2
#define WAKEUP_TIME_SEC 10

void app_main(void) {

    // Configuración del LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Verificar causa de despertar
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

    if (cause == ESP_SLEEP_WAKEUP_TIMER) {
        printf("Despertar por temporizador\n");
    } else {
        printf("Arranque normal\n");
    }

    // Estado activo
    printf("Sistema activo\n");
    gpio_set_level(LED_PIN, 1);

    vTaskDelay(pdMS_TO_TICKS(5000)); // 5 segundos activo

    // Preparar Deep Sleep
    printf("Entrando en Deep Sleep por %d segundos...\n", WAKEUP_TIME_SEC);
    gpio_set_level(LED_PIN, 0);

    esp_sleep_enable_timer_wakeup(WAKEUP_TIME_SEC * 1000000ULL);

    esp_deep_sleep_start();
}
