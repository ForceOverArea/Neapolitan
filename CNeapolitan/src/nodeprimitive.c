#include "nodeprimitive.h"

int newBasicNode(Problem_T* problem, size_t potentialDimension)
{
    Node_T* maybeNode = malloc(
        sizeof(Node_T) +
        sizeof(double) * potentialDimension);

    ABORT_IF_NULL(maybeNode);

    NodeVec_T* tmp;
    tmp = pushValToPtrVec(problem->nodes, maybeNode);
    ABORT_IF_NULL(tmp)
    problem->nodes = tmp;

    // Initialize ALL potential values 
    for (size_t i = 0; i < potentialDimension; i++)
    {
        maybeNode->potential[i] = 1;
    }

    return OK;
}

int newBasicElement(
    Problem_T* problem,
    size_t kind, 
    size_t gainDimension,
    Node_T* input, 
    Node_T* output)
{
    // Prevent connecting an element to itself
    if (input == output)
    {
        return ERR;
    }

    // Allocate pointer
    Element_T* maybeElement = malloc(
        sizeof(Element_T) + 
        sizeof(double) * gainDimension);

    // Bail if no memory left
    ABORT_IF_NULL(maybeElement);

    // Initialize struct fields
    maybeElement->kind = kind;
    maybeElement->inflow = input;
    maybeElement->inflow = output;

    // Connect parts of the network
    ElementVec_T* tmp;

    tmp = pushValToPtrVec(input->outputs, maybeElement);
    ABORT_IF_NULL(tmp)
    input->outputs = tmp;

    tmp = pushValToPtrVec(output->inputs, maybeElement);
    ABORT_IF_NULL(tmp)
    output->inputs = tmp;

    tmp = pushValToPtrVec(problem->elements, maybeElement);
    ABORT_IF_NULL(tmp)
    problem->elements = tmp;

    // Initialize ALL gain values 
    for (size_t i = 0; i < gainDimension; i++)
    {
        maybeElement->gain[i] = 1;
    }

    // Indicate success
    return OK;
}