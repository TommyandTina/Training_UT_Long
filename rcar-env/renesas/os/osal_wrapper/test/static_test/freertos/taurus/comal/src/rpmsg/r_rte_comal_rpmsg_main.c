

/* TAURUS side implementation of the data exchange with the guest

   This communication layer implements RPMSG on virtio.
 */


/*******************************************************************************
   Section: Includes
 */
#include "rcar-xos/osal/r_osal.h"
#include "r_taurus_bridge.h"
#include "r_rte_comal_api.h"
#include "r_taurus_platform_peri_ids.h"
#include "r_taurus_protocol_ids.h"

#include "taurus_rpmsg.h"
#include "virtio_config.h"
#include "resource_table.h"
#include "taurus_memory_barrier.h"

#include "r_print_api.h"

/*******************************************************************************
   Section: Local Defines
 */
/* Max number of guest that can be handled */
#define RTE_COMAL_GUEST_MAX_NUM         8

/* Max lenght of RPMSG service name */
#define RTE_COMAL_SERVICE_NAME_LEN      RPMSG_NAME_SIZE


/* MFIS interrupts */
#define IRQ_MFIS_ARIICR0        256
#define IRQ_MFIS_ARIICR1        257
#define IRQ_MFIS_ARIICR2        258
#define IRQ_MFIS_ARIICR3        259
#define IRQ_MFIS_ARIICR4        260
#define IRQ_MFIS_ARIICR5        261
#define IRQ_MFIS_ARIICR6        262
#define IRQ_MFIS_ARIICR7        263

/* MFIS registers */
#define MFIS_BASE 0xe6260000

#define IICR(n) (0x0400 + n * 0x8)
#define EICR(n) (0x0404 + n * 0x8)
#define IMBR(n) (0x0440 + n * 0x4)
#define EMBR(n) (0x0460 + n * 0x4)

#define LOC_MFIS_REG_WRITE(REG, VAL)    LOC_WRITE32(MFIS_BASE + REG, VAL)
#define LOC_MFIS_REG_READ(REG)          LOC_READ32(MFIS_BASE + REG)

#define LOC_WRITE32(ADDR, VAL)  *((volatile unsigned int *)((size_t)ADDR)) = (VAL)
#define LOC_READ32(ADDR)        *((volatile unsigned int *)((size_t)ADDR))

/* Mutex create resouce ID */
#define LOC_TAURUS_RESOURCE_ID  0x8000U                        /* RESOURCE_ID_BSP         */
#define LOC_TAURUS_MUTEX_CREATE (LOC_TAURUS_RESOURCE_ID + 71U) /* OSAL_MUTEX_TAURUS_RPMSG */

/* timeout (s)  */
#define LOC_WAIT_TIME_MS    3000 /* 3 sec */


#ifndef RESOURCE_TABLE_BASE
#error "RESOURCE_TABLE_BASE not defined!"
#endif
static struct cr7_resource_table *cr7_res_table = (struct cr7_resource_table *)RESOURCE_TABLE_BASE;


/*******************************************************************************
   Section: Global Functions
 */

/*******************************************************************************
   Section: Local Variables
 */

typedef struct {
    char name[RTE_COMAL_SERVICE_NAME_LEN];
    unsigned int service_port;
    unsigned int guest_port;                /* only one guest per service at the moment */
    R_TAURUS_Peripheral_Id_t peripheral_id;
} R_RTE_COMAL_Rpmsg_Service_t;

#define RPMSG_SERVICE_SENTINEL { .name = "", .service_port = 0, .guest_port = 0, .peripheral_id = R_TAURUS_INVALID_PERI_ID }

