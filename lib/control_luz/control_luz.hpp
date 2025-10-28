#ifndef CONTROL_LUZ_H
#define CONTROL_LUZ_H

struct ControlLuz
{
    bool getEstadoLuz();
    void encenderLuz();
    void apagarLuz();

    ControlLuz(void (*callbackActivaLuz)(), void (*callbackDesactivaLuz)());
    ControlLuz() = delete;

private:
    bool estadoLuz = false;
    void (*const callbackActivaLuz)();
    void (*const callbackDesactivaLuz)();
};
#endif
