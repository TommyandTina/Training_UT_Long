/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : main.c
 * Version      : -
 * Product Name : impsample_dsp_cdnn
 * Device(s)    : -
 * Description  : IMP Sample Code for CDNN
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History      : Version DD.MM.YYYY Description
 *              : v0.0.1 03.10.2022 New Created 
 *              : v0.1.0 04.25.2022 Modify argument and input parameter to DSP
 *********************************************************************************************************************/
/*
=======================================================================================================================
 Includes   <System Includes> , "Project Includes"
=======================================================================================================================
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

/* IMP Framework */
#include "rcar-xos/impfw/r_impfw_version.h"

/* Demo */
#include "imp_demo.h"

/*
=======================================================================================================================
 Public Macro definitions
=======================================================================================================================
*/

/* IMPFWDEMO_XXX can configure by user */
/* declare use core */ 
#define IMPFWDEMO_DSPONLY_NUM   (1U)
#define IMPFWDEMO_DSPONLY_USECORES \
        {0, IMPFW_CORE_TYPE_DSP,       0u, 0, NULL }

#define IMPFWDEMO_DSPCNN_NUM   (2U)
#define IMPFWDEMO_DSPCNN_USECORES \
        {0, IMPFW_CORE_TYPE_DSP,       0u, 0, NULL }, \
        {1, IMPFW_CORE_TYPE_CNN,       0u, 0, NULL }

#define IMPFWDEMO_DSPCNNDMAC1_NUM   (3U)
#define IMPFWDEMO_DSPCNNDMAC1_USECORES \
        {0, IMPFW_CORE_TYPE_DMAC_SLIM, 0u, 0, NULL }, \
        {1, IMPFW_CORE_TYPE_DSP,       0u, 0, NULL }, \
        {2, IMPFW_CORE_TYPE_CNN,       0u, 0, NULL }

#define IMPFWDEMO_DSPCNNDMAC2_NUM   (4U)
#define IMPFWDEMO_DSPCNNDMAC2_USECORES \
        {0, IMPFW_CORE_TYPE_DMAC_SLIM, 0u, 0, NULL }, \
        {1, IMPFW_CORE_TYPE_DMAC_SLIM, 2u, 0, NULL }, \
        {2, IMPFW_CORE_TYPE_DSP,       0u, 0, NULL }, \
        {3, IMPFW_CORE_TYPE_CNN,       0u, 0, NULL }
//      {loop_num, core_type, core_num, pair_id, attrhandle}
//      When adding cores to be paired, they should be added before the DSP.
//      Start up order (loop_num) is n, 0, 1, 2,...,n-1 (n:last, n-1:DSP)
#define IMPFWDEMO_USECORE_MAX       (4U)

/* valid for instance 0,1,2 */
#define IMPFWDEMO_INSTANCE          (IMPFW_INSTANCE_0)

#define IMPFWDEMO_MIN_INFILE        (3U)
#define IMPFWDEMO_MAX_INFILE        (6U)             /* "QData", "Input image", "Input config", "Command List(CNN)", "Command List(SDMAC00)", "Command List(SDMAC10)" */
#define IMPFWDEMO_CDNN_WORKSIZE     (0x20000000U)    /* 512MB (T.B.D.) */
#define IMPFWDEMO_CDNN_PRG_DATA     (0x10000000U)    /* 256MB (Program), 256MB (Data) */
#define IMPFWDEMO_CDNN_DSP_BA       (0x40000000U)    /* 0x40000000 - 0xBFFFFFFF */
#define IMPFWDEMO_OSAL_MQ_TIMEOUT   (1800000U)       /* 30min */
#define IMPFWDEMO_CDNN_MAX_OUT      (0x01000000U)

/* macro used to log print */
#define LOGPRINT(ARG)               (void)(printf ARG, fflush(stdout))
#define DBGPRINT(ARG)               //(void)(printf ARG, fflush(stdout))


/* Fixed variables */
#define IMPFW_DSP_APPNUM            (1U)    /* 2 kind dsp sample */
#define IMPFW_WORKSIZE              (65536U)
#define IMPFW_MAIN_WORKSIZE         (IMPFW_WORKAREA_MAIN_SIZE)
#define IMPFW_ATTR_WORKSIZE         (IMPFW_WORKAREA_ATTR_SIZE * (IMPFWDEMO_USECORE_MAX + 1))
#define IMPFW_QUE_WORKSIZE          (IMPFW_WORKAREA_QUE_SIZE * (IMPFWDEMO_USECORE_MAX + 1))
#define IMPFW_DSP_WORKSIZE          (4096U)
#define IMPFW_DSP_ATTR_WORKSIZE     (160U * (IMPFWDEMO_USECORE_MAX + 1))
#define IMPFW_NUM_FILE              (IMPFWDEMO_MAX_INFILE + 1U)    /* Command List, Weight, Input image, Output image */
#define ALIGN_N(n, a)               (((n) % (a)) ? ((n) + ((a) - (n) % (a))) : (n))

/*
=======================================================================================================================
 Public type definitions
=======================================================================================================================
*/
typedef char char_t;

typedef struct
{
    uint32_t                  request_num;
    bool                      complete;
    st_impfw_core_info_t      core;
    st_osal_time_t            time_cb_start[16];
    st_osal_time_t            time_cb_trap[16];
} st_additional_data_t;

typedef struct
{
    e_impfw_callback_reason_t reason;      /* callback reason */
    int32_t                   code;        /* callback code   */
    uint32_t                  request_num; /* index of st_additional_data_t */
} st_callbackmessage_t;

typedef enum {
    ARG_QD = 0,     /* QData file */
    ARG_IM,         /* Input image file */
    ARG_IC,         /* Input image config file */
    ARG_CL_CNN,     /* Command List file */
    ARG_CL_SDMA0,   /* Command List file */
    ARG_CL_SDMA2,   /* Command List file */
    OUT_IM          /* Output image file */
} e_argtype_t;

typedef struct {
    uint32_t    addr;
    uint32_t    size;
} st_meminfo_t;

typedef struct{
    uint32_t shared_size;   /*! Size of shared buffer */
    uint32_t qdata_ptr;     /*! Pointer of QData */
    uint32_t input_ptr;     /*! Pointer of input image */
    //cdnnDatabufferParameters_t input_param;
    unsigned int width;     /*! Buffer width (in elements) */
    unsigned int height;    /*! Buffer height (in elements) */
    int dataType;           /*! Buffer element data type (E_CDNN_BUFFER_DATA_TYPE) */
    unsigned int bytePerElement;
    unsigned int depth;     /*! Element size (in bits) */
    unsigned int nChannels; /*! Number of channels */
    float scaleFactor;      /*! Represents the scale factor in which the fixed point output is represented.*/
    unsigned int dataOrder; /*! Enum of data order: NCHW, NCWH, and so on*/
    unsigned int nInputs;   /*! Number of input maps */
    int resizeWidth;        /*! Buffer width (in elements) */
    int resizeHeight;       /*! Buffer height (in elements) */
    int eSkipPreprocess;    /*! Determines if the buffer skip preprocess is required (E_CDNN_BUFFER_SKIP_PREPROCESS)*/
    int eResize;            /*! Determines if the input requires resize (E_CDNN_BUFFER_IMAGE_RESIZE) */
    int eChannelSwap;       /*! Determines if swap channels is required (E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP)*/
    int eTranspose;         /*! Determines if buffer transpose is required (E_CDNN_BUFFER_IMAGE_TRANSPOSE) */
    int padding[4];         /*! Margin dimensions (in elements), where 0 = left, 1 = right, 2 = top, 3 = bottom */
    uint32_t pBufferName;   /*! The name of the buffer */
    unsigned int bufferId;  /*! The buffer parameter index in the layer it belongs to (for example: filename "dataa_0" bufferId=0)  */
    unsigned int bufferType;/*! Buffer type: data type indicating that it is a buffer to store data, for example, zero-point data (mean data)*/
    float f32ZeroPoint;     /*! Buffer zero-point value */
    char BufferName[64];    /*! Buffer for pBuffName */
    uint32_t conv_format;   /*! Convert input format from NHWC to NCHW */
    uint32_t output_ptr;    /*! Pointer of output buffer */
    uint32_t output_buff_size;  /*! Size of output buffer */
    uint32_t output_size;   /*! Size of inference result */
    uint32_t output_fp32;	/*! Output froat32 format */
    uint32_t do_debug;      /*! Enable CDNN debug option (Not supported) */
    uint32_t debug_level;	/*! debug level */
    uint32_t debug_param;	/*! debug param */
    uint32_t do_profile;    /*! Enable CDNN profile (Not supported) */
    uint32_t clptr_cnnip;	/*! cl address of CNN-IP (temp) */
    uint32_t clptr_dma0ip	/*! cl address of CNN-IP (temp) */;
    uint32_t clptr_dma1ip;	/*! cl address of CNN-IP (temp) */
    uint32_t num_queue;		/*! number of input queue */
} st_cdnn_shared_data_t;

