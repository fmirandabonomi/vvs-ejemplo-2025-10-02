#ifndef CONTROL_LUZ_H
#define CONTROL_LUZ_H

struct ControlLuz
{
    bool getEstadoLuz() const;
    void encenderLuz();
    void apagarLuz();

    ControlLuz(void (*callbackActivaLuz)(), void (*callbackDesactivaLuz)()) noexcept;
    ControlLuz() = delete;

private:
    void (*const callbackActivaLuz)();
    void (*const callbackDesactivaLuz)();
    bool estadoLuz = false;
};
#endif
