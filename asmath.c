#include <asmath.h>
/* ADX BMI2 */

/* v = a + b */
uint64_t add(const uint64_t a, const uint64_t b, uint8_t *c)
{
    register uint64_t v asm("rax");

    asm volatile("addq %2, %1\n\t"
                 "setc (%3)\n\t"
                 "movq %1, %0"
                 : "=r"(v)
                 : "r"(a), "r"(b), "r"(c)
                 : "cc");
    return v;
}

/* v = a + b + CF */
uint64_t adc(const uint64_t a, const uint64_t b, uint8_t *c)
{
    register uint64_t v asm("rax");

    asm volatile("adcxq %2, %1\n\t"
                 "setc (%3)\n\t"
                 "movq %1, %0"
                 : "=r"(v)
                 : "r"(a), "r"(b), "r"(c)
                 : "cc");
    return v;
}

/* r[2] = a * b */
void mul(uint64_t *r, const uint64_t a, const uint64_t b)
{
    register uint64_t v1 asm("rsi") = a;
    register uint64_t v2 asm("rdx") = b;

    asm volatile("mulxq %0, %1, %0" : "+r"(v1), "+r"(v2));
    r[0] = v1;
    r[1] = v2;
}

/* v = a - b */
uint64_t sub(const uint64_t a, const uint64_t b, uint8_t *c)
{
    register uint64_t v asm("rax");

    asm volatile("subq %2, %1\n\t"
                 "setc (%3)\n\t"
                 "movq %1, %0"
                 : "=r"(v)
                 : "r"(a), "r"(b), "r"(c)
                 : "cc");
    return v;
}

/* (q|r) r[2] = r0 / r1 */
int div64(uint64_t *r)
{
    if (!(r[1] > 0))
        return 0;

    register uint64_t v1 asm("rax") = r[0];
    register uint64_t v2 asm("rdx") = 0;
    register uint64_t v3 asm("rcx") = r[1];

    asm volatile("divq %2" : "+r"(v1), "+r"(v2) : "r"(v3));
    r[0] = v1;
    r[1] = v2;

    return 1;
}

/* (q|r) r[2] = r[2] / a; with q < 2^64 */
int div128(uint64_t *r, const uint64_t a)
{
    if (!(a > r[0]))
        return 0;

    register uint64_t v1 asm("rax") = r[1];
    register uint64_t v2 asm("rdx") = r[0];
    register uint64_t v3 asm("rsi") = a;

    asm volatile("divq %2" : "+r"(v1), "+r"(v2) : "r"(v3));
    r[0] = v1;
    r[1] = v2;

    return 1;
}
