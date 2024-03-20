#ifndef ELEMPRIMITIVES_H_
#define ELEMPRIMITIVES_H_

#include "vec.h"

/**
 * A node in a nodal analysis problem.
 * 
 * Nodes can be connected to 1 or more elements,
 * and can have an N-dimensional "potential" value
 * (e.g. voltage, pressure, temperature, etc).
 */
typedef struct GenericNode_S
{
    /**
     * The mathematical vector value representing 
     * the nodal potential value for this node.
     */
    Vec_T* potentialVector;
    /**
     * Pointers to the nodes "inputting" flux to 
     * this node and the nodes "outputting" flux 
     * from this node.
     */
    Vec_T* inputs, * outputs;
    /**
     * A pointer to an element that locked this 
     * node. This is used to know what the nodal 
     * potential should be based on which element
     * is driving it.
     */
    void* lockingElement;
} 
GenericNode_T;

/**
 * 
 */
typedef Vec_T* (*FluxCalculation_P)(
    GenericElement_T*, 
    GenericNode_T*, 
    GenericNode_T*);

/**
 * An element in a nodal analysis problem.
 * 
 * Elements must have exactly 2 nodes connected to 
 * make sense in the context of the problem. Elements
 * have an N-dimensional "gain" value (e.g. resistance,
 * orifice coef., thermal conductivity, etc). 
 */
typedef struct GenericElement_S
{   
    /**
     * The nodes connected to the input and output
     * ends of this element, respectively. 
     */
    GenericNode_T* inputNode, outputNode;
    /**
     * The mathematical vector value representing 
     * the gain for this element.
     */
    Vec_T* gainVector;
} 
GenericElement_T;



#endif