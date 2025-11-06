/* emucrc32.c - Portable CRC32 wrapper for Snes360 / FCE360 style projects
 *
 * Primary path: use zlib's crc32() so we don't duplicate logic.
 * Fallback path (EMUCRC32_STANDALONE): small self-contained CRC32 if zlib isn't built.
 *
 * 
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Public API expected by older emulator codebases */
unsigned long emu_crc32(unsigned long crc, const unsigned char *buf, unsigned int len);
unsigned long calc_crc32(const void *data, size_t length);

#ifdef __cplusplus
}
#endif

/* ---------- Standalone CRC32 (define EMUCRC32_STANDALONE) - checked first to override zlib ---------- */
#if defined(EMUCRC32_STANDALONE)

  /* Include zlib.h just for type definitions (uLong, Bytef, uInt) */
  #ifdef __cplusplus
  extern "C" {
  #endif
  #include "zlib.h"
  #ifdef __cplusplus
  }
  #endif

  /* Polynomial 0xEDB88320, reflected CRC32, standard initial/final XORs */
  static unsigned long emu_crc32_slow(unsigned long crc, const unsigned char *buf, unsigned int len)
  {
      unsigned int i;
      int b;
      unsigned long mask;
      
      crc = crc ^ 0xFFFFFFFFu;
      for (i = 0; i < len; ++i) {
          crc ^= (unsigned long)buf[i];
          for (b = 0; b < 8; ++b) {
              mask = -(crc & 1u);
              crc = (crc >> 1) ^ (0xEDB88320u & mask);
          }
      }
      return crc ^ 0xFFFFFFFFu;
  }

  unsigned long emu_crc32(unsigned long crc, const unsigned char *buf, unsigned int len)
  {
      /* This variant treats 'crc' as the incoming state (same as zlib).
         For simple one-shot: pass crc=0. */
      unsigned long init;
      unsigned long out;
      unsigned int i;
      int b;
      unsigned long mask;
      
      if (!buf || len == 0) return crc;
      /* Reconstruct the internal state (undo final XOR) if caller passed a zlib-style state.
         If you always call with crc=0, you can ignore this nuance. */
      init = crc ? (crc ^ 0xFFFFFFFFu) : 0xFFFFFFFFu;
      /* Process */
      out = init;
      for (i = 0; i < len; ++i) {
          out ^= (unsigned long)buf[i];
          for (b = 0; b < 8; ++b) {
              mask = -(out & 1u);
              out = (out >> 1) ^ (0xEDB88320u & mask);
          }
      }
      return out ^ 0xFFFFFFFFu;
  }

  unsigned long calc_crc32(const void *data, size_t length)
  {
      return emu_crc32(0, (const unsigned char*)data, (unsigned int)length);
  }

  /* Provide crc32() function for unzip code that expects zlib's crc32() */
  uLong ZEXPORT crc32(uLong crc, const Bytef *buf, uInt len)
  {
      return (uLong)emu_crc32((unsigned long)crc, (const unsigned char*)buf, (unsigned int)len);
  }

/* ---------- Preferred: use zlib if available ---------- */
#elif defined(ZLIB) || defined(HAVE_ZLIB) || defined(ZLIB_VERSION)
  #ifdef __cplusplus
  extern "C" {
  #endif
  #include "zlib.h"
  #ifdef __cplusplus
  }
  #endif

  /* Map to zlib's implementation */
  unsigned long emu_crc32(unsigned long crc, const unsigned char *buf, unsigned int len)
  {
      return (unsigned long)crc32((uLong)crc, (const Bytef*)buf, (uInt)len);
  }

  unsigned long calc_crc32(const void *data, size_t length)
  {
      return (unsigned long)crc32(0L, (const Bytef*)data, (uInt)length);
  }

#else
  /* If neither zlib nor standalone is enabled, fail clearly at compile time. */
  #error "emucrc32.c: zlib not found. Define HAVE_ZLIB or include zlib.h, or build with EMUCRC32_STANDALONE."
#endif

