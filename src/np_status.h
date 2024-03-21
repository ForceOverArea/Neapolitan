#ifndef NP_STATUS_H_
#define NP_STATUS_H_

#define PROPOGATE_ERROR(routine)\
    NpStatus_T stat = (routine);\
    if (OK != stat)\
    {\
        return stat;\
    }

typedef enum NpStatus_E
{
    OK,

    // System-related errors

    OUT_OF_MEMORY,

    // Vector-related errors

    VECTOR_LEN_MISMATCH,

    // Matrix-related errors

    INVALID_SIZE_GIVEN,
    MATRIX_DIMENSION_MISMATCH,
    ZERO_DETERMINANT,
    UNDEFINED_VALUE,

    // Last value:
    UNKNOWN_ERROR,
}
NpStatus_T;

#endif