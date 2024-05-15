/*
 * CRC32 using the polynomial from IEEE-802.3
 *
 * Authors: Lasse Collin <lasse.collin@tukaani.org>
 *          Igor Pavlov
 *
 * This file has been put into the public domain.
 * You can do whatever you want with this file.
 */

/*
 * This is not the fastest implementation, but it is pretty compact.
 * The fastest versions of xz_crc32() on modern CPUs without hardware
 * accelerated CRC instruction are 3-5 times as fast as this version,
 * but they are bigger and use more memory for the lookup table.
 */

#include <stdint.h>
#include <string.h>

static uint32_t xz_crc32_table[256];

void xz_crc32_init(void)
{
    const uint32_t poly = 0xEDB88320u;

    uint32_t i;
    uint32_t j;
    uint32_t r;

    for(i = 0; i < 256u; ++i) {
        r = i;
        for(j = 0; j < 8u; ++j) {
            r = (r >> 1) ^ (poly & ~((r & 1u) - 1u));
        }
        xz_crc32_table[i] = r;
    }
}

uint32_t xz_crc32(const uint8_t *buf, size_t size, uint32_t crc)
{
    crc = ~crc;

    while(size != 0u) {
        crc = xz_crc32_table[*buf++ ^ (crc & 0xFFu)] ^ (crc >> 8);
        --size;
    }

    return ~crc;
}
