#include <asmath.h>
/* ADX BMI2 */

/* v = a + b */
uint64_t add(const uint64_t a, const uint64_t b, uint8_t *c)
{
    register uint64_t v = a;

    asm volatile("addq %1, %0\n\t"
                 "setc (%2)"
                 : "+r"(v)
                 : "r"(b), "r"(c)
                 : "cc");
    return v;
}

/* v = a + b + CF */
uint64_t adcx(const uint64_t a, const uint64_t b, uint8_t *c)
{
    register uint64_t v = a;

    asm volatile("adcxq %1, %0\n\t"
                 "setc (%2)"
                 : "+r"(v)
                 : "r"(b), "r"(c)
                 : "cc");
    return v;
}

/* r[2] = a * b */
void mulx(uint64_t *r, const uint64_t a, const uint64_t b)
{
    register uint64_t v1 = a;
    register uint64_t v2 asm("rdx") = b;

    asm volatile("mulxq %0, %1, %0" : "+r"(v1), "+r"(v2));
    r[0] = v1;
    r[1] = v2;
}

/* v = a - b */
uint64_t sub(const uint64_t a, const uint64_t b, uint8_t *c)
{
    register uint64_t v = a;

    asm volatile("subq %1, %0\n\t"
                 "setc (%2)"
                 : "+r"(v)
                 : "r"(b), "r"(c)
                 : "cc");
    return v;
}

/* v = a - b - CF */
uint64_t sbb(const uint64_t a, const uint64_t b, uint8_t *c)
{
    register uint64_t v = a;

    asm volatile("sbbq %1, %0\n\t"
                 "setc (%2)"
                 : "+r"(v)
                 : "r"(b), "r"(c)
                 : "cc");
    return v;
}

/* (q|r) r[2] = r0 / r1 */
uint8_t div64(uint64_t *r)
{
    if (!(r[1] > 0))
        return 0;

    register uint64_t v1 asm("rax") = r[0];
    register uint64_t v2 asm("rdx") = 0;
    register uint64_t v3 = r[1];

    asm volatile("divq %2" : "+r"(v1), "+r"(v2) : "r"(v3));
    r[0] = v1;
    r[1] = v2;

    return 1;
}

/* (q|r) r[2] = r[2] / a; with q < 2^64 */
uint8_t div128(uint64_t *r, const uint64_t a)
{
    if (!(a > r[0]))
        return 0;

    register uint64_t v1 asm("rax") = r[1];
    register uint64_t v2 asm("rdx") = r[0];
    register uint64_t v3 = a;

    asm volatile("divq %2" : "+r"(v1), "+r"(v2) : "r"(v3));
    r[0] = v1;
    r[1] = v2;

    return 1;
}

/* v = a bits */
uint8_t bsr(const uint64_t a)
{
    register uint64_t v = 0;

    asm volatile("bsrq %1, %0\n\t"
                 "je 1f\n\t"
                 "incq %0\n\t"
                 "1:"
                 : "+r"(v)
                 : "r"(a)
                 : "cc");
    return (uint8_t)v;
}
