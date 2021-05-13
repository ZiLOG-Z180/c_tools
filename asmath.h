#include <stdint.h>
/* ADX BMI2 */
/* v = a + b */
uint64_t add(const uint64_t a, const uint64_t b, uint8_t *c);
/* v = a + b + CF */
uint64_t adcx(const uint64_t a, const uint64_t b, uint8_t *c);
/* r[2] = a * b */
void mulx(uint64_t *r, const uint64_t a, const uint64_t b);
/* v = a - b */
uint64_t sub(const uint64_t a, const uint64_t b, uint8_t *c);
/* v = a - b - CF */
uint64_t sbb(const uint64_t a, const uint64_t b, uint8_t *c);
/* (q|r) r[2] = r0 / r1 */
uint8_t div64(uint64_t *r);
/* (q|r) r[2] = r[2] / a; with q < 2^64 */
uint8_t div128(uint64_t *r, const uint64_t a);
/* v = a bits */
uint8_t bsr(const uint64_t a);
