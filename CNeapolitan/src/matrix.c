#include "matrix.h"
#include "neapsymbols.h"

int newMatrix(Matrix_T* mat, size_t rows, size_t cols)
{
    DoubleVec_T* data = newVecWithCapacity(rows * cols);
    if (NULL == data)
    {
        return ERR;
    }
    
    *mat = (Matrix_T){ rows, cols, data }; 
    
    for (size_t i = 0; i < rows * cols; i++)
    {
        pushValToDoubleVec(mat->data, 0.0);
    }

    return OK;
}

int newIdentityMatrix(Matrix_T* mat, size_t n)
{
    DoubleVec_T* data = newVecWithCapacity(n * n);
    if (NULL == data)
    {
        return ERR;
    }
    
    *mat = (Matrix_T){ n, n, data }; 
    
    for (size_t i = 0; i < n * n; i++)
    {
        // If element is on the diagonal
        if (n * n / i == n && 
            n * n % i == 0) 
        {
            pushValToDoubleVec(mat->data, 1.0);
            continue;
        }

        pushValToDoubleVec(mat->data, 0.0);
    }

    return OK;
}

void inplaceRowSwap(Matrix_T* mat, size_t r1, size_t r2)
{
    double tmp;
    for (size_t j = 0; j < mat->cols; j++)
    {
        tmp = *(indexMatrix(mat, r1, j));
        *(indexMatrix(mat, r1, j)) = *(indexMatrix(mat, r2, j));
        *(indexMatrix(mat, r2, j)) = tmp;
    }
}

void inplaceRowScale(Matrix_T* mat, size_t row, double scalar)
{
    for (size_t j = 0; j < mat->cols; j++)
    {
        *(indexMatrix(mat, row, j)) *= scalar;
    }
}

void inplaceRowAdd(Matrix_T* mat, size_t r1, size_t r2)
{
    for (size_t j = 0; j < mat->cols; j++)
    {
        double addend = *(indexMatrix(mat, r2, j));
        *(indexMatrix(mat, r1, j)) += addend;
    }
}

void inplaceScaledRowAdd(Matrix_T* mat, size_t r1, size_t r2, double scalar)
{
    for (size_t j = 0; j < mat->cols; j++)
    {
        double addend = *(indexMatrix(mat, r2, j));
        *(indexMatrix(mat, r1, j)) += (addend * scalar);
    }
}

int mulMatrix(Matrix_T* product, const Matrix_T* left, const Matrix_T* right)
{
    if (left->cols != right->rows)
    {
        return ERR;
    } 

    size_t n = left->cols;
    if (!newMatrix(product, left->rows, right->cols))
    {
        return ERR;
    }

    for (size_t i = 0; i < left->rows; i++)
    {
        for (size_t j = 0; j < right->cols; j++)
        {
            for (size_t x = 0; x < n; x++)
            {
                double loperend = *(indexMatrix(left,  i, x));
                double roperend = *(indexMatrix(right, x, j));
                *(indexMatrix(product, i, j)) = loperend * roperend;
            }
        }
    }

    return OK;
}

