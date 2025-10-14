#ifndef CONTROL_LUZ_H
#define CONTROL_LUZ_H

struct ControlLuz
{
    bool getEstadoLuz();
    void encenderLuz();
    void apagarLuz();

private:
    bool estadoLuz = false;
};
#endif
