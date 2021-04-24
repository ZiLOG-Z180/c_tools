#include <stdint.h>
/* AVX AVX2 */

/* r[4] = reverse a[4] */
void swap256(uint64_t *r, const uint64_t *a)
{
    asm volatile("vpermq $27, (%1), %%ymm8\n\t"
                 "vmovdqu %%ymm8, (%0)"
                 :
                 : "r"(r), "r"(a)
                 : "ymm8");
}

/* r[4] = (a[4] == b[4]) */
void cmpeq256(uint64_t *r, const uint64_t *a, const uint64_t *b)
{
    asm volatile("vmovdqu (%1), %%ymm8\n\t"
                 "vpcmpeqq (%2), %%ymm8, %%ymm8\n\t"
                 "vmovdqu %%ymm8, (%0)"
                 :
                 : "r"(r), "r"(a), "r"(b)
                 : "ymm8");
}

/* r[4] = (a[4] > b[4]) */
void cmpgt256(uint64_t *r, const uint64_t *a, const uint64_t *b)
{
    asm volatile("vmovdqu (%1), %%ymm8\n\t"
                 "vpcmpgtq (%2), %%ymm8, %%ymm8\n\t"
                 "vmovdqu %%ymm8, (%0)"
                 :
                 : "r"(r), "r"(a), "r"(b)
                 : "ymm8");
}

/* r[4] = a[4] + b[4] carry-less */
void add256(uint64_t *r, const uint64_t *a, const uint64_t *b)
{
    asm volatile("vmovdqu (%1), %%ymm8\n\t"
                 "vpaddq (%2), %%ymm8, %%ymm8\n\t"
                 "vmovdqu %%ymm8, (%0)"
                 :
                 : "r"(r), "r"(a), "r"(b)
                 : "ymm8");
}

/* r[4] = a[4] - b[4] carry-less */
void sub256(uint64_t *r, const uint64_t *a, const uint64_t *b)
{
    asm volatile("vmovdqu (%1), %%ymm8\n\t"
                 "vpsubq (%2), %%ymm8, %%ymm8\n\t"
                 "vmovdqu %%ymm8, (%0)"
                 :
                 : "r"(r), "r"(a), "r"(b)
                 : "ymm8");
}
