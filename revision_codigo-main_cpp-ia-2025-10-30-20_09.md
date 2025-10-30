Voy a crear una lista de tareas para organizar el análisis y las mejoras que propondré sobre main.cpp, luego hago el análisis y propongo cambios concretos (sin editar archivos aún). Resultado esperado: lista de problemas, mejoras organizativas y cambios de código recomendados.

Created 5 todos

Progreso: ya creé la lista de tareas y empecé el análisis (primer ítem en progreso). Abajo tienes el diagnóstico detallado y recomendaciones concretas para main.cpp.

## Resumen corto

Identifiqué varias categorías de problemas y oportunidades:

- errores o malas llamadas a la API (p. ej. `servidorWeb.begin(PUERTO_WEB)`).
- diseño con demasiadas variables y funciones globales (difícil de testear y mantener).
- problemas de memoria y fiabilidad por uso extensivo de `String`.
- falta de validación/gestión de errores (e.g., argumento `direccion`).
- mezcla de responsabilidades (WiFi, MDNS, servidor web, HTML, lógica de control) en un único fichero.

A continuación detallo los problemas observados, impacto y recomendaciones (incluyendo snippets y un plan de reorganización).

## Problemas concretos y su impacto

1. Llamada incorrecta a `servidorWeb.begin(PUERTO_WEB)`
   - Evidencia: `static WebServer servidorWeb(PUERTO_WEB);` y luego `servidorWeb.begin(PUERTO_WEB);`
   - Impacto: La API de `WebServer` espera `begin()` sin parámetros (puede compilar o producir error dependiendo de la versión). Es redundante y potencialmente una llamada inválida.
   - Recomendación: usar `servidorWeb.begin();` (sin parámetro) o solo crear `servidorWeb(PUERTO_WEB)` y luego `servidorWeb.begin();`.

2. Uso intensivo de `String` y literales "static const String"
   - Evidencia: muchas variables HTML definidas como `static const String paginaEncabezado = "...";`
   - Impacto: en entornos Arduino/ESP32 el uso de `String` dinámico puede fragmentar heap y causar fallos de memoria a largo plazo.
   - Recomendación: utilizar `const char[]` en PROGMEM o la macro `F()` para literales, o construir la respuesta con `String` local mínimo y preferir buffers estáticos. Para HTML grande, almacenar en PROGMEM (flash) con `const char PAGE[] PROGMEM = R"rawliteral(...)"` o usar `server.send_P()`.

3. Falta de validación del parámetro `direccion`
   - Evidencia: se lee `String direccion = servidorWeb.arg("direccion");` y solo se comprueba `if (direccion != "")` y luego se LOGgea.
   - Impacto: entrada no validada -> valores fuera de rango, strings no numéricos, inyección/errores.
   - Recomendación: parsear con `toInt()` y verificar límites (min/max). Responder con error o ignorar si inválido.

4. Mezcla de responsabilidades en `main.cpp`
   - Evidencia: WiFi (connect + eventos), MDNS, servidor web, HTML, lógica de dirección y control de LED todo en un archivo.
   - Impacto: código difícil de mantener, testar y reutilizar.
   - Recomendación: separar en módulos:
     - `network.cpp/h`: inicialización de WiFi, eventos, MDNS.
     - `webserver.cpp/h`: rutas y handlers, validación de parámetros.
     - `ui_pages.h` o `web_pages.h`: constantes HTML en PROGMEM.
     - `main.cpp`: composición/arranque sólo (instanciar y conectar módulos).
     - `control_luz` ya existe: mantener la lógica hardware ahí y exponer una interfaz clara.

5. Variables globales y funciones `static` de alcance de fichero
   - Evidencia: `static ControlLuz miControl(enciendeLed, apagaLed);` y handlers `static void ...`
   - Impacto: dificulta testeo e inyección de dependencias; reduce claridad sobre el ownership.
   - Recomendación: mover la instancia a `main()` o permitir inyección (crear `ControlLuz` en main y pasar referencias a los módulos que lo necesiten). Preferir lambdas o std::function si la API lo soporta.

6. Manejo de MDNS al perder IP
   - Evidencia: en `ARDUINO_EVENT_WIFI_STA_LOST_IP` se hace `MDNS.end();`
   - Impacto: correcto conceptualmente, pero no se comprueba si MDNS estaba iniciado. Recomendación: llevar una bandera `mdnsActive` o usar el retorno de `MDNS.begin()` y sólo `MDNS.end()` si fue iniciado. También re-intentar MDNS si vuelve a obtener IP.

