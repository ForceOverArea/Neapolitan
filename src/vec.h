#ifndef VEC_H_
#define VEC_H_

#include <stdlib.h>

#define MIN_VEC_CAPACITY 8

union VecElement_U
{
    double floating;
    void* pointer;
    size_t integral;
};

typedef union VecElement_U VecElement_T;

struct Vec_S
{
    size_t len;
    size_t capacity;
    VecElement_T elements[];
};

typedef struct Vec_S Vec_T;

inline VecElement_T doubleElem(double elem)
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