#include "r_taurus_bridge.h"
#include "r_perserv_api.h"
#include "r_taurus_platform.h"
#include "r_taurus_platform_peri_ids.h"

#if defined(R_TAURUS_ENABLE_VIRTDEV)
#include "r_virtdev_sv_api.h"
#endif

#if defined(R_TAURUS_ENABLE_RVGC)
#include "r_rvgc_sv_api.h"
#endif

#if defined(R_TAURUS_ENABLE_CAMERA)
#include "r_camera_sv_api.h"
#endif

#if defined(R_TAURUS_ENABLE_CAN)
#include "r_can_sv_api.h"
#endif

#if defined(R_TAURUS_ENABLE_ETHER)
#include "r_ether_sv_api.h"
#endif

#if defined(R_TAURUS_ENABLE_VIDEO)
#include "r_video_sv_api.h"
#endif


R_TAURUS_PeripheralServer_t *R_TAURUS_PF_Devtable[] = {

#if defined(R_TAURUS_ENABLE_VIRTDEV)
    [R_TAURUS_VIRTDEV_PERI_ID] = &R_Virtdev_Server,
#endif

#if defined(R_TAURUS_ENABLE_RVGC)
    [R_TAURUS_RVGC_PERI_ID] = &R_Rvgc_Server,
#endif

#if defined(R_TAURUS_ENABLE_CAMERA)
    [R_TAURUS_CAMERA_PERI_ID] = &R_Camera_Server,
#endif

#if defined(R_TAURUS_ENABLE_CAN)
    [R_TAURUS_CAN_PERI_ID] = &R_Can_Server,
#endif

#if defined(R_TAURUS_ENABLE_ETHER)
    [R_TAURUS_ETHER_PERI_ID] = &R_Ether_Server,
#endif

#if defined(R_TAURUS_ENABLE_VIDEO)
    [R_TAURUS_VIDEO_PERI_ID] = &R_Video_Server,
#endif

    0
};


R_TAURUS_PeripheralServer_t * *R_PF_GetDeviceTable(int Guest)
{
    return R_TAURUS_PF_Devtable;
}