7. Macro LOG con `Serial.printf` sin protección
   - Evidencia: `#define LOG(...) Serial.printf(__VA_ARGS__)`
   - Impacto: si Serial no está inicializado o en contextos de interrupción, puede ser un problema. Además no hay niveles de log.
   - Recomendación: mantener, pero envolver con chequeo `if (Serial)` o crear función `logf(...)` que no llame si `!Serial`.

8. HTML con acentos y entidades hard-coded
   - Evidencia: strings contienen `&aacute;` etc. OK, pero mejor separar plantilla y dinámicos.
   - Recomendación: usar plantillas pequeñas y formatear solo las partes dinámicas (estado, dispositivo, dirección). Usar `String` mínimo y `reserve()` si se va a usar.

9. Seguridad: credenciales WiFi en claro
   - Evidencia: `const char *ssid = "UNT"; const char *password = "wifi.unt";`
   - Impacto: en repo o dispositivo expuesto, las credenciales quedan fijas. Recomendación: mover a configuración fuera del repositorio (config.h), usar build flags o un sistema de provisioning (WiFiManager).

10. Posibilidad de bloqueo por `delay(ESPERA_LAZO_MS)`
    - Evidencia: `delay(10)` en loop.
    - Impacto: delay corto es aceptable; sin embargo, el resto del código debería ser no bloqueante. Si se amplía el proyecto, preferir `millis()` para temporizadores y evitar delays largos.
    - Recomendación: para 10 ms está bien, documentar y preferir `yield()` o `vTaskDelay()` en contextos FreeRTOS si necesario.

## Pequeños errores/consistencia y limpieza

- Usar `constexpr` o `const` en lugar de `#define` para constantes de C++ (p. ej. `ESPERA_LAZO_MS`, `HTTP_OK`) para mejorar el tipeado.
- `#include <Arduino.h>` arriba; OK. Ordenar includes.
- `servidorWeb.onNotFound(atiendeNotFound);` debería enviar página HTML de not found en vez de plain text si se desea coherencia.
- `String direccion` se convierte luego a `LOG` pero no se aplica a `miControl` (probablemente porque `ControlLuz` aún no soporta dirección). Si la intención es modificar dirección, hay que exponer método `miControl.setDireccion(int)` y validar límites.

## Recomendaciones de reorganización (arquitectura mínima)

Objetivo: hacer `main.cpp` pequeño y mantener responsabilidades separadas.

Propuesta de ficheros:

- main.cpp:
  - Inicializa Serial
  - Crea `ControlLuz` y pasa callbacks
  - Crea `NetworkManager` y `WebManager` instancias
  - Llama a `network.begin()` y `web.begin()`
  - Loop: `web.handle()` o `network.loop()` (mínimo)

- `lib/network/NetworkManager.hpp/cpp`:
  - Clase `NetworkManager` responsable de conectar a WiFi, registrar eventos y gestionar MDNS.
  - Interfaz: `bool begin(const char* ssid, const char* pass)`; `void loop()`.
  - Señales/callbacks para estado (onConnected, onGotIP, onDisconnected).

- `lib/web/WebManager.hpp/cpp`:
  - Clase `WebManager` encapsula `WebServer`.
  - Interfaz: `begin()`, `handleClient()`, `registerRoutes(ControlLuz&)` o aceptar lambdas para handlers.
  - Validación de parámetros, respuestas (200/302/404).
  - Plantillas HTML en PROGMEM (o `web_pages.h`).

- control_luz (ya existe):
  - Asegurarse de API pública: `encenderLuz()`, `apagarLuz()`, `getEstadoLuz()`, `setDireccion(int)`, `getDireccion()`.

Contrato mínimo entre módulos (2-4 bullets)

- Input: `WebManager` recibe referencias a `ControlLuz` para ejecutar acciones en respuestas HTTP.
- Output: `NetworkManager` notifica al `WebManager` cuando hay IP para activar MDNS.
- Errores: los métodos devuelven bool/enum y exponen callbacks para fallo de inicio.

## Cambios de código concretos y snippets recomendados

1) Corregir begin del servidor:
Reemplazar

```cpp
servidorWeb.begin(PUERTO_WEB);
```

por

```cpp
servidorWeb.begin();
```

2) Validación y parseo de `direccion`:
Sugerencia para `atiendeComando()`:

