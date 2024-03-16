#ifndef ELEMENTTYPES_H_
#define ELEMENTTYPES_H_

/// @brief An enum that determines the globally 
/// unique value for a given element type.
enum ElementTypes
{
    // Steady-state, 1D circuits
    RESISTOR,
    VOLTAGESOURCE,
    CURRENTSOURCE,
    // Steady-state AC circuits
    IMPEDANCE,
    INDUCTOR,
    CAPACITOR,
    ACSOURCE,
    // Static 1D structures
    LINK,
};

#endif