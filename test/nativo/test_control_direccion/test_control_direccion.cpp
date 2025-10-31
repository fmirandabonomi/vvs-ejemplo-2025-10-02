#include "control_direccion.hpp"
#include "unity.h"

static ControlDireccion *controlDireccion;

static int direccion;

static void setDireccion(int nuevaDireccion)
{
    direccion = nuevaDireccion;
}

void setUp(void)
{
    // Configuración previa a cada prueba
    direccion = 10;
    controlDireccion = new ControlDireccion(setDireccion);
}

void tearDown(void)
{
    // Limpieza posterior a cada prueba
    delete controlDireccion;
}

// Corresponde al caso de uso UC1 (Observa estado del spot) por lectura de dirección
// Corresponde al caso de uso UC3 (Modifica dirección del spot) por establecer la dirección inicial
// Al inicializar el control de dirección debe establecer la dirección en cero
// La dirección leída debe coincidir con la dirección establecida
void test_direccion_inicial_es_cero_lee_direccion(void)
{
    TEST_ASSERT_EQUAL(0, controlDireccion->leeDireccion());
    TEST_ASSERT_EQUAL(0, direccion);
}

// Corresponde al caso de uso UC3 (Modifica dirección del spot)
// Corresponde al caso de uso UC1 (Observa estado del spot) por lectura de dirección
// Al modificar la dirección a 45, la dirección leída debe ser 45
void test_modifica_direccion_a_45_y_lee_direccion(void)
{
    controlDireccion->estableceDireccion(45);
    TEST_ASSERT_EQUAL(45, controlDireccion->leeDireccion());
    TEST_ASSERT_EQUAL(45, direccion);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_direccion_inicial_es_cero_lee_direccion);
    RUN_TEST(test_modifica_direccion_a_45_y_lee_direccion);
    return UNITY_END();
}
