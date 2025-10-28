#include "control_luz.hpp"
#include <Arduino.h>
#include <assert.h>

#define PERIODO_PARPADEO_MS 200

static ControlLuz *miControl;

static void enciendeLed()
{
    digitalWrite(9, HIGH);
}
static void apagaLed()
{
    digitalWrite(9, LOW);
}

void setup()
{
    miControl = new ControlLuz(enciendeLed, apagaLed);
    assert(miControl);
}

void loop()
{
    // put your main code here, to run repeatedly:
    miControl->encenderLuz();
    delay(PERIODO_PARPADEO_MS / 2);
}
