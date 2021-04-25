#include <mpmath.h>

/* r[S+1] = a[S] * b */
int mul_arr(mpx_t *m)
{
    for (int8_t i = 0; i <= m->S; i++)
        m->r[i] = 0;

    if (m->S == 0)
        return 0;
    else if (*m->b == 0)
        return 1;
    else if (*m->b == 1)
    {
        for (int8_t i = 0; i < m->S; i++)
            m->r[i + 1] = m->a[i];
        return 1;
    }

    register uint64_t v1 = (uint64_t)m->r;
    register uint64_t v2 = (uint64_t)m->a;
    register uint64_t v3 asm("rdx") = *m->b;
    register uint8_t i = m->S;

    asm volatile("movzbq %3, %%rcx\n\t"
                 "1:\n\t"
                 "mulxq -8(%1, %%rcx, 8), %%r11, %%r10\n\t"
                 "addq %%r11, (%0, %%rcx, 8)\n\t"
                 "adcq %%r10, -8(%0, %%rcx, 8)\n\t"
                 "loop 1b"
                 :
                 : "r"(v1), "r"(v2), "r"(v3), "r"(i)
                 : "rcx", "r10", "r11", "cc");
    return 1;
}
