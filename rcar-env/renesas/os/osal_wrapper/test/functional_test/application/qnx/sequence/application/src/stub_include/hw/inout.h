#ifndef _STUB_INOUT_H
#define _STUB_INOUT_H      1

#include <sys/types.h>
#include <stdint.h>

extern uint16_t in16( uintptr_t port );
extern uint32_t in32( uintptr_t port );
extern uint8_t in8( uintptr_t port );
extern void out16( uintptr_t port, uint16_t val );
extern void out32( uintptr_t port, uint32_t val );
extern void out8( uintptr_t port, uint8_t val );

#endif  /* _STUB_INOUT_H */
