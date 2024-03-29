#include "matrix.h"

NpStatus_E tryNewMatrix(Matrix_S* maybeMatrix, size_t rows, size_t cols)
{
    if (0 == rows || 0 == cols)
    {
        return INVALID_SIZE_GIVEN;
    }

    size_t numElems = rows * cols;
    maybeMatrix->elements = newVec(numElems);
    if (NULL == maybeMatrix->elements)
    {
        return OUT_OF_MEMORY;
    }

    maybeMatrix->rows = rows;
    maybeMatrix->cols = cols;

    for (size_t i = 0; i < numElems; i++)
    {
        // For matrices, zero all elements on creation:
        maybeMatrix->elements->elements[i].floating = 0;
    }

    return OK;
}

NpStatus_E tryNewIdentityMatrix(Matrix_S* maybeMatrix, size_t n)
{
    if (0 == n)
    {
        return INVALID_SIZE_GIVEN;
    }

    size_t numElems = n * n;
    maybeMatrix->elements = newVec(numElems);
    if (NULL == maybeMatrix->elements)
    {
        return OUT_OF_MEMORY;
    }

    maybeMatrix->rows = n;
    maybeMatrix->cols = n;

    for (size_t i = 0; i < numElems; i++)
    {
        // For matrices, zero all elements on creation:
        maybeMatrix->elements->elements[i].floating = 0;
    }

    return OK;
}

void inplaceRowSwap(Matrix_S* mat, size_t r1, size_t r2)
{
    FLOATING tmp;
    for (size_t j = 0; j < mat->cols; j++)
    {
        tmp = *(indexMatrix(mat, r1, j));
        *(indexMatrix(mat, r1, j)) = *(indexMatrix(mat, r2, j));
        *(indexMatrix(mat, r2, j)) = tmp;
    }
}

void inplaceRowScale(Matrix_S* mat, size_t row, FLOATING scalar)
{
    for (size_t j = 0; j < mat->cols; j++)
    {
        *(indexMatrix(mat, row, j)) *= scalar;
    }
}

void inplaceRowAdd(Matrix_S* mat, size_t r1, size_t r2)
{
    for (size_t j = 0; j < mat->cols; j++)
    {
        FLOATING addend = *(indexMatrix(mat, r2, j));
        *(indexMatrix(mat, r1, j)) += addend;
    }
}

void inplaceScaledRowAdd(Matrix_S* mat, size_t r1, size_t r2, FLOATING scalar)
{
    for (size_t j = 0; j < mat->cols; j++)
    {
        FLOATING addend = *(indexMatrix(mat, r2, j));
        *(indexMatrix(mat, r1, j)) += (addend * scalar);
    }
}

NpStatus_E tryMultiplyMatrix(Matrix_S* product, const Matrix_S* left, const Matrix_S* right)
{
    if (left->cols != right->rows)
    {
        return MATRIX_DIMENSION_MISMATCH;
    } 

    size_t n = left->cols;
    PROPOGATE_ERROR(
        tryNewMatrix(product, left->rows, right->cols)
    )

    for (size_t i = 0; i < left->rows; i++)
    {
        for (size_t j = 0; j < right->cols; j++)
        {
            for (size_t x = 0; x < n; x++)
            {
                FLOATING loperend = *(indexMatrix(left,  i, x));
                FLOATING roperend = *(indexMatrix(right, x, j));
                *(indexMatrix(product, i, j)) = loperend * roperend;
            }
        }
    }

    return OK;
}

NpStatus_E tryAugmentMatrix(Matrix_S* augment, const Matrix_S* left, const Matrix_S* right)
{
    if (left->rows != right->rows)
    {
        return MATRIX_DIMENSION_MISMATCH;
    }

    size_t n = left->rows;

    PROPOGATE_ERROR(
        tryNewMatrix(augment, n, left->cols + right->cols)
    )

    for (size_t i = 0; i < augment->rows; i++)
    {
        for (size_t j = 0; j < augment->cols; j++)
        {
            if (j < left->cols) // take value from the left matrix
            {
                *(indexMatrix(augment, i, j)) = *(indexMatrix(left, i, j));
            }
            else // take value from the right matrix
            {
                *(indexMatrix(augment, i, j)) = *(indexMatrix(right, i, j - left->cols));
            }
        }
    }

    return OK;
}

NpStatus_E subset(Matrix_S* slice, const Matrix_S* mat, size_t r1, size_t c1, size_t r2, size_t c2)
{
    PROPOGATE_ERROR(
        tryNewMatrix(slice, r2 - r1 + 1, c2 - c1 + 1)
    )

    for (size_t i = r1; i <= r2; i++)
    {
        for (size_t j = c1; j <= c2; j++)
        {
            *(indexMatrix(slice, i - r1, j - c1)) = *(indexMatrix(mat, i, j));
        }
    }

    return OK;
}

