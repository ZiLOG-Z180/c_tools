/* CPU CRC32 coded by Wojciech Lawren

  Copyright (C) 2021, Wojciech Lawren, All rights reserved.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdint.h>
/* SSE4.2 */
/* poly: 0x11EDC6F41 */
uint32_t _crc32c(const uint32_t _crc, const uint8_t *s, const uint64_t N)
{
    register uint64_t crc = (uint64_t)~_crc;
    register uint64_t nq = N / 8;
    register uint8_t nr = N % 8;

    if (nq)
        asm volatile("movq %2, %%rcx\n\t"
                     "xorl %%r11d, %%r11d\n\t"
                     "1:\n\t"
                     "crc32q (%1, %%r11, 8), %0\n\t"
                     "incq %%r11\n\t"
                     "loop 1b"
                     : "+r"(crc)
                     : "r"(s), "r"(nq)
                     : "rcx", "r11");

    if (nr)
        asm volatile("movzbq %2, %%rcx\n\t"
                     "xorl %%r11d, %%r11d\n\t"
                     "1:\n\t"
                     "crc32b (%1, %%r11), %0\n\t"
                     "incb %%r11b\n\t"
                     "loop 1b"
                     : "+r"(crc)
                     : "r"(s + nq * 8), "r"(nr)
                     : "rcx", "r11");

    return ~(uint32_t)crc;
}
