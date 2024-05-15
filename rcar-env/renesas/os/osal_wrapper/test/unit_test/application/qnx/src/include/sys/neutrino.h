#ifndef _STUB_NEUTRINO_H
#define _STUB_NEUTRINO_H      1

#include <sys/siginfo.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>

enum _THREAD_STATE {
        STATE_INTR = 16,
};

// qnx710/target/qnx7/usr/include/sys/neutrino.h
#define _NTO_TCTL_IO_PRIV                               1
#define _NTO_TCTL_IO                                    14
#define _NTO_TCTL_IO_LEVEL                              19
#define _NTO_TCTL_IO_LEVEL_INHERIT              0x40000000u
#define _NTO_TCTL_RUNMASK                               4
#define _NTO_TCTL_RUNMASK_GET_AND_SET                   6
#define _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT           10
#define _NTO_INTR_FLAGS_TRK_MSK                         0x08u
#define _NTO_TIMEOUT_INTR                      (1<<STATE_INTR)

extern int InterruptAttachEvent(int intr, const struct sigevent* event, unsigned flags);
extern int InterruptDetach(int id);
extern int InterruptMask(int intr, int id);
extern int InterruptUnmask(int intr, int id);
extern int InterruptWait( int flags, const uint64_t * timeout );
extern int ThreadCtl(int cmd, void * data);
extern int TimerTimeout(clockid_t id, int flags, const struct sigevent * notify, const uint64_t * ntime, uint64_t * otime );
extern uint64_t ClockCycles( void );

#endif  /* _STUB_NEUTRINO_H */
