#ifndef NODEPRIMITIVE_H_
#define NODEPRIMITIVE_H_

#include "matrix.h" // Also includes vec.h, stdlib.h
#include "neapsymbols.h"

struct Node_S
{
    PtrVec_T* inputs;
    PtrVec_T* outputs;
    int isLocked;
    double potential[];
};

/// @brief A node in a nodal analysis problem 
typedef struct Node_S Node_T;

struct Element_S
{
    size_t kind;
    Node_T* inflow;
    Node_T* outflow;
    double gain[];
};

/// @brief A basic 2-node element in a nodal analysis problem
typedef struct Element_S Element_T;

/// @brief A vector of element pointers
typedef PtrVec_T ElementVec_T;

/// @brief A vector of node pointers
typedef PtrVec_T NodeVec_T;

typedef struct Problem_S
{
    ElementVec_T* elements;
    NodeVec_T* nodes; 
};

/// @brief A struct for managing all the elements and nodes in a problem
typedef struct Problem_S Problem_T;

/// @brief Initializes a new `Problem_T` with no elements or nodes.
/// @return A new, empty `Problem_T`
inline Problem_T initProblem()
{
    Problem_T problem = 
    { 
        newPtrVec(), 
        newPtrVec(), 
    };

    return problem;
}

/// @brief A routine that creates a new node within a problem.
/// @param problem A reference to the `Problem_T` to add a `Node_T` to. 
/// @param potentialDimension The dimension of the added node's potential value.
/// @return an `int` value representing success (1) or failure (0) 
int newBasicNode(Problem_T* problem, size_t potentialDimension);

/// @brief A routine that creates a new element within a problem. 
/// @param problem The `Problem_T` to add the element in
/// @param kind The `ElementType` enum variant that this element should have.
/// @param input The node to connect to the element's input
/// @param output The node to connect to the element's output
/// @param gainDimension The dimension of this element's gain. (e.g. a simple resistor will be 1, an impedance value will be 2, etc.)
/// @return an integer value indicating success (1) or failure (0)
int newBasicElement(
    Problem_T* problem, 
    size_t kind, 
    size_t gainDimension,
    Node_T* input, 
    Node_T* output);

/// @brief Expands to a constructor function for adding 
/// an element to a problem.
#define ELEMENT_CONSTRUCTOR(funcName, kind, dim)        \
inline int funcName ## (Problem_T* problem, size_t a, size_t b)\
{                                                       \
    return newBasicElement(                             \
        problem,                                        \
        kind,                                           \
        (problem->nodes + a),                           \
        (problem->nodes + b),                           \
        (size_t)(dim));                                 \
}

#endif