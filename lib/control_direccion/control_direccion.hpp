#ifndef CONTROL_DIRECCION_HPP
#define CONTROL_DIRECCION_HPP

struct ControlDireccion
{
    int leeDireccion() const;
    void estableceDireccion(int nuevaDireccion);
    explicit ControlDireccion(void (*callbackCambiaDireccion)(int));
    ControlDireccion() = delete;

private:
    void (*const callbackCambiaDireccion)(int);
    int direccion;
};

#endif
