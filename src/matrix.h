#ifndef MATRIX_H_
#define MATRIX_H_

#include "vec.h" // also includes stdlib.h, stdbool.h, np_status.h

typedef struct Matrix_S
{
    size_t rows, cols;
    Vec_S* elements;
}
Matrix_S;

NpStatus_E tryNewMatrix(Matrix_S* maybeMatrix, size_t rows, size_t cols);

NpStatus_E tryNewIdentityMatrix(Matrix_S* maybeMatrix, size_t n);

FLOATING* indexMatrix(const Matrix_S* mat, size_t i, size_t j)
{
    return &(mat->elements->elements[i * mat->cols + j].floating);
}

void inplaceRowSwap(Matrix_S* mat, size_t r1, size_t r2);

void inplaceRowScale(Matrix_S* mat, size_t row, FLOATING scalar);

void inplaceRowAdd(Matrix_S* mat, size_t r1, size_t r2);

void inplaceScaledRowAdd(Matrix_S* mat, size_t r1, size_t r2, FLOATING scalar);

NpStatus_E tryMultiplyMatrix(Matrix_S* product, const Matrix_S* left, const Matrix_S* right);

NpStatus_E tryAugmentMatrix(Matrix_S* augment, const Matrix_S* left, const Matrix_S* right);

NpStatus_E subset(Matrix_S* slice, const Matrix_S* mat, size_t r1, size_t c1, size_t r2, size_t c2);

NpStatus_E tryInplaceInvert(Matrix_S* mat);

#endif