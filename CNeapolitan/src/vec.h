#ifndef VEC_H_
#define VEC_H_

#include <stdlib.h>

#define FLOATING_UNIT_SIZE double
#define POINTER_UNIT_SIZE void*
#define INTEGRAL_UNIT_SIZE size_t
#define MIN_VEC_LENGTH 8

union UnitSized_U
{
    FLOATING_UNIT_SIZE fVal;
    POINTER_UNIT_SIZE pVal;
    INTEGRAL_UNIT_SIZE iVal;
};

typedef union UnitSized_U UnitSized_T;

struct Vec_S
{
    size_t len;
    size_t capacity;
    UnitSized_T data[];
};

// Generic case for values of any practical type

typedef struct Vec_S Vec_T;

/// @brief Allocates a new typeless `Vec_T*`. 
/// Note that this must be destroyed with `free` 
/// to avoid a memory leak.
/// @return a new `Vec_T*` with capacity 
/// `MIN_VEC_LENGTH` and len `0`.
Vec_T* newVec();

/// @brief Allocates a new typeless `Vec_T*` with 
/// at least the specified capacity. Note that this 
/// must be destroyed with `free` to avoid a memory 
/// leak. 
/// @param cap The capacity the vector should have.
/// @return a new `Vec_T*` with the given capacity and len `0`.
Vec_T* newVecWithCapacity(size_t cap);

/// @brief Pushes a new value to the back of a `Vec_T`. If adding another element
/// exceeds the `capacity` of this `Vec_T`, then it is reallocated to have double the 
/// capacity.
/// @param vec The `Vec_T` to add a value to.
/// @param val The `UnitSized_T` value to add to the `Vec_T`.
/// @return The (possibly) new `Vec_T*` pointing to the `Vec_T`
/// if the operation was successful.
Vec_T* pushValToVec(Vec_T* vec, UnitSized_T val);

/// @brief Returns a mutable pointer to a value stored in the `Vec_T`.
/// @param vec The `Vec_T*` to get the value from.
/// @param idx The index to get a pointer to.
/// @return Either a `UnitSized_T*` on success, or `NULL` if
/// the index was out of bounds.
inline UnitSized_T* getVecIndexAsRef(Vec_T* vec, size_t idx)
{
    if (idx >= vec->len)
    {
        return NULL;
    }
    return &(vec->data[idx]);
}

/// @brief Returns the last value stored in the `Vec_T` and reduces the `len` 
/// of the `Vec_T` by one.
/// @param vec The `Vec_T*` to get a value from.
/// @return The last element of the `Vec_T`.
inline UnitSized_T popValBackFromVec(Vec_T* vec)
{
    UnitSized_T retVal = vec->data[vec->len - 1];
    vec->len--;
    return retVal;
}                       

// Specific cases for double values

/// @brief A specific type of vec for storing `double` values.
typedef Vec_T DoubleVec_T;

inline DoubleVec_T* newDoubleVec()
{
    return newVec();
}

inline DoubleVec_T* pushValToDoubleVec(DoubleVec_T* vec, double val)
{
    UnitSized_T unit;
    unit.fVal = val;
    return pushValToVec(vec, unit);
}

inline double* getDoubleVecIndexAsRef(DoubleVec_T* vec, size_t idx)
{
    return &(getVecIndexAsRef(vec, idx)->fVal);
}

inline double popValBackFromDoubleVec(DoubleVec_T* vec)
{
    return popValBackFromVec(vec).fVal;
}

// Specific cases for pointer values

typedef Vec_T PtrVec_T;

inline PtrVec_T* newPtrVec()
{
    return newVec();
}

inline PtrVec_T* pushValToPtrVec(PtrVec_T* vec, void* val)
{
    UnitSized_T unit;
    unit.pVal = val;
    return pushValToVec(vec, unit);
}

inline void* getPtrVecIndex(PtrVec_T* vec, size_t idx)
{
    return getVecIndexAsRef(vec, idx)->pVal;
}

inline void* popValBackFromDoubleVec(PtrVec_T* vec)
{
    return popValBackFromVec(vec).pVal;
}

// Specific cases for integral values

typedef Vec_T IntVec_T;

inline IntVec_T* newPtrVec()
{
    return newVec();
}

inline IntVec_T* pushValToIntVec(IntVec_T* vec, size_t val)
{
    UnitSized_T unit;
    unit.pVal = val;
    return pushValToVec(vec, unit);
}

inline size_t* getIntVecIndex(IntVec_T* vec, size_t idx)
{
    return &(getVecIndexAsRef(vec, idx)->iVal);
}

inline size_t popValBackFromIntVec(IntVec_T* vec)
{
    return popValBackFromVec(vec).iVal;
}

#endif