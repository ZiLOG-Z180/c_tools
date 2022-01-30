#include <mpmath.h>
/* BMI2 CMOV */
/* r[S] = a[S] + b[S] */
uint8_t add_mpi(mpx_t *m)
{
    mpr_t tr = {0};
    register uint16_t of = 1;

    register uint64_t v1 = (uint64_t)tr;
    register uint64_t v2 = (uint64_t)m->a;
    register uint64_t v3 = (uint64_t)m->b;

    asm volatile("movl %4, %%ecx\n\t"
                 "clc\n\t"
                 "1:\n\t"
                 "movq -8(%2, %%rcx, 8), %%rax\n\t"
                 "adcq -8(%3, %%rcx, 8), %%rax\n\t"
                 "movq %%rax, -8(%1, %%rcx, 8)\n\t"
                 "loop 1b\n\t"
                 "cmovcw %%cx, %0"
                 : "+r"(of)
                 : "r"(v1), "r"(v2), "r"(v3), "n"((uint8_t)PRCS)
                 : "rax", "rcx", "cc");

    cp_mpi(m->r, tr);
    if (of == 0)
        return OF;
    return OK;
}

/* r[S] = a[S] * b */
uint8_t mul_mpi(mpx_t *m)
{
    if (*m->b == 0 || lsr_mpi(m->a) == PRCS)
    {
        null_mpi(m->r);
        return OK;
    }
    else if (*m->b == 1)
    {
        cp_mpi(m->r, m->a);
        return OK;
    }

    mpp_t tr = {0};

    register uint64_t v1 = (uint64_t)tr;
    register uint64_t v2 = (uint64_t)m->a;
    register uint64_t v3 = (uint64_t)m->b;

    asm volatile("movq (%2), %%rdx\n\t"
                 "movl %3, %%ecx\n\t"
                 "1:\n\t"
                 "mulxq -8(%1, %%rcx, 8), %%r11, %%rax\n\t"
                 "addq %%r11, (%0, %%rcx, 8)\n\t"
                 "adcq %%rax, -8(%0, %%rcx, 8)\n\t"
                 "loop 1b\n\t"
                 :
                 : "r"(v1), "r"(v2), "r"(v3), "n"((uint8_t)PRCS)
                 : "rax", "rdx", "rcx", "r11", "cc");

    cp_mpi(m->r, &tr[1]);
    if (tr[0] != 0)
        return OF;
    return OK;
}

/* a[S] = a[S] * 2^N */
uint8_t mul_mpi_2n(mpi_t a, const uint16_t N)
{
    if (N == 0)
        return OK;

    int8_t ai = lsr_mpi(a);

    if (ai == PRCS)
        return OK;

    uint64_t t1[2] = {N, 64};
    div64(t1);

    if (t1[0] > ai)
        return OF;

    sl_mpi(a, t1[0]);

    if (t1[1] == 0)
        return OK;

    uint64_t x1 = x2n[t1[1]];
    mpx_t m0 = {a, NULL, a, &x1};
    mul_mpi(&m0);
    return OK;
}

/* r[S] = a[S] * b[S] */
uint8_t mul_mpi_n(mpx_t *m)
{
    int8_t bi = lsr_mpi(m->b);
    int8_t ai = lsr_mpi(m->a);

    if (bi == PRCS || ai == PRCS)
    {
        null_mpi(m->r);
        return OK;
    }
    else if (bi == PRCS - 1)
    {
        mpx_t m1 = {m->r, NULL, m->a, &m->b[PRCS - 1]};
        mul_mpi(&m1);
        return OK;
    }
    else if (ai == PRCS - 1 && m->a[ai] == 1)
    {
        cp_mpi(m->r, m->b);
        return OK;
    }

    int8_t i = PRCS - bi - 1;

    if (i > ai)
        return OF;

    mpr_t mp[3] = {{0}, {0}, {0}};          // r, tp, a<-S
    mpx_t m1 = {mp[1], NULL, mp[2], NULL};  // tp = a<-S * b[S]
    mpx_t m2 = {mp[0], NULL, mp[0], mp[1]}; // r += tp

    cp_mpi(mp[2], m->a);
    sl_mpi(mp[2], i);

    for (; i >= 0; i--)
    {
        m1.b = &m->b[bi++];
        mul_mpi(&m1);
        add_mpi(&m2);
        sr_mpi(mp[2], 1);
    }

    cp_mpi(m->r, mp[0]);
    return OK;
}
