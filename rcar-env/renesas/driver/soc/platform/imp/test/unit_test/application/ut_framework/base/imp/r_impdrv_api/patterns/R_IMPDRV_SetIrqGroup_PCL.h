#ifndef R_IMPDRV_SETIRQGROUP_PCL_H
#define R_IMPDRV_SETIRQGROUP_PCL_H

#include "R_IMPDRV_SetIrqGroup.h"

static struct TEST_R_IMPDRV_SetIrqGroup_Pattern R_IMPDRV_SetIrqGroup_PCL[] =
{
	/* "Test Case ID             ", handle                    , *p_irq_param       , irq_group               , group_core_num            , core_type                    , core_num , core_type                       , core_num , core_type                       , core_num , core_type                       , core_num , core_type                       , core_num , core_type                       , core_num , checksum , R_OSAL_ThsyncMutexLockForTimePeriod , R_OSAL_ThsyncMutexUnlock     , R_OSAL_IoRead32[6]                                                                                                                                                     }, { ercd                     ,  */
	/* R_IMPDRV_SetIrqGroup_0001 */ { { TEST_ADDR_CHECKSUM_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 4 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 268355, 268355, 0, 3221493827 } , TRUE  }                 }, { IMPDRV_EC_OK              } },
	/* R_IMPDRV_SetIrqGroup_0002 */ { { TEST_ADDR_CHECKSUM_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 4 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {137283, 137283, 137283, 137283, 137283, 3221362755 } , TRUE  }  }, { IMPDRV_EC_OK              } },
	/* R_IMPDRV_SetIrqGroup_0003 */ { { TEST_ADDR_CHECKSUM_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 4 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 723, 723, 0, 3221226195 } , TRUE  }                       }, { IMPDRV_EC_OK              } },
	/* R_IMPDRV_SetIrqGroup_0004 */ { { TEST_ADDR_NULL            , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_ARGNULL      } },
	/* R_IMPDRV_SetIrqGroup_0005 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NULL     , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_ARGNULL      } },
	/* R_IMPDRV_SetIrqGroup_0006 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { 0                     , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_SetIrqGroup_0007 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 0                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_SetIrqGroup_0008 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_1    , IMPDRV_IRQGROUP_MAXID + 1 , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_SetIrqGroup_0009 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_IMP    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_SetIrqGroup_0010 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_INVALID , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_SetIrqGroup_0011 */ { { TEST_ADDR_UNINIT_HANDLE   , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SEQSTATE     } },
	/* R_IMPDRV_SetIrqGroup_0012 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_FAIL, TRUE }          , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0013 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_FAIL, TRUE }   , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0014 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_FAIL, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                             }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0015 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_2    , IMPDRV_IRQGROUP_MAXID + 1 , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_SetIrqGroup_0016 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_NONE , IMPDRV_IRQGROUP_MAXID + 1 , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_PARAM        } },
	/* R_IMPDRV_SetIrqGroup_0017 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_PSCOUT  , 0 }      , [1] = { IMPDRV_CORE_TYPE_PSCOUT , 0 }      , [2] = { IMPDRV_CORE_TYPE_PSCOUT , 0 }      , [3] = { IMPDRV_CORE_TYPE_PSCOUT , 1 }      , [4] = { IMPDRV_CORE_TYPE_PSCOUT , 0 }      , [5] = { IMPDRV_CORE_TYPE_PSCOUT , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_NOTSUPPORT   } },
	/* R_IMPDRV_SetIrqGroup_0018 */ { { TEST_ADDR_CHECKSUM_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 4 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, { 0, 0, 0 , 0, 1 } , TRUE  }                                     }, { IMPDRV_EC_NG_CHECKFAIL    } },
	/* R_IMPDRV_SetIrqGroup_0019 */ { { TEST_ADDR_CHECKSUM_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 4 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, { 0, 0, 0 , 0, 262144 } , TRUE  }                                }, { IMPDRV_EC_NG_CHECKFAIL    } },
	/* R_IMPDRV_SetIrqGroup_0020 */ { { TEST_ADDR_CHECKSUM_HANDLE , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 4 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, { 0, 0, 0 , 0, 262144 } , TRUE  }                                }, { IMPDRV_EC_NG_CHECKFAIL    } },
	/* R_IMPDRV_SetIrqGroup_0021 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_PAR, TRUE }           , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0022 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_TIME, TRUE }          , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0023 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_BUSY, TRUE }          , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0024 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_HANDLE, TRUE }        , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0025 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_STATE, TRUE }         , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0026 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_STATE, TRUE }  , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0027 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_HANDLE, TRUE } , { { OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0028 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_PAR, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                              }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0029 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_DEV, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                              }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0030 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_IO, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                               }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0031 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_HANDLE, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                           }, { IMPDRV_EC_NG_SYSTEMERROR  } },
	/* R_IMPDRV_SetIrqGroup_0032 */ { { TEST_ADDR_READY_HANDLE    , TEST_ADDR_NOT_NULL , { IMPDRV_IRQ_GROUP_0    , 6                         , { { IMPDRV_CORE_TYPE_IMP     , 0 }      , [1] = { IMPDRV_CORE_TYPE_OCV    , 0 }      , [2] = { IMPDRV_CORE_TYPE_DMAC   , 0 }      , [3] = { IMPDRV_CORE_TYPE_IMP    , 1 }      , [4] = { IMPDRV_CORE_TYPE_CNN    , 0 }      , [5] = { IMPDRV_CORE_TYPE_DMAC   , 1 } } }  , { 5 }    , { OSAL_RETURN_OK, TRUE }            , { OSAL_RETURN_OK, TRUE }     , { { OSAL_RETURN_STATE, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK, OSAL_RETURN_OK }, {0, 0, 571, 0, 0, 2048 } , TRUE  }                            }, { IMPDRV_EC_NG_SYSTEMERROR  } }
};

#endif


