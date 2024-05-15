#ifndef _STUB_SYSPAGE_H
#define _STUB_SYSPAGE_H      1

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

typedef struct {
    _Uint64t        cycles_per_sec;
} syspage_entry_info;

extern void *qtime;

#define SYSPAGE_ENTRY( dummy ) ((syspage_entry_info *)(dummy))

#endif  /* _STUB_SYSPAGE_H */
