#ifndef ELEMPRIMITIVES_H_
#define ELEMPRIMITIVES_H_

#include <memory.h>
#include <stdbool.h>

#include "np_status.h"
#include "vec.h" // also includes stdlib.h

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
     * 
     * If this value is not NULL, then the node is
     * a degree of freedom in the problem
     */
    void* lockingElement;
} 
GenericNode_T;

/**
 * The function signature used by functions that govern the 
 * flux or flow through elements in the problem 
 */
typedef NpStatus_T (*FluxCalculation_P)(
    Vec_T*,
    Vec_T*, 
    GenericNode_T*, 
    GenericNode_T*,
    bool);

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
    GenericNode_T* inputNode, * outputNode;
    /**
     * The mathematical vector value representing 
     * the gain for this element.
     */
    Vec_T* gainVector;
    /**
     * Indicates whether this element drives the output node's potential. 
     * If this is false and the element's flux function is `observeFlux`,
     * it is assumed that the input node's potential is driven instead.
     */
    bool drivesOutputPotential;
    /**
     * The function to use to determine the flux or 
     * flow through this element given its gain and 
     * two connected nodes.
     */
    FluxCalculation_P flux;
} 
GenericElement_T;

/**
 * Initializes a new, unlocked node with empty vectors for new elements
 * and an uninitialized potential vector of the given dimension. 
 */
NpStatus_T tryNewNode(GenericNode_T* node, size_t dimension);

/**
 * Initializes a new generic element with no connected nodes, no
 * flux function, and empty gain potential vectors of the given 
 * dimension. 
 */
NpStatus_T tryNewElement(GenericElement_T* elem, size_t dimension, FluxCalculation_P func);

/**
 * Performs a flux balance on the given node, returning a vector value 
 * containing the unaccounted-for flow in the system at that node.
 */
NpStatus_T fluxDiscrepancy(Vec_T* fluxDiscrep, GenericNode_T* node);

/**
 * `FluxCalculation_P` function for elements that calculate a flux 
 * value through some product of the element's gain value and the 
 * adjacent nodes' potential values.
 */
NpStatus_T normalFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool _dnu);

/**
 * `FluxCalculation_P` function for elements that calculate a flux 
 * value by adjusting the potential of one of their nodes and observing 
 * the flow discrepancy at the adjusted node.
 */
NpStatus_T observeFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool drivesOutput);

/**
 * `FluxCalculation_P` function for elements force a constant flux into/out 
 * of their two adjacent nodes.
 */
NpStatus_T forceFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool _dnu);

#endif