/* List of services announced to the guest by the TAURUS server. */
static R_RTE_COMAL_Rpmsg_Service_t guest1_services[] = {
#if defined(R_TAURUS_ENABLE_VIRTDEV)
    {
        .name = "taurus-virtdev",
        .service_port = TAURUS_PROTOCOL_VIRTDEV_ID,
        .guest_port = 0,
        .peripheral_id = R_TAURUS_VIRTDEV_PERI_ID
    },
#endif
#if defined(R_TAURUS_ENABLE_RVGC)
    {
        .name = "taurus-rvgc",
        .service_port = TAURUS_PROTOCOL_RVGC_ID,
        .guest_port = 0,
        .peripheral_id = R_TAURUS_RVGC_PERI_ID
    },
#endif
#if defined(R_TAURUS_ENABLE_CAMERA)
    {
        .name = "taurus-camera",
        .service_port = TAURUS_PROTOCOL_CAMERA_ID,
        .guest_port = 0,
        .peripheral_id = R_TAURUS_CAMERA_PERI_ID
    },
#endif
#if defined(R_TAURUS_ENABLE_CAN)
    {
        .name = "taurus-can",
        .service_port = TAURUS_PROTOCOL_CAN_ID,
        .guest_port = 0,
        .peripheral_id = R_TAURUS_CAN_PERI_ID
    },
#endif
#if defined(R_TAURUS_ENABLE_ETHER)
    {
        .name = "taurus-ether",
        .service_port = TAURUS_PROTOCOL_ETHER_ID,
        .guest_port = 0,
        .peripheral_id = R_TAURUS_ETHER_PERI_ID
    },
#endif
#if defined(R_TAURUS_ENABLE_VIDEO)
    {
        .name = "taurus-video",
        .service_port = TAURUS_PROTOCOL_VIDEO_ID,
        .guest_port = 0,
        .peripheral_id = R_TAURUS_VIDEO_PERI_ID
    },
#endif
    RPMSG_SERVICE_SENTINEL
};

/* R_RTE_COMAL_Rpmsg_Buf_Desc_t
 *
 * struct used to save all the info related to a virtqueue buffer
 * extracted by R_RTE_COMAL_ReadCmd(). These info are needed to
 * release the buffer in R_RTE_COMAL_SendResult() after we are done
 * processing it. */
typedef struct {
    int16_t head;
    struct taurus_rpmsg_hdr *    msg;
    uint32_t msg_len;
    struct taurus_virtqueue *    virtqueue;
} R_RTE_COMAL_Rpmsg_Buf_Desc_t;

#define COMAL_RPMSG_BUF_DESC_RX 0
#define COMAL_RPMSG_BUF_DESC_TX 1

typedef struct {
    int guest;
    unsigned int initialized;
    struct taurus_rpmsg_dev rpmsg_dev;
    R_RTE_COMAL_Rpmsg_Service_t         *services;
    R_RTE_COMAL_Rpmsg_Buf_Desc_t buf_in_use[2];
    osal_mutex_handle_t mutex;
} R_RTE_COMAL_Rpmsg_State_t;

static R_RTE_COMAL_Rpmsg_State_t locRpmsgState[RTE_COMAL_GUEST_MAX_NUM];


static int loc_rpmsg_service_port_to_peripheral(int Guest, unsigned int ServicePort, R_RTE_COMAL_Rpmsg_Service_t * * service)
{
    R_RTE_COMAL_Rpmsg_State_t *rpmsg_state;
    R_RTE_COMAL_Rpmsg_Service_t *service_iter;
    int peri_id = -1;

    rpmsg_state = &locRpmsgState[Guest];
    service_iter = rpmsg_state->services;

    for (; service_iter->peripheral_id != R_TAURUS_INVALID_PERI_ID; service_iter++) {
        if (ServicePort == service_iter->service_port) {
            peri_id = service_iter->peripheral_id;
            if (service != NULL)
                *service = service_iter;
            break;
        }
    }
    return peri_id;
}

static void locFatalError(int Err)
{
    R_PRINT_Log("[ComalRpmsgMain]: NG\r\n");
    while (Err) {
    }
}

static int virtqueue_kick(struct taurus_virtqueue *vq)
{
    uint32_t regval;

    /* If requested, do not kick the ARM host */

    taurus_memory_cache_invalidate(vq->vring.avail, sizeof(*vq->vring.avail));
    if (vq->vring.avail->flags & VRING_AVAIL_F_NO_INTERRUPT) {
        R_PRINT_Log("[ComalRpmsgMain]: virtqueue_kick VRING_AVAIL_F_NO_INTERRUPT NG\r\n");
        return -1;
    }

    regval = ((vq->id << 1) | 0x1);

    /* Generate a system event to kick the ARM */
    LOC_MFIS_REG_WRITE(EICR(0), regval);

    return 0;
}

