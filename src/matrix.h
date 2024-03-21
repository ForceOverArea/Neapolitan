#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdbool.h>
#include "np_status.h"
#include "vec.h" // Also includes stdlib.h

typedef struct Matrix_S
{
    size_t rows, cols;
    Vec_T* elements;
}
Matrix_T;

NpStatus_T tryNewMatrix(Matrix_T* maybeMatrix, size_t rows, size_t cols);

NpStatus_T tryNewIdentityMatrix(Matrix_T* maybeMatrix, size_t n);

FLOATING* indexMatrix(const Matrix_T* mat, size_t i, size_t j)
{
    return &(mat->elements->elements[i * mat->cols + j].floating);
}

void inplaceRowSwap(Matrix_T* mat, size_t r1, size_t r2);

void inplaceRowScale(Matrix_T* mat, size_t row, FLOATING scalar);

void inplaceRowAdd(Matrix_T* mat, size_t r1, size_t r2);

void inplaceScaledRowAdd(Matrix_T* mat, size_t r1, size_t r2, FLOATING scalar);

NpStatus_T tryMultiplyMatrix(Matrix_T* product, const Matrix_T* left, const Matrix_T* right);

NpStatus_T tryAugmentMatrix(Matrix_T* augment, const Matrix_T* left, const Matrix_T* right);

NpStatus_T subset(Matrix_T* slice, const Matrix_T* mat, size_t r1, size_t c1, size_t r2, size_t c2);

NpStatus_T tryInplaceInvert(Matrix_T* mat);

#endif