```cpp
static void atiendeComando()
{
    String luz = servidorWeb.arg("luz");
    String direccionStr = servidorWeb.arg("direccion");

    if (luz == "encender") {
        miControl.encenderLuz();
    } else if (luz == "apagar") {
        miControl.apagarLuz();
    }

    if (direccionStr.length() > 0) {
        long dir = direccionStr.toInt(); // toInt devuelve 0 si no es numérico
        // Mejor hacer una validación más robusta:
        bool isNumber = true;
        for (size_t i = 0; i < direccionStr.length(); ++i) {
            char c = direccionStr[i];
            if (i==0 && c=='-') continue;
            if (c < '0' || c > '9') { isNumber = false; break; }
        }
        if (isNumber) {
            int direccion = (int)dir;
            // aplicar límites, por ejemplo -90..90
            if (direccion < -90) direccion = -90;
            if (direccion > 90) direccion = 90;
            miControl.setDireccion(direccion); // asegurar que existe este método
            LOG("Direccion %d\n", direccion);
        } else {
            LOG("Parametro direccion inválido: %s\n", direccionStr.c_str());
            // opcional: devolver 400 Bad Request
        }
    }

    redirigeRaiz();
}
```

3) Evitar Strings de gran tamaño en RAM — usar PROGMEM:
Ejemplo de página simple en PROGMEM:

```cpp
#include <pgmspace.h>
const char paginaEncabezado[] PROGMEM = R"rawliteral(
<html><head><title>Control de Luz</title></head><body><h1>Control de Luz</h1>
)rawliteral";
```

y luego usar `server.send_P()` o copiar desde `pgm_read_byte`.

4) Reemplazar `#define` por `constexpr`:

```cpp
constexpr int HTTP_OK = 200;
constexpr int HTTP_REDIRECT = 302;
constexpr int HTTP_NOT_FOUND = 404;
constexpr unsigned long ESPERA_LAZO_MS = 10;
constexpr int PUERTO_WEB = 80;
```

5) Menos globals, usar objetos e inyección:

- Crear `ControlLuz miControl(...)` dentro de `setup()` o `main` y pasar por referencia a `WebManager::registerRoutes(&miControl)`.

6) Manejo de MDNS con bandera:

```cpp
static bool mdnsActive = false;

...

if (MDNS.begin(dispositivo)) {
    mdnsActive = true;
    LOG("MDNS iniciado con exito\n");
} else {
    mdnsActive = false;
    LOG("Error al iniciar MDNS\n");
}

...

case ARDUINO_EVENT_WIFI_STA_LOST_IP:
    LOG("Se perdió la IP\n");
    if (mdnsActive) {
        MDNS.end();
        mdnsActive = false;
        LOG("MDNS finalizado\n");
    }
    break;
```

## Casos bordes y pruebas sugeridas

- WiFi no disponible / credenciales incorrectas -> verificar reintentos y estados.
- Envío de `direccion` vacía / texto no-numérico / fuera de rango.
- Revisión de uso de heap antes/después de servir muchas peticiones (stress test).
- Comprobación de MDNS después de reconexión: MDNS se debe reiniciar si fue finalizado.
- Validar que logueo no bloquea y que `handleClient()` se llama con suficiente frecuencia.

Pruebas a ejecutar (manuales/hardware):

- Conectar a la red, abrir `/`, presionar enlaces de encender/apagar, modificar dirección con valores válidos e inválidos.
- Forzar desconexión de WiFi y ver logs de evento.
- Enviar muchas peticiones HTTP para detectar fugas de memoria.

## Mejoras opcionales (prioritarias)

- Usar `ESPAsyncWebServer` (mayor rendimiento y no bloqueante).
- Añadir HTTPS/Autenticación básica para proteger endpoints.
- Mecanismo de configuración que no incluya credenciales en el repo (WiFiManager).
- Soporte OTA (e.g., `ArduinoOTA`) para actualizaciones remotas.
- Registro de telemetría (estado de la lámpara y última dirección) en NVS/EEPROM.

## Pequeñas refactorizaciones inmediatas (rápidas, bajo riesgo)

1. Cambiar `servidorWeb.begin(PUERTO_WEB)` por `servidorWeb.begin();`
2. Convertir `#define` a `constexpr`.
3. Validar y parsear `direccion` antes de usarla.
4. Reemplazar literales HTML grandes por PROGMEM o `F()`.

Si quieres, puedo:

- aplicar automáticamente los cambios de bajo riesgo (1–3) y ejecutar una compilación rápida (PlatformIO) para comprobar que el proyecto compila; o
- crear la estructura modular propuesta (archivos `NetworkManager`, `WebManager`) y mover la lógica ahí con tests mínimos.

¿Qué prefieres que haga ahora? (Puedo empezar por las correcciones rápidas y ejecutar una compilación en la workspace.)