void locWaitForResTableInit(struct cr7_resource_table *res_table)
{
    /* Workaround:
     * The CR7 is supposed to start before the other cores, but the
     * virtqueues are set up by Integrity running on the CA5x cores.
     * As soon as we start, invalidate the contents of the resource
     * table. Then check when the same memory location change value,
     * meaning that the other side has completed the initialization.
     */
    volatile uint32_t *p = (volatile uint32_t *)res_table;
    e_osal_return_t osal_ret = OSAL_RETURN_FAIL;

    /* *p = 0xBADDCAFE; */

    /* If enabled, data cache should be flushed here */
    taurus_memory_cache_flush((void *)p, sizeof(*p));

    do {
        osal_ret = R_OSAL_ThreadSleepForTimePeriod(100);
        if (OSAL_RETURN_OK == osal_ret) {
            taurus_memory_cache_invalidate((void *)p, sizeof(*p));
        }
    } while (*p != 0x1);
}

/*******************************************************************************
   Function: R_RTE_COMAL_Init
 */

int R_RTE_COMAL_Init(int Guest)
{

    R_RTE_COMAL_Rpmsg_State_t *rpmsg_state;
    volatile uint8_t *status;
    struct fw_rsc_vdev *rpmsg_dev;
    struct fw_rsc_vdev_vring *vring0;
    struct fw_rsc_vdev_vring *vring1;
    R_RTE_COMAL_Rpmsg_Service_t *service;
    e_osal_return_t osal_ret = OSAL_RETURN_FAIL;

    switch (Guest) {
    case 1:
        taurus_memory_cache_invalidate((void *)cr7_res_table, sizeof(*cr7_res_table));
#if 0
        locWaitForResTableInit(cr7_res_table);
#endif
        rpmsg_dev = &cr7_res_table->rpmsg1;
        vring0 = &cr7_res_table->rpmsg1_vring0;
        vring1 = &cr7_res_table->rpmsg1_vring1;
        service = &guest1_services[0];
        break;
    default:
        /* Currently only Guest=1 is supported */
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_Init Guest(%d) failed\r\n", Guest);
        return -1;
    }

    rpmsg_state = &locRpmsgState[Guest];
    rpmsg_state->guest = Guest;
    rpmsg_state->services = service;

    osal_ret = R_OSAL_ThsyncMutexCreate(LOC_TAURUS_MUTEX_CREATE, &rpmsg_state->mutex);
    if (OSAL_RETURN_OK != osal_ret) {
        R_PRINT_Log("[ComalRpmsgMain]: COMAL_Init Mutex create failed(%d)\n", osal_ret);
        return -1;
    } else {
        /* Make sure the Linux drivers are ready for RPMsg communication */
        status = &rpmsg_dev->status;
        do {
            osal_ret = R_OSAL_ThreadSleepForTimePeriod(50);
            if (OSAL_RETURN_OK == osal_ret) {
                taurus_memory_cache_invalidate((void *)status, sizeof(*status));
            } else {
                return -1;
            }
        } while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

        /* Initialize the RPMsg rpmsg_dev structure */
        taurus_rpmsg_init(&rpmsg_state->rpmsg_dev, vring0, vring1, virtqueue_kick);

        /* Announce supported services to the guest */
        for (; service->peripheral_id != R_TAURUS_INVALID_PERI_ID; service++) {
            taurus_rpmsg_channel(RPMSG_NS_CREATE,
                                 &rpmsg_state->rpmsg_dev,
                                 service->name,
                                 service->service_port);
        }

        rpmsg_state->initialized = 1;
    }
    return 0;
}


/*******************************************************************************
   Function: R_RTE_COMAL_DeInit
 */

int R_RTE_COMAL_DeInit(int Guest)
{
    R_RTE_COMAL_Rpmsg_State_t *rpmsg_state;
    e_osal_return_t osal_ret = OSAL_RETURN_FAIL;

    rpmsg_state = &locRpmsgState[Guest];

    osal_ret = R_OSAL_ThsyncMutexDestroy(rpmsg_state->mutex);
    if (OSAL_RETURN_OK != osal_ret)
    {
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_DeInit Mutex destroy failed(%d)\n", osal_ret);
    }

    return 0;
}