/*
=======================================================================================================================
 Public global variables
=======================================================================================================================
*/
extern uint8_t expected_result[256];
/*
=======================================================================================================================
 Private global variables
=======================================================================================================================
*/
/* Work memory for IMPFW and Driver */
static uint64_t  s_workmain_impfwinit[IMPFW_MAIN_WORKSIZE / sizeof(uint64_t)];
static uint64_t  s_workattr_impfwinit[IMPFW_ATTR_WORKSIZE / sizeof(uint64_t)];
static uint64_t  s_workque_impfwinit[IMPFW_QUE_WORKSIZE / sizeof(uint64_t)];
static uint64_t  s_workmain_dspinit[IMPFW_DSP_WORKSIZE / sizeof(uint64_t)];
static uint64_t  s_workattr_dspinit[IMPFW_DSP_ATTR_WORKSIZE / sizeof(uint64_t)];

/* callback parameter for application */
static osal_mq_handle_t     s_queuecallback = OSAL_MQ_HANDLE_INVALID;
static uint32_t             s_request_num;
static st_additional_data_t s_additional_data[IMPFWDEMO_USECORE_MAX + 1];

/* File input/output */
static FILE                *s_fp[IMPFWDEMO_MAX_INFILE];
static uint32_t             s_file_size[IMPFW_NUM_FILE];
/* Allocation the each data in buffer */
static uint32_t             s_ofs_size[IMPFW_NUM_FILE];
static uint32_t             s_hw_addr[IMPFW_NUM_FILE];
static uint32_t             s_dump_outfile = 0;
static uint32_t             s_inf_num = 1U;
static int32_t              s_num_arg;
static int32_t              s_usecore_num;
static st_usecore_info_t   *s_usecore_info;
static st_usecore_info_t    s_usecore_info1[IMPFWDEMO_DSPONLY_NUM] = {
    IMPFWDEMO_DSPONLY_USECORES
};
static st_usecore_info_t    s_usecore_info2[IMPFWDEMO_DSPCNN_NUM] = {
    IMPFWDEMO_DSPCNN_USECORES
};
static st_usecore_info_t    s_usecore_info3[IMPFWDEMO_DSPCNNDMAC1_NUM] = {
    IMPFWDEMO_DSPCNNDMAC1_USECORES
};
static st_usecore_info_t    s_usecore_info4[IMPFWDEMO_DSPCNNDMAC2_NUM] = {
    IMPFWDEMO_DSPCNNDMAC2_USECORES
};
static st_impfw_core_info_t s_impfw_pair_core[IMPFWDEMO_USECORE_MAX - 1];
static osal_milli_sec_t s_execute_timeout = IMPFWDEMO_OSAL_MQ_TIMEOUT;
/*
=======================================================================================================================
 Private function prototypes
=======================================================================================================================
*/
static int32_t callbackfunc (e_impfw_callback_reason_t reason,
                            const st_impfw_core_info_t * const core,
                            int32_t code,
                            void * const callbackargs);
static void callbackfunc_fatal (e_impfw_fatalcode_t  reason, uint32_t value);
static const char_t * reason_tostr (e_impfw_callback_reason_t reason);
static const char_t * fatal_code_tostr(e_impfw_fatalcode_t code);
static int32_t imp_demo (void);
static int32_t set_shared_data(st_buffer_t *dsp_shared_buffer, st_cdnn_shared_data_t **shared_ptr);
static int32_t parse_input_config(FILE *fp, st_cdnn_shared_data_t *shared_ptr);
/*
=======================================================================================================================
 Public function definitions
=======================================================================================================================
*/
/**********************************************************************************************************************
 * Function Name: main
 * Description  : main
 * Arguments    : int argc
 *                char *argv[]
 * Return Value : int
 *********************************************************************************************************************/
