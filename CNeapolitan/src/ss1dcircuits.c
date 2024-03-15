#include "ss1dcircuits.h"

struct Node_T* newNode1D()
{
    Node_T* node = (Node_T*)malloc(sizeof(Node_T) + sizeof(double));
    
    node->inputs = newPtrVec();
    node->outputs = newPtrVec();
    node->potential[0] = 1;
    node->isLocked = 0;
    
    return node;
}

int addNode1D(Problem_T* problem)
{
    Problem_T* maybeNew = pushValToPtrVec(problem, newNode1D());
    if (maybeNew == NULL)
    {
        return ERR;
    }
    problem = maybeNew;
    return 1;
}

int resistor(Problem_T problem, size_t a, size_t b)
{
    return newBasicElement(
        problem, 
        RESISTOR, 
        (problem.nodes + a),
        (problem.nodes + b),
        (size_t)1);
}

int voltageSource(Problem_T problem, size_t a, size_t b)
{
    // Cannot connect a node to itself 
    if (a == b)
    {
        return ERR;
    }
    // Push element to element vector
    ElementVec_T* tmp;
}