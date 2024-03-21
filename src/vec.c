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
