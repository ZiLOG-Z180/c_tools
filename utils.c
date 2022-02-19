#include <mpmath.h>
/* AVX AVX2 */
/* i = a[i] > 0 */
uint8_t lsr_mpi(const mpi_t a)
{
    uint8_t i = 0;

    for (; i < PRCS; i++)
        if (a[i] > 0)
            break;

    return i;
}

/* v = a[S] bits */
uint16_t bsr_mpi(const mpi_t a)
{
    uint8_t i = lsr_mpi(a);
    if (i == PRCS)
        return 0;
    return (bsr(a[i]) + ((PRCS - i - 1) * 64));
}

/* v = a[S] > b[S] */
uint8_t cmp_mpi(const mpi_t a, const mpi_t b)
{
    for (uint8_t i = 0; i < PRCS; i++)
    {
        if (a[i] > b[i])
            return GT;
        else if (a[i] < b[i])
            return LT;
    }
    return EQ;
}

/* a = a<-S */
void sl_mpi(mpi_t a, const uint8_t S)
{
    if (S == 0)
        return;
    mpr_t tr = {0};
    if (S < PRCS)
        cp_arr(tr, &a[S], PRCS - S);
    cp_mpi(a, tr);
}

/* a = a->S */
void sr_mpi(mpi_t a, const uint8_t S)
{
    if (S == 0)
        return;
    mpr_t tr = {0};
    if (S < PRCS)
        cp_arr(&tr[S], a, PRCS - S);
    cp_mpi(a, tr);
}

/* r[S] = a[S] */
void cp_arr(mpi_t r, const mpi_t a, const uint8_t S)
{
    for (uint8_t i = 0; i < S; i++)
        r[i] = a[i];
}
