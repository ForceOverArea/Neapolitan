#ifndef NEAPSYMBOLS_H_
#define NEAPSYMBOLS_H_

/// @brief Value only to be used to ensure that `(1 / DX) = DISCRETE_INF`
#define _DX_VALUE 1E-4

/// @brief A crude approximation for infinitismal quantity.
const double DX = _DX_VALUE;

/// @brief A crude approximation for infinity. This should only be used for 
/// approximating improper integrals that are assumed to converge.  
const double DISCRETE_INF = 1 / _DX_VALUE; 

/// @brief Return value inticating success on "try-routine" pattern functions
const int OK = 1;

/// @brief Return value inticating failure on "try-routine" pattern functions
const int ERR = 0;

/// @brief Causes a wrapping function `ERR` 
/// if the given pointer is `NULL`.
#define ABORT_IF_NULL(var)  \
if (NULL == (var))          \
{                           \
    return ERR;             \
}                   

#endif