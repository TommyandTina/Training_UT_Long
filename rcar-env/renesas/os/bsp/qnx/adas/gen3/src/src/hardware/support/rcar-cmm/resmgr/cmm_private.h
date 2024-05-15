/*
 * Renesas Proprietary Information.
 * The information contained herein is confidential property of
 * Renesas Electronics Corporation
 *
 * Copyright (C) Renesas Electronics Corporation 2018 All rights reserved.
 */

#ifndef _CMM_PRIVATE_H_INCLUDED
#define _CMM_PRIVATE_H_INCLUDED

#include <sys/resmgr.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/slog.h>
#include <hw/inout.h>
#include <mqueue.h>
#include <semaphore.h>
#include <devctl.h>

#define CMM_LUT_NUM 256
#define CMM_CLU_NUM (17 * 17 * 17)
#define CMM_HGO_NUM 64

#define BIT(x)	(1 << (x))

#define CMM_CLU_ADDR		0x0A00
#define CMM_CLU_DATA		0x0A04

#define MAX_MSG_SIZE		IOBUF_SIZE				// max size of a message, should be max(IOBUF, devctl_t)
#define IOBUF_SIZE			1024					// size of io_buffer used to for msg read/write with client
#define VERBOSE_QUIET		0

#define DU0_INTID			288						// DU0 Interrupt id
#define DU1_INTID			300						// DU1 Interrupt id
#define DU2_INTID			301						// DU2 Interrupt id
#define DU3_INTID			302						// DU3 Interrupt id

//
// define register bit
//

// DSRCR
#define VBCL				BIT(11)

// DEF7Rm
#define DEF7R_CODE			(0x7779 << 16)
#define DEF7R_CMME1			BIT(6)					// 1:The CMM processing is performed for display data of the DU1 and DU3.
#define DEF7R_CMME0			BIT(4)					// 1:The CMM processing is performed for display data of the DU0 and DU2.

// RMSTPCR7/SMSTPCR7
#define CMM0_DISABLE		BIT(11)					// 1:Disable CMM0
#define CMM1_DISABLE		BIT(10)					// 1:Disable CMM1
#define CMM2_DISABLE		BIT(9)					// 1:Disable CMM2
#define CMM3_DISABLE		BIT(8)					// 1:Disable CMM3

// DEF5R
#define DEFE5				BIT(0)					// 1:Extensional functions are enabled.

// DIER
#define VBK_ENABLE			BIT(11)					// 1:enable VBK interrupt
// DSYSRm
#define DRES				BIT(9)					// 1:Display Reset
#define DEN					BIT(8)					// 1:Display Enable

// CM2_CTL0
#define DBUF				BIT(0)					// 1:LUT/HGO Double Buffer Mode Enable
#define CMM_CTL0_YC			BIT(8)					// CMM processing format
#define HISTS				BIT(20)					// 1: Histogram data is measured before LUT processing.
#define CLUDB				BIT(24)					// 1:CLU Double Buffer Mode Enable

#define CMM_CTL0_HISTS				BIT(20)
#define CMM_CTL0_TM1_MASK			(3 << 16)
#define CMM_CTL0_TM1_BT601_YC240	(0 << 16)
#define CMM_CTL0_TM1_BT601_YC255	BIT(16)
#define CMM_CTL0_TM1_BT709_RG255	(2 << 16)
#define CMM_CTL0_TM1_BT709_RG235	(3 << 16)
#define CMM_CTL0_TM0_MASK			(3 << 12)
#define CMM_CTL0_TM0_BT601_YC240	(0 << 12)
#define CMM_CTL0_TM0_BT601_YC255	BIT(12)
#define CMM_CTL0_TM0_BT709_RG255	(2 << 12)
#define CMM_CTL0_TM0_BT709_RG235	(3 << 12)
#define CMM_CTL0_TM_BT601_YC240		(CMM_CTL0_TM1_BT601_YC240 |\
					 CMM_CTL0_TM0_BT601_YC240)
#define CMM_CTL0_TM_BT601_YC255		(CMM_CTL0_TM1_BT601_YC255 |\
					 CMM_CTL0_TM0_BT601_YC255)
#define CMM_CTL0_TM_BT709_RG255		(CMM_CTL0_TM1_BT709_RG255 |\
					 CMM_CTL0_TM0_BT709_RG255)
#define CMM_CTL0_TM_BT709_RG235		(CMM_CTL0_TM1_BT709_RG235 |\
					 CMM_CTL0_TM0_BT709_RG235)

// CM2_CTL1
#define BFS					BIT(0)					// This bit specifies referenced plane of 1D-LUT and 3D-LUT.

// CM2_LUT_CTRL
#define LUT_EN				BIT(0)					// 1:1D-LUT Enable

// CM2_CLU_CTRL
#define AAI					BIT(28)					// Automatic Table Address Increment
#define MVS					BIT(24)					// Improved characteristics
#define CLU_EN				BIT(0)					// 3D-LUT Enable

// CM2_HGO_REGRST
#define HGO_RESET			BIT(0)					// 1:HGO reset

//
// define register address
//

// CPG register for module standby
#define RMSTPCR7				0xE615012C	// Real time module stop control register 7
#define SMSTPCR7				0xE615014C	// System module stop control register 7

// DU base address
#define DU0_CR_BASEADDRESS		0xFEB00000		// DU0 Display Unit System Control Register Base address
#define DU1_CR_BASEADDRESS		0xFEB30000		// DU1 Display Unit System Control Register Base address
#define DU2_CR_BASEADDRESS		0xFEB40000		// DU2 Display Unit System Control Register Base address
#define DU3_CR_BASEADDRESS		0xFEB70000		// DU3 Display Unit System Control Register Base address

