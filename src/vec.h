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
VecElement_U;

typedef struct Vec_S
{
    size_t len;
    size_t capacity;
    VecElement_U elements[];
} 
Vec_S;

#define FLT_ELEM(x) (VecElement_U){ .floating = (x) }

#define PTR_ELEM(x) (VecElement_U){ .pointer = (x) }

#define INT_ELEM(x) (VecElement_U){ .integral = (x) }

Vec_S* newVec(size_t numElems);

Vec_S* newVecWithLen(size_t numElems);

Vec_S* pushToVec(Vec_S* vec, VecElement_U elem);

VecElement_U popFromVec(Vec_S* vec);

NpStatus_E elementWiseAdd(Vec_S* sum, Vec_S* lAddend, Vec_S* rAddend, bool isFloat);

NpStatus_E elementWiseDiff(Vec_S* diff, Vec_S* minuend, Vec_S* subtrahend, bool isFloat);

NpStatus_E elementWiseProd(Vec_S* prod, Vec_S* lFactor, Vec_S* rFactor, bool isFloat);

NpStatus_E addAssignElementWise(Vec_S* sum, Vec_S* addend);

NpStatus_E subAssignElementWise(Vec_S* difference, Vec_S* subtrahend);

void elementWiseAbs(Vec_S* vec);

void zeroVector(Vec_S* vec);

#endif