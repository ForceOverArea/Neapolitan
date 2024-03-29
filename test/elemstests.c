#include "harness.h"
#include "elems.h"

TEST(normal_elem_calculates_flux_correctly)
{
    GenericNode_S n1, n2;
    GenericElement_S e1;

    // Vecs for elements initialized here
    ASSERT_OK(tryNewNode(&n1, 1))
    ASSERT_OK(tryNewNode(&n2, 1))

    // Vec for gain initialized here:
    ASSERT_OK(tryNewElement(&e1, 1, normalFlux))

    e1.inputNode = &n1;
    e1.outputNode = &n2;

    pushToVec(n1.outputs, PTR_ELEM(&e1));
    pushToVec(n2.inputs,  PTR_ELEM(&e1));

    // Delta V of 3V
    n1.potentialVector->elements[0] = FLT_ELEM(9); // 9V
    n2.potentialVector->elements[0] = FLT_ELEM(0); // 0V

    // Resistance of 3 ohms
    FLOATING resistance = 3;
    pushToVec(e1.gainVector, FLT_ELEM(1/resistance));

    Vec_S* current = newVec(1); 
    pushToVec(current, FLT_ELEM(3));
    ASSERT(NULL != current)

    ASSERT_OK(fluxDiscrepancy(current, &n1))
    ASSERT_EQ(-resistance, current->elements[0].floating, %lf)

    ASSERT_OK(fluxDiscrepancy(current, &n2))
    ASSERT_EQ(resistance, current->elements[0].floating, %lf)

    free(current);

    END_TEST
}

int main(void)
{
    normal_elem_calculates_flux_correctly();
    return 0;
}