// cmm base address
#define CMM0_BASEADDRESS		0xFEA40000		// CMM0 Base address
#define CMM1_BASEADDRESS		0xFEA50000		// CMM1 Base address
#define CMM2_BASEADDRESS		0xFEA60000		// CMM2 Base address
#define CMM3_BASEADDRESS		0xFEA70000		// CMM3 Base address

// register size
#define DU_REGSIZE				0x80000
#define CMM_REGSIZE				0x01000


//
// define register offset
//

// DU register offset
#define DSYSR0					0x0000			// Display unit system control register m
#define DSMR					0x0004
#define DSSR					0x0008			// Display unit status register
#define DSRCR					0x000C			// Display unit status register clear register n
#define DIER					0x0010			// Display unit interrupt enable register
#define HDSR					0x0040			// Horizontal Display Start Register
#define HDER					0x0044			// Horizontal Display End Register
#define DEF5R					0x00E0			// Display unit extensional Function Control 5 Register
#define DEF7Rm					0x00EC			// Display unit extensional function control 7 register

// CMM register offset
#define CM2_LUT_CTRL			0x0000			// LUT Control Register
#define CM2_CLU_CTRL			0x0100			// CLU Control Register
#define CM2_CTL0				0x0180			// CMM Control Register0
#define CM2_CTL1				0x0184			// CMM Control Register1
#define CM2_HGO_MODE			0x0208			// HGO Mode Register
#define CM2_HGO_OFFSET			0x0200			// HGO OFFSET
#define CM2_HGO_SIZE			0x0204			// HGO SIZE
#define CMM_HGO_LB_TH			0x020C			// HGO LB Detection Threshold Register
#define CMM_HGO_LB0_H			0x0210			// HGO Horizontal Position Register for LB Detection Zone-0
#define CMM_HGO_LB0_V			0x0214			// HGO Vertical Position Register for LB Detection Zone-0
#define CMM_HGO_LB1_H			0x0218			// HGO Horizontal Position Register for LB Detection Zone-1
#define CMM_HGO_LB1_V			0x021C			// HGO Vertical Position Register for LB Detection Zone-1
#define CMM_HGO_LB2_H			0x0220			// HGO Horizontal Position Register for LB Detection Zone-2
#define CMM_HGO_LB2_V			0x0224			// HGO Vertical Position Register for LB Detection Zone-2
#define CMM_HGO_LB3_H			0x0228			// HGO Horizontal Position Register for LB Detection Zone-3
#define CMM_HGO_LB3_V			0x022C			// HGO Vertical Position Register for LB Detection Zone-3

#define CMM_HGO_R_HISTO(n)		(0x0230 + ((n) * 4))
#define CMM_HGO_G_HISTO(n)		(0x0340 + ((n) * 4))
#define CMM_HGO_B_HISTO(n)		(0x0450 + ((n) * 4))

#define CM2_HGO_REGRST			0x05fc			// HGO Parameter Register Reset
#define CMM_LUT_TBLA(n)			(0x0600 + ((n) * 4))

#define CM2_CLU_ADDRESS			0x0A00			// CLU side A table address
#define CM2_CLU_DATA			0x0A04			// CLU side A table data register

#define CMM_LUT_TBLB(n)			(0x0B00 + ((n) * 4))
#define CM2_CLU_ADDR2			0x0F00			// CLU side B table address
#define CM2_CLU_DATA2			0x0F04			// CLU side B table data register

typedef struct cmm_dev {
	uint32_t			id;
	iofunc_attr_t		att;
	dispatch_t			*dpp;
	dispatch_context_t	*ctp;
	_Uintptrt cpg;
	_Uintptrt du_control;
	_Uintptrt du;
	_Uintptrt cmm;
	struct sigevent *g_event;
	uint32_t	chid;
	uint32_t	irqcoid;
	unsigned char active;
} cmm_dev_t;

extern cmm_dev_t *dev;
extern mqd_t g_send_que_lut;
extern mqd_t g_send_que_clu;
extern mqd_t g_send_que_hgo;
extern mqd_t g_recv_que_lut;
extern mqd_t g_recv_que_clu;
extern mqd_t g_recv_que_hgo;

extern sem_t g_sem;

int cmm_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
int cmm_slogf(const int Verbose, const char *fmt, ...);

#define __iomem
#define iowrite32(value, addr) out32((uintptr_t)addr, (uint32_t)value)
#define ioread32(addr) in32((uintptr_t)addr)

/******************************************************************************
Function:		du/cmm_write_reg
Description:	Write to register
Returns:		void
******************************************************************************/
static inline void ducmm_write_reg(unsigned int data, void __iomem *base, unsigned int offset)
{
	unsigned int __iomem *reg = (unsigned int __iomem *)base;
	reg += (offset >> 2);
	iowrite32(data, reg);
}

/******************************************************************************
Function:		du/cmm_read_reg
Description:	Read from register
Returns:		void
******************************************************************************/
static inline unsigned int ducmm_read_reg(void __iomem *base, unsigned int offset)
{
	unsigned int __iomem *reg = (unsigned int __iomem *)base;
	reg += (offset >> 2);
	return ioread32(reg);
}

void* vsync_handler (void *arg);
#endif /* _CMM_PRIVATE_H_INCLUDED */
