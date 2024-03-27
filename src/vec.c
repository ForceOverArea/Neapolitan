#include "vec.h"

Vec_S* newVec(size_t numElems)
{
    if (0 == numElems)
    {
        numElems = MIN_VEC_CAPACITY;
    }

    Vec_S* retVal = malloc(
        sizeof(Vec_S) + 
        sizeof(VecElement_U) * numElems);

    if (NULL == retVal)
    {
        return NULL;
    }

    retVal->capacity = MIN_VEC_CAPACITY;
    retVal->len = 0;

    return retVal;
}

Vec_S* newVecWithLen(size_t numElems)
{
    if (0 == numElems)
    {
        numElems = MIN_VEC_CAPACITY;
    }

    Vec_S* retVal = malloc(
        sizeof(Vec_S) + 
        sizeof(VecElement_U) * numElems);

    if (NULL == retVal)
    {
        return NULL;
    }

    retVal->capacity = MIN_VEC_CAPACITY;
    retVal->len = numElems;

    return retVal;
}

Vec_S* pushToVec(Vec_S* vec, VecElement_U elem)
{
    Vec_S* retVal = vec; 
    if (vec->len + 1 > vec->capacity)
    {
        Vec_S* tmp = realloc(retVal, 
            sizeof(Vec_S) +
            sizeof(VecElement_U) * retVal->capacity * 2);

        if (NULL == tmp)
        {
            return NULL;
        }

        retVal->capacity *= 2;
    }

    size_t back = retVal->len;
    retVal->elements[back] = elem;
    retVal->len += 1;

    return retVal;
}

VecElement_U popFromVec(Vec_S* vec)
{
    size_t back = vec->len;
    VecElement_U retVal = vec->elements[back];
    vec->len -= 1;

    return retVal;
}

NpStatus_E elementWiseAdd(Vec_S* sum, Vec_S* lAddend, Vec_S* rAddend, bool isFloat)
{
    #define OPERATOR +
    #include "vecops.h"
    #undef OPERATOR
}

#include <stdio.h>

NpStatus_E elementWiseDiff(Vec_S* diff, Vec_S* minuend, Vec_S* subtrahend, bool isFloat)
{
    #define OPERATOR -
    #include "vecops.h"
    #undef OPERATOR
}

NpStatus_E elementWiseProd(Vec_S* prod, Vec_S* lFactor, Vec_S* rFactor, bool isFloat)
{
    #define OPERATOR *
    #include "vecops.h"
    #undef OPERATOR
}
