#include "r_taurus_bridge.h"

#include "r_perserv_api.h"

#include "r_camera_sv_api.h"
#include "r_camera_drv_api.h"

#include "r_print_api.h"

static int locInit(int Guest)
{
    return 0;
}

static int locDeInit(int Guest)
{
    return 0;
}

static void locExecute(int Guest, R_TAURUS_CmdMsg_t *Cmd, R_TAURUS_ResultMsg_t *Result)
{
    int x;

    Result->Id      = Cmd->Id;
    Result->Per     = Cmd->Per;
    Result->Channel = Cmd->Channel;
    Result->Aux    = 0;
    Result->Result = R_TAURUS_RES_ERROR;
    switch (Cmd->Cmd)
    {
        case R_TAURUS_CMD_NOP:
            Result->Result = R_TAURUS_RES_COMPLETE;
            break;
        case R_TAURUS_CMD_OPEN:
            x = R_Camera_DRV_Open(Guest, Cmd);
            Result->Aux = (uint64_t)x;
            if (x == 0)
            {
                Result->Result  = R_TAURUS_RES_COMPLETE;
            }
            else
            {
                R_PRINT_Log("[CameraSvMain]: R_Camera_DRV_Open failed(%d).\r\n", x);
            }
            break;
        case R_TAURUS_CMD_CLOSE:
            x = R_Camera_DRV_Close(Guest, Cmd);
            if (x == 0)
            {
                Result->Result = R_TAURUS_RES_COMPLETE;
            }
            else
            {
                R_PRINT_Log("[CameraSvMain]: R_Camera_DRV_Close failed(%d).\r\n", x);
            }
            break;
        case R_TAURUS_CMD_READ:
            x = R_Camera_DRV_Read(Guest, Cmd);
            Result->Aux = (uint64_t)x;
            if (x == 0)
            {
                Result->Result = R_TAURUS_RES_COMPLETE;
            }
            else
            {
                R_PRINT_Log("[CameraSvMain]: R_Camera_DRV_Read failed(%d).\r\n", x);
            }
            break;
        case R_TAURUS_CMD_WRITE:
            x = R_Camera_DRV_Write(Guest, Cmd);
            Result->Aux = (uint64_t)x;
            if (x == 0) {
                Result->Result = R_TAURUS_RES_COMPLETE;
            }
            else
            {
                R_PRINT_Log("[CameraSvMain]: R_Camera_DRV_Write failed(%d).\r\n", x);
            }
            break;
        case R_TAURUS_CMD_IOCTL:
            x = R_Camera_DRV_IoCtl(Guest, Cmd);
            Result->Aux = (uint64_t)x;
            if (x == 0)
            {
                Result->Result = R_TAURUS_RES_COMPLETE;
            }
            else
            {
                R_PRINT_Log("[CameraSvMain]: R_Camera_DRV_IoCtl failed(%d).\r\n", x);
            }
            break;
        case R_TAURUS_CMD_STATUS:
            x = R_Camera_DRV_Status(Guest, Cmd);
            Result->Aux = (uint64_t)x;
            if (x == 0)
            {
                Result->Result = R_TAURUS_RES_COMPLETE;
            }
            else
            {
                R_PRINT_Log("[CameraSvMain]: R_Camera_DRV_Status failed(%d).\r\n", x);
            }
            break;
        default:
            R_PRINT_Log("[CameraSvMain]: Command error(%d).\r\n", Cmd->Cmd);
            break;
    }
}

static void locSetSendSignalImpl(R_TAURUS_PeripheralServer_SendSignalImpl_t SendSignalImpl)
{
    return R_Camera_DRV_SetSendSignalImpl(SendSignalImpl);
}

R_TAURUS_PeripheralServer_t R_Camera_Server =
{
    "Camera",
    locInit,
    locDeInit,
    locExecute,
    locSetSendSignalImpl
};
