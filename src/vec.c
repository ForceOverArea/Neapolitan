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
    if (sum->len != lAddend->len || lAddend->len != rAddend->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    size_t n = sum->len;
    if (isFloat)
    {
        for (size_t i = 0; i < n; i++)
        {
            sum->elements[i].floating = 
                lAddend->elements[i].floating + rAddend->elements[i].floating;
        }
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            sum->elements[i].integral = 
                lAddend->elements[i].integral + rAddend->elements[i].integral;
        }
    }

    return OK;
}

NpStatus_E elementWiseDiff(Vec_S* diff, Vec_S* minuend, Vec_S* subtrahend, bool isFloat)
{
    if (diff->len != minuend->len || minuend->len != subtrahend->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    size_t n = diff->len;
    if (isFloat)
    {
        for (size_t i = 0; i < n; i++)
        {
            diff->elements[i].floating = 
                minuend->elements[i].floating * subtrahend->elements[i].floating;
        }
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            diff->elements[i].integral = 
                minuend->elements[i].integral * subtrahend->elements[i].integral;
        }
    }

    return OK;
}

NpStatus_E elementWiseProd(Vec_S* prod, Vec_S* lFactor, Vec_S* rFactor, bool isFloat)
{
    if (prod->len != lFactor->len || lFactor->len != rFactor->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    size_t n = prod->len;
    if (isFloat)
    {
        for (size_t i = 0; i < n; i++)
        {
            prod->elements[i].floating = 
                lFactor->elements[i].floating * rFactor->elements[i].floating;
        }
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            prod->elements[i].integral = 
                lFactor->elements[i].integral * rFactor->elements[i].integral;
        }
    }

    return OK;
}

NpStatus_E addAssignElementWise(
    Vec_S* sum,
    Vec_S* addend)
{
    if (sum->len != addend->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    // Calculate element-wise potential 
    for (size_t i = 0; i < sum->len; i++)
    {
        sum->elements[i].floating += addend->elements[i].floating;
    }

    return OK;
}

NpStatus_E subAssignElementWise(
    Vec_S* difference,
    Vec_S* subtrahend)
{
    if (difference->len != subtrahend->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    // Calculate element-wise potential 
    for (size_t i = 0; i < difference->len; i++)
    {
        difference->elements[i].floating -= subtrahend->elements[i].floating;
    }

    return OK;
}