static inline NpStatus_E tryInplaceInvert2(Matrix_S* mat)
{
    FLOATING a11 = *(indexMatrix(mat, 0, 0));
    FLOATING a12 = *(indexMatrix(mat, 0, 1));
    FLOATING a21 = *(indexMatrix(mat, 1, 0));
    FLOATING a22 = *(indexMatrix(mat, 1, 1));

    FLOATING det = a11 * a22 - a12 * a21;

    if (0 == det)
    {
        return ZERO_DETERMINANT;
    }

    *(indexMatrix(mat, 0, 0)) = +a22 / det;
    *(indexMatrix(mat, 1, 0)) = -a21 / det;
    *(indexMatrix(mat, 0, 1)) = -a12 / det;
    *(indexMatrix(mat, 1, 1)) = +a11 / det;

    return OK;
}

static inline NpStatus_E tryInplaceInvert3(Matrix_S* mat)
{
    FLOATING a11 = *(indexMatrix(mat, 0, 0));
    FLOATING a12 = *(indexMatrix(mat, 0, 1));
    FLOATING a13 = *(indexMatrix(mat, 0, 2));
    FLOATING a21 = *(indexMatrix(mat, 1, 0));
    FLOATING a22 = *(indexMatrix(mat, 1, 1));
    FLOATING a23 = *(indexMatrix(mat, 1, 2));
    FLOATING a31 = *(indexMatrix(mat, 2, 0));
    FLOATING a32 = *(indexMatrix(mat, 2, 1));
    FLOATING a33 = *(indexMatrix(mat, 2, 2));

    FLOATING det = a11 * a22 * a33 + 
                 a21 * a32 * a13 + 
                 a31 * a12 * a23 - 
                 a11 * a32 * a23 - 
                 a31 * a22 * a13 - 
                 a21 * a12 * a33;

    if (0 == det)
    {
        return ZERO_DETERMINANT;
    }
    
    *(indexMatrix(mat, 0, 0)) = (a22 * a33 - a23) * a32 / det;
    *(indexMatrix(mat, 1, 0)) = (a23 * a31 - a21) * a33 / det;
    *(indexMatrix(mat, 2, 0)) = (a21 * a32 - a22) * a31 / det;
    *(indexMatrix(mat, 0, 1)) = (a13 * a32 - a12) * a33 / det;
    *(indexMatrix(mat, 1, 1)) = (a11 * a33 - a13) * a31 / det;
    *(indexMatrix(mat, 2, 1)) = (a12 * a31 - a11) * a32 / det;
    *(indexMatrix(mat, 0, 2)) = (a12 * a23 - a13) * a22 / det;
    *(indexMatrix(mat, 1, 2)) = (a13 * a21 - a11) * a23 / det;
    *(indexMatrix(mat, 2, 2)) = (a11 * a22 - a12) * a21 / det;

    return OK;
}

