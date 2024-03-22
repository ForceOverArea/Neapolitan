#include "harness.h"
#include "elemprimitives.h"

TEST(normal_elem_calculates_flux_correctly)
{
    GenericNode_T n1, n2;
    GenericElement_T e1;

    ASSERT_OK(tryNewNode(&n1, 1))
    ASSERT_OK(tryNewNode(&n2, 1))

    ASSERT_OK(tryNewElement(&e1, 1, normalFlux))

    e1.inputNode = &n1;
    e1.outputNode = &n2;

    // Delta V of 3V
    n1.potentialVector->elements[0].floating = 3; // 3V
    n2.potentialVector->elements[0].floating = 0; // 0V

    FLOATING resistance = 3; // ohms
    FLOATING conductance = 1/resistance;

    e1.gainVector->elements[0].floating = conductance;

    Vec_T* current = newVec(1); 
    ASSERT(NULL != current)

    ASSERT_OK(fluxDiscrepancy(current, &n1))
    ASSERT_EQ(current->elements[0].floating, -resistance, %lf)
    
    ASSERT_OK(fluxDiscrepancy(current, &n2))
    ASSERT_EQ(current->elements[0].floating, resistance, %lf)

    END_TEST
}

int main(void)
{
    normal_elem_calculates_flux_correctly();
    return 0;
}