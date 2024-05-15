#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <fcntl.h>
#include <poll.h>
#include <glob.h>
#include <regex.h>
#include <stdint.h>
#include <stdarg.h>
#include <linux/cmemdrv.h>
#include <linux/renesas_uioctl.h>
#include <dirent.h>

#ifdef _USE_STUB
void init_stub_time();
uint64_t get_stub_time();

#define STUB_CMEM_SIZE    (0x4000)
typedef union _stub_cmem_area_t
{
    void* dummy;
    char  array[STUB_CMEM_SIZE];
} stub_cmem_area_t;
stub_cmem_area_t * get_cmem_area(void);
#else
void init_stub_time(){;}
uint64_t get_stub_time(){return 0;}
void * get_cmem_area(void){return NULL;}
#endif
