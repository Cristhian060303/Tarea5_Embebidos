# Tarea #5 – Sistemas Embebidos  
## ESP32 – UART, FreeRTOS y Gestión de Energía

Este repositorio contiene el desarrollo completo de la **Tarea #5 de la asignatura Sistemas Embebidos**, cuyo objetivo es implementar aplicaciones avanzadas sobre el microcontrolador **ESP32**, integrando comunicación serial UART, ejecución concurrente de tareas mediante **FreeRTOS** y técnicas de **ahorro de energía**, utilizando el entorno de desarrollo **PlatformIO en Visual Studio Code**.

---

## Estructura del repositorio

Cada ejercicio se desarrolló como un proyecto independiente:

.
├── Ejercicio1_UART2  
│   ├── platformio.ini  
│   └── src/main.c  
├── Ejercicio2_FreeRTOS  
│   ├── platformio.ini  
│   └── src/main.c  
├── Ejercicio3_Energia  
│   ├── platformio.ini  
│   └── src/main.c  
└── Ejercicio4_UART_FreeRTOS  
    ├── platformio.ini  
    └── src/main.c  

---

## Requisitos del entorno

- Visual Studio Code  
- PlatformIO IDE  
- ESP32 Dev Module  
- Drivers USB del ESP32 (CP210x / CH340)  
- Extensión Wokwi (opcional, para simulación)  
- Hardware real ESP32 (obligatorio para Ejercicio 3)

---

## Ejercicio 1 – Comunicación Serial Avanzada con UART2

Descripción:  
Se implementa un sistema de comandos seriales utilizando exclusivamente el puerto **UART2** del ESP32. El sistema recibe comandos de texto desde un terminal serial, los interpreta y ejecuta acciones internas, enviando respuestas estructuradas por el mismo puerto UART.

Funcionamiento:  
El ESP32 inicializa el puerto UART2 con baud rate configurable y realiza lecturas no bloqueantes. Cada comando recibido es limpiado de caracteres de fin de línea y comparado con un conjunto de comandos definidos, ejecutando la acción correspondiente.

Comandos soportados:  
- status  
- led on  
- led off  
- info  
- reset  

Simulación:  
Ejercicio simulado en Wokwi.

---

## Ejercicio 2 – Sistema Multitarea con FreeRTOS

Descripción:  
Se diseña un sistema multitarea utilizando **FreeRTOS**, implementando tres tareas concurrentes con diferentes prioridades y periodos de ejecución.

Funcionamiento:  
El sistema se divide en:
- Una tarea de lectura de un sensor virtual  
- Una tarea de control de un LED  
- Una tarea de monitoreo por puerto serial  

Cada tarea se ejecuta de manera concurrente utilizando `xTaskCreate()` y `vTaskDelay()`, demostrando planificación de tareas y evitando bloqueos del sistema.

Simulación:  
Ejercicio simulado en Wokwi.

---

## Ejercicio 3 – Ahorro de Energía en el ESP32

Descripción:  
Se implementa un sistema de ahorro energético utilizando el modo **Deep Sleep** del ESP32, evaluado exclusivamente en **hardware real**, tal como lo exige la consigna.

Funcionamiento:  
El sistema entra en un estado activo durante un tiempo determinado, enciende un LED y luego entra en modo Deep Sleep. El microcontrolador despierta automáticamente mediante un temporizador RTC y repite el ciclo.

Simulación:  
No aplica.  
Evidencia:  
Pruebas realizadas en hardware real.

---

## Ejercicio 4 – Sistema Integrado UART + FreeRTOS

Descripción:  
Se integra la comunicación UART con la ejecución multitarea mediante FreeRTOS, desarrollando un sistema completo y modular.

Funcionamiento:  
El sistema cuenta con:
- Una tarea dedicada a la recepción e interpretación de comandos UART  
- Una tarea para el control del LED  
- Una tarea de monitoreo del sistema  

Las tareas se ejecutan concurrentemente y comparten variables globales para mantener el estado del sistema.

Simulación:  
Ejercicio simulado en Wokwi.

---

## Compilación del proyecto

Desde la carpeta del ejercicio correspondiente:

pio run

---

## Carga y ejecución en el ESP32

pio run -t upload

Monitor serial:

pio device monitor

---

## Evidencia

Las capturas de simulación (Ejercicios 1, 2 y 4) y la evidencia de pruebas en hardware real (Ejercicio 3) se incluyen en el documento PDF de entrega.

---

## Enlaces a los repositorios

- Ejercicio 1: https://github.com/Cristhian060303/Tarea5_Embebidos/tree/main/T5E1
- Ejercicio 2: https://github.com/Cristhian060303/Tarea5_Embebidos/tree/main/T5E2
- Ejercicio 3: https://github.com/Cristhian060303/Tarea5_Embebidos/tree/main/T5E3  
- Ejercicio 4: https://github.com/Cristhian060303/Tarea5_Embebidos/tree/main/T5E4

---

## Autor

Cristhian Bastidas

---

## Licencia

Proyecto desarrollado con fines académicos.


