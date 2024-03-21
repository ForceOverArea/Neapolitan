#ifndef VEC_H_
#define VEC_H_

#include <stdlib.h>

#define MIN_VEC_CAPACITY 8

#define FLOATING double
#define POINTER void*
#define INTEGRAL size_t

typedef union VecElement_U
{
    FLOATING floating;    // size A
    POINTER pointer;      // size B
    INTEGRAL integral;    // size B
} 
VecElement_T;

typedef struct Vec_S
{
    size_t len;
    size_t capacity;
    VecElement_T elements[];
} 
Vec_T;

inline VecElement_T FLOATINGElem(FLOATING elem)
{
    VecElement_T retVal;
    retVal.floating = elem;
    return retVal;
}

inline VecElement_T ptrElem(void* elem)
{
    VecElement_T retVal;
    retVal.pointer = elem;
    return retVal;
}

inline VecElement_T intElem(size_t elem)
{
    VecElement_T retVal;
    retVal.integral = elem;
    return retVal;
}

Vec_T* newVec(size_t numElems);

Vec_T* pushToVec(Vec_T* vec, VecElement_T elem);

VecElement_T popFromVec(Vec_T* vec);

#endif