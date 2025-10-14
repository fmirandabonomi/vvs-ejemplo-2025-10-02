#include "control_luz.h"
#include "unity.h"

static ControlLuz *controlador;

void setUp()
{
    controlador = new ControlLuz();
}
void tearDown()
{
    delete controlador;
}

// Corresponde a los casos de uso de "Observar el estado de la luz"
// El estado inicial de la luz debe ser apagado
static void test_el_estado_inicial_de_la_luz_es_apagado()
{
    TEST_ASSERT_FALSE(controlador->getEstadoLuz());
}

static void test_debe_poder_encenderse_la_luz()
{
    controlador->encenderLuz();
    TEST_ASSERT_TRUE(controlador->getEstadoLuz());
}

static void test_debe_poder_apagarse_la_luz()
{
    controlador->encenderLuz();
    controlador->apagarLuz();
    TEST_ASSERT_FALSE(controlador->getEstadoLuz());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_el_estado_inicial_de_la_luz_es_apagado);
    RUN_TEST(test_debe_poder_encenderse_la_luz);
    RUN_TEST(test_debe_poder_apagarse_la_luz);
    return UNITY_END();
}
