#ifndef NEAPSYMBOLS_H_
#define NEAPSYMBOLS_H_

/// @brief Value only to be used to ensure that `(1 / DX) = DISCRETE_INF`
#define _DX_VALUE 1E-4

/// @brief A crude approximation for infinitismal quantity.
const double DX = _DX_VALUE;

/// @brief A crude approximation for infinity. This should only be used for 
/// approximating improper integrals that are assumed to converge.  
const double DISCRETE_INF = 1 / _DX_VALUE; 

/// @brief Return value inticating process success
#define OK 1

/// @brief Return value inticating process failure
#define ERR 0

/// @brief Alias for 1 for better legibility
#define TRUE 1

///@brief Alias for 1 for better legibility
#define FALSE 0

/// @brief Causes a wrapping function to return 
/// `ERR` if the given pointer is `NULL`.
#define ABORT_IF_NULL(var)  \
if (NULL == (void*)(long long)(var))\
{                           \
    return ERR;             \
}                   

#endif