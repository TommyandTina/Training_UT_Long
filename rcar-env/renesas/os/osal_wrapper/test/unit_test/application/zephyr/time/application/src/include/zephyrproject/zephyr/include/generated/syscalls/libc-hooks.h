/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_LIBC_HOOKS_H
#define Z_INCLUDE_SYSCALLS_LIBC_HOOKS_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_zephyr_read_stdin(char * buf, int nbytes);

__pinned_func
static inline int zephyr_read_stdin(char * buf, int nbytes)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; char * val; } parm0 = { .val = buf };
		union { uintptr_t x; int val; } parm1 = { .val = nbytes };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_ZEPHYR_READ_STDIN);
	}
#endif
	compiler_barrier();
	return z_impl_zephyr_read_stdin(buf, nbytes);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define zephyr_read_stdin(buf, nbytes) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZEPHYR_READ_STDIN, zephyr_read_stdin, buf, nbytes); 	retval = zephyr_read_stdin(buf, nbytes); 	sys_port_trace_syscall_exit(K_SYSCALL_ZEPHYR_READ_STDIN, zephyr_read_stdin, buf, nbytes, retval); 	retval; })
#endif
#endif


extern int z_impl_zephyr_write_stdout(const void * buf, int nbytes);

__pinned_func
static inline int zephyr_write_stdout(const void * buf, int nbytes)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const void * val; } parm0 = { .val = buf };
		union { uintptr_t x; int val; } parm1 = { .val = nbytes };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_ZEPHYR_WRITE_STDOUT);
	}
#endif
	compiler_barrier();
	return z_impl_zephyr_write_stdout(buf, nbytes);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define zephyr_write_stdout(buf, nbytes) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZEPHYR_WRITE_STDOUT, zephyr_write_stdout, buf, nbytes); 	retval = zephyr_write_stdout(buf, nbytes); 	sys_port_trace_syscall_exit(K_SYSCALL_ZEPHYR_WRITE_STDOUT, zephyr_write_stdout, buf, nbytes, retval); 	retval; })
#endif
#endif


extern int z_impl_zephyr_fputc(int c, FILE * stream);

__pinned_func
static inline int zephyr_fputc(int c, FILE * stream)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = c };
		union { uintptr_t x; FILE * val; } parm1 = { .val = stream };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_ZEPHYR_FPUTC);
	}
#endif
	compiler_barrier();
	return z_impl_zephyr_fputc(c, stream);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define zephyr_fputc(c, stream) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZEPHYR_FPUTC, zephyr_fputc, c, stream); 	retval = zephyr_fputc(c, stream); 	sys_port_trace_syscall_exit(K_SYSCALL_ZEPHYR_FPUTC, zephyr_fputc, c, stream, retval); 	retval; })
#endif
#endif


extern size_t z_impl_zephyr_fwrite(const void *ZRESTRICT ptr, size_t size, size_t nitems, FILE *ZRESTRICT stream);

__pinned_func
static inline size_t zephyr_fwrite(const void *ZRESTRICT ptr, size_t size, size_t nitems, FILE *ZRESTRICT stream)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const void *ZRESTRICT val; } parm0 = { .val = ptr };
		union { uintptr_t x; size_t val; } parm1 = { .val = size };
		union { uintptr_t x; size_t val; } parm2 = { .val = nitems };
		union { uintptr_t x; FILE *ZRESTRICT val; } parm3 = { .val = stream };
		return (size_t) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_ZEPHYR_FWRITE);
	}
#endif
	compiler_barrier();
	return z_impl_zephyr_fwrite(ptr, size, nitems, stream);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define zephyr_fwrite(ptr, size, nitems, stream) ({ 	size_t retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZEPHYR_FWRITE, zephyr_fwrite, ptr, size, nitems, stream); 	retval = zephyr_fwrite(ptr, size, nitems, stream); 	sys_port_trace_syscall_exit(K_SYSCALL_ZEPHYR_FWRITE, zephyr_fwrite, ptr, size, nitems, stream, retval); 	retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */