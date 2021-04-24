#include <stdint.h>
/* ADX BMI2 */
#define __att __attribute__((noinline, nonnull))
/* v = a + b */
uint64_t add(const uint64_t a, const uint64_t b, uint8_t *c) __att;
/* v = a + b + CF */
uint64_t adc(const uint64_t a, const uint64_t b, uint8_t *c) __att;
/* r[2] = a * b */
void mul(uint64_t *r, const uint64_t a, const uint64_t b) __att;
/* v = a - b */
uint64_t sub(const uint64_t a, const uint64_t b, uint8_t *c) __att;
/* (q|r) r[2] = r0 / r1 */
int div64(uint64_t *r) __att;
/* (q|r) r[2] = r[2] / a; with q < 2^64 */
int div128(uint64_t *r, const uint64_t a) __att;
