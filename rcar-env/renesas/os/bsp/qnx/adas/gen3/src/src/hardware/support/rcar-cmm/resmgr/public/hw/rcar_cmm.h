/*
 * Renesas Proprietary Information.
 * The information contained herein is confidential property of
 * Renesas Electronics Corporation
 *
 * Copyright (C) Renesas Electronics Corporation 2018 All rights reserved.
 */

#ifndef _CMM_H_INCLUDED
#define _CMM_H_INCLUDED

#include <sys/resmgr.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/slog.h>
#include <hw/inout.h>
#include <mqueue.h>
#include <semaphore.h>
#include <devctl.h>

#define CMM_RESMGR_NAME			"cmm"
#define CMM_DEV_NAME			"/dev/" CMM_RESMGR_NAME			// device name

/* DU_CMM_CONFIG: DU-CMM config */
#define CSC_CONVERT_NONE			0
#define CSC_CONVERT_BT601_YCbCr240	1
#define CSC_CONVERT_BT601_YCbCr255	2
#define CSC_CONVERT_BT709_RGB255	3
#define CSC_CONVERT_BT709_RGB235	4

//
// dvctl function
//
#define CMM_SET_LUT			1	// set 1D-LUT
#define CMM_SET_CLU			2	// set 3D-LUT
#define CMM_SET_HGOINFO		3	// set HGO param
#define CMM_GET_HGO			4	// get HGO
#define CMM_WAIT_EVENT		5	// Wait for completion of du_cmm processing(LUT/CLU/HGO set/get)
#define CMM_START_HGO		6	// reset HGO
#define CMM_START_LUT		7	// enable LUT
#define CMM_START_CLU		8	// enable CLU
#define CMM_SET_LUT_CLU		9	// set 1D-LUT/3D-LUT
#define CMM_START_STOP		10	// enable/diable 1D-LUT/3D-LUT
#define CMM_CONFIG			11	// set CSC

typedef struct {
	uint32_t  length;
	uint32_t  data;
} cmm_data_addr_t;

typedef struct {
	uint32_t  lut_length;
	void*  lut;
} cmm_lut_virt_t;

typedef struct {
	uint32_t  hgo_length;
	void*  hgo;
} cmm_hgo_virt_t;

typedef struct {
	// Histogram Source Component Setting
	unsigned char maxrgb;						// 0: The histogram is generated from 3 color components independently.
												// 1: The histogram is generated from the maximum value of input R, G and B data.
	// Offset Binary Mode for R/Cr/H Component
	unsigned char ofsb_r;						// 0: Straight binary
												// 1: Offset binary
	// Offset Binary Mode for G/Y/S Component
	unsigned char ofsb_g;						// 0: Straight binary
												// 1: Offset binary
	// Offset Binary Mode for B/Cb/V Component
	unsigned char ofsb_b;						// 0: Straight binary
												// 1: Offset binary
	// Horizontal Pixel Skipping Mode for Histogram Detection
	unsigned char hratio;						// 0: No skipping for horizontal pixels.
												// 1: Horizontal 1/2 skipping. One pixel is discarded from every two pixels before a histogram is created.
												// 2: Horizontal 1/4 skipping. Three pixels are discarded from every four pixels before a histogram is created.
	// Vertical Pixel Skipping Mode for Histogram Detection
	unsigned char vratio;						// 0: No skipping for vertical pixels.
												// 1: vertical 1/2 skipping. One pixel is discarded from every two pixels before a histogram is created.
												// 2: vertical 1/4 skipping. Three pixels are discarded from every four pixels before a histogram is created.
	uint32_t hoffset;
	uint32_t voffset;

	uint32_t hsize;
	uint32_t vsize;

	unsigned char hists;						// Histogram selector
												// 0: Histogram data is measured before CLU processing
												// 1: Histogram data is measured before LUT processing
} cmm_hgo_info_t;

/*
	Define the actual devctl commnads
	there are four flavors of commands
	__DIOF      Data IO From the driver
	__DIOT      Data IO To the driver
	__DIOTF     Data IO To and From the driver
	__DION      a command with No associated data
*/
#define DCMD_CMD_CODE                   100
#define DCMD_CMM_SET_LUT    			__DIOT(_DCMD_MISC,  DCMD_CMD_CODE + 1, cmm_data_addr_t)
#define DCMD_CMM_SET_CLU		        __DIOT(_DCMD_MISC,  DCMD_CMD_CODE + 2, cmm_data_addr_t)
#define DCMD_CMM_SET_HGOINFO			__DIOT(_DCMD_MISC,  DCMD_CMD_CODE + 3, cmm_hgo_info_t)
#define DCMD_CMM_GET_HGO				__DIOT(_DCMD_MISC,  DCMD_CMD_CODE + 4, cmm_data_addr_t)
#define DCMD_CMM_WAIT_CMD				__DION(_DCMD_MISC,  DCMD_CMD_CODE + 5)
#define DCMD_CMM_START_HGO				__DION(_DCMD_MISC,  DCMD_CMD_CODE + 6)
#define DCMD_CMM_START_STOP				__DIOT(_DCMD_MISC,  DCMD_CMD_CODE + 7, unsigned char)
#define DCMD_CMM_CONFIG					__DIOT(_DCMD_MISC,  DCMD_CMD_CODE + 8, unsigned char)

#endif /* _CMM_H_INCLUDED */
