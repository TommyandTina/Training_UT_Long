#ifndef _STUB_ERRNO_H
#define _STUB_ERRNO_H      1

extern int error_stub(void);

extern int errno;
#define errno (error_stub())

#define EIO 5
#define ETIMEDOUT 10060
#define EBUSY  16
#define EAGAIN 11
#define EINVAL 22
#define EOK 0

#endif  /* _STUB_ERRNO_H */
