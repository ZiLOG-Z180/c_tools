#include <mpmath.h>

/* (r|rem) r[S] = a[S] / b */
int div_arr(mpx_t *m)
{
    for (int8_t i = 0; i < m->S; i++)
        m->r[i] = 0;
    *m->rem = 0;

    if (*m->b == 0 || m->S == 0)
        return 0;
    else if (*m->b == 1)
    {
        for (int8_t i = 0; i < m->S; i++)
            m->r[i] = m->a[i];
        return 1;
    }

    register uint64_t v1 = (uint64_t)m->r;
    register uint64_t v2 = (uint64_t)m->rem;
    register uint64_t v3 = (uint64_t)m->a;
    register uint64_t v4 = (uint64_t)m->b;
    register uint8_t i = m->S;

    asm volatile("xorq %%rdx, %%rdx\n\t"
                 "xorq %%r11, %%r11\n\t"
                 "movzbq %4, %%rcx\n\t"
                 "1:\n\t"
                 "movq (%2, %%r11, 8), %%rax\n\t"
                 "divq (%3)\n\t"
                 "movq %%rax, (%0, %%r11, 8)\n\t"
                 "incq %%r11\n\t"
                 "loop 1b\n\t"
                 "movq %%rdx, (%1)"
                 :
                 : "r"(v1), "r"(v2), "r"(v3), "r"(v4), "r"(i)
                 : "rax", "rdx", "rcx", "r11", "cc");
    return 1;
}
