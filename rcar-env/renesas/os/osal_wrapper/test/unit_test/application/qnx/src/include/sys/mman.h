#ifndef _STUB_MMAN_H
#define _STUB_MMAN_H      1

#include <stdint.h>
#include <sys/types.h>

// qnx710/target/qnx7/usr/include/sys/mman.h
#define NOFD                (-1)
#define POSIX_TYPED_MEM_ALLOCATE_CONTIG    0x0002
#define PROT_READ       0x00000100
#define PROT_WRITE      0x00000200
# define PROT_NOCACHE   0x00000800
#define MAP_SHARED      0x00000001
#define MAP_FAILED      ((void *)-1)
#define MS_SYNC                 0x0000002
#define MS_CACHE_ONLY          0x02000000
#define MS_INVALIDATE           0x0000004

#define MAP_PHYS        0x00010000
#define MAP_ANON        0x00080000

// _Uintptrt -> uintptr_t
# define MAP_DEVICE_FAILED   ((uintptr_t)MAP_FAILED)

extern int posix_typed_mem_open(const char *name, int oflag, int tflag);
extern void * mmap( void * addr, size_t len, int prot, int flags, int fildes, off_t off );
extern int msync(void *addr, size_t len, int flags);
extern int munmap(void *addr, size_t len);
extern int posix_typed_mem_open(const char *name, int oflag, int tflag);
extern uintptr_t mmap_device_io( size_t len, uint64_t io );
extern int munmap_device_io( uintptr_t io, size_t len );
extern int mem_offset64( const void * addr, int fd, size_t length, off64_t * offset, size_t * contig_len );
extern int close(int fildes);
extern int mlock(const void *addr, size_t len);
extern int munlock(const void *addr, size_t len); 
extern void * mmap_device_memory(void * addr, size_t len, int prot, int flags, uint64_t physical);
extern int munmap_device_memory( void * addr, size_t len );

#endif  /* _STUB_MMAN_H */
