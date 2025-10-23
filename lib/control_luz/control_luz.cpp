#include "control_luz.h"

bool ControlLuz::getEstadoLuz()
{
    return this->estadoLuz;
}

void ControlLuz::encenderLuz()
{
    this->estadoLuz = true;
    this->callbackActivaLuz();
}

void ControlLuz::apagarLuz()
{
    this->estadoLuz = false;
    this->callbackDesactivaLuz();
}

ControlLuz::ControlLuz(void (*callbackActivaLuz)(), void (*callbackDesactivaLuz)()) : callbackActivaLuz(callbackActivaLuz), callbackDesactivaLuz(callbackDesactivaLuz)
{
    while (!callbackActivaLuz || !callbackDesactivaLuz)
        continue;

    this->apagarLuz();
}
