# Casos de uso para el sistema

## UC1 Observar localmente el estado de la luz

Actores : Usuario

Descripción: El usuario observa el estado en que se encuentra la luz conectada al módulo ESP32 utilizando el panel de control local.

Precondiciones:

- 1p1 El módulo ESP32 se encuentra alimentado y posee conectividad WiFi
- 1p2 La computadora del usuario está conectada a la misma red WiFi que el módulo

Flujo principal:

- Usuario abre en el navegador IP/dashboard.html
- Sistema genera dinámicamente una panel de control incluyendo una indicación del estado de la luz
- Usuario observa la página abierta en el navegador y haya en ella el estado del led