/*******************************************************************************
   Function: R_RTE_COMAL_ReadCmd
 */

int R_RTE_COMAL_ReadCmd(int Guest, R_TAURUS_CmdMsg_t *Cmd)
{
    R_RTE_COMAL_Rpmsg_State_t *          rpmsg_state;
    R_RTE_COMAL_Rpmsg_Buf_Desc_t *       buf_in_use;
    R_TAURUS_CmdMsg_t *                  rpmsg_cmd_hdr;
    int16_t head;
    struct taurus_rpmsg_hdr *            msg;
    uint32_t msg_len;
    struct taurus_virtqueue *            virtqueue;
    uint32_t src, dst;
    R_RTE_COMAL_Rpmsg_Service_t *        service;
    e_osal_return_t                      osal_ret = OSAL_RETURN_FAIL;

    if ((Guest < 0) || (Guest >= RTE_COMAL_GUEST_MAX_NUM)) {
        return -1;
    }

    rpmsg_state = &locRpmsgState[Guest];

    if (!rpmsg_state->initialized) {
        R_PRINT_Log("[ComalRpmsgMain]: COMAL_Init Mutex create failed(%d)\n", osal_ret);
        return -1;
    }

    /* In order to avoid copying buffers around, R_RTE_COMAL_ReadCmd()
     * grabs two buffers from the virtqueues which are directly passed
     * to the peripheral driver via Par1 and Par2 fields in the
     * R_TAURUS_CmdMsg_t struct. The first buffer holds the received message with
     * the input data and the other is an empty buffer where the
     * peripheral driver put the output data.
     *
     * We make a copy of the buffers' descriptors so that we can later
     * return the two buffers to the virtqueues once the peripheral
     * has finished processing them. This takes place in
     * R_RTE_COMAL_SendResult().
     *
     * Until a different solution is found, this approach basically
     * makes R_RTE_COMAL_ReadCmd() and R_RTE_COMAL_SendResult() not
     * reentrant (They are thread-safe though, as long as each thread
     * uses a different Guest id).
     */
    if (rpmsg_state->buf_in_use[0].msg || rpmsg_state->buf_in_use[1].msg) {
        /* This should not happen */
        locFatalError(-1);
    }

    /* Get an available buffer from the RX queue */
    virtqueue = &rpmsg_state->rpmsg_dev.rvq;

    do {
        osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(rpmsg_state->mutex, LOC_WAIT_TIME_MS);
        if (OSAL_RETURN_OK == osal_ret) {
            msg = taurus_virtqueue_get_avail_buf(virtqueue, &head, &msg_len);
            osal_ret = R_OSAL_ThsyncMutexUnlock(rpmsg_state->mutex);
            if (OSAL_RETURN_OK != osal_ret) {
                R_PRINT_Log("[ComalRpmsgMain]: COMAL_Readcmd Mutex unlock failed(%d)\n", osal_ret);
            }
        }
        else
        {
            R_PRINT_Log("[ComalRpmsgMain]: COMAL_Readcmd Mutex lock failed(%d)\n", osal_ret);
        }


#if 1
        /* Workaround to avoid locComServerApp is looping here.
           In a clean implementation it should sleep and get woken up
           by an interrupt signalling when the guest has provided
           a new message
         */
        if (!msg) {
            osal_ret = R_OSAL_ThreadSleepForTimePeriod(2);
        }
#endif
    } while (!msg);

    if (OSAL_RETURN_OK == osal_ret) {
        /* invalidate message header */
        taurus_memory_cache_invalidate(msg, sizeof(*msg));
        /* temporary store source and destination of the message */
        src = msg->src;
        dst = msg->dst;

        /* Save the info into rpmsg_state struct so we are able to release
         * this buffer later */
        buf_in_use = &rpmsg_state->buf_in_use[COMAL_RPMSG_BUF_DESC_RX];
        buf_in_use->head = head;
        buf_in_use->msg = msg;
        buf_in_use->msg_len = msg_len;
        buf_in_use->virtqueue = virtqueue;

        /* invalidate message body */
        taurus_memory_cache_invalidate((void *)msg->data, msg_len);
        rpmsg_cmd_hdr = (R_TAURUS_CmdMsg_t *)msg->data;

        /* copy the header into the TAURUS Cmd struct */
        Cmd->Id = rpmsg_cmd_hdr->Id;
        Cmd->Channel = rpmsg_cmd_hdr->Channel;
        Cmd->Cmd = rpmsg_cmd_hdr->Cmd;
        /* Par1 is the cmd id for the peripheral */
        Cmd->Par1 = rpmsg_cmd_hdr->Par1;
        /* Par2 is the address in memory of the input message */
        Cmd->Par2 = (intptr_t)msg->data;

        /* Now we need to find which peripheral this command is for */
        service = 0; 
        Cmd->Per = loc_rpmsg_service_port_to_peripheral(Guest, dst, &service);
        if (Cmd->Per == -1) {
            /* We received a command for a peripheral that we don't
             * know... just set the TAURUS Cmd to an invalid value and return so
             * that the command parser will reply with a NACK */
            Cmd->Cmd = ~0x0;
        }
        /* Save the guest port for this service */
        if (service->guest_port == 0)
            service->guest_port = src;

        /* We need a buffer where the pheriperal can write the output.
         * Get an available buffer from the TX queue */
        virtqueue = &rpmsg_state->rpmsg_dev.svq;

        do {
            osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(rpmsg_state->mutex, LOC_WAIT_TIME_MS);
            if (OSAL_RETURN_OK == osal_ret) {
                msg = taurus_virtqueue_get_avail_buf(virtqueue, &head, &msg_len);
                osal_ret = R_OSAL_ThsyncMutexUnlock(rpmsg_state->mutex);
                if (OSAL_RETURN_OK != osal_ret) {
                    R_PRINT_Log("[ComalRpmsgMain]: COMAL_Readcmd Mutex unlock failed(%d)\n", osal_ret);
                }
            }
            else {
                R_PRINT_Log("[ComalRpmsgMain]: COMAL_Readcmd Mutex lock failed(%d)\n", osal_ret);
            }
        } while (!msg);

        if (OSAL_RETURN_OK == osal_ret) {
            /* set source and destination for the reply */
            msg->src = dst;
            msg->dst = src;

            /* Save the info into rpmsg_state struct so we are able to release
             * this buffer later */
            buf_in_use = &rpmsg_state->buf_in_use[COMAL_RPMSG_BUF_DESC_TX];
            buf_in_use->head = head;
            buf_in_use->msg = msg;
            buf_in_use->msg_len = msg_len;
            buf_in_use->virtqueue = virtqueue;

            /* Par3 is the address in memory of the output message */
            Cmd->Par3 = (intptr_t)msg->data;
        } else {
            R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_ReadCmd R_OSAL_ThsyncMutexLockForTimePeriod failed(%d)\n", osal_ret);
            return -1;
        }
    } else {
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_ReadCmd R_OSAL_ThsyncMutexLockForTimePeriod failed(%d)\n", osal_ret);
        return -1;
    }

    return 0;
}


