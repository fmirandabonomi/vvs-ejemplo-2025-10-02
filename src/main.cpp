#include "control_luz.hpp"
#include <Arduino.h>

#define PERIODO_PARPADEO_MS 200
#define PIN_LED 9

static ControlLuz *miControl;

static void enciendeLed()
{
    digitalWrite(PIN_LED, HIGH);
}
static void apagaLed()
{
    digitalWrite(PIN_LED, LOW);
}

void setup()
{
    miControl = new ControlLuz(enciendeLed, apagaLed);
    while (!(bool)miControl)
    { // Lazo infinito en caso de error de memoria.
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
    miControl->encenderLuz();
    delay(PERIODO_PARPADEO_MS / 2);
}
