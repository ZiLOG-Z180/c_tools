#include <stdint.h>
/* BMI2 */
#define __att __attribute__((noinline, nonnull))

typedef struct
{
    uint64_t *r;
    uint64_t *rem;
    uint64_t *a;
    uint64_t *b;
    uint8_t S;
} mpx_t;

/* (r|rem) r[S] = a[S] / b */
int div_arr(mpx_t *m) __att;
/* r[S+1] = a[S] * b */
int mul_arr(mpx_t *m) __att;
