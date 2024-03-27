/**
 * This header file is meant to be included multiple times 
 * in vec.c in order to commonize the source for various vector 
 * operations. 
 */
#ifdef OPERATOR
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
#else
#error "The 'OPERATOR' macro must be defined as a binary operator in order to include vecops.h!"
#endif