/*******************************************************************************
   Function: R_RTE_COMAL_SendResult
 */

int R_RTE_COMAL_SendResult(int Guest, const R_TAURUS_ResultMsg_t *Result)
{
    R_RTE_COMAL_Rpmsg_State_t *          rpmsg_state;
    R_RTE_COMAL_Rpmsg_Buf_Desc_t *       buf_in_use;
    R_TAURUS_ResultMsg_t *               rpmsg_res_hdr;
    R_TAURUS_ResultMsg_t rpmsg_ack;
    struct taurus_rpmsg_hdr *            msg;
    uint32_t msg_len;
    int ret;
    e_osal_return_t                      osal_ret = OSAL_RETURN_FAIL;

    if (Guest < 0 || Guest >= RTE_COMAL_GUEST_MAX_NUM) {
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_SendResult Guest(%d) failed\n", Guest);
        return -1;
    }

    rpmsg_state = &locRpmsgState[Guest];

    if (!rpmsg_state->initialized) {
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_SendResult not initialized failed\n");
        return -1;
    }

    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(rpmsg_state->mutex, LOC_WAIT_TIME_MS);
    if (OSAL_RETURN_OK != osal_ret) {
        R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendResult Mutex lock failed(%d)\n", osal_ret);
        return -1;
    }

    if (!(rpmsg_state->buf_in_use[0].msg) || !(rpmsg_state->buf_in_use[1].msg)) {
        /* This should not happen */
        R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendResult Mutex lock failed(%d)\n", osal_ret);
        locFatalError(-1);
    }

    if (Result->Result == R_TAURUS_RES_ACK) {
        /* We are just sending back the ACK, so it's not time yet to
         * release the buffers. We will release the buffers next time
         * R_RTE_COMAL_SendResult() is called, when we have the result
         * from the peripheral.*/

        rpmsg_ack.Id = Result->Id;
        rpmsg_ack.Channel = Result->Channel;
        rpmsg_ack.Result = Result->Result;
        rpmsg_ack.Aux = 0;

        buf_in_use = &rpmsg_state->buf_in_use[COMAL_RPMSG_BUF_DESC_TX];

        ret = taurus_rpmsg_send(&rpmsg_state->rpmsg_dev,
                                buf_in_use->msg->src,
                                buf_in_use->msg->dst,
                                (void *)&rpmsg_ack,
                                sizeof(rpmsg_ack));

        osal_ret = R_OSAL_ThsyncMutexUnlock(rpmsg_state->mutex);
        if (OSAL_RETURN_OK == osal_ret) {
            return ret;
        } else {
            R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendResult Mutex unlock failed(%d)\n", osal_ret);
            return -1;
        }
    }

    /* Return the used inbound buffer */
    buf_in_use = &rpmsg_state->buf_in_use[COMAL_RPMSG_BUF_DESC_RX];
    /* TODO: This is not nice, why mix taurus_virtqueue_add_used_buf and taurus_rpmsg_send  ? */
    /* TODO: Shouldn't taurus_rpmsg_send be the only one to call virtqueue functions ? */
    /* TODO: It make adding of the caching stuff more complicated because inteface is not clear */
    msg = buf_in_use->msg;
    msg_len = buf_in_use->msg_len;
    /* Clean message header */
    taurus_memory_cache_flush((void *)msg, sizeof(*msg));
    /* Clean message body */
    taurus_memory_cache_flush((void *)msg->data, msg_len);
    if (taurus_virtqueue_add_used_buf(buf_in_use->virtqueue, buf_in_use->head, buf_in_use->msg_len) < 0) {
        osal_ret = R_OSAL_ThsyncMutexUnlock(rpmsg_state->mutex);
        if (OSAL_RETURN_OK != osal_ret) {
            R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendResult Mutex unlock failed(%d)\n", osal_ret);
        }
        return -1;
    }

    /* Kick the ARM host */
    if (buf_in_use->virtqueue->notify)
        buf_in_use->virtqueue->notify(buf_in_use->virtqueue);

    buf_in_use->msg = NULL;


    /* Return the used inbound buffer */
    buf_in_use = &rpmsg_state->buf_in_use[COMAL_RPMSG_BUF_DESC_TX];

    rpmsg_res_hdr = (R_TAURUS_ResultMsg_t *)buf_in_use->msg->data;
    rpmsg_res_hdr->Id = Result->Id;
    rpmsg_res_hdr->Per = Result->Per;
    rpmsg_res_hdr->Channel = Result->Channel;
    rpmsg_res_hdr->Result = Result->Result;

    /* Result.Aux holds the size of the answer written by the
     * peripheral */
    buf_in_use->msg->len = rpmsg_res_hdr->Aux;

    msg = buf_in_use->msg;
    msg_len = buf_in_use->msg_len;
    /* Clean message header */
    taurus_memory_cache_flush((void *)msg, sizeof(*msg));
    /* Clean message body */
    taurus_memory_cache_flush((void *)msg->data, msg_len);

    /* Add the used buffer */
    if (taurus_virtqueue_add_used_buf(buf_in_use->virtqueue, buf_in_use->head, buf_in_use->msg_len) < 0) {
        osal_ret = R_OSAL_ThsyncMutexUnlock(rpmsg_state->mutex);
        if (OSAL_RETURN_OK != osal_ret) {
            R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendResult Mutex unlock failed(%d)\n", osal_ret);
        }
        return -1;
    }

    /* Kick the ARM host */
    if (buf_in_use->virtqueue->notify)
        buf_in_use->virtqueue->notify(buf_in_use->virtqueue);

    buf_in_use->msg = NULL;

    osal_ret = R_OSAL_ThsyncMutexUnlock(rpmsg_state->mutex);
    if (OSAL_RETURN_OK != osal_ret) {
        R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendResult Mutex unlock failed(%d)\n", osal_ret);
        return -1;
    }

    return 0;
}


