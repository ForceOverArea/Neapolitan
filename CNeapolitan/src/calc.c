#include "calc.h"

/// @brief Sets up a for loop and variable `sum_<wrt>` 
/// to hold the sum of the series produced by the for loop.
///
/// This is not intended for sharing with other files.
#define INLINE_INTEGRAL(start, end, wrt)    \
double sum_ ## wrt = 0;                     \
for (double wrt = (start); wrt <= (end); wrt += DX)

double derivativeAt(Function1D_T f, double x, double* valAt)
{
    double y = f(x);
    double yp = f(x + DX);

    if (NULL != valAt)
    {
        (*valAt) = y;
    }

    return (yp - y) / DX;
}

double integralOn(Function1D_T f, double start, double end)
{
    INLINE_INTEGRAL(start, end, x)
    {
        sum_x += (f(x) * DX);
    }
    return sum_x;
}

double laplaceTransformAt(Function1D_T f, double s)
{
    INLINE_INTEGRAL(0, DISCRETE_INF, t)
    {
        sum_t += f(t) * exp(-s * t);
    }
    return sum_t;
}