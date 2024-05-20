#ifndef R_IMPDRV_ATTRSETCLBRKADDR_PCL_H
#define R_IMPDRV_ATTRSETCLBRKADDR_PCL_H

#include "R_IMPDRV_AttrSetClBrkAddr.h"

static struct TEST_R_IMPDRV_AttrSetClBrkAddr_Pattern R_IMPDRV_AttrSetClBrkAddr_PCL[] =
{
	/* "Test Case ID                  ", handle                 , *p_core_info       , core_type                   , core_num , cl_brk_addr , mutex_handle , mutex_time_period , resource_used , checksum , R_OSAL_ThsyncMutexLockForTimePeriod , R_OSAL_ThsyncMutexUnlock  }, { ercd                     ,  */
	/* R_IMPDRV_AttrSetClBrkAddr_0001 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { 0, FALSE }                        , { 0, FALSE }              }, { IMPDRV_EC_OK              } },
	/* R_IMPDRV_AttrSetClBrkAddr_0002 */ { { TEST_ADDR_NULL         , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_ARGNULL      } },
	/* R_IMPDRV_AttrSetClBrkAddr_0003 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NULL     , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_ARGNULL      } },
	/* R_IMPDRV_AttrSetClBrkAddr_0004 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 3           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_AttrSetClBrkAddr_0005 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 2           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_AttrSetClBrkAddr_0006 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 1           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_AttrSetClBrkAddr_0007 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_PSCOUT   , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_NOTSUPPORT   } },
	/* R_IMPDRV_AttrSetClBrkAddr_0008 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_DTA      , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_NOTSUPPORT   } },
	/* R_IMPDRV_AttrSetClBrkAddr_0009 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_LDMAC    , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_NOTSUPPORT   } },
	/* R_IMPDRV_AttrSetClBrkAddr_0010 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMR      , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_NOTSUPPORT   } },
	/* R_IMPDRV_AttrSetClBrkAddr_0011 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP_SLIM , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_NOTSUPPORT   } },
	/* R_IMPDRV_AttrSetClBrkAddr_0012 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 8 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_AttrSetClBrkAddr_0013 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_PAR, TRUE }           , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_AttrSetClBrkAddr_0014 */ { { TEST_ADDR_READY_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 3 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_NOTSUPPORT   } },
	/* R_IMPDRV_AttrSetClBrkAddr_0015 */ { { TEST_ADDR_INIT_HANDLE  , TEST_ADDR_NOT_NULL , { IMPDRV_CORE_TYPE_IMP      , 0 }      , 4           , { 1          , 1 }               , { 0 }         , { 0 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }  }, { IMPDRV_EC_NG_SEQSTATE     } }
};

#endif


