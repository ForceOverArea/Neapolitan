/**
 * This header file is meant to be included multiple times 
 * in vec.c in order to commonize the source for various vector 
 * operations. 
 */
#ifdef OPERATOR
    if (result->len != lOperend->len || lOperend->len != rOperend->len)
    {
        return VECTOR_LEN_MISMATCH;
    }

    size_t n = result->len;
    if (isFloat)
    {
        for (size_t i = 0; i < n; i++)
        {
            result->elements[i].floating = 
                lOperend->elements[i].floating * rOperend->elements[i].floating;
        }
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            result->elements[i].integral = 
                lOperend->elements[i].integral * rOperend->elements[i].integral;
        }
    }

    return OK;
#else
#error "The 'OPERATOR' macro must be defined as a binary operator in order to include vecops.h!"
#endif