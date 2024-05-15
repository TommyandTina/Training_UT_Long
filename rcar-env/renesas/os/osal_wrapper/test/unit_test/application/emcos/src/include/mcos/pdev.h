#include <stdint.h>
#include <pthread.h>  //?
#include <mcos/uring.h>

#define PDEV_DCTL_GROUP (uint32_t)(((uint32_t)'H' << 8UL) | (uint32_t)'W')
#define PDEV_DCTL_CMDGROUP_MASK 0xFUL
#define PDEV_DCTL_CMDGROUP_SHIFT 14UL
#define PDEV_DCTL_CMD_MASK 0xFUL
#define PDEV_DCTL_CMD_SHIFT 8UL
#define PDEV_DCTL_MINIARG_MASK 0xFFUL

#define PDEV_DCTL_MEMCMD 1UL
#define PDEV_DCTL_INTERRUPT 2UL
#define PDEV_DCTL_CLOCK 3UL
#define PDEV_DCTL_IOMMU 4UL

#define PDEVCMD(c,e,arg)      _DCTL(PDEV_DCTL_GROUP,\
        ((uint32_t)(c) << PDEV_DCTL_CMDGROUP_SHIFT) | ((uint32_t)(e) << PDEV_DCTL_CMD_SHIFT) | (uint32_t)(arg))
#define PDEVCMDIN(c,e,arg)    _DCTLIN(PDEV_DCTL_GROUP,\
        ((uint32_t)(c) << PDEV_DCTL_CMDGROUP_SHIFT) | ((uint32_t)(e) << PDEV_DCTL_CMD_SHIFT) | (uint32_t)(arg))
#define PDEVCMDOUT(c,e,arg)   ((int)_DCTLOUT(PDEV_DCTL_GROUP,\
        ((uint32_t)(c) << PDEV_DCTL_CMDGROUP_SHIFT) | ((uint32_t)(e) << PDEV_DCTL_CMD_SHIFT) | (uint32_t)(arg)))
#define PDEVCMDINOUT(c,e,arg) ((int)_DCTLINOUT(PDEV_DCTL_GROUP,\
        ((uint32_t)(c) << PDEV_DCTL_CMDGROUP_SHIFT) | ((uint32_t)(e) << PDEV_DCTL_CMD_SHIFT) | (uint32_t)(arg)))

typedef int (*memrange_cmd_func_t)(uint_fast8_t code, uint64_t from, uint64_t to,
        uint64_t len);

typedef struct pdev_itr_config
{
    /** Thread identifier to receive interrupt message */
    pthread_t tid;
    /** Communication channel to receive interrupt message */
    uring_t uring;
    /** Parameter for the interrupt message to provide context */
    uintptr_t param;
} pdev_itr_config_t;

typedef struct pdev_memrange
{
    /** Physical start address of the range */
    uint64_t base;
    /** Range length.
     * If size is 0 in BSP initialization, then it is allocated during startup.
     * If size is not a multiple of MMU page size, mmap() is forbidden.
     */
    uint32_t len;
    /** mapping attributes allowed, with POSIX PROT_-style values */
    uint32_t attrs;
    /** Memory handler function. Not accessible to user-mode. */
    memrange_cmd_func_t func;
} pdev_memrange_t;


#define PDEV_INTERRUPT_GET_COUNT PDEVCMDOUT(PDEV_DCTL_INTERRUPT,1UL, 0UL)
#define PDEV_MEMRANGE_GET_INFO(idx)  PDEVCMDOUT(PDEV_DCTL_MEMCMD,1UL,(uint32_t)(idx))
#define PDEV_INTERRUPT_GET_IID(intno) PDEVCMDOUT(PDEV_DCTL_INTERRUPT,2UL,(uint32_t)(intno))
#define PDEV_INTERRUPT_MASK(intno) PDEVCMD(PDEV_DCTL_INTERRUPT,7UL,(uint32_t)(intno))
#define PDEV_INTERRUPT_SET_CONFIG(intno)  PDEVCMDIN(PDEV_DCTL_INTERRUPT,3UL,(uint32_t)(intno))
#define PDEV_INTERRUPT_UNCONFIG(intno)  PDEVCMD(PDEV_DCTL_INTERRUPT,5UL,(uint32_t)(intno))
#define PDEV_INTERRUPT_UNMASK(intno)  PDEVCMD(PDEV_DCTL_INTERRUPT,6UL,(uint32_t)(intno))

typedef __off_t     off_t;
#define off_t       __off_t
