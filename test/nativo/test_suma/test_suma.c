#include "suma.h"
#include "unity.h"

void setUp(void)
{

}
void tearDown(void)
{

}

// Corresponde al requerimiento de "Sumar dos enteros"
static void test_debe_sumar_dos_enteros(void)
{
    {
        const int a = 23;
        const int b = 31;
        const int valor_esperado = a + b;
        
        const int obtenido = suma(a,b);
        
        TEST_ASSERT_EQUAL_INT(valor_esperado, obtenido);
    }
    {
        const int a = 17;
        const int b = 11;
        const int valor_esperado = a + b;
        
        const int obtenido = suma(a,b);
        
        TEST_ASSERT_EQUAL_INT(valor_esperado, obtenido);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_debe_sumar_dos_enteros);
    return UNITY_END();
}