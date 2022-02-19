/* Unsigned 64 bit Arbitrary Precision Arithmetic coded by Wojciech Lawren
                  ai...a2 a1 a0 -> i = PRCS - 1

  Copyright (C) 2021, Wojciech Lawren, All rights reserved.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <asmath.h>
#include <const.h>
#include <stdint.h>
/* BMI2 CMOV AVX AVX2 */
#define NULL ((void *)0)
/* precision 8 particular */
#define null_mpi(r)                                                                                                    \
    asm volatile("vpxor %%xmm8, %%xmm8, %%xmm8\n\t"                                                                    \
                 "vmovdqu %%ymm8, (%0)\n\t"                                                                            \
                 "vmovdqu %%ymm8, 32(%0)\n\t"                                                                          \
                 :                                                                                                     \
                 : "r"(r)                                                                                              \
                 : "ymm8")

#define cp_mpi(r, a)                                                                                                   \
    asm volatile("vmovdqu (%1), %%ymm8\n\t"                                                                            \
                 "vmovdqu 32(%1), %%ymm9\n\t"                                                                          \
                 "vmovdqu %%ymm8, (%0)\n\t"                                                                            \
                 "vmovdqu %%ymm9, 32(%0)\n\t"                                                                          \
                 :                                                                                                     \
                 : "r"(r), "r"(a)                                                                                      \
                 : "ymm8", "ymm9")

#define PRCS 8                               // precision (1 : 255)
#define MODULUS 0, 0, 0, 0, -1, -1, -1, -977 // secp256k1
/* --- end 8 --- */

typedef uint64_t mpr_t[PRCS];
typedef uint64_t mpp_t[PRCS + 1];
typedef uint64_t *mpi_t;

typedef struct
{
    mpi_t r;
    mpi_t rem;
    mpi_t a;
    mpi_t b;
} mpx_t;

#define ZD 0
#define OF 1
#define OK 2

#define EQ 0
#define LT 1
#define GT 2

/* r[S] = a[S] + b[S] */
uint8_t add_mpi(mpx_t *m);
/* r[S] = a[S] - b[S] */
uint8_t sub_mpi(mpx_t *m);
/* r[S] = a[S] * b */
uint8_t mul_mpi(mpx_t *m);
/* (r|rem) r[S] = a[S] / b */
uint8_t div_mpi(mpx_t *m);
/* r[S] = a[S] * b[S] */
uint8_t mul_mpi_n(mpx_t *m);
/* (r|rem) r[S] = a[S] / b[S] */
uint8_t div_mpi_n(mpx_t *m); // 4FD
/* a[S] = a[S] * 2^N */
uint8_t mul_mpi_2n(mpi_t a, const uint16_t N);
/* (r|rem) a[S] = a[S] / 2^N */
uint8_t div_mpi_2n(mpi_t a, mpi_t rem, const uint16_t N);
/* a = a mod MODULUS */
void mod_mpi(mpi_t a);
// utils
/* i = a[i] > 0 */
uint8_t lsr_mpi(const mpi_t a);
/* v = a[S] bits */
uint16_t bsr_mpi(const mpi_t a);
/* v = a[S] > b[S] */
uint8_t cmp_mpi(const mpi_t a, const mpi_t b);
/* a = a<-S */
void sl_mpi(mpi_t a, const uint8_t S);
/* a = a->S */
void sr_mpi(mpi_t a, const uint8_t S);
/* r[S] = a[S] */
void cp_arr(mpi_t r, const mpi_t a, const uint8_t S);
