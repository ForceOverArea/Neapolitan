#include "vec.h"

Vec_T* newVec(size_t numElems)
{
    if (0 == numElems)
    {
        numElems = MIN_VEC_CAPACITY;
    }

    Vec_T* retVal = malloc(
        sizeof(Vec_T) + 
        sizeof(VecElement_T) * numElems);

    if (NULL == retVal)
    {
        return NULL;
    }

    retVal->capacity = MIN_VEC_CAPACITY;
    retVal->len = 0;

    return retVal;
}

Vec_T* pushToVec(Vec_T* vec, VecElement_T elem)
{
    Vec_T* retVal = vec; 
    if (vec->len + 1 > vec->capacity)
    {
        Vec_T* tmp = realloc(retVal, 
            sizeof(Vec_T) +
            sizeof(VecElement_T) * retVal->capacity * 2);

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

VecElement_T popFromVec(Vec_T* vec)
{
    size_t back = vec->len;
    VecElement_T retVal = vec->elements[back];
    vec->len -= 1;

    return retVal;
}

NpStatus_T elementWiseAdd(Vec_T* sum, Vec_T* lAddend, Vec_T* rAddend, bool isFloat)
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

NpStatus_T elementWiseDiff(Vec_T* diff, Vec_T* minuend, Vec_T* subtrahend, bool isFloat)
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
                minuend->elements[i].floating - subtrahend->elements[i].floating;
        }
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            diff->elements[i].integral = 
                minuend->elements[i].integral - subtrahend->elements[i].integral;
        }
    }

    return OK;
}

NpStatus_T elementWiseProd(Vec_T* prod, Vec_T* lFactor, Vec_T* rFactor, bool isFloat)
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
