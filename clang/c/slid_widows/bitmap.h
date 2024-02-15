#ifndef _BITMASK_H_
#define _BITMASK_H_
#include "stdint.h"

/* only support x86 arch */

#define BITS_PER_LONG       (sizeof(long) * 8)
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)

#define BITS_PER_BYTE    8
#define BITS_PER_TYPE(type)     (sizeof(type) * BITS_PER_BYTE)
#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)   DIV_ROUND_UP(nr, BITS_PER_TYPE(long))
#define DECLARE_BITMAP(name, bits)      \
        unsigned long name[BITS_TO_LONGS(bits)]

static inline unsigned int bitmap_ffs(unsigned long word)
{
    asm("rep; bsf %1,%0"
        : "=r" (word)
        : "rm" (word));
    return (unsigned int)word;
}

static inline unsigned int bitmap_ffu(unsigned long *bitmap,
        unsigned int nbits, unsigned int start)
{
    unsigned long tmp = 0;
    unsigned long mask = 0;
    unsigned int ret = 0;

    if (start >= nbits) {
        return nbits;
    }

    tmp = bitmap[start / BITS_PER_LONG] ^ (~0ULL);
    mask = (~0ULL) << (start & (BITS_PER_LONG - 1));
    tmp &= mask;
    start = start & ~(BITS_PER_LONG - 1);

    /* look up block by block */
    while (!tmp) {
        start += BITS_PER_LONG;
        if (start >= nbits) {
            return nbits;
        }
        tmp = bitmap[start / BITS_PER_LONG] ^ (~0ULL);
    }

    ret = start + bitmap_ffs(tmp);
    if (ret > nbits) {
        ret = nbits;
    }
    return ret;
}


static inline void bitmap_or(unsigned long *dst, const unsigned long *bitmap1,
                const unsigned long *bitmap2, int bits)
{
    int k;
    int nr = BITS_TO_LONGS(bits);

    for (k = 0; k < nr; k++)
        dst[k] = bitmap1[k] | bitmap2[k];
}


static inline int bitmap_and(unsigned long *dst, const unsigned long *bitmap1,
                const unsigned long *bitmap2, int bits)
{
    int k;
    int nr = BITS_TO_LONGS(bits);
    unsigned long result = 0;

    for (k = 0; k < nr; k++)
        result |= (dst[k] = bitmap1[k] & bitmap2[k]);
    return result != 0;
}

static inline int bitmap_get(const unsigned long *addr, int nr)
{
    uint8_t v;

    asm("btl %2,%1; setc %0" : "=qm" (v) : "m" (*addr), "Ir" (nr));
    return v;
}

static inline void bitmap_set(unsigned long *addr, int nr)
{
    asm("btsl %1,%0" : "+m" (*addr) : "Ir" (nr));
}

static inline void bitmap_clear(unsigned long *addr, int nr)
{
    asm volatile("btrl %1,%0" : "+m" (*addr) : "Ir" (nr));
}
#endif
