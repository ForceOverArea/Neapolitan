#ifndef MATRIX_H_
#define MATRIX_H_

#include "vec.h" // Also includes stdlib.h

/// @brief Represents an MxN matrix.
struct Matrix_S
{
    size_t rows;
    size_t cols;
    DoubleVec_T* data;
};

/// @brief An abbreviation for `struct Matrix_S`.
typedef struct Matrix_S Matrix_T;

/// @brief A routine that allocates a new `Matrix_T` within the given pointer
/// @param mat A reference to the `Matrix_T` to populate
/// @param rows The number of rows the matrix should have
/// @param cols The number of columns the matrix should have
/// @return An integer value indicating success (1) or failure (0)
int newMatrix(Matrix_T* mat, size_t rows, size_t cols);

/// @brief Returns a mutable pointer to a value stored in the `Matrix_T`.
/// @param mat The `Matrix_T*` to get the value from.
/// @param i The row to access
/// @param j The column to access
/// @return A pointer to the value at the given row and column.
inline double* indexMatrix(const Matrix_T* mat, size_t i, size_t j)
{
    return getDoubleVecIndexAsRef(mat->data, i * mat->cols + j);
}

/// @brief Swaps two rows in the given `Matrix_T`
/// @param mat A pointer to the `Matrix_T` to mutate.
/// @param r1 The first row to swap
/// @param r2 The second row to swap
void inplaceRowSwap(Matrix_T* mat, size_t r1, size_t r2);

/// @brief Calculates the Matrix product of two matrices
/// @param left The 'left' matrix pointer
/// @param right The 'right' matrix pointer
/// @return The product of the left and right matrix
Matrix_T* mulMatrix(const Matrix_T* left, const Matrix_T* right);

/// @brief Augments two matrices, producing a third 
/// matrix with the left's columns appended to the right's.
/// @param left The left matrix to use
/// @param right The right matrix to use
/// @return A new pointer to the augment of `left` and `right`
Matrix_T* augmentMatrix(const Matrix_T* left, const Matrix_T* right);

/// @brief Takes a slice of a matrix, copying the 
/// sliced values to a newly-allocated `Matrix_T`.
/// @param mat A pointer to the matrix to take a slice from
/// @param r1 The first (top) row in the slice 
/// @param c1 The first (left) column in the slice
/// @param r2 The last row in the slice
/// @param c2 The last column in the slice
/// @return A new `Matrix_T` containing the selected elements
Matrix_T* subset(const Matrix_T* mat, size_t r1, size_t c1, size_t r2, size_t c2);

/// @brief Tries to invert a `Matrix_T` in-place, 
/// mutating it's value to the inverse. 
///
/// It is generally not advised to continue working
/// with the same `Matrix_T` instance after failing 
/// to invert it, but Matrices of size 4x4 or smaller 
/// WILL NOT be mutated when this routine indicates 
/// failure.
/// @param mat A pointer to the `Matrix_T` to invert
/// @return An `int` (boolean) value indicating 
/// success (1) or failure (0). 
int tryInplaceInvert(Matrix_T* mat);

#endif