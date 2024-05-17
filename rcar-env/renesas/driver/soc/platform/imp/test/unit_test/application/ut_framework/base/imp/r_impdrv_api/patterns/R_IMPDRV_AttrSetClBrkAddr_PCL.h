#ifndef R_IMPDRV_ATTRSETCLBRKADDR_PCL_H
#define R_IMPDRV_ATTRSETCLBRKADDR_PCL_H

#include "R_IMPDRV_AttrSetClBrkAddr.h"

static struct TEST_R_IMPDRV_AttrSetClBrkAddr_Pattern R_IMPDRV_AttrSetClBrkAddr_PCL[] =
{
	/* "Test Case ID                  ", handle                , *p_core_info       , core_type              , core_num , cl_brk_addr , mutex_handle , mutex_time_period , resource_used , checksum , R_OSAL_ThsyncMutexLockForTimePeriod , R_OSAL_ThsyncMutexUnlock  }, { ercd         ,  */
	/* R_IMPDRV_AttrSetClBrkAddr_0001 */ { { TEST_ADDR_INIT_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 4 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_OK  } }
};

#endif


