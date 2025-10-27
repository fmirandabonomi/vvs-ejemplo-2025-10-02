#include "control_luz.hpp"
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

// cppcheck-suppress unusedFunction ; Es llamada desde el main de arduino
void setup()
{
    miControl = new ControlLuz(enciendeLed, apagaLed);
    while (!miControl)
        ;
}

// cppcheck-suppress unusedFunction ; Es llamada desde el main de arduino
void loop()
{
    // put your main code here, to run repeatedly:
    miControl->encenderLuz();
    delay(100);
}