int main(int argc, char * argv[])
{
    /* Return value from functions */
    e_osal_return_t          ret_osal_init   = OSAL_RETURN_FAIL;
    e_osal_return_t          ret_osal_mq     = OSAL_RETURN_FAIL;
    e_osal_return_t          ret_osal_mmngr  = OSAL_RETURN_FAIL;
    int32_t                  ret_sample      = IMPDEMO_OK;
    uint32_t                 i;
    struct stat              stbuf;
    s_num_arg = argc - 1;
    s_usecore_num = s_num_arg - 2;

    /* Check arguments and open the input files */
    if ((IMPFWDEMO_MIN_INFILE <= s_num_arg) &&
        (s_num_arg <= IMPFWDEMO_MAX_INFILE))
    {
        /* Open all files */
        for (i = 0U; i < s_num_arg; i++) {
            s_fp[i] = fopen(argv[i+1], "rb");
            if (s_fp[i] != NULL)
            {
                if (stat(argv[i+1], &stbuf) == -1) {
                    ret_sample = IMPDEMO_NG;
                }
                else
                {
                    /* set file size */
                    s_file_size[i] = (uint32_t)stbuf.st_size;
                }
            }
            else
            {
                 LOGPRINT(("File not found %s\n", argv[i+1]));
                 ret_sample = IMPDEMO_NG;
            }
        }
        s_file_size[OUT_IM] = s_file_size[ARG_IM];
        /* If the file does not exist, it will exit with an error. */
        if (ret_sample == IMPDEMO_NG)
        {
            for (i = 0U; i < s_num_arg; i++) {
                if (s_fp[i] != NULL) fclose(s_fp[i]);
            }
            return ret_sample;
        }
    }
    else
    {
        /* usage error */
        LOGPRINT(("usage: impsample_dsp_cdnn <QData file> <Image file> <Image config> <CL file(cnn)> <CL file(sdmac)>\n"));
        LOGPRINT(("DSP Only      <QData file> <Image file> <Image config>\n"));
        LOGPRINT(("DSP+CNN       <QData file> <Image file> <Image config> <CL file(cnn)>\n"));
        LOGPRINT(("DSP+CNN+SDMAC <QData file> <Image file> <Image config> <CL file(cnn)> <CL file(sdmac)>\n"));
        return IMPDEMO_NG;
    }

    /* init */
    ret_osal_init = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != ret_osal_init)
    {
        ret_sample = IMPDEMO_NG;
        LOGPRINT(("<< [ERROR] R_OSAL_Initialize  : code %d>>\n",
                    (int32_t)ret_osal_init)); /* cast to int32_t */
    }
    else
    {
        /* Open Memory Maneger */
        ret_osal_mmngr = mmngr_open();
        if(OSAL_RETURN_OK != ret_osal_mmngr)
        {
            ret_sample = IMPDEMO_NG;
            LOGPRINT(("<< [ERROR] mmngr_open  : code %d>>\n",
                (int32_t)ret_osal_mmngr)); /* cast to int32_t */
        }
        else
        {
            /* Create Message Queue for callback */
            st_osal_mq_config_t      mq_config;
            ret_osal_mq = R_OSAL_MqInitializeMqConfigSt(&mq_config);
            if (OSAL_RETURN_OK != ret_osal_mq)
            {
                ret_sample   = IMPDEMO_NG;
                LOGPRINT(("<< [ERROR] R_OSAL_MqInitializeMqConfigSt  : code %d>>\n",
                            (int32_t)ret_osal_mq)); /* cast to int32_t */
            }
            else
            {
                mq_config.max_num_msg = 63;
                mq_config.msg_size    = sizeof(st_callbackmessage_t);
                ret_osal_mq = R_OSAL_MqCreate(&mq_config, mq_id_for_callback[0], &s_queuecallback);
                if (OSAL_RETURN_OK != ret_osal_mq)
                {
                    ret_sample = IMPDEMO_NG;
                    LOGPRINT(("<< [ERROR] R_OSAL_MqCreate  : code %d>>\n",
                                (int32_t)ret_osal_mq)); /* cast to int32_t */
                }
            }
        }

        if(IMPDEMO_OK == ret_sample)
        {
            LOGPRINT(("\nDSP Demo start\n\n"));
            ret_sample = imp_demo();
            if(IMPDEMO_OK != ret_sample)
            {
                LOGPRINT(("<< [ERROR] dsp_demo  : code %d>>\n",
                            (int32_t)ret_sample)); /* cast to int32_t */
            }
            LOGPRINT(("\nDSP Demo end\n\n"));
        }

        for (i = 0U; i < s_num_arg; i++) {
            if (s_fp[i] != NULL) fclose(s_fp[i]);
        }

        /* quit */
        if(OSAL_RETURN_OK == ret_osal_mq)
        {
            /* Delete Message Queue */
            ret_osal_mq = R_OSAL_MqDelete(s_queuecallback);
            if (OSAL_RETURN_OK != ret_osal_mq)
            {
                ret_sample = IMPDEMO_NG;
                LOGPRINT(("<< [ERROR] R_OSAL_MqDelete  : code %d>>\n",
                            (int32_t)ret_osal_mq)); /* cast to int32_t */
            } 
        }

        if(OSAL_RETURN_OK == ret_osal_mmngr)
        {
            /* Close Memory Manager */
            ret_osal_mmngr = mmngr_close();
            if (OSAL_RETURN_OK != ret_osal_mmngr)
            {
                LOGPRINT(("<< [ERROR] mmngr_close  : code %d>>\n",
                            (int32_t)ret_osal_mmngr)); /* cast to int32_t */
                ret_sample = IMPDEMO_NG;
            }
        }

        /* Deinitialize OSAL */
        ret_osal_init = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != ret_osal_init)
        {
            ret_sample = IMPDEMO_NG;
            LOGPRINT(("<< [ERROR] R_OSAL_Deinitialize  : code %d>>\n",
                        (int32_t)ret_osal_init)); /* cast to int32_t */
        }
    }
    LOGPRINT(("impsample_dsp_cdnn return =  %d (OK:0, NG:else)\n", ret_sample));
    return ret_sample;
}
/**********************************************************************************************************************
 * End of function main
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: imp_demo
 * Description  : sample of imp core
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
static int32_t imp_demo(void)
{
    /* Return value from functions */
    int32_t                 ret_demo;
    e_osal_return_t         ret_osal  = OSAL_RETURN_OK;
    e_impfw_api_retcode_t   impfw_init = IMPFW_EC_NG_SYSTEMERROR;
    e_impfw_api_retcode_t   impfw_ret = IMPFW_EC_NG_SYSTEMERROR;
    st_impfw_dsp_info_t     dsp_info;
    /*---- DSP Shared Memory ----*/
    st_buffer_t             dsp_shared_buffer;
    st_buffer_t             alloc_buffer;
    st_buffer_t             dummy_buffer;
    uint32_t                loop_num;
    st_cdnn_shared_data_t   *shared_data;
    st_impfw_core_info_t    core;
    uint32_t loop_dsp_app;
    impfw_ctrl_handle_t      impfwctl;
    uint32_t loop_pair_core;
    uint32_t loop_pair_core_max = 0U;
    uint32_t pair_core = 0U;
    st_impfw_core_info_t     gCoremap[IMPFW_COREMAP_MAXID]={0};
    
    if (s_usecore_num >= 2) {
        s_usecore_info = s_usecore_info2;
        loop_pair_core_max = s_usecore_num;
        gCoremap[0].core_type = IMPFW_CORE_TYPE_CNN;
        gCoremap[0].core_num  = 0U;
        gCoremap[9].core_type = IMPFW_CORE_TYPE_DSP;
        gCoremap[9].core_num  = 0U;
        if (s_usecore_num >= 3) {
            s_usecore_info = s_usecore_info3;
            gCoremap[1].core_type = IMPFW_CORE_TYPE_DMAC_SLIM;
            gCoremap[1].core_num  = 0U;
        }
        if (s_usecore_num >= 4) {
            s_usecore_info = s_usecore_info4;
            gCoremap[2].core_type = IMPFW_CORE_TYPE_DMAC_SLIM;
            gCoremap[2].core_num  = 2U;
        }
    }
    else
    {
        s_usecore_info = s_usecore_info1;
    }

    /* Clear buffer value */
    memset(&alloc_buffer, 0, sizeof(alloc_buffer));
    memset(&dsp_shared_buffer, 0, sizeof(dsp_shared_buffer));
    memset(&dsp_info, 0, sizeof(dsp_info));
    (void)buffer_alloc(&dummy_buffer, IMPFWDEMO_CDNN_DSP_BA); /* for DSP ISS, not used */

    /* Allocate memory for DSP Shared memory */
    ret_demo = buffer_alloc(&alloc_buffer, IMPFWDEMO_CDNN_WORKSIZE + 0x1000U);
    if(IMPDEMO_OK == ret_demo)
    {
        /* buffer alignment */
        dsp_shared_buffer.buffer     = alloc_buffer.buffer;
        dsp_shared_buffer.buffersize = IMPFWDEMO_CDNN_WORKSIZE;
        dsp_shared_buffer.hw_addr    = ALIGN_N(alloc_buffer.hw_addr, 0x1000U);
        dsp_shared_buffer.cpu_addr   = (void*)((uintptr_t)alloc_buffer.cpu_addr 
                                                + (dsp_shared_buffer.hw_addr - alloc_buffer.hw_addr));
    }

    /*----- IMP Framework Setting -----*/
    if(IMPDEMO_OK == ret_demo)
    {
        /* IMP Framework Initialization */
        impfw_init = imp_demo_impfw_init(&impfwctl);
        if (IMPFW_EC_OK != impfw_init)
        {
            ret_demo = IMPDEMO_NG;
            LOGPRINT(("<< [ERROR] imp_demo_impfw_init  : code %d>>\n",
                            (int32_t)impfw_ret)); /* cast to int32_t */
        }
        else
        {
            /*---- Initialize IMP Framework for DSP ----*/
            st_impfw_init_dspdata_t      impfw_dspinit;

            memset(&s_workmain_dspinit, 0, sizeof(s_workmain_dspinit));
            memset(&s_workattr_dspinit, 0, sizeof(s_workattr_dspinit));
            impfw_dspinit.work_area_info[IMPFW_WORKAREA_TYPE_MAIN].p_work_addr = (void *)&s_workmain_dspinit; /* cast to void * */
            impfw_dspinit.work_area_info[IMPFW_WORKAREA_TYPE_MAIN].work_size   = sizeof(s_workmain_dspinit);
            impfw_dspinit.work_area_info[IMPFW_WORKAREA_TYPE_ATTR].p_work_addr = (void *)&s_workattr_dspinit; /* cast to void * */
            impfw_dspinit.work_area_info[IMPFW_WORKAREA_TYPE_ATTR].work_size   = sizeof(s_workattr_dspinit);

            impfw_ret = R_IMPFW_InitDsp(impfwctl, &impfw_dspinit);
            if (IMPFW_EC_OK != impfw_ret)
            {
                ret_demo = IMPDEMO_NG;
                LOGPRINT(("<< [ERROR] R_IMPFW_InitDsp  : code %d>>\n",
                            (int32_t)impfw_ret)); /* cast to int32_t */
            }
            else
            {
                LOGPRINT(("[SUCCESS] R_IMPFW_InitDsp\n"));
            }
        }

        if (IMPFW_EC_OK == impfw_ret)
        {
            /* DSP and CDNN Initialization */
            impfw_attr_handle_t dspAttrHandle;
            e_impfw_api_retcode_t   dsp_ret = IMPFW_EC_NG_SYSTEMERROR;
            ret_demo = set_shared_data(&dsp_shared_buffer, &shared_data);
    		if (ret_demo == IMPDEMO_OK)
    		{
        		LOGPRINT(("[SUCCESS] set_shared_data is OK.\n"));
                core.core_type = IMPFW_CORE_TYPE_DSP;
                core.core_num  = 0;
                dsp_info.dsp_data.addr_phys = (uintptr_t)dsp_shared_buffer.hw_addr;
                dsp_info.dsp_data.size      = dsp_shared_buffer.buffersize;
                dsp_ret = R_IMPFW_AttrInit(impfwctl, &core, &dspAttrHandle);
                if (dsp_ret != IMPFW_EC_OK)
                {
                    ret_demo = IMPDEMO_NG;
                    LOGPRINT(("<< [ERROR] R_IMPFW_AttrInit  : code %d, core_type:%d, core_num:%d>>\n",
                                    (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                }
                else
                {
                    LOGPRINT(("[SUCCESS] R_IMPFW_AttrInit   core_type:%d, core_num:%d\n",
                                    core.core_type, core.core_num));
                }
            }
            if (dsp_ret == IMPFW_EC_OK)
            {
	            dsp_ret = R_IMPFW_AttrSetDsp(dspAttrHandle, &dsp_info, IMPFW_REQ_PRIORITY_0);
                if (dsp_ret != IMPFW_EC_OK)
                {
                    ret_demo = IMPDEMO_NG;
                    LOGPRINT(("<< [ERROR] R_IMPFW_AttrSetDsp  : code %d, core_type:%d, core_num:%d>>\n",
                                    (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                }
                else
                {
                    LOGPRINT(("[SUCCESS] R_IMPFW_AttrSetDsp   core_type:%d, core_num:%d\n",
                                    core.core_type, core.core_num));
                }
            }
            if (dsp_ret == IMPFW_EC_OK)
            {
	            dsp_ret = imp_demo_impfw_execute(impfwctl, &core, dspAttrHandle);
                if (dsp_ret != IMPFW_EC_OK)
                {
                    ret_demo = IMPDEMO_NG;
                    LOGPRINT(("<< [ERROR] imp_demo_impfw_execute  : code %d, core_type:%d, core_num:%d>>\n",
                                    (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                }
                else
                {
                    LOGPRINT(("    for DSP App core_type:%d, core_num:%d\n",   core.core_type, core.core_num));
                }
            }
			else
			{
        		LOGPRINT(("[ERROR] set_shared_data faile. ret_demo=%d\n", ret_demo));
        	}

            /* initialize */
            for(loop_num = 0U; loop_num < s_usecore_num; loop_num++)
            {
                core.core_type = s_usecore_info[loop_num].core_type;
                core.core_num  = s_usecore_info[loop_num].core_num;

                LOGPRINT(("--- Start core_type %d, core_num %d Setting ---\n",
                                    core.core_type, core.core_num));

                /* IMP Attr Initialization */
                impfw_ret = R_IMPFW_AttrInit(impfwctl, &core, &s_usecore_info[loop_num].attrhandle);
                if (IMPFW_EC_OK != impfw_ret)
                {
                    ret_demo = IMPDEMO_NG;
                    LOGPRINT(("<< [ERROR] R_IMPFW_AttrInit  : code %d, core_type:%d, core_num:%d>>\n",
                                    (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                }
                else
                {
                    LOGPRINT(("[SUCCESS] R_IMPFW_AttrInit   core_type:%d, core_num:%d\n",
                                    core.core_type, core.core_num));
                }
            }
        }
    }

    shared_data->num_queue = s_inf_num;
    /* set and execute */
    for (loop_dsp_app = 0U; loop_dsp_app < IMPFW_DSP_APPNUM; loop_dsp_app++)
    {
        if ((IMPDEMO_OK == ret_demo) && (IMPFW_EC_OK == impfw_ret))
        {
            for(loop_num = 0U; loop_num < s_usecore_num; loop_num++)
            {
                core.core_type = s_usecore_info[loop_num].core_type;
                core.core_num  = s_usecore_info[loop_num].core_num;

                /* --- For DSP setting --- */
                if (core.core_type == IMPFW_CORE_TYPE_DSP)
                {
                    if ((IMPDEMO_OK == ret_demo) && (IMPFW_EC_OK == impfw_ret))
                    {
                        /*----- DSP's param Setting -----*/
                        dsp_info.dsp_data.addr_phys = dsp_shared_buffer.hw_addr;
                        dsp_info.dsp_data.size      = dsp_shared_buffer.buffersize;

                        /* [1st Application] IMP Attr Setting for DSP */
                        impfw_ret = R_IMPFW_AttrSetDsp(
                                                s_usecore_info[loop_num].attrhandle,
                                                &dsp_info,
                                                IMPFW_REQ_PRIORITY_0);
                        if (IMPFW_EC_OK != impfw_ret)
                        {
                            ret_demo = IMPDEMO_NG;
                            LOGPRINT(("<< [ERROR] R_IMPFW_AttrSetDsp for 1st App  : code %d, core_type:%d, core_num:%d>>\n",
                                            (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                        }
                        else
                        {
                            LOGPRINT(("[SUCCESS] R_IMPFW_AttrSetDsp for 1st App core_type:%d, core_num:%d\n",
                                            core.core_type, core.core_num));
                        }
                    }
                }

                /* --- For CNN setting --- */
                if (core.core_type == IMPFW_CORE_TYPE_CNN)
                {
                    if (IMPFW_EC_OK == impfw_ret)
                    {
                        impfw_ret = R_IMPFW_AttrSetCl(
                                        s_usecore_info[loop_num].attrhandle,
                                        (uintptr_t)s_hw_addr[ARG_CL_CNN],
                                        s_file_size[ARG_CL_CNN],
                                        IMPFW_REQ_PRIORITY_0);
                        if (IMPFW_EC_OK == impfw_ret)
                        {
                            LOGPRINT(("[SUCCESS] R_IMPFW_AttrSetCl (CNN)\n"));
                        }
                        else
                        {
                            ret_demo = IMPDEMO_NG;
                            LOGPRINT(("<< [ERROR] R_IMPFW_AttrSetCL (CNN)  : code %d>>\n",
                                            (int32_t)impfw_ret)); /* cast to int32_t */
                        }
                    }
                }

                /* --- For SDMAC setting --- */
                if (core.core_type == IMPFW_CORE_TYPE_DMAC_SLIM)
                {
                    e_argtype_t cur_type = ARG_CL_SDMA0;
                    if (s_usecore_info[loop_num].core_num == 2)
                    {
                        cur_type = ARG_CL_SDMA2;
                    }
                    if (IMPFW_EC_OK == impfw_ret)
                    {
                        impfw_ret = R_IMPFW_AttrSetCl(
                                        s_usecore_info[loop_num].attrhandle,
                                        (uintptr_t)s_hw_addr[cur_type],
                                        s_file_size[cur_type],
                                        IMPFW_REQ_PRIORITY_0);
                        if (IMPFW_EC_OK == impfw_ret)
                        {
                            LOGPRINT(("[SUCCESS] R_IMPFW_AttrSetCl (SDMAC)\n"));
                        }
                        else
                        {
                            ret_demo = IMPDEMO_NG;
                            LOGPRINT(("<< [ERROR] R_IMPFW_AttrSetCL (SDMAC)  : code %d>>\n",
                                            (int32_t)impfw_ret)); /* cast to int32_t */
                        }
                    }
                }

                /* pair configuration */
                if (loop_pair_core_max > 0) {
                    for(loop_pair_core = 0; loop_pair_core < loop_pair_core_max; loop_pair_core++)
                    {
                        if(loop_pair_core == loop_num)
                        {
                            continue;
                        }
                        s_impfw_pair_core[pair_core].core_type = s_usecore_info[loop_pair_core].core_type;
                        s_impfw_pair_core[pair_core].core_num  = s_usecore_info[loop_pair_core].core_num;
                        pair_core++;
                    }
                    pair_core = 0;
                    impfw_ret = R_IMPFW_AttrSetPair(
                                            s_usecore_info[loop_num].attrhandle,
                                            s_usecore_info[loop_num].pair_id,
                                            s_usecore_num-1,
                                            s_impfw_pair_core);
                    if (IMPFW_EC_OK != impfw_ret)
                    {
                        ret_demo = IMPDEMO_NG;
                        LOGPRINT(("<< [ERROR] R_IMPFW_AttrSetPair  : code %d, core_type:%d, core_num:%d>>\n",
                                    (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                    }
                    else
                    {
                        LOGPRINT(("[SUCCESS] R_IMPFW_AttrSetPair\n"));
                    }

                    impfw_ret = R_IMPFW_AttrSetCoremap(s_usecore_info[loop_num].attrhandle, gCoremap);
                    if (IMPFW_EC_OK != impfw_ret)
                    {
                        ret_demo = IMPDEMO_NG;
                        LOGPRINT(("<< [ERROR] R_IMPFW_AttrSetCoremap  : code %d, core_type:%d, core_num:%d>>\n",
                                        (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                    }
                    else
                    {
                        LOGPRINT(("[SUCCESS] R_IMPFW_AttrSetCoremap\n"));
                    }
                }

                /* [DSP Application] Execute IMPFW */
                impfw_ret = imp_demo_impfw_execute(impfwctl, &core, s_usecore_info[loop_num].attrhandle);
                if (IMPFW_EC_OK != impfw_ret)
                {
                    ret_demo = IMPDEMO_NG;
                    LOGPRINT(("<< [ERROR] imp_demo_impfw_execute for App  : code %d, core_type:%d, core_num:%d>>\n",
                                    (int32_t)impfw_ret, core.core_type, core.core_num)); /* cast to int32_t */
                }
                else
                {
                    LOGPRINT(("    for DSP App core_type:%d, core_num:%d\n",   core.core_type, core.core_num));
                }

                LOGPRINT(("--- End   core_type %d, core_num %d Setting ---\n",
                                core.core_type, core.core_num));
            }//End of loop by s_usecore_num

            /* Wait until TRAP from Core */
            ret_osal = imp_demo_impfw_waitcallback();
            if (OSAL_RETURN_OK != ret_osal)
            {
                ret_demo = IMPDEMO_NG;
                LOGPRINT(("<< [ERROR] imp_demo_impfw_waitcallback  : code %d>>\n",
                    (int32_t)impfw_ret)); /* cast to int32_t */

            }
            LOGPRINT(("--- DSP Application End   ---\n"));
            {   /* Performance from start callback to trap callback */
                osal_nano_sec_t time_diff;
                osal_nano_sec_t time_diff_ns;
                osal_nano_sec_t time_diff_ms;
                for (loop_num = 0U; loop_num < s_usecore_num; loop_num++)
                {
                    R_OSAL_ClockTimeCalculateTimeDifference(
                                &s_additional_data[loop_num].time_cb_trap[s_additional_data[loop_num].core.core_type],
                                &s_additional_data[loop_num].time_cb_start[s_additional_data[loop_num].core.core_type],
                                &time_diff);
                    time_diff_ms = time_diff / 1000000;
                    time_diff_ns = time_diff % 1000000;
                LOGPRINT(("CoreType = %d, CoreNum = %d, Execution time = %6lu ms, %3u us\n", 
                                s_additional_data[loop_num].core.core_type,
                                s_additional_data[loop_num].core.core_num,
                                (unsigned long)time_diff_ms,
                            (unsigned int)time_diff_ns/1000));
                }
            }

        	buffer_sync(&dsp_shared_buffer);
            {/* Check Output Data */
                void *out_buff = (void *)((uintptr_t)dsp_shared_buffer.cpu_addr + 
                                   ((uintptr_t)s_hw_addr[OUT_IM] - dsp_shared_buffer.hw_addr));
                if (s_dump_outfile != 0)
                {
                    /* Dump Output Data */
                    char file_name[32];
                    FILE *image_out;
                    sprintf(file_name, "%s%d%s", "output_data", loop_dsp_app, ".raw");
                    image_out = fopen(file_name, "wb");
                    fwrite((void *)out_buff, shared_data->output_size, 1, image_out);
                    fclose(image_out);
                }
            }
        }
    }

    /* IMP Framework Finalization */
    if (IMPFW_EC_OK == impfw_ret)
    {
        impfw_ret = R_IMPFW_Quit(impfwctl);
        if (IMPFW_EC_OK != impfw_ret)
        {
            ret_demo = IMPDEMO_NG;
            LOGPRINT(("<< [ERROR] R_IMPFW_Quit  : code %d>>\n",
                (int32_t)impfw_ret)); /* cast to int32_t */
        }
        else
        {
            LOGPRINT(("[SUCCESS] R_IMPFW_Quit\n"));
        }
    }

    /*---- Deallocate memory for External Memory ----*/
    if (NULL != alloc_buffer.cpu_addr)
    {
        ret_osal = buffer_free(&alloc_buffer);
        if (OSAL_RETURN_OK != ret_osal)
        {
            ret_demo = IMPDEMO_NG;
        }
    }
    if (NULL != dummy_buffer.cpu_addr)
    {
        ret_osal = buffer_free(&dummy_buffer);
        if (OSAL_RETURN_OK != ret_osal)
        {
            ret_demo = IMPDEMO_NG;
        }
    }

    return ret_demo;
}
/**********************************************************************************************************************
 * End of function imp_demo
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: imp_demo_impfw_init
 * Description  : sample of imp core
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
e_impfw_api_retcode_t imp_demo_impfw_init(impfw_ctrl_handle_t *p_impfwctl)
{
    e_impfw_api_retcode_t    impfw_ret;
    st_impfw_initdata_t      impfwinit;

    /* Setting Initialzie Cores */
    st_impfw_core_info_t impfw_usecore[IMPFWDEMO_USECORE_MAX];
    uint32_t loop_num;
    for(loop_num = 0; loop_num < s_usecore_num; loop_num++)
    {
        impfw_usecore[loop_num].core_type = s_usecore_info[loop_num].core_type;
        impfw_usecore[loop_num].core_num  = s_usecore_info[loop_num].core_num;
    }

    /*----- IMP Framework Initialization -----*/
    memset(&impfwinit,            0, sizeof(impfwinit));
    memset(&s_workmain_impfwinit, 0, sizeof(s_workmain_impfwinit));
    memset(&s_workattr_impfwinit, 0, sizeof(s_workattr_impfwinit));
    memset(&s_workque_impfwinit,  0, sizeof(s_workque_impfwinit));

    impfwinit.work_area_info[IMPFW_WORKAREA_TYPE_MAIN].p_work_addr = (void *)&s_workmain_impfwinit; /* cast to void * */
    impfwinit.work_area_info[IMPFW_WORKAREA_TYPE_MAIN].work_size   = sizeof(s_workmain_impfwinit);
    impfwinit.work_area_info[IMPFW_WORKAREA_TYPE_ATTR].p_work_addr = (void *)&s_workattr_impfwinit; /* cast to void * */
    impfwinit.work_area_info[IMPFW_WORKAREA_TYPE_ATTR].work_size   = sizeof(s_workattr_impfwinit);
    impfwinit.work_area_info[IMPFW_WORKAREA_TYPE_QUE] .p_work_addr = (void *)&s_workque_impfwinit; /* cast to void * */
    impfwinit.work_area_info[IMPFW_WORKAREA_TYPE_QUE] .work_size   = sizeof(s_workque_impfwinit);

    impfwinit.instance_num  = IMPFWDEMO_INSTANCE;
    impfwinit.use_core_num  = s_usecore_num;
    impfwinit.core_info     = impfw_usecore;
    impfwinit.callback_func_fatal = callbackfunc_fatal;
    impfwinit.fw_resource.max_queue_num  = s_usecore_num + 1;
    impfwinit.fw_resource.max_msg_num    = s_usecore_num * 3 + 1;
    impfwinit.fw_resource.msg_id[0]      = mq_id_for_send_msg[0];
    impfwinit.fw_resource.msg_id[1]      = mq_id_for_receive_msg[0];
    impfwinit.fw_resource.mutex_id[0]    = mutex_id_for_core_state[0];
    impfwinit.fw_resource.mutex_id[1]    = mutex_id_for_attr_state[0];
    impfwinit.fw_resource.mutex_id[2]    = mutex_id_for_impfw_api[0];
    impfwinit.fw_resource.task_id[0]     = thread_id_for_impfw[0];
    impfwinit.fw_resource.timeout        = 10000u;
    impfwinit.fw_resource.task_priority  = OSAL_THREAD_PRIORITY_TYPE6;
    impfwinit.fw_resource.task_stacksize = 8192u;
    impfwinit.drv_resource.mutex_id      = mutex_id_for_drv[0];
    impfwinit.drv_resource.mutex_timeout = 10000u;
    impfwinit.drv_resource.int_priority  = OSAL_INTERRUPT_PRIORITY_TYPE7;

    impfw_ret = R_IMPFW_Init(&impfwinit, p_impfwctl);
    if (IMPFW_EC_OK != impfw_ret)
    {
        LOGPRINT(("<< [ERROR] R_IMPFW_Init  : code %d>>\n",
            (int32_t)impfw_ret)); /* cast to int32_t */
    }
    else
    {
        LOGPRINT(("[SUCCESS] R_IMPFW_Init\n"));
    }

    return impfw_ret;
}
/**********************************************************************************************************************
 * End of function imp_demo_impfw_init
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: imp_demo_impfw_execute
 * Description  : initialize impfw_ctrl_handle
 * Arguments    : impfw_ctrl_handle_t p_impfwctl
 *              : st_impfw_core_info_t *p_core
 *              : impfw_attr_handle_t impfw_attr
 * Return Value : e_impfw_api_retcode_t
 *********************************************************************************************************************/
e_impfw_api_retcode_t imp_demo_impfw_execute(impfw_ctrl_handle_t impfwctl, st_impfw_core_info_t *p_core, impfw_attr_handle_t impfw_attr)
{
    e_impfw_api_retcode_t    impfw_ret;
    void *                   callbackargs = &s_additional_data[s_request_num];

    s_additional_data[s_request_num].complete       = false;
    s_additional_data[s_request_num].request_num    = s_request_num;
    s_additional_data[s_request_num].core.core_type = p_core->core_type;
    s_additional_data[s_request_num].core.core_num  = p_core->core_num;

    impfw_ret = R_IMPFW_Execute(impfwctl, p_core, impfw_attr, callbackfunc, callbackargs);
    if (IMPFW_EC_OK != impfw_ret)
    {
        LOGPRINT(("<< [ERROR] R_IMPFW_Execute  : code %d>>",
            (int32_t)impfw_ret)); /* cast to int32_t */
    }
    else
    {
        s_request_num++;
        LOGPRINT(("[SUCCESS] R_IMPFW_Execute"));
    }

    return impfw_ret;
}

/**********************************************************************************************************************
 * End of function imp_demo_impfw_execute
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: imp_demo_impfw_waitcallback
 * Description  : wait all callback during 5sec.
 * Arguments    : None
 * Return Value : e_impfw_api_retcode_t
 *********************************************************************************************************************/
e_osal_return_t imp_demo_impfw_waitcallback(void)
{
    e_osal_return_t       ret_osal = OSAL_RETURN_FAIL;
    st_callbackmessage_t  message;
    st_additional_data_t  *data;
    uint32_t index;
    uint32_t complete_count;

    complete_count = 0U;
    while(complete_count < s_request_num)
    {
        /* Wait to receive messages for a specified period of time */
        st_osal_mq_config_t mq_config;
        ret_osal = R_OSAL_MqGetConfig(s_queuecallback, &mq_config);
        ret_osal = R_OSAL_MqReceiveForTimePeriod(
                                            s_queuecallback, s_execute_timeout,
                                            &message, mq_config.msg_size);
        if (OSAL_RETURN_OK != ret_osal)
        {
            LOGPRINT(("<< [ERROR] R_OSAL_MqReceiveForTimePeriod  : code %d>>\n",
                (int32_t)ret_osal)); /* cast to int32_t */
            break;
        }
        else
        {
            index = message.request_num;
            data  = &s_additional_data[index];
            LOGPRINT(("CallBack : status %s, Core type %d, Core num %d, callbackargs s_additional_data[%d] %d\n",
                reason_tostr(message.reason),
                (int32_t)(data->core.core_type), /* cast to int32_t */
                (int32_t)(data->core.core_num),  /* cast to int32_t */
                index,
                message.code));

            switch (message.reason)
            {
                case IMPFW_CB_TRAP:           /* fall through */
                case IMPFW_CB_INT:            /* fall through */
                case IMPFW_CB_INT_PBCOVF:     /* fall through */
                case IMPFW_CB_ERROR_ILLEGAL:  /* fall through */
                case IMPFW_CB_INT_SBO0ME:     /* fall through */
                case IMPFW_CB_TRAP_SBO0ME:
                    complete_count ++;
                    break;
                default:
                    break;
            }
        }
    }

    if (OSAL_RETURN_OK != ret_osal)
    {
        index = 0U;
        while ( index < s_request_num)
        {
            st_additional_data_t *data = &s_additional_data[index];
            if (false == data->complete)
            {
                LOGPRINT(("<< [ERROR] no callback Core type %d, Core num %d>>\n",
                    (int32_t)data->core.core_type,  /* cast to int32_t */
                    (int32_t)data->core.core_num)); /* cast to int32_t */
            }
            index ++;
        }
    }
    s_request_num = 0U;
    return ret_osal;
}
/**********************************************************************************************************************
 * End of function imp_demo_impfw_waitcallback
 ********************************************************************************************************************/

/*
=======================================================================================================================
 Private function definitions
=======================================================================================================================
*/
/**********************************************************************************************************************
 * Function Name: callbackfunc
 * Description  : callback of IMPFW
 * Arguments    : e_impfw_callback_reason_t reason
 *                const st_impfw_core_info_t * const core
 *                int32_t code
 *                void const * callbackargs
 * Return Value : None
 *********************************************************************************************************************/
static int32_t callbackfunc(e_impfw_callback_reason_t reason,        /* CL execution Callback status     */
    const st_impfw_core_info_t * const core, /* Core ID related to this callback */
    int32_t code,                            /* INT code or TRAP code or -1(unavailable) */
    void * const callbackargs)               /* The pointer to user data */
{
    /* OSAL */
    e_osal_return_t ret_osal;
    st_additional_data_t *data;
    uint32_t             index;
    st_callbackmessage_t message;

    data = (st_additional_data_t *)callbackargs;

    if ((data->core.core_type != core->core_type) || (data->core.core_num != core->core_num))
    {
        LOGPRINT(("<< [ERROR] callbackargs invalid>>\n"));
    }
    else
    {
        switch (reason)
        {
            case IMPFW_CB_STARTED:
                R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &data->time_cb_start[core->core_type]);
                break;
            case IMPFW_CB_TRAP:           /* fall through */
            case IMPFW_CB_INT:            /* fall through */
            case IMPFW_CB_INT_PBCOVF:     /* fall through */
            case IMPFW_CB_ERROR_ILLEGAL:  /* fall through */
            case IMPFW_CB_INT_SBO0ME:     /* fall through */
            case IMPFW_CB_TRAP_SBO0ME:
                R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &data->time_cb_trap[core->core_type]);
                data->complete = true;
                break;

            case IMPFW_CB_ERROR_INTERNAL: /* fall through */
            case IMPFW_CB_USIER:
                ; /* fatal error */
                break;

            case IMPFW_CB_END:            /* fall through */
            case IMPFW_CB_WUPCOV:         /* fall through */
            case IMPFW_CB_PBCOVF:         /* fall through */
            case IMPFW_CB_HPINT:          /* fall through */
            case IMPFW_CB_APIPINT:        /* fall through */
            case IMPFW_CB_MSCO:           /* fall through */
            case IMPFW_CB_DRVERR:         /* fall through */
            case IMPFW_CB_RTTERR:         /* fall through */
            case IMPFW_CB_RESOURCEFULL:   /* fall through */
            default:
                ; /* nothing to do */
                break;
        }

        index = data->request_num;

        message.request_num = index;
        message.reason      = reason;
        message.code        = code;

        st_osal_mq_config_t mq_config;
        ret_osal = R_OSAL_MqGetConfig(s_queuecallback, &mq_config);
        ret_osal = R_OSAL_MqSendForTimePeriod(
                                        s_queuecallback, s_execute_timeout,
                                        &message, mq_config.msg_size);
        if (OSAL_RETURN_OK != ret_osal)
        {
            LOGPRINT(("<< [ERROR] R_OSAL_MqSendForTimePeriod  : code %d>>\n",
            (int32_t)ret_osal)); /* cast to int32_t */
        }
    }
    return ((int32_t)0);
}
/**********************************************************************************************************************
 * End of function callbackfunc
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: callbackfunc_fatal
 * Description  : callback of fatal
 * Arguments    : e_impfw_fatalcode_t  reason
 *                uint32_t value
 * Return Value : None
 *********************************************************************************************************************/
static void callbackfunc_fatal(e_impfw_fatalcode_t error, uint32_t code)
{
    LOGPRINT(("callbackfunc_fatal : fatalcond:%s, code:%u\n",
        fatal_code_tostr(error), code));
}
/**********************************************************************************************************************
 * End of function callbackfunc_fatal
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: reason_tostr
 * Description  : return string of reason
 * Arguments    : e_impfw_callback_reason_t reason
 * Return Value : string
 *********************************************************************************************************************/
static const char_t * reason_tostr(e_impfw_callback_reason_t reason)
{
    char_t * p_msg;

    switch (reason)
    {
        case IMPFW_CB_STARTED:
            p_msg = "STARTED       ";
            break;
        case IMPFW_CB_TRAP:
            p_msg = "TRAP          ";
            break;
        case IMPFW_CB_INT:
            p_msg = "INT           ";
            break;
        case IMPFW_CB_INT_PBCOVF:
            p_msg = "INT_PBCOVF    ";
            break;
        case IMPFW_CB_ERROR_ILLEGAL:
            p_msg = "ERROR_ILLEGAL ";
            break;
        case IMPFW_CB_ERROR_INTERNAL:
            p_msg = "ERROR_INTERNAL";
            break;
        case IMPFW_CB_USIER:
            p_msg = "USIER         ";
            break;
        case IMPFW_CB_INT_SBO0ME:
            p_msg = "INT_SBO0ME    ";
            break;
        case IMPFW_CB_TRAP_SBO0ME:
            p_msg = "TRAP_SBO0ME   ";
            break;
        case IMPFW_CB_END:
            p_msg = "END           ";
            break;
        case IMPFW_CB_WUPCOV:
            p_msg = "WUPCOV        ";
            break;
        case IMPFW_CB_PBCOVF:
            p_msg = "PBCOVF        ";
            break;
        case IMPFW_CB_HPINT:
            p_msg = "HPINT         ";
            break;
        case IMPFW_CB_APIPINT:
            p_msg = "APIPINT       ";
            break;
        case IMPFW_CB_MSCO:
            p_msg = "MSCO          ";
            break;
        case IMPFW_CB_DRVERR:
            p_msg = "DRVERR        ";
            break;
        case IMPFW_CB_RTTERR:
            p_msg = "RTTERR        ";
            break;
        case IMPFW_CB_RESOURCEFULL:
            p_msg = "RESOURCEFULL  ";
            break;
        default:
            p_msg = "UNDEFINED     ";
            break;
    }
    return p_msg;
}
/**********************************************************************************************************************
 * End of function reason_tostr
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: fatal_code_tostr
 * Description  : return string of code
 * Arguments    : e_impfw_fatalcode_t code
 * Return Value : string
 *********************************************************************************************************************/
static const char_t * fatal_code_tostr(e_impfw_fatalcode_t code)
{
    char_t * p_msg;

    switch (code)
    {
        case IMPFW_FATALERR_FW_ERROR:
            p_msg = "FATALERR_FW          ";
            break;
        case IMPFW_FATALERR_DRV_ERROR:
            p_msg = "FATALERR_DRV         ";
            break;
        case IMPFW_FATALERR_UNEXPECT_INT:
            p_msg = "FATALERR_UNEXPECT_INT";
            break;
        default:
            p_msg = "UNDEFINED            ";
            break;
    }
    return p_msg;
}
/**********************************************************************************************************************
 * End of function fatal_code_tostr
 ********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: set_shared_data
 * Description  : Set data to memory shared with DSP side
 * Arguments    : st_buffer_t *dsp_shared_buffer
 *              : st_cdnn_shared_data_t **shared_ptr
 * Return Value : int32_t
 *********************************************************************************************************************/
static int32_t set_shared_data(st_buffer_t *dsp_shared_buffer, st_cdnn_shared_data_t **shared_ptr)
{
    e_osal_return_t         ret_osal;
    int32_t                 ret_demo = IMPDEMO_OK;
    uint32_t                loop_num;
    uintptr_t               rd_addr;
    uintptr_t               next_hw_addr;
    uintptr_t               ofs_in_buf = ALIGN_N(sizeof(st_cdnn_shared_data_t), 0x100U); /* 256 byte (T.B.D.) */

    /* copy from input files */
    rd_addr = (uintptr_t)dsp_shared_buffer->cpu_addr + IMPFWDEMO_CDNN_PRG_DATA + ofs_in_buf;
    /* Rewrite all input data to 0x00 */
    fillmemory((void*)rd_addr, 0x00, IMPFWDEMO_CDNN_WORKSIZE - (IMPFWDEMO_CDNN_PRG_DATA + (uint32_t)ofs_in_buf));
    for (loop_num = 0U; loop_num < s_num_arg; loop_num++)
    {
        if (loop_num != ARG_IC)
        {
            size_t ret;
            fseek(s_fp[loop_num], 0, SEEK_SET);
            ret = fread((void*)rd_addr, 1, s_file_size[loop_num], s_fp[loop_num]);
            if (ret != s_file_size[loop_num])
            {
                ret_demo = IMPDEMO_NG;
                LOGPRINT(("[ERROR] Read input file size\n"));
            }

            /* calculate shared buffer size */
            s_ofs_size[loop_num] = ALIGN_N(s_file_size[loop_num], 0x1000U);    /* 4K align (T.B.D.) */
            /* set hw address for each input data */
            if (loop_num == 0U)
            {
                s_hw_addr[loop_num] = (uint32_t)(dsp_shared_buffer->hw_addr + IMPFWDEMO_CDNN_PRG_DATA + ofs_in_buf);
            }
            else
            {
                s_hw_addr[loop_num] = next_hw_addr;
            }
            rd_addr     += s_ofs_size[loop_num];
            next_hw_addr = s_hw_addr[loop_num] + s_ofs_size[loop_num];
        }
    }
    /* OUT_IM */
    s_hw_addr[OUT_IM] = next_hw_addr;

    if (ret_demo == IMPDEMO_OK)
    {
        /* Shared data Setting */
        *shared_ptr = (st_cdnn_shared_data_t*)((uintptr_t)dsp_shared_buffer->cpu_addr + IMPFWDEMO_CDNN_PRG_DATA);
        ret_demo = parse_input_config(s_fp[ARG_IC], *shared_ptr);
        if (ret_demo == IMPDEMO_OK)
        {
            (*shared_ptr)->shared_size = IMPFWDEMO_CDNN_WORKSIZE;
            (*shared_ptr)->qdata_ptr   = (uint32_t)(uintptr_t)s_hw_addr[ARG_QD];
            (*shared_ptr)->input_ptr   = (uint32_t)(uintptr_t)s_hw_addr[ARG_IM];
            (*shared_ptr)->output_ptr  = (uint32_t)(uintptr_t)s_hw_addr[OUT_IM];
            (*shared_ptr)->pBufferName = (uint32_t)((uintptr_t)(*shared_ptr)->BufferName);
            (*shared_ptr)->clptr_cnnip = (uint32_t)(uintptr_t)s_hw_addr[ARG_CL_CNN];
            (*shared_ptr)->clptr_dma0ip = (uint32_t)(uintptr_t)s_hw_addr[ARG_CL_SDMA0];
            (*shared_ptr)->clptr_dma1ip = (uint32_t)(uintptr_t)s_hw_addr[ARG_CL_SDMA2];
            LOGPRINT(("QData   %08X\n", (*shared_ptr)->qdata_ptr));
            LOGPRINT(("input   %08X\n", (*shared_ptr)->input_ptr));
            LOGPRINT(("output  %08X\n", (*shared_ptr)->output_ptr));
            LOGPRINT(("CNN CL  %08X\n", (*shared_ptr)->clptr_cnnip));
            LOGPRINT(("DMA0 CL %08X\n", (*shared_ptr)->clptr_dma0ip));
            LOGPRINT(("DMA1 CL %08X\n", (*shared_ptr)->clptr_dma1ip));

            if ((*shared_ptr)->output_buff_size == 0)
				(*shared_ptr)->output_buff_size = s_file_size[ARG_IM];
            if ((*shared_ptr)->output_buff_size > IMPFWDEMO_CDNN_MAX_OUT)
            {
                ret_demo = IMPDEMO_NG;
                LOGPRINT(("<< [ERROR] sum of input/QData/CL/output buffer over flow>>\n"));
            }
            else
            {
                /* Cache flush and Incalidate */
                ret_osal = buffer_sync(dsp_shared_buffer);
                if (OSAL_RETURN_OK != ret_osal)
                {
                    ret_demo = IMPDEMO_NG;
                    LOGPRINT(("<< [ERROR] buffer_sync  : code %d>>\n", (int32_t)ret_osal));
                }
            }
        }
    }
    return ret_demo;
}
/**********************************************************************************************************************
 * End of function set_shared_data
 ********************************************************************************************************************/
/**********************************************************************************************************************
 * Function Name: parse_input_config
 * Description  : Set parameters to be notified to CDNN by reading input configuration.
 * Arguments    : FILE *fp
 *              : st_cdnn_shared_data_t *shared_ptr
 * Return Value : int32_t
 *********************************************************************************************************************/
static int32_t parse_input_config(FILE *fp, st_cdnn_shared_data_t *shared_ptr)
{
    char str[256];
    char *param;
    char *pval;

    while (fgets(str, 256, fp) != NULL)
    {
        param = strtok((char*)str,"="); //split by "="
        pval  = strtok(NULL,"\n");      //split by "\n"
        if ((param != NULL) && (pval != NULL))
        {
            if (strcmp(param, "width") == 0) shared_ptr->width = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "height") == 0) shared_ptr->height = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "dataType") == 0) shared_ptr->dataType = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "bytePerElement") == 0) shared_ptr->bytePerElement = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "depth") == 0) shared_ptr->depth = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "nChannels") == 0) shared_ptr->nChannels = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "scaleFactor") == 0) shared_ptr->scaleFactor = strtof(pval, NULL);
            else if (strcmp(param, "dataOrder") == 0) shared_ptr->dataOrder = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "nInputs") == 0) shared_ptr->nInputs = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "resizeWidth") == 0) shared_ptr->resizeWidth = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "resizeHeight") == 0) shared_ptr->resizeHeight = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "eSkipPreprocess") == 0) shared_ptr->eSkipPreprocess = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "eResize") == 0) shared_ptr->eResize = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "eChannelSwap") == 0) shared_ptr->eChannelSwap = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "eTranspose") == 0) shared_ptr->eTranspose = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "padding0") == 0) shared_ptr->padding[0] = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "padding1") == 0) shared_ptr->padding[1] = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "padding2") == 0) shared_ptr->padding[2] = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "padding3") == 0) shared_ptr->padding[3] = (int32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "pBufferName") == 0) (void)strcpy(shared_ptr->BufferName, pval);
            else if (strcmp(param, "bufferId") == 0) shared_ptr->bufferId = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "bufferType") == 0) shared_ptr->bufferType = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "f32ZeroPoint") == 0) shared_ptr->f32ZeroPoint = strtof(pval, NULL);
            else if (strcmp(param, "do_debug") == 0) shared_ptr->do_debug = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "debug_level") == 0) shared_ptr->debug_level = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "debug_param") == 0) shared_ptr->debug_param = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "do_profile") == 0) shared_ptr->do_profile = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "conv_format") == 0) shared_ptr->conv_format = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "output_buff_size") == 0) shared_ptr->output_buff_size = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "output_fp32") == 0) shared_ptr->output_fp32 = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "dump_outfile") == 0) s_dump_outfile = (uint32_t)strtol(pval, NULL, 10);
            else if (strcmp(param, "timeout") == 0)
            {
                uint32_t tmout = (uint32_t)strtol(pval, NULL, 10);
                s_execute_timeout = (tmout != 0) ? tmout : s_execute_timeout;
            }
            else
            {
                LOGPRINT(("<< [ERROR] unknown parameter %s>>\n", param));
                return IMPDEMO_NG;
            }
        }
    }
    return IMPDEMO_OK;
}
/**********************************************************************************************************************
 * End of function parse_input_config
 ********************************************************************************************************************/

/*
=======================================================================================================================
 End of file
=======================================================================================================================
*/
