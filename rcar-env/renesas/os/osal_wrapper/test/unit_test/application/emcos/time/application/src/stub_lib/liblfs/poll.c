/*
    Copyright(C) 2005 eSOL Co., Ltd. All rights reserved.

    This software is protected by the law and the agreement concerning
    a Japanese country copyright method, an international agreement,
    and other intellectual property right and may be used and copied
    only in accordance with the terms of such license and with the inclusion
    of the above copyright notice.

    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by eSOL Co.,Ltd.
*/
/*============================================================================
 poll.c   POSIX Layer   poll() library function
============================================================================*/
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

int poll(struct pollfd pfd[], nfds_t nfd, int tmo)
{
    int             cnt;
    int             num;
    int             act;
    int             max;
    struct timeval  val;
    struct timeval* ptr;
    fd_set*         rfp;
    fd_set*         wfp;
    fd_set*         efp;
    fd_set          rfd;
    fd_set          wfd;
    fd_set          efd;
    struct stat     sta;

    if (nfd < 0 || nfd > OPEN_MAX)
    {
        errno = EINVAL;
        return -1;
    }
 /* Convert from milliseconds to timeval */
    ptr = 0;
    if (tmo != -1)
    {
        ptr = &val;
        val.tv_sec = tmo / 1000;
        val.tv_usec = (tmo % 1000) * 1000;
    }
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    FD_ZERO(&efd);
    for (max = 0, act = 0, num = 0, cnt = 0; cnt < nfd; cnt++)
    {
     /* Ignore negative fds */
        pfd[cnt].revents = 0;
        if (pfd[cnt].fd >= 0)
        {
         /* If bad file descriptor or non-supported file type */
            if (    (fstat(pfd[cnt].fd, &sta) != 0)
                 || (    !S_ISREG(sta.st_mode)
                      && !S_ISDIR(sta.st_mode)
                      && !S_ISFIFO(sta.st_mode)
                      && !S_ISSOCK(sta.st_mode)
                      && !S_TYPEISSHM(&sta)
                      && !S_ISCHR(sta.st_mode)  ) )
            {
                num += 1;
                pfd[cnt].revents = POLLNVAL;
            }
         /* Regular files always report ready */
            else if (    S_ISREG(sta.st_mode)
                      || S_ISDIR(sta.st_mode)
                      || S_TYPEISSHM(&sta)    )
            {
                num += 1;
                pfd[cnt].revents = (pfd[cnt].events
                                    & (POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI
                                       | POLLOUT | POLLWRNORM | POLLWRBAND));
            }
         /* Prepare select bits */
            else
            {
                if ((pfd[cnt].events & (POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI
                                        | POLLOUT | POLLWRNORM | POLLWRBAND)) != 0
                    || S_ISSOCK(sta.st_mode))
                {
                    act += 1;
                    if (max < pfd[cnt].fd)
                    {
                        max = pfd[cnt].fd;
                    }
                    if ((pfd[cnt].events & (POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI)) != 0)
                    {
                        FD_SET(pfd[cnt].fd, &rfd);
                    }
                    if ((pfd[cnt].events & (POLLOUT | POLLWRNORM | POLLWRBAND)) != 0)
                    {
                        FD_SET(pfd[cnt].fd, &wfd);
                    }
                 /* Support error only for sockets */
                    if (S_ISSOCK(sta.st_mode))
                    {
                        FD_SET(pfd[cnt].fd, &efd);
                    }
                }
            }
        }
    }
 /* If no bad or ready descriptors were found */
    if (num == 0)
    {
     /* Will block if timeout/undefinite and no valid descriptors */
        rfp = 0;
        wfp = 0;
        efp = 0;
        if (act != 0)
        {
            rfp = &rfd;
            wfp = &wfd;
            efp = &efd;
        }
     /* Use select() */
        if ((num = select(max + 1, rfp, wfp, efp, ptr)) < 0)
        {
         /* If bad file descriptor check what is it.
            Will return zero if none is bad */
            if (num < 0 && errno == EBADF)
            {
                for (num = 0, cnt = 0; cnt < nfd; cnt++)
                {
                    if (    (pfd[cnt].fd >= 0)
                         && (    (fstat(pfd[cnt].fd, &sta) != 0)
                              || (    !S_ISREG(sta.st_mode)
                                   && !S_ISDIR(sta.st_mode)
                                   && !S_ISFIFO(sta.st_mode)
                                   && !S_ISSOCK(sta.st_mode)
                                   && !S_TYPEISSHM(&sta)
                                   && !S_ISCHR(sta.st_mode) ) ) )
                    {
                        num += 1;
                        pfd[cnt].revents = POLLNVAL;
                    }
                }
            }
        }
     /* Calculate actual selected descriptors number */
        else if (num != 0)
        {
            for (num = 0, cnt = 0; cnt < nfd; cnt++)
            {
                if (pfd[cnt].fd >= 0)
                {
                    if (FD_ISSET(pfd[cnt].fd, &rfd))
                    {
                        pfd[cnt].revents |= (pfd[cnt].events
                                             & (POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI));
                    }
                    if (FD_ISSET(pfd[cnt].fd, &wfd))
                    {
                        pfd[cnt].revents |= (pfd[cnt].events
                                             & (POLLOUT | POLLWRNORM | POLLWRBAND));
                    }
                 /* We do not support POLLHUP status */
                    if (FD_ISSET(pfd[cnt].fd, &efd))
                    {
                     /* CHECKME! socket OOB data is exceptional cond but not error */
                        pfd[cnt].revents |= POLLERR;
                    }
                    if (pfd[cnt].revents != 0)
                    {
                        num += 1;
                    }
                }
            }
        }
     /* Timeout */
        else
        {
         /* MISRA */
        }
    }
    return num;
}
