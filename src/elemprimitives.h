#ifndef ELEMPRIMITIVES_H_
#define ELEMPRIMITIVES_H_

#include <memory.h>
#include <stdbool.h>
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
typedef bool (*FluxCalculation_P)(
    Vec_T*,
    Vec_T*, 
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
 * Performs a flux balance on the given node, returning a vector value 
 * containing the unaccounted-for flow in the system at that node.
 */
bool fluxDiscrepancy(Vec_T* fluxDiscrep, GenericNode_T* node);

/**
 * `FluxCalculation_P` function for elements that calculate a flux 
 * value through some product of the element's gain value and the 
 * adjacent nodes' potential values.
 */
bool normalFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool _dnu);

/**
 * `FluxCalculation_P` function for elements that calculate a flux 
 * value by adjusting the potential of one of their nodes and observing 
 * the flow discrepancy at the adjusted node.
 */
bool observeFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool drivesOutput);

/**
 * `FluxCalculation_P` function for elements force a constant flux into/out 
 * of their two adjacent nodes.
 */
bool forceFlux(Vec_T* flux, Vec_T* gain, GenericNode_T* input, GenericNode_T* output, bool _dnu);

#endif