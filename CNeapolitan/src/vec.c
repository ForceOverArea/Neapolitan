#include "vec.h"

Vec_T* newVec()
{
    Vec_T* retVal = malloc(
        sizeof(Vec_T) +
        sizeof(UnitSized_T) * MIN_VEC_LENGTH);

    if (NULL == retVal)
    {
        return NULL;
    }

    retVal->capacity = MIN_VEC_LENGTH;
    retVal->len = 0;

    return retVal;
}

Vec_T* newVecWithCapacity(size_t cap)
{
    if (MIN_VEC_LENGTH >= cap)
    {
        return newVec();
    }
    
    Vec_T* retVal = malloc(
        sizeof(Vec_T) +
        sizeof(UnitSized_T) * cap);

    if (NULL == retVal)
    {
        return NULL;
    }

    retVal->capacity = cap;
    retVal->len = 0;

    return retVal;
}

Vec_T* pushValToVec(Vec_T* vec, UnitSized_T val)
{
    if (vec->len + 1 <= vec->capacity)
    {
        Vec_T* tmp = realloc(vec, 
            sizeof(Vec_T) +
            sizeof(UnitSized_T) * vec->capacity * 2);

        if (NULL == tmp)
        {
            return NULL;
        }

        vec = tmp;
        vec->capacity *= 2;
        vec->data[vec->len] = val;
        vec->len++;
    }
    else // No allocation is needed
    {
        vec->data[vec->len] = val;
        vec->len++;
    }
    return vec;
}
