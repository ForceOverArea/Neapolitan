#ifndef VEC_H_
#define VEC_H_

#include <stdlib.h>
#include <stdbool.h>

#include "np_status.h"

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

#define FLT_ELEM(x) (VecElement_T){ .floating = (x) }

#define PTR_ELEM(x) (VecElement_T){ .pointer = (x) }

#define INT_ELEM(x) (VecElement_T){ .integral = (x) }

Vec_T* newVec(size_t numElems);

Vec_T* pushToVec(Vec_T* vec, VecElement_T elem);

VecElement_T popFromVec(Vec_T* vec);

NpStatus_T elementWiseAdd(Vec_T* sum, Vec_T* lAddend, Vec_T* rAddend, bool isFloat);

NpStatus_T elementWiseDiff(Vec_T* diff, Vec_T* minuend, Vec_T* subtrahend, bool isFloat);

NpStatus_T elementWiseProd(Vec_T* diff, Vec_T* minuend, Vec_T* subtrahend, bool isFloat);

#endif