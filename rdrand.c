#include <rdrand.h>

uint64_t rand64()
{
    register uint64_t v asm("rax");
    RD(RAND);
    return v;
}

uint32_t rand32()
{
    register uint32_t v asm("eax");
    RD(RAND);
    return v;
}

uint16_t rand16()
{
    register uint16_t v asm("ax");
    RD(RAND);
    return v;
}

uint64_t seed64()
{
    register uint64_t v asm("rax");
    RD(SEED);
    return v;
}

uint32_t seed32()
{
    register uint32_t v asm("eax");
    RD(SEED);
    return v;
}

uint16_t seed16()
{
    register uint16_t v asm("ax");
    RD(SEED);
    return v;
}

uint8_t rand_arr(uint64_t *r, const uint16_t S)
{
    for (uint16_t i = 0; i < S; i++)
        if (!(r[i] = rand64()))
            return 0;
    return 1;
}

uint8_t _rand_bytes(uint8_t *r, const uint16_t S)
{
    if (S % 8)
        return 0;
    return rand_arr((uint64_t *)r, S / 8);
}
