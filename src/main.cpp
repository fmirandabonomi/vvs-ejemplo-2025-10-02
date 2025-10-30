#include "control_luz.hpp"
#include <Arduino.h>
//
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

#define PIN_LED LED_BUILTIN

#define HTTP_OK 200
#define HTTP_REDIRECT 302
#define HTTP_NOT_FOUND 404
#define ESPERA_LAZO_MS 10
#define PUERTO_WEB 80

#define LOG(...) Serial.printf(__VA_ARGS__)

const char *ssid = "UNT";
const char *password = "wifi.unt";

const char *dispositivo = "spot_fer";

static void enciendeLed();
static void apagaLed();
static ControlLuz miControl(enciendeLed, apagaLed);

static void eventoWiFi(WiFiEvent_t evento);

static WebServer servidorWeb(PUERTO_WEB);

static void atiendeRaiz()
{
    static const String paginaEncabezado = "<html><head><title>Control de Luz</title></head><body><h1>Control de Luz</h1>";
    static const String id_pre = "<p>Dispositivo: ";
    static const String id_post = "</p><hr>";
    static const String paginaEstadoOn = "<p>La luz est&aacute; <strong>ENCENDIDA</strong>. <a href=\"comando?luz=apagar\">Apagar</a></p>";
    static const String paginaEstadoOff = "<p>La luz est&aacute; <strong>APAGADA</strong>. <a href=\"comando?luz=encender\">Encender</a><br></p>";
    static const String paginaPie = "<hr></body></html>";
    String mensaje = paginaEncabezado + id_pre + dispositivo + id_post + (miControl.getEstadoLuz() ? paginaEstadoOn : paginaEstadoOff) + paginaPie;
    servidorWeb.send(HTTP_OK, "text/html", mensaje);
}

static void redirigeRaiz()
{
    servidorWeb.sendHeader("Location", "/");
    servidorWeb.send(HTTP_REDIRECT, "text/plain", "");
}

static void atiendeComando()
{

    String luz = servidorWeb.arg("luz");
    if (luz == "encender")
    {
        miControl.encenderLuz();
    }
    else if (luz == "apagar")
    {
        miControl.apagarLuz();
    }
    redirigeRaiz();
}

static void atiendeNotFound()
{
    servidorWeb.send(HTTP_NOT_FOUND, "text/plain", "Recurso no encontrado");
}

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(eventoWiFi);
    WiFi.begin(ssid, password);
    servidorWeb.on("/", atiendeRaiz);
    servidorWeb.on("/comando", atiendeComando);
    servidorWeb.onNotFound(atiendeNotFound);
    servidorWeb.begin(PUERTO_WEB);
    pinMode(PIN_LED, OUTPUT);
}

void loop()
{
    servidorWeb.handleClient();
    delay(ESPERA_LAZO_MS);
}

static void enciendeLed()
{
    digitalWrite(PIN_LED, HIGH);
}
static void apagaLed()
{
    digitalWrite(PIN_LED, LOW);
}

static void eventoWiFi(WiFiEvent_t evento)
{
    switch (evento)
    {
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        LOG("Conectado a la red WiFi\n");
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        LOG("Obtenida IP: %s\n", WiFi.localIP().toString().c_str());
        if (MDNS.begin(dispositivo)) // NOLINT(bugprone-branch-clone) ; Los efectos de las ramas son distintos
        {
            LOG("MDNS iniciado con exito\n");
        }
        else
        {
            LOG("Error al iniciar MDNS\n");
        }
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        LOG("Desconectado de la red WiFi\n");
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        LOG("Se perdió la IP\n");
        MDNS.end();
        LOG("MDNS finalizado\n");
        break;
    default:
        break;
    }
}