static R_RTE_COMAL_Rpmsg_Service_t * loc_signalid_to_service(int Guest, R_TAURUS_SignalId_t Signal)
{
    R_RTE_COMAL_Rpmsg_State_t *rpmsg_state;
    R_RTE_COMAL_Rpmsg_Service_t *service_iter;
    R_RTE_COMAL_Rpmsg_Service_t *ret = NULL;
    uint8_t service_id;

    rpmsg_state = &locRpmsgState[Guest];
    service_iter = rpmsg_state->services;

    /* The service's id that wants to notify the guest is
     * encoded in the most significant byte of R_TAURUS_SignalId_t
     */
    service_id = (uint8_t)(Signal >> ((sizeof(Signal) - 1) * 8));

    for (; service_iter->peripheral_id != R_TAURUS_INVALID_PERI_ID; service_iter++) {
        if (service_iter->service_port == service_id) {
            ret = service_iter;
            break;
        }
    }
    return ret;
}

/*******************************************************************************
   Function: R_RTE_COMAL_SendSignal
 */

int R_RTE_COMAL_SendSignal(int Guest, R_TAURUS_SignalId_t Signal)
{
    R_RTE_COMAL_Rpmsg_State_t   * rpmsg_state;
    R_TAURUS_ResultMsg_t          signal_msg;
    R_RTE_COMAL_Rpmsg_Service_t * service;
    int ret;
    e_osal_return_t               osal_ret = OSAL_RETURN_FAIL;

    if (Guest < 0 || Guest >= RTE_COMAL_GUEST_MAX_NUM) {
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_SendSignal Guest(%d) failed\n", Guest);
        return -1;
    }

    rpmsg_state = &locRpmsgState[Guest];

    if (!rpmsg_state->initialized) {
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_SendSignal not initialized failed\n");
        return -1;
    }

    service =  loc_signalid_to_service(Guest, Signal);
    if (!service) {
        R_PRINT_Log("[ComalRpmsgMain]: R_RTE_COMAL_SendSignal service NULL failed\n");
        return -1;
    }

    /* To signal a guest, a R_TAURUS_ResultMsg_t message is sent with
     * transaction Id=0 and Result=R_TAURUS_CMD_NOP. The
     * R_TAURUS_SignalId_t is copied into the Aux field. */
    signal_msg.Id = 0;
    signal_msg.Per = service->service_port;
    signal_msg.Channel = 0;
    signal_msg.Result = R_TAURUS_CMD_NOP;
    signal_msg.Aux = Signal;

    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(rpmsg_state->mutex, LOC_WAIT_TIME_MS);
    if (OSAL_RETURN_OK == osal_ret) {
        ret = taurus_rpmsg_send(&rpmsg_state->rpmsg_dev,
                                service->service_port,
                                service->guest_port,
                                (void *)&signal_msg,
                                sizeof(signal_msg));

        osal_ret = R_OSAL_ThsyncMutexUnlock(rpmsg_state->mutex);
        if (OSAL_RETURN_OK != osal_ret) {
            R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendSignal Mutex unlock failed(%d)\n", osal_ret);
            return -1;
        }
    } else {
        R_PRINT_Log("[ComalRpmsgMain]: COMAL_SendSignal Mutex lock failed(%d)\n", osal_ret);
        return -1;
    }

    return ret;
}
