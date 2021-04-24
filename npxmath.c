#include <math.h>
#include <stdint.h>

#define _N 1.8446744073709551616e+19
#define is_zero(A) (A[0] == 0 && A[1] == 0)

/* r[2] = a[2] / b[2] with (log2(a) - log2(b)) < 64 -> max approx 1 */
int div2f(uint64_t *r, const uint64_t *a, const uint64_t *b)
{
    if (is_zero(b))
        return 0;

    const long double N = _N;
    long double f1, f2;

    f1 = N * a[0] + a[1];
    f2 = N * b[0] + b[1];

    if (!(log2(f1) - log2(f2) < 64.0))
        return 0;

    f1 /= f2;

    r[0] = 0;
    r[1] = (uint64_t)(f1);
    return 1;
}