static inline NpStatus_E tryInplaceInvert4(Matrix_S* mat)
{
    FLOATING a11 = *(indexMatrix(mat, 0, 0));
    FLOATING a12 = *(indexMatrix(mat, 1, 0));
    FLOATING a13 = *(indexMatrix(mat, 2, 0));
    FLOATING a14 = *(indexMatrix(mat, 3, 0));
    FLOATING a21 = *(indexMatrix(mat, 0, 1));
    FLOATING a22 = *(indexMatrix(mat, 1, 1));
    FLOATING a23 = *(indexMatrix(mat, 2, 1));
    FLOATING a24 = *(indexMatrix(mat, 3, 1));
    FLOATING a31 = *(indexMatrix(mat, 0, 2));
    FLOATING a32 = *(indexMatrix(mat, 1, 2));
    FLOATING a33 = *(indexMatrix(mat, 2, 2));
    FLOATING a34 = *(indexMatrix(mat, 3, 2));
    FLOATING a41 = *(indexMatrix(mat, 0, 3));
    FLOATING a42 = *(indexMatrix(mat, 1, 3));
    FLOATING a43 = *(indexMatrix(mat, 2, 3));
    FLOATING a44 = *(indexMatrix(mat, 3, 3));

    FLOATING det = a11 * a22 * a33 * a44 + a11 * a23 * a34 * a42 + a11 * a24 * a32 * a43 +
                 a12 * a21 * a34 * a43 + a12 * a23 * a31 * a44 + a12 * a24 * a33 * a41 + 
                 a13 * a21 * a32 * a44 + a13 * a22 * a34 * a41 + a13 * a24 * a31 * a42 + 
                 a14 * a21 * a33 * a42 + a14 * a22 * a34 * a43 + a14 * a23 * a32 * a41 -
                 a11 * a22 * a34 * a43 - a11 * a23 * a32 * a44 - a11 * a24 * a33 * a42 -
                 a12 * a21 * a33 * a44 - a12 * a23 * a34 * a41 - a12 * a24 * a31 * a43 -
                 a13 * a21 * a34 * a42 - a13 * a22 * a31 * a44 - a13 * a24 * a32 * a41 -
                 a14 * a21 * a32 * a43 - a14 * a22 * a33 * a41 - a14 * a23 * a31 * a42;

    if (0 == det)
    {
        return ZERO_DETERMINANT;
    }

    *(indexMatrix(mat, 0, 0)) = a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43 - a22 * a34 * a43 - a23 * a32 * a44 - a24 * a33 * a42 / det;
    *(indexMatrix(mat, 0, 1)) = a12 * a34 * a43 + a13 * a32 * a44 + a14 * a33 * a42 - a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43 / det;
    *(indexMatrix(mat, 0, 2)) = a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43 - a12 * a24 * a43 - a13 * a22 * a44 - a14 * a23 * a42 / det;
    *(indexMatrix(mat, 0, 3)) = a12 * a24 * a33 + a13 * a22 * a34 + a14 * a23 * a32 - a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33 / det;
    *(indexMatrix(mat, 1, 0)) = a21 * a34 * a43 + a23 * a31 * a44 + a24 * a33 * a41 - a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43 / det;
    *(indexMatrix(mat, 1, 1)) = a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43 - a11 * a34 * a43 - a13 * a31 * a44 - a14 * a33 * a41 / det;
    *(indexMatrix(mat, 1, 2)) = a11 * a24 * a43 + a13 * a21 * a44 + a14 * a23 * a41 - a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43 / det;
    *(indexMatrix(mat, 1, 3)) = a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33 - a11 * a24 * a33 - a13 * a21 * a34 - a14 * a23 * a31 / det;
    *(indexMatrix(mat, 2, 0)) = a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42 - a21 * a34 * a42 - a22 * a31 * a44 - a24 * a32 * a41 / det;
    *(indexMatrix(mat, 2, 1)) = a11 * a34 * a42 + a12 * a31 * a44 + a14 * a32 * a41 - a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42 / det;
    *(indexMatrix(mat, 2, 2)) = a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42 - a11 * a24 * a42 - a12 * a21 * a44 - a14 * a22 * a41 / det;
    *(indexMatrix(mat, 2, 3)) = a11 * a24 * a32 + a12 * a21 * a34 + a14 * a22 * a31 - a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32 / det;
    *(indexMatrix(mat, 3, 0)) = a21 * a33 * a42 + a22 * a31 * a43 + a23 * a32 * a41 - a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42 / det;
    *(indexMatrix(mat, 3, 1)) = a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42 - a11 * a33 * a42 - a12 * a31 * a43 - a13 * a32 * a41 / det;
    *(indexMatrix(mat, 3, 2)) = a11 * a23 * a42 + a12 * a21 * a43 + a13 * a22 * a41 - a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42 / det;
    *(indexMatrix(mat, 3, 3)) = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a11 * a23 * a32 - a12 * a21 * a33 - a13 * a22 * a31 / det;

    return OK;
}

static inline NpStatus_E tryInplaceInvertN(Matrix_S* mat)
{
    // Assertion that rows == cols has already 
    // happened prior to this function call.
    size_t n = mat->cols;
    Matrix_S inv; 

    PROPOGATE_ERROR(
        tryNewIdentityMatrix(&inv, n)
    )

    FLOATING scalar;
    for (size_t j = 0; j < n; j++)
    {
        for (size_t i = 0; i < n; i++)
        {
            if (i == j)
            {
                continue;
            }
            else
            {
                if (*(indexMatrix(mat, i, j)) == 0)
                {
                    return UNDEFINED_VALUE;
                }
                scalar = *(indexMatrix(mat, i, j)) / (*(indexMatrix(mat, j, j)));
                inplaceScaledRowAdd(mat, i, j, -scalar);
                inplaceScaledRowAdd(&inv, i, j, -scalar);
            }
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        scalar = 1 / (*(indexMatrix(mat, i, i)));
        // We only need to do this operation for the inverse copy here
        // since the changes to one row will not affect changes to
        // the others.
        inplaceRowScale(&inv, i, scalar);
    }

    free(mat->elements);    // Prevent memory leak
    *mat = inv;             // Assign `inv` to `mat` contents

    return OK;
}

NpStatus_E tryInplaceInvert(Matrix_S* mat)
{
    NpStatus_E retVal = ZERO_DETERMINANT;
    if (mat->cols != mat->rows)
    {
        return MATRIX_DIMENSION_MISMATCH;
    }

    switch(mat->cols)
    {
    case 0:
        break;
    case 1:
        if (mat->elements->elements[0].floating != 0)
        {
            mat->elements->elements[0].floating = (1 / mat->elements->elements[0].floating);
            retVal = OK;
        }
        break;
    case 2:
        retVal = tryInplaceInvert2(mat);
        break;
    case 3:
        retVal = tryInplaceInvert3(mat);
        break;
    case 4:
        retVal = tryInplaceInvert4(mat);
        break;
    default:
        retVal = tryInplaceInvertN(mat);
        break;
    }

    return retVal;
}
