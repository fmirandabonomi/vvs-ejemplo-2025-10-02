#ifndef CONTROL_LUZ_H
#define CONTROL_LUZ_H

struct ControlLuz
{
    bool getEstadoLuz();
    void encenderLuz();
    void apagarLuz();

    ControlLuz(void (*callbackActivaLuz)(), void (*callbackDesactivaLuz)())
        : callbackActivaLuz(callbackActivaLuz), callbackDesactivaLuz(callbackDesactivaLuz)
    {
        while (!callbackActivaLuz || !callbackDesactivaLuz)
            ;

        this->apagarLuz();
    }
    ControlLuz() = delete;

private:
    bool estadoLuz = false;
    void (*const callbackActivaLuz)();
    void (*const callbackDesactivaLuz)();
};
#endif