int augmentMatrix(Matrix_T* augment, const Matrix_T* left, const Matrix_T* right)
{
    if (left->rows != right->rows)
    {
        return ERR;
    }

    size_t n = left->rows;
    if (!newMatrix(augment, n, left->cols + right->cols))
    {
        return ERR;
    }

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

int subset(Matrix_T* slice, const Matrix_T* mat, size_t r1, size_t c1, size_t r2, size_t c2)
{
    if (!newMatrix(slice, r2 - r1 + 1, c2 - c1 + 1))
    {
        return ERR;
    }

    for (size_t i = r1; i <= r2; i++)
    {
        for (size_t j = c1; j <= c2; j++)
        {
            *(indexMatrix(slice, i - r1, j - c1)) = *(indexMatrix(mat, i, j));
        }
    }

    return OK;
}

static inline int tryInplaceInvert2(Matrix_T* mat)
{
    double a11 = *(indexMatrix(mat, 0, 0));
    double a12 = *(indexMatrix(mat, 0, 1));
    double a21 = *(indexMatrix(mat, 1, 0));
    double a22 = *(indexMatrix(mat, 1, 1));

    double det = a11 * a22 - a12 * a21;

    if (0 == det)
    {
        return ERR;
    }

    *(indexMatrix(mat, 0, 0)) = a22 / det;
    *(indexMatrix(mat, 0, 1)) = a21 / det;
    *(indexMatrix(mat, 1, 0)) = a12 / det;
    *(indexMatrix(mat, 1, 1)) = a11 / det;

    return OK;
}

static inline int tryInplaceInvert3(Matrix_T* mat)
{
    double a11 = *(indexMatrix(mat, 0, 0));
    double a12 = *(indexMatrix(mat, 0, 1));
    double a13 = *(indexMatrix(mat, 0, 2));
    double a21 = *(indexMatrix(mat, 1, 0));
    double a22 = *(indexMatrix(mat, 1, 1));
    double a23 = *(indexMatrix(mat, 1, 2));
    double a31 = *(indexMatrix(mat, 2, 0));
    double a32 = *(indexMatrix(mat, 2, 1));
    double a33 = *(indexMatrix(mat, 2, 2));

    double det = a11 * a22 * a33 + 
                 a21 * a32 * a13 + 
                 a31 * a12 * a23 - 
                 a11 * a32 * a23 - 
                 a31 * a22 * a13 - 
                 a21 * a12 * a33;

    if (0 == det)
    {
        return ERR;
    }
    
    // FIXME: Double check that this works as expected:
    *(indexMatrix(mat, 0, 0)) = (a22 * a33 - a23) * a32 / det;
    *(indexMatrix(mat, 0, 1)) = (a23 * a31 - a21) * a33 / det;
    *(indexMatrix(mat, 0, 2)) = (a21 * a32 - a22) * a31 / det;
    *(indexMatrix(mat, 1, 0)) = (a13 * a32 - a12) * a33 / det;
    *(indexMatrix(mat, 1, 1)) = (a11 * a33 - a13) * a31 / det;
    *(indexMatrix(mat, 1, 2)) = (a12 * a31 - a11) * a32 / det;
    *(indexMatrix(mat, 2, 0)) = (a12 * a23 - a13) * a22 / det;
    *(indexMatrix(mat, 2, 1)) = (a13 * a21 - a11) * a23 / det;
    *(indexMatrix(mat, 2, 2)) = (a11 * a22 - a12) * a21 / det;

    return OK;
}

static inline int tryInplaceInvert4(Matrix_T* mat)
{
    double a11 = *(indexMatrix(mat, 0, 0));
    double a12 = *(indexMatrix(mat, 1, 0));
    double a13 = *(indexMatrix(mat, 2, 0));
    double a14 = *(indexMatrix(mat, 3, 0));
    double a21 = *(indexMatrix(mat, 0, 1));
    double a22 = *(indexMatrix(mat, 1, 1));
    double a23 = *(indexMatrix(mat, 2, 1));
    double a24 = *(indexMatrix(mat, 3, 1));
    double a31 = *(indexMatrix(mat, 0, 2));
    double a32 = *(indexMatrix(mat, 1, 2));
    double a33 = *(indexMatrix(mat, 2, 2));
    double a34 = *(indexMatrix(mat, 3, 2));
    double a41 = *(indexMatrix(mat, 0, 3));
    double a42 = *(indexMatrix(mat, 1, 3));
    double a43 = *(indexMatrix(mat, 2, 3));
    double a44 = *(indexMatrix(mat, 3, 3));

    double det = a11 * a22 * a33 * a44 + a11 * a23 * a34 * a42 + a11 * a24 * a32 * a43 +
                 a12 * a21 * a34 * a43 + a12 * a23 * a31 * a44 + a12 * a24 * a33 * a41 + 
                 a13 * a21 * a32 * a44 + a13 * a22 * a34 * a41 + a13 * a24 * a31 * a42 + 
                 a14 * a21 * a33 * a42 + a14 * a22 * a34 * a43 + a14 * a23 * a32 * a41 -
                 a11 * a22 * a34 * a43 - a11 * a23 * a32 * a44 - a11 * a24 * a33 * a42 -
                 a12 * a21 * a33 * a44 - a12 * a23 * a34 * a41 - a12 * a24 * a31 * a43 -
                 a13 * a21 * a34 * a42 - a13 * a22 * a31 * a44 - a13 * a24 * a32 * a41 -
                 a14 * a21 * a32 * a43 - a14 * a22 * a33 * a41 - a14 * a23 * a31 * a42;

    if (0 == det)
    {
        return ERR;
    }

    *(indexMatrix(mat, 0, 0)) = a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43 - a22 * a34 * a43 - a23 * a32 * a44 - a24 * a33 * a42 / det;
    *(indexMatrix(mat, 1, 0)) = a12 * a34 * a43 + a13 * a32 * a44 + a14 * a33 * a42 - a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43 / det;
    *(indexMatrix(mat, 2, 0)) = a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43 - a12 * a24 * a43 - a13 * a22 * a44 - a14 * a23 * a42 / det;
    *(indexMatrix(mat, 3, 0)) = a12 * a24 * a33 + a13 * a22 * a34 + a14 * a23 * a32 - a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33 / det;
    *(indexMatrix(mat, 0, 1)) = a21 * a34 * a43 + a23 * a31 * a44 + a24 * a33 * a41 - a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43 / det;
    *(indexMatrix(mat, 1, 1)) = a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43 - a11 * a34 * a43 - a13 * a31 * a44 - a14 * a33 * a41 / det;
    *(indexMatrix(mat, 2, 1)) = a11 * a24 * a43 + a13 * a21 * a44 + a14 * a23 * a41 - a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43 / det;
    *(indexMatrix(mat, 3, 1)) = a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33 - a11 * a24 * a33 - a13 * a21 * a34 - a14 * a23 * a31 / det;
    *(indexMatrix(mat, 0, 2)) = a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42 - a21 * a34 * a42 - a22 * a31 * a44 - a24 * a32 * a41 / det;
    *(indexMatrix(mat, 1, 2)) = a11 * a34 * a42 + a12 * a31 * a44 + a14 * a32 * a41 - a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42 / det;
    *(indexMatrix(mat, 2, 2)) = a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42 - a11 * a24 * a42 - a12 * a21 * a44 - a14 * a22 * a41 / det;
    *(indexMatrix(mat, 3, 2)) = a11 * a24 * a32 + a12 * a21 * a34 + a14 * a22 * a31 - a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32 / det;
    *(indexMatrix(mat, 0, 3)) = a21 * a33 * a42 + a22 * a31 * a43 + a23 * a32 * a41 - a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42 / det;
    *(indexMatrix(mat, 1, 3)) = a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42 - a11 * a33 * a42 - a12 * a31 * a43 - a13 * a32 * a41 / det;
    *(indexMatrix(mat, 2, 3)) = a11 * a23 * a42 + a12 * a21 * a43 + a13 * a22 * a41 - a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42 / det;
    *(indexMatrix(mat, 3, 3)) = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a11 * a23 * a32 - a12 * a21 * a33 - a13 * a22 * a31 / det;

    return OK;
}

static inline int tryInplaceInvertN(Matrix_T* mat)
{
    // Assertion that rows == cols has already 
    // happened prior to this function call.
    size_t n = mat->cols;
    Matrix_T* inv = newIdentityMatrix(n);
    double scalar;

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
                    return ERR;
                }
                scalar = *(indexMatrix(mat, i, j)) / (*(indexMatrix(mat, j, j)));
                inplaceScaledRowAdd(mat, i, j, -scalar);
                inplaceScaledRowAdd(inv, i, j, -scalar);
            }
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        scalar = 1 / (*(indexMatrix(mat, i, i)));
        // We only need to do this operation for the inverse copy here
        // since the changes to one row will not affect changes to
        // the others.
        inplaceRowScale(inv, i, scalar);
    }

    free(mat);
    mat = inv;

    return OK;
}

int tryInplaceInvert(Matrix_T* mat)
{
    int retVal = ERR;
    if (mat->cols != mat->rows)
    {
        return ERR;
    }

    switch(mat->cols)
    {
    case 0:
        break;
    case 1:
        if (mat->data->data[0].fVal != 0)
        {
            mat->data->data[0].fVal = (1 / mat->data->data[0].fVal);
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
