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
/* return CRC32 of string s */
/* CPU instruction uses different inputs to the CRC32 algorithm (known as the polynomial) which is apparently more
   robust and is used in networks, filesystems, that sort of thing. It gives different results to the "standard"
   polynomial, typically used in compression. */
/* https://github.com/robn/crc32-bench */
uint32_t crc32(const uint32_t _crc, const void *s, const uint32_t l)
{
    register uint32_t crc = _crc;
    register uint32_t lq = l / 4;
    register uint8_t lr = l % 4;

    if (lq)
    {
        asm volatile("movl %2, %%ecx\n\t"
                     "xorl %%r11d, %%r11d\n\t"
                     "1:\n\t"
                     "crc32l (%1, %%r11, 4), %0\n\t"
                     "incb %%r11b\n\t"
                     "loop 1b"
                     : "+r"(crc)
                     : "r"(s), "r"(lq)
                     : "rcx", "r11");
    }

    if (lr)
    {
        asm volatile("movzbq %2, %%rcx\n\t"
                     "xorl %%r11d, %%r11d\n\t"
                     "1:\n\t"
                     "crc32b (%1, %%r11), %0\n\t"
                     "incb %%r11b\n\t"
                     "loop 1b"
                     : "+r"(crc)
                     : "r"(s + lq * 4), "r"(lr)
                     : "rcx", "r11");
    }

    return crc;
}
