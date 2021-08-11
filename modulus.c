#include <mpmath.h>

static const mpr_t _mx = {MODULUS};

/* a = a mod MODULUS */
void mod_mpi(mpi_t a)
{
    mpi_t mx = (mpi_t)&_mx;
    uint8_t cmp = cmp_mpi(a, mx);

    if (cmp == LT)
        return;
    else if (cmp == EQ)
    {
        null_mpi(a);
        return;
    }

    mpr_t tr = {0};
    mpx_t m1 = {tr, a, a, mx};
    div_mpi_n(&m1);
}
