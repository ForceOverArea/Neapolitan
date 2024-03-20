#include "harness.h"
#include "matrix.h"

TEST(matrix_inverts_correctly)
{
    Matrix_T mat2x2;
    ASSERT(tryNewMatrix(&mat2x2, 2, 2))

    double* a11 = indexMatrix(&mat2x2, 0, 0);
    double* a12 = indexMatrix(&mat2x2, 0, 1);
    double* a21 = indexMatrix(&mat2x2, 1, 0);
    double* a22 = indexMatrix(&mat2x2, 1, 1);

    *a11 = -1.0;
    *a12 = +1.5;
    *a21 = +1.0;
    *a22 = -1.0;

    ASSERT(tryInplaceInvert(&mat2x2))

    ASSERT_EQ(2.0, *a11, %lf)
    ASSERT_EQ(3.0, *a12, %lf)
    ASSERT_EQ(2.0, *a21, %lf)
    ASSERT_EQ(2.0, *a22, %lf)

    END_TEST
}

int main()
{
    matrix_inverts_correctly();
    return 0;
}