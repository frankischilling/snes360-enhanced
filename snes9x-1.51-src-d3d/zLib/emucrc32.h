/* emucrc32.h - Portable CRC32 wrapper header for Snes360 / FCE360 style projects */

#ifndef EMUCRC32_H
#define EMUCRC32_H

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

#endif /* EMUCRC32_H */

