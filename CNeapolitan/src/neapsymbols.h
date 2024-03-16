#ifndef NEAPSYMBOLS_H_
#define NEAPSYMBOLS_H_

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
if (NULL == (void*)(size_t)(var))\
{                           \
    return ERR;             \
}                   

#endif