
#ifndef R_CAMERA_DRV_API_H
#define R_CAMERA_DRV_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rcar-xos/osal/r_osal.h"

int R_Camera_DRV_Open(int Guest, R_TAURUS_CmdMsg_t *Cmd);

int R_Camera_DRV_Close(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Camera_DRV_Read(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Camera_DRV_Write(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Camera_DRV_IoCtl(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Camera_DRV_Status(int Guest, R_TAURUS_CmdMsg_t *Cmd);

void R_Camera_DRV_SetSendSignalImpl(int (*SendSignalImpl)(int Guest, R_TAURUS_SignalId_t Signal));

#ifdef __cplusplus
}
#endif


#endif /* R_CAMERA_DRV_API_H */
