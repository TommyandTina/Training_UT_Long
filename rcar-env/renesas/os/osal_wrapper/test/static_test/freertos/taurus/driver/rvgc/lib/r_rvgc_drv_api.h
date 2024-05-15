
#ifndef R_RVGC_DRV_API_H
#define R_RVGC_DRV_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rcar-xos/osal/r_osal.h"

int R_Rvgc_DRV_Open(int Guest, R_TAURUS_CmdMsg_t *Cmd);

int R_Rvgc_DRV_Close(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Rvgc_DRV_Read(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Rvgc_DRV_Write(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Rvgc_DRV_IoCtl(int Guest, R_TAURUS_CmdMsg_t *Cmd);

ssize_t R_Rvgc_DRV_Status(int Guest, R_TAURUS_CmdMsg_t *Cmd);

void R_Rvgc_DRV_SetSendSignalImpl(int (*SendSignalImpl)(int Guest, R_TAURUS_SignalId_t Signal));

#ifdef __cplusplus
}
#endif


#endif /* R_RVGC_DRV_API_H */
