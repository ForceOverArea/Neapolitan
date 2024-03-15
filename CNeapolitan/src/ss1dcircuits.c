#include "ss1dcircuits.h"

ELEMENT_CONSTRUCTOR(resistor, RESISTOR, 1)

ELEMENT_CONSTRUCTOR(voltageSourceHelper, VOLTAGESOURCE, 1)

ELEMENT_CONSTRUCTOR(currentSourceHelper, CURRENTSOURCE, 1)

int voltageSource(Problem_T* problem, size_t a, size_t b)
{
    ABORT_IF_NULL(voltageSourceHelper(problem, a, b));

    problem->elements
}