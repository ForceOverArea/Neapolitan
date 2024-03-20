#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdbool.h>
#include "vec.h" // Also includes stdlib.h

struct Matrix_S
{
    size_t rows, cols;
    Vec_T* elements;
};

typedef struct Matrix_S Matrix_T;

bool tryNewMatrix(Matrix_T* maybeMatrix, size_t rows, size_t cols);

bool tryNewIdentityMatrix(Matrix_T* maybeMatrix, size_t n);

double* indexMatrix(const Matrix_T* mat, size_t i, size_t j)
{
    return &(mat->elements->elements[i * mat->cols + j].floating);
}

void inplaceRowSwap(Matrix_T* mat, size_t r1, size_t r2);

void inplaceRowScale(Matrix_T* mat, size_t row, double scalar);

void inplaceRowAdd(Matrix_T* mat, size_t r1, size_t r2);

void inplaceScaledRowAdd(Matrix_T* mat, size_t r1, size_t r2, double scalar);

bool tryMultiplyMatrix(Matrix_T* product, const Matrix_T* left, const Matrix_T* right);

bool tryAugmentMatrix(Matrix_T* augment, const Matrix_T* left, const Matrix_T* right);

bool subset(Matrix_T* slice, const Matrix_T* mat, size_t r1, size_t c1, size_t r2, size_t c2);

bool tryInplaceInvert(Matrix_T* mat);

#endif