#include "control_direccion.hpp"

ControlDireccion::ControlDireccion(void (*callbackCambiaDireccion)(int))
    // cppcheck-suppress misra-c2012-12.3 ; Inicializa el miembro constante
    : callbackCambiaDireccion(callbackCambiaDireccion), direccion(0)
{
    this->callbackCambiaDireccion(this->direccion);
}

int ControlDireccion::leeDireccion() const
{
    return this->direccion;
}

void ControlDireccion::estableceDireccion(int nuevaDireccion)
{
    this->direccion = nuevaDireccion;
}
