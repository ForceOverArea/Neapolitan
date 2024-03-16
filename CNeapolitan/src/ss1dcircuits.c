#include "ss1dcircuits.h"

//                  Function Name           Element Kind    Gain Dimension
ELEMENT_CONSTRUCTOR(resistorHelper,         RESISTOR,       1)
ELEMENT_CONSTRUCTOR(voltageSourceHelper,    VOLTAGESOURCE,  1)
ELEMENT_CONSTRUCTOR(currentSourceHelper,    CURRENTSOURCE,  1)

int resistor(Problem_T* problem, size_t a, size_t b, double resistance)
{
    ABORT_IF_NULL(voltageSourceHelper(problem, a, b))
    
    size_t back = problem->elements->len;
    Element_T* lastElem = problem->elements->data[back].pVal;
    lastElem->gain[0] = resistance;

    return OK;
}

int voltageSource(Problem_T* problem, size_t i, size_t o, double voltage)
{
    ABORT_IF_NULL(voltageSourceHelper(problem, i, o))
    
    size_t back = problem->elements->len;
    Element_T* lastElem = problem->elements->data[back].pVal;
    lastElem->gain[0] = voltage;

    Node_T* input = problem->nodes->data[i].pVal;
    Node_T* output = problem->nodes->data[o].pVal;

    if (!output->isLocked)
    {
        output->isLocked = TRUE;
    }
    else if (output->isLocked && !input->isLocked)
    {
        input->isLocked = TRUE;
    }
    else
    {
        return ERR;
    }

    return OK;
}

int currentSource(Problem_T* problem, size_t i, size_t o, double current)
{
    ABORT_IF_NULL(voltageSourceHelper(problem, i, o))
    
    size_t back = problem->elements->len;
    Element_T* lastElem = problem->elements->data[back].pVal;
    lastElem->gain[0] = current;

    return OK;
}