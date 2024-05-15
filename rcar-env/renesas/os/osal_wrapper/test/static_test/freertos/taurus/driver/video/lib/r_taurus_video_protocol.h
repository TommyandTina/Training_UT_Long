#ifndef R_TAURUS_VIDEO_PROTOCOL_H
#define R_TAURUS_VIDEO_PROTOCOL_H

#include "r_taurus_bridge.h"
#include "r_taurus_protocol_ids.h"

#ifndef __packed
# define __packed       __attribute__((__packed__))
#endif

/**@defgroup group2 Taurus VIDEO server internal protocol commands
 *
 * @{
 */


/**
 * @brief VIDEO signal identifiers
 *
 */

#define VIDEO_PROTOCOL_OPEN         ((TAURUS_PROTOCOL_VIDEO_ID << 24) | 0xE00000)

struct taurus_video_open_in {
    uint64_t cookie;
} __packed;

struct taurus_video_open_out {
    uint64_t cookie;
    uint64_t res;
} __packed;


#define VIDEO_PROTOCOL_CLOSE        ((TAURUS_PROTOCOL_VIDEO_ID << 24) | 0xE00001)

struct taurus_video_close_in {
    uint64_t cookie;
} __packed;

struct taurus_video_close_out {
    uint64_t cookie;
    uint64_t res;
} __packed;


#define VIDEO_PROTOCOL_READ         ((TAURUS_PROTOCOL_VIDEO_ID << 24) | 0xE00002)

struct taurus_video_read_in {
    uint64_t cookie;
} __packed;

struct taurus_video_read_out {
    uint64_t cookie;
    uint64_t res;
} __packed;


#define VIDEO_PROTOCOL_WRITE        ((TAURUS_PROTOCOL_VIDEO_ID << 24) | 0xE00003)

struct taurus_video_write_in {
    uint64_t cookie;
} __packed;

struct taurus_video_write_out {
    uint64_t cookie;
    uint64_t res;
} __packed;


#define VIDEO_PROTOCOL_IOCTL_REGREAD    ((TAURUS_PROTOCOL_VIDEO_ID << 24) | 0xE00004)

struct taurus_video_ioc_read_in {
    uint64_t cookie;
    uint32_t reg_offset;
} __packed;

struct taurus_video_ioc_read_out {
    uint64_t cookie;
    uint32_t read_data;
    uint64_t res;
} __packed;


#define VIDEO_PROTOCOL_IOCTL_REGWRITE   ((TAURUS_PROTOCOL_VIDEO_ID << 24) | 0xE00005)

struct taurus_video_ioc_write_in {
    uint64_t cookie;
    uint32_t reg_offset;
    uint32_t reg_data;
} __packed;

struct taurus_video_ioc_write_out {
    uint64_t cookie;
    uint64_t res;
} __packed;


#define VIDEO_PROTOCOL_STATUS           ((TAURUS_PROTOCOL_VIDEO_ID << 24) | 0xE00006)

struct taurus_video_status_in {
    uint64_t cookie;
} __packed;

struct taurus_video_status_out {
    uint64_t cookie;
} __packed;


/** @} */

/**@defgroup group3 Taurus VIDEO server protocol API
 *
 * @{
 */

/*******************************************************/

struct taurus_video_cmd_msg {
    R_TAURUS_CmdMsg_t hdr;
    uint32_t type;
    union {
        struct taurus_video_open_in open;
        struct taurus_video_close_in close;
        struct taurus_video_read_in read;
        struct taurus_video_write_in write;
        struct taurus_video_ioc_read_in ioctl_regread;
        struct taurus_video_ioc_write_in ioctl_regwrite;
        struct taurus_video_status_in status;
    } params;
};

struct taurus_video_res_msg {
    R_TAURUS_ResultMsg_t hdr;
    uint32_t type;
    union {
        struct taurus_video_open_out open;
        struct taurus_video_close_out close;
        struct taurus_video_read_out read;
        struct taurus_video_write_out write;
        struct taurus_video_ioc_read_out ioctl_regread;
        struct taurus_video_ioc_write_out ioctl_regwrite;
        struct taurus_video_status_out status;
    } params;
};


/** @} */

#endif /* R_TAURUS_VIDEO_PROTOCOL_H */
