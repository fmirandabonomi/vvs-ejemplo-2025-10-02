#include "control_luz.h"
#include <Arduino.h>

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
    while (!miControl)
        continue;
}

void loop()
{
    // put your main code here, to run repeatedly:
    miControl->encenderLuz();
    delay(100);
}
