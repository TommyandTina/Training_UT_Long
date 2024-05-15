#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_usr_cfg/freertos/r_osal_usr_cfg_control.h"

#include "r_taurus_bridge.h"

#include "r_video_drv_api.h"
#include "r_taurus_video_protocol.h"

#include "r_cio_api.h"
#include "r_cio_video_public.h"

#include "r_print_api.h"

/*******************************************************************************
   Section: Local Defines
 */
#define LOC_NUM_VIDEO_IF    1U

/*******************************************************************************
   Section: Local Types
 */
struct VideoInstance
{
    char CioName[64];
    int Handle;
    int Initialized;
};

/*******************************************************************************
   Section: Local Variables
 */
static int (*locSendSignal)(int Guest, R_TAURUS_SignalId_t Signal) = 0;

static struct VideoInstance loc_VideoInstances[LOC_NUM_VIDEO_IF] = {
    {
        .CioName = "RCar-Video:0",
        .Handle = -1,
        .Initialized = 0,
    },
};

static struct VideoInstance *videoInstance;


static int loc_ioctl_reg_read(struct taurus_video_ioc_read_in *in, struct taurus_video_ioc_read_out *out)
{
    int ret = 0;
    r_cio_video_regread_t ioc_param;

    ioc_param.regOffset = in->reg_offset;

    ret = R_CIO_IoCtl(videoInstance->Handle, (void *)R_CIO_VIDEO_REGREAD, (void *)&ioc_param, 0);
    if (0 != ret) {
        R_PRINT_Log("[TaurusVideoMain]: loc_ioctl_reg_read R_CIO_IoCtl(R_CIO_VIDEO_REGREAD) failed(%d).\r\n", ret);
    } else {
        out->cookie = in->cookie;
        out->read_data = ioc_param.readData;
    }
    out->res = ret;

    return ret;
}


static int loc_ioctl_reg_write(struct taurus_video_ioc_write_in *in, struct taurus_video_ioc_write_out *out)
{
    int ret = 0;
    r_cio_video_regwrite_t ioc_param;

    ioc_param.regOffset = in->reg_offset;
    ioc_param.writeData = in->reg_data;

    ret = R_CIO_IoCtl(videoInstance->Handle, (void *)R_CIO_VIDEO_REGWRITE, (void *)&ioc_param, 0);
    if (0 != ret) {
        R_PRINT_Log("[TaurusVideoMain]: loc_ioctl_reg_write R_CIO_IoCtl(R_CIO_VIDEO_REGWRITE) failed(%d).\r\n", ret);
    } else {
        out->cookie = in->cookie;
    }
    out->res = ret;
    return ret;
}


int R_VIDEO_DRV_Open(int Guest, R_TAURUS_CmdMsg_t *Cmd)
{
    struct taurus_video_cmd_msg    *cmd;
    struct taurus_video_res_msg    *res;
    struct taurus_video_open_in    *params_in;
    struct taurus_video_open_out   *params_out;

    int handle;
    int ret = -1;

    cmd = (struct taurus_video_cmd_msg *)(size_t)Cmd->Par2;
    res = (struct taurus_video_res_msg *)(size_t)Cmd->Par3;

    params_in  = &cmd->params.open;
    params_out = &res->params.open;

    params_out->cookie = params_in->cookie;

    videoInstance = &loc_VideoInstances[0];

    if (1 == videoInstance->Initialized)
    {
        params_out->res = 2;
        R_PRINT_Log("[TaurusVideoMain]: R_VIDEO_DRV_Open already initialize(%d).\r\n", params_out->res);
    }
    else
    {
        handle = R_CIO_Open("RCar-Video:0", "rwb");
        if (handle <= 0)
        {
            params_out->res = 2;
            R_PRINT_Log("[TaurusVideoMain]: R_VIDEO_DRV_Open R_CIO_Open failed(%d).\r\n", params_out->res);
        }
        else
        {
            videoInstance->Handle = handle;
            videoInstance->Initialized = 1;
            params_out->res = 0;
            ret = 0;
        }
    }
    return ret;
}


int R_VIDEO_DRV_Close(int Guest, R_TAURUS_CmdMsg_t *Cmd)
{
    struct taurus_video_cmd_msg    *cmd;
    struct taurus_video_res_msg    *res;
    struct taurus_video_close_in   *params_in;
    struct taurus_video_close_out  *params_out;

    int ret = -1;

    cmd = (struct taurus_video_cmd_msg *)(size_t)Cmd->Par2;
    res = (struct taurus_video_res_msg *)(size_t)Cmd->Par3;

    params_in  = &cmd->params.close;
    params_out = &res->params.close;

    params_out->cookie = params_in->cookie;

    videoInstance = &loc_VideoInstances[0];
    if (0 == videoInstance->Initialized)
    {
        params_out->res = 2;
        R_PRINT_Log("[TaurusVideoMain]: R_VIDEO_DRV_Close not initialize(%d).\r\n", params_out->res);
    }
    else
    {
        ret = R_CIO_Close(videoInstance->Handle);
        if (0 != ret)
        {
            params_out->res = 3;
            R_PRINT_Log("[TaurusVideoMain]: R_VIDEO_DRV_Close R_CIO_Close failed(%d).\r\n", params_out->res);
        }
        else
        {
            videoInstance->Initialized = 0;
            videoInstance->Handle = -1;
            params_out->res = 0;
        }
    }
    return ret;
}


ssize_t R_VIDEO_DRV_Read(int Guest, R_TAURUS_CmdMsg_t *Cmd)
{
    return 0;
}


ssize_t R_VIDEO_DRV_Write(int Guest, R_TAURUS_CmdMsg_t *Cmd)
{
    return 0;
}


ssize_t R_VIDEO_DRV_IoCtl(int Guest, R_TAURUS_CmdMsg_t *Cmd)
{
    ssize_t x;
    uint32_t ioctl_id;
    struct taurus_video_cmd_msg     *cmd;
    struct taurus_video_res_msg     *res;

    ioctl_id = Cmd->Par1;

    cmd = (struct taurus_video_cmd_msg *)(size_t)Cmd->Par2;
    res = (struct taurus_video_res_msg *)(size_t)Cmd->Par3;

    switch (ioctl_id)
    {
    case VIDEO_PROTOCOL_IOCTL_REGREAD:
        x = loc_ioctl_reg_read(&cmd->params.ioctl_regread, &res->params.ioctl_regread);
        break;
    case VIDEO_PROTOCOL_IOCTL_REGWRITE:
        x = loc_ioctl_reg_write(&cmd->params.ioctl_regwrite, &res->params.ioctl_regwrite);
        break;
    default:
        x = 1;
        R_PRINT_Log("[TaurusVideoMain]: R_VIDEO_DRV_IoCtl ioctl_id(%d) failed(%d).\r\n", ioctl_id, x);
        break;
    }
    return x;
}


ssize_t R_VIDEO_DRV_Status(int Guest, R_TAURUS_CmdMsg_t *Cmd)
{
    return 0;
}


void R_VIDEO_DRV_SetSendSignalImpl(int (*SendSignalImpl)(int Guest, R_TAURUS_SignalId_t Signal))
{
    locSendSignal = SendSignalImpl;
}
