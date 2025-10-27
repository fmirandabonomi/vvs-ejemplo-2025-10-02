#include "control_luz.hpp"

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
