#include "control_luz.hpp"

bool ControlLuz::getEstadoLuz() const
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

ControlLuz::ControlLuz(void (*callbackActivaLuz)(), void (*callbackDesactivaLuz)())
    // cppcheck-suppress misra-c2012-12.3 ; [C++] Aquí la coma es el separador de la lista de inicialización
    : callbackActivaLuz(callbackActivaLuz), callbackDesactivaLuz(callbackDesactivaLuz)
{
    while (!(bool)callbackActivaLuz || !(bool)callbackDesactivaLuz)
    {
    }

    this->apagarLuz();
}
