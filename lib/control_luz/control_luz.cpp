#include "control_luz.h"

bool ControlLuz::getEstadoLuz()
{
    return this->estadoLuz;
}

void ControlLuz::encenderLuz()
{
    this->estadoLuz = true;
}

void ControlLuz::apagarLuz()
{
    this->estadoLuz = false;
}
