#include <mpmath.h>

/* r[S] = a[S] - b[S] */
uint8_t sub_mpi(mpx_t *m)
{
    register uint64_t v2;
    register uint64_t v3;

    if (cmp_mpi(m->b, m->a) == GT)
    {
        v2 = (uint64_t)m->b;
        v3 = (uint64_t)m->a;
        *m->rem = OF;
    }
    else
    {
        v2 = (uint64_t)m->a;
        v3 = (uint64_t)m->b;
        *m->rem = OK;
    }

    mpr_t tr = {0};
    register uint64_t v1 = (uint64_t)tr;

    asm volatile("movl %3, %%ecx\n\t"
                 "clc\n\t"
                 "1:\n\t"
                 "movq -8(%1, %%rcx, 8), %%rax\n\t"
                 "sbbq -8(%2, %%rcx, 8), %%rax\n\t"
                 "movq %%rax, -8(%0, %%rcx, 8)\n\t"
                 "loop 1b"
                 :
                 : "r"(v1), "r"(v2), "r"(v3), "n"((uint8_t)PRCS)
                 : "rax", "rcx", "cc");

    cp_mpi(m->r, tr);
    return OK;
}

/* (r|rem) r[S] = a[S] / b */
uint8_t div_mpi(mpx_t *m)
{
    if (*m->b == 0)
        return ZD;
    else if (*m->b == 1)
    {
        cp_mpi(m->r, m->a);
        *m->rem = 0;
        return OK;
    }
    else if (lsr_mpi(m->a) == PRCS)
    {
        *m->rem = 0;
        return OK;
    }

    mpr_t tr = {0};

    register uint64_t v1 = (uint64_t)tr;
    register uint64_t v2 = (uint64_t)m->rem;
    register uint64_t v3 = (uint64_t)m->a;
    register uint64_t v4 = (uint64_t)m->b;

    asm volatile("xorl %%edx, %%edx\n\t"
                 "xorl %%r11d, %%r11d\n\t"
                 "movl %4, %%ecx\n\t"
                 "1:\n\t"
                 "movq (%2, %%r11, 8), %%rax\n\t"
                 "divq (%3)\n\t"
                 "movq %%rax, (%0, %%r11, 8)\n\t"
                 "incb %%r11b\n\t"
                 "loop 1b\n\t"
                 "movq %%rdx, (%1)"
                 :
                 : "r"(v1), "r"(v2), "r"(v3), "r"(v4), "n"((uint8_t)PRCS)
                 : "rax", "rdx", "rcx", "r11", "cc");

    cp_mpi(m->r, tr);
    return OK;
}

/* (r|rem) a[S] = a[S] / 2^N */
uint8_t div_mpi_2n(mpi_t a, mpi_t rem, const uint16_t N)
{
    null_mpi(rem);
    int8_t ai = lsr_mpi(a);

    if (N == 0 || ai == PRCS)
        return OK;

    uint64_t t1[2] = {N, 64};
    div64(t1);

    if (t1[0] > PRCS - ai)
    {
        cp_mpi(rem, a);
        null_mpi(a);
        return OK;
    }

    cp_arr(&rem[PRCS - t1[0]], &a[PRCS - t1[0]], t1[0]);
    sr_mpi(a, t1[0]);

    if (t1[1] == 0)
        return OK;

    uint64_t r1 = 0, d1 = x2n[t1[1]];
    mpx_t m0 = {a, &r1, a, &d1};
    div_mpi(&m0);
    rem[PRCS - t1[0] - 1] = r1;
    return OK;
}

/* (r|rem) r[S] = a[S] / b[S] */
uint8_t div_mpi_n(mpx_t *m)
{
    int8_t cmp = lsr_mpi(m->b);

    if (cmp == PRCS)
        return ZD;
    else if (cmp == PRCS - 1)
    {
        null_mpi(m->rem);
        mpx_t m1 = {m->r, &m->rem[PRCS - 1], m->a, &m->b[PRCS - 1]};
        div_mpi(&m1);
        return OK;
    }

    cmp = cmp_mpi(m->a, m->b);

    if (cmp == LT)
    {
        null_mpi(m->r);
        cp_mpi(m->rem, m->a);
        return OK;
    }
    else if (cmp == EQ)
    {
        null_mpi(m->r);
        m->r[PRCS - 1] = 1;
        null_mpi(m->rem);
        return OK;
    }

    int16_t n = bsr_mpi(m->a) - bsr_mpi(m->b);
    int16_t bs = 0;
    uint64_t r1 = 0, d1 = 2;

    mpr_t mp[4] = {{0}, {0}, {0}, {0}};    // a|rem, q, bn, n
    mpx_t m1 = {mp[0], &r1, mp[0], mp[2]}; // a  -= bn
    mpx_t m2 = {mp[1], &r1, mp[1], mp[3]}; // q  += n
    mpx_t m3 = {mp[2], &r1, mp[2], &d1};   // bn /= 2
    mpx_t m4 = {mp[3], &r1, mp[3], &d1};   // n  /= 2

    cp_mpi(mp[0], m->a);
    cp_mpi(mp[2], m->b);
    mul_mpi_2n(mp[2], n);

    mp[3][PRCS - 1] = 1;
    mul_mpi_2n(mp[3], n);

    for (; n >= 0; n--)
    {
        if (cmp_mpi(mp[2], mp[0]) != GT)
        {
            sub_mpi(&m1);
            add_mpi(&m2);
            bs = bsr_mpi(mp[2]) - bsr_mpi(mp[0]);

            if (bs > 1)
            {
                d1 = x2n[bs];
                n -= bs - 1;
            }
        }

        div_mpi(&m3);
        div_mpi(&m4);
        d1 = 2;
    }

    cp_mpi(m->rem, mp[0]);
    cp_mpi(m->r, mp[1]);
    return OK;
}
