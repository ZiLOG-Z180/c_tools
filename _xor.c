#include <stdint.h>

/* r[S] = a[S] ^ b[S] */
/* (a[S] ^ b[S]) ^ b[S] = a[S] */
void _xor(uint8_t *r, const uint8_t *a, const uint8_t *b, const uint16_t S)
{
    for (uint16_t i = 0; i < S; i++)
        r[i] = a[i] ^ b[i];
}

// AVX
void _xor128(uint8_t *r, const uint8_t *a, const uint8_t *b)
{
    asm volatile("vmovdqu (%1), %%xmm8\n\t"
                 "vpxor (%2), %%xmm8, %%xmm8\n\t"
                 "vmovdqu %%xmm8, (%0)"
                 :
                 : "r"(r), "r"(a), "r"(b)
                 : "ymm8");
}

// AVX2
void _xor256(uint8_t *r, const uint8_t *a, const uint8_t *b)
{
    asm volatile("vmovdqu (%1), %%ymm8\n\t"
                 "vpxor (%2), %%ymm8, %%ymm8\n\t"
                 "vmovdqu %%ymm8, (%0)"
                 :
                 : "r"(r), "r"(a), "r"(b)
                 : "ymm8");
}
