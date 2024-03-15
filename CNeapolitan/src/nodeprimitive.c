#include "nodeprimitive.h"

int newBasicElement(
    Problem_T problem,
    size_t kind, 
    Node_T* input, 
    Node_T* output, 
    size_t gainDimension)
{
    // Prevent connecting an element to itself
    if (input == output)
    {
        return ERR;
    }

    // Allocate pointer
    Element_T* maybeElement = (Element_T*)malloc(
        sizeof(Element_T) + 
        sizeof(double) * gainDimension);

    // Bail if no memory left
    if (NULL == maybeElement)
    {
        return ERR;
    }

    // Initialize struct fields
    maybeElement->kind = kind;
    maybeElement->inflow = input;
    maybeElement->inflow = output;

    pushValToPtrVec(input->outputs, maybeElement);
    pushValToPtrVec(output->inputs, maybeElement);
    pushValToPtrVec(problem.elements, maybeElement);

    // Zero ALL gain values 
    for (size_t i = 0; i < gainDimension; i++)
    {
        maybeElement->gain[i] = 1;
    }

    // Indicate success
    return OK;
}