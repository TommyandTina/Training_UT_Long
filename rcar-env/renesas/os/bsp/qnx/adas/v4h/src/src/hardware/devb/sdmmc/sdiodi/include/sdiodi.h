/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#ifndef _SDIODI_H_INCLUDED
#define _SDIODI_H_INCLUDED

#include <inttypes.h>
#include <sys/types.h>

#define	SDIO_SUCCESS				0
#define	SDIO_FAILURE				1
#define	SDIO_FALSE					0
#define	SDIO_TRUE					1
#define SDIO_TIME_DEFAULT			1000
#define SDIO_BKOPS_MAX_TIMEOUT		(4 * 60 * 1000)

#define SDIO_DATA_PTR_V( _p )		( (void *)(uintptr_t)(_p) )
#define SDIO_DATA_PTR_P( _p )		( (uintptr_t)(_p) )

struct sdio_cmd;
struct sdio_device;
struct sdio_connection;
typedef struct _sdio_sge				sdio_sge_t;
typedef struct _sdio_cid				sdio_cid_t;
typedef struct _sdio_csd				sdio_csd_t;
typedef struct _sdio_ecsd				sdio_ecsd_t;
typedef struct _sdio_hc_info			sdio_hc_info_t;
typedef struct _sdio_dev_info			sdio_dev_info_t;
typedef struct _sdio_funcs				sdio_funcs_t;
typedef struct _sdio_connect_parm		sdio_connect_parm_t;
typedef struct _sdio_device_ident		sdio_device_ident_t;
typedef struct _sdio_device_instance	sdio_device_instance_t;

#define SDIO_RSP_SIZE					16
#define SDIO_CID_SIZE					16
#define SDIO_CSD_SIZE					16

struct _sdio_cid {
	_Uint32t	mid;		// Manufacture ID
	_Uint32t	oid;		// OEM ID
	char		pnm[8];		// Product name
	_Uint32t	prv;		// Product revision
	_Uint32t	psn;		// Product serial number
	_Uint32t	month;		// Month
	_Uint32t	year;		// Year
};

struct _sdio_csd {
#define CSD_STRUCT_VER_10		0	// MMCA V1.0 / SD Standard Capacity
#define CSD_STRUCT_VER_20		1	// SD High Capacity / Extended Capacity
#define CSD_STRUCT_VER_30		2	// SD Ultra Capacity
#define CSD_STRUCT_VER_11		1	// MMCA V1.1
#define CSD_STRUCT_VER_12		2	// EMMC Version 4.1–4.2–4.3-4.41-4.5-4.51-5.0-5.01-5.1
#define CSD_STRUCT_VER_EXT_CSD	3	// EMMC Version is coded in the CSD_STRUCTURE byte in EXT_CSD
	_Uint8t		csd_structure;		// CSD structure

#define	CSD_SPEC_VER_0			0   // 1.0 - 1.2
#define	CSD_SPEC_VER_1			1   // 1.4 -
#define	CSD_SPEC_VER_2			2   // 2.0 - 2.2
#define	CSD_SPEC_VER_3			3   // 3.1 - 3.2 - 3.31
#define	CSD_SPEC_VER_4			4   // 4.0 - 4.1
	_Uint8t		spec_vers;

	_Uint8t		taac;
	_Uint8t		nsac;
	_Uint8t		tran_speed;
	_Uint8t		rsvd;

//                                   Class
#define CCC_BASIC		( 1 << 0 )	// 0 Basic: CMD 0-10,12-15,19
#define CCC_BLOCK_READ	( 1 << 2 )	// 2 Block Read: CMD 16-18
#define CCC_BLOCK_WRITE	( 1 << 4 )	// 4 Block Write: CMD 16,24-27
#define CCC_ERASE		( 1 << 5 )	// 5 Erase:  CMD 32-39
#define CCC_WRITE_PROT	( 1 << 6 )	// 6 Write Protection: CMD 28-31
#define CCC_LOCK_DEVICE	( 1 << 7 )	// 7 Lock Devcie: CMD 16,40,42
#define CCC_APP_SPEC	( 1 << 8 )	// 8 Application specific: CMD 55-57, ACMD 6,13,22,23,41,42,51
#define CCC_IO_MODE		( 1 << 9 )	// 9 (9) I/O mode: CMD 5,39,40,52,53
#define CCC_SWITCH		( 1 << 10 )	// 10 High speed switch: CMD 6,34-37,50
	_Uint16t	ccc;

	_Uint8t		read_bl_len;
	_Uint8t		read_bl_partial;
	_Uint8t		write_blk_misalign;
	_Uint8t		read_blk_misalign;

	_Uint8t		dsr_imp;
	_Uint8t		rsvd2[3];

#define CSD_VER_10_CSIZE_MAX		0xfff			// 12 bits
#define CSD_VER_20_CSIZE_MAX		0x3fffff		// 22 bits
#define CSD_VER_30_CSIZE_MAX		0xfffffff		// 28 bits
	_Uint32t	c_size;

	_Uint8t		vdd_r_curr_min;
	_Uint8t		vdd_r_curr_max;
	_Uint8t		vdd_w_curr_min;
	_Uint8t		vdd_w_curr_max;

	_Uint8t		c_size_mult;
	_Uint8t		erase_blk_en;
	_Uint8t		erase_grp_size;
	_Uint8t		erase_grp_mult;

	_Uint8t		sector_size;
	_Uint8t		wp_grp_size;
	_Uint8t		wp_grp_enable;
	_Uint8t		r2w_factor;

	_Uint8t		write_bl_len;
	_Uint8t		write_bl_partial;
	_Uint8t		copy;
	_Uint8t		write_protect;

	_Uint8t		ecc;
	_Uint8t		rsvd3[3];

	_Uint32t	dtr_max;
	_Uint32t	blksz;
	_Uint64t	sectors;
};

struct _sdio_ecsd {
	_Uint32t		blksz;
	_Uint32t		sectors;
	_Uint32t		acc_size;
	_Uint32t		dtr_max_hs;
	_Uint32t		s_a_timeout;		// 100ns units

	_Uint8t			ext_csd_rev;
	_Uint8t			card_type;
	_Uint8t			driver_strength;

	_Uint8t			erase_grp_def;
	_Uint8t			hc_erase_group_size;
	_Uint8t			hc_wp_grp_size;
	_Uint8t			user_wp;
	_Uint8t			part_config;
	_Uint8t			strobe;
	_Uint8t			bkops_en;
};

struct _sdio_sge {
	_Uint64t sg_address;				// Scatter/Gather address
	_Uint32t sg_count;					// Scatter/Gather count
	_Uint32t rsvd;
};

// command flags
#define	SCF_CTYPE_BC		(1 << 0)
#define	SCF_CTYPE_BCR		(1 << 1)
#define	SCF_CTYPE_AC		(1 << 2)
#define	SCF_CTYPE_ADTC		(1 << 3)

#define	SCF_RSP_PRESENT		(1 << 4)
#define	SCF_RSP_136			(1 << 5)	// 136 bit response
#define	SCF_RSP_CRC			(1 << 6)	// expect valid crc
#define	SCF_RSP_BUSY		(1 << 7)	// card may send busy
#define	SCF_RSP_OPCODE		(1 << 8)	// response contains opcode

#define	SCF_RSP_NONE		(0)
#define	SCF_RSP_R1			(SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_OPCODE)
#define	SCF_RSP_R1B			(SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_OPCODE | SCF_RSP_BUSY)
#define	SCF_RSP_R2			(SCF_RSP_PRESENT | SCF_RSP_136 | SCF_RSP_CRC)
#define	SCF_RSP_R3			(SCF_RSP_PRESENT)
#define	SCF_RSP_R6			(SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_OPCODE)
#define	SCF_RSP_R7			(SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_OPCODE)

#define	SCF_DIR_IN			(1 << 9)	// data read
#define	SCF_DIR_OUT			(1 << 10)	// data write
#define	SCF_DATA_MSK		(SCF_DIR_IN | SCF_DIR_OUT)
#define	SCF_APP_CMD			(1 << 11)	// app command (cmd 55)
#define	SCF_SBC				(1 << 12)	// auto issue set block count (cmd 23)
#define	SCF_WAIT_DRDY		(1 << 13)	// wait ready for data
#define	SCF_SBC_RLW			(1 << 14)	// set block count (cmd 23) with reliable write bit
#define	SCF_SUA				(1 << 15)	// set upper address

// driver internal
#define	SCF_DATA_PHYS		(1 << 24)	// data physical address
#define	SCF_MULTIBLK		(1 << 25)

// command status
#define CS_CMD_INPROG		0x00
#define CS_CMD_CMP			0x01
#define CS_CMD_ABORTED		0x02
#define CS_CMD_CMP_ERR		0x03
#define CS_CMD_IDX_ERR		0x04
#define CS_CMD_TO_ERR		0x05
#define CS_CMD_CRC_ERR		0x06
#define CS_CMD_END_ERR		0x07
#define CS_DATA_TO_ERR		0x08
#define CS_DATA_CRC_ERR		0x09
#define CS_DATA_END_ERR		0x0a
#define CS_CARD_REMOVED		0x0b

#define SDIO_CONNECT_WILDCARD	(-1)
struct _sdio_device_ident {
	_Uint16t		vid;						// Vendor ID
	_Uint16t		did;						// Device ID
#define DEV_TYPE_MMC		1
#define DEV_TYPE_SD			2
#define DEV_TYPE_SDIO		3
	_Uint16t		dtype;						// Device Type
	_Uint16t		ccd;						// Class code
};

struct _sdio_device_instance {
	_Uint32t			path;
	_Uint32t			func;
	_Uint32t			generation;
	sdio_device_ident_t	ident;
};

struct _sdio_dev_info {
#define DEV_TYPE_MMC		1
#define DEV_TYPE_SD			2
	_Uint32t			dtype;
#ifndef DEV_FLAG_WP
#define DEV_FLAG_WP				0x1
#define DEV_FLAG_CARD_LOCKED	0x2
#endif
	_Uint32t			flags;

	_Uint32t			mid;			// Manufacture ID

	_Uint32t			oid;			// OEM ID
	_Uint8t				pnm[8];			// Product name
	_Uint32t			prv;			// Product revision
	_Uint32t			psn;			// Product serial number
	_Uint32t			month;			// Month
	_Uint32t			year;			// Year
	_Uint8t				vu[8];			// Vendor Unique ie SanDisk fw revision

	_Uint32t			rca;

	_Uint32t			spec_vers;
	_Uint32t			spec_rev;

	_Uint32t			security;

#define DEV_CAP_HC				(1ULL << 0)	// high capacity
#define DEV_CAP_HS				(1ULL << 1)	// high speed
#define DEV_CAP_HS200			(1ULL << 2)	// high speed 200
#define DEV_CAP_DDR50			(1ULL << 3)	// DDR
#define DEV_CAP_UHS				(1ULL << 4)	// UHS
#define DEV_CAP_TRIM			(1ULL << 5)	// TRIM supported
#define DEV_CAP_SECURE			(1ULL << 6)	// Secure Purge supported
#define DEV_CAP_SECURE_TRIM		(DEV_CAP_SECURE | DEV_CAP_TRIM)
#define DEV_CAP_SANITIZE		(1ULL << 7)	// SANITIZE supported
#define DEV_CAP_BKOPS			(1ULL << 8)	// Background Operations supported
#define DEV_CAP_CMD23			(1ULL << 9)	// CMD23 supported
#define DEV_CAP_SLEEP			(1ULL << 10)	// SLEEP/AWAKE supported
#define DEV_CAP_ASSD			(1ULL << 11)	// ASSD
#define DEV_CAP_HPI_CMD12		(1ULL << 12)
#define DEV_CAP_HPI_CMD13		(1ULL << 13)
#define DEV_CAP_DISCARD			(1ULL << 14)	// Discard supported
#define DEV_CAP_CACHE			(1ULL << 15)
#define DEV_CAP_HS400			(1ULL << 16)
#define DEV_CAP_PWROFF_NOTIFY	(1ULL << 17)	// Power off notify supported
#define DEV_CAP_HS400ES			(1ULL << 18)
#define DEV_CAP_BKOPS_AUTO		(1ULL << 19)	// Auto Background Operations supported
#define DEV_CAP_UC				(1ULL << 20)	// Ultra Capacity (2TB - 128TB)
#define DEV_CAP_WR_REL			(1ULL << 21)	// Reliable Write supported
#define DEV_CAP_WR_REL_ENH		(1ULL << 22)	// Enhanced Reliable Write supported
	_Uint64t			caps;

	_Uint32t			dtr;			// current data transfer rate
#define TIMING_HS400ES		10
#define TIMING_HS400		9
#define TIMING_HS200		8
#define TIMING_SDR104		7
#define TIMING_SDR50		6
#define TIMING_SDR25		5
#define TIMING_SDR12		4
#define TIMING_DDR50		3
#define TIMING_HS			2
#define TIMING_LS			1
	_Uint32t			timing;			// current timing
	_Uint32t			bus_width;		// current bus width
	_Uint64t			sectors;
	_Uint32t			sector_size;
	_Uint32t			super_page_size;
	_Uint32t			native_sector_size;
	_Uint32t			wp_size;
	_Uint32t			erase_size;
	_Uint32t			optimal_trim_size;
	_Uint32t			optimal_read_size;
	_Uint32t			optimal_write_size;
#define SPEED_CLASS_0	0x00
#define SPEED_CLASS_2	0x01
#define SPEED_CLASS_4	0x02
#define SPEED_CLASS_6	0x03
#define SPEED_CLASS_10	0x04
	_Uint32t			speed_class;

	_Uint32t			rel_wr_sec_c;

	_Uint32t			rsvd[14];
};

struct _sdio_hc_info {
#define SDIO_NAME_MAX		64
	char			name[SDIO_NAME_MAX];

#define	HC_CAP_SLOT_TYPE_EMBEDDED	(1ULL << 0)	// embedded card
#define	HC_CAP_PIO					(1ULL << 1)	// supports PIO
#define	HC_CAP_DMA					(1ULL << 2)	// supports DMA 32 bit address
#define	HC_CAP_DMA64				(3ULL << 2)	// supports DMA 64 bit address
#define	HC_CAP_DMA_MSK				(HC_CAP_DMA | HC_CAP_DMA64)
#define	HC_CAP_BW4					(1ULL << 4)	// 4 bit bus supported
#define	HC_CAP_BW8					(1ULL << 5)	// 8 bit bus supported
#define HC_CAP_BW_MSK				( HC_CAP_BW4 | HC_CAP_BW8 )
#define	HC_CAP_ACMD12				(1ULL << 6)	// auto stop cmd(12) supported
#define	HC_CAP_ACMD23				(1ULL << 7)	// auto set block count cmd(23) supported
#define HC_CAP_SLEEP				(1ULL << 8)

#define	HC_CAP_HS					(1ULL << 9)	// High speed device supported
#define	HC_CAP_SDR12				(1ULL << 10)
#define	HC_CAP_SDR25				(1ULL << 11)
#define	HC_CAP_SDR50				(1ULL << 12)
#define	HC_CAP_SDR104				(1ULL << 13)
#define	HC_CAP_DDR50				(1ULL << 14)	// Dual Data Rate supported
#define HC_CAP_UHS( _caps )			( (_caps) & ( HC_CAP_SDR12 | HC_CAP_SDR25 | \
										HC_CAP_SDR50 | HC_CAP_SDR104 | HC_CAP_DDR50) )
#define	HC_CAP_HS200				(1ULL << 15)
#define	HC_CAP_HS400				(1ULL << 16)
#define HC_CAP_TIMING_MSK			( HC_CAP_HS | HC_CAP_DDR50 |		\
										HC_CAP_SDR12 | HC_CAP_SDR25 |	\
										HC_CAP_SDR50 | HC_CAP_SDR104 |	\
										HC_CAP_HS200 | HC_CAP_HS400 | HC_CAP_HS400ES)

#define	HC_CAP_XPC_3_3V				(1ULL << 17)	// > 150mA at 3.3V is supported
#define	HC_CAP_XPC_3_0V				(1ULL << 18)	// > 150mA at 3.0V is supported
#define	HC_CAP_XPC_1_8V				(1 << 19)	// > 150mA at 1.8V is supported
#define HC_CAP_XPC( _caps )			( ( ( _caps ) >> 17 ) & 0x07 )

#define	HC_CAP_200MA				(1ULL << 20)	// 200mA at 1.8V
#define	HC_CAP_400MA				(1ULL << 21)	// 400mA at 1.8V
#define	HC_CAP_600MA				(1ULL << 22)	// 600mA at 1.8V
#define	HC_CAP_800MA				(1ULL << 23)	// 800mA at 1.8V
#define HC_CAP_CURRENT( _caps )		( ( ( ( _caps ) >> 20 ) ) & 0x0f )

#define	HC_CAP_DRV_TYPE_B			(1ULL << 24)
#define	HC_CAP_DRV_TYPE_A			(1ULL << 25)
#define	HC_CAP_DRV_TYPE_C			(1ULL << 26)
#define	HC_CAP_DRV_TYPE_D			(1ULL << 27)
#define	HC_CAP_DRV_TYPES( _caps )	( ( ( ( _caps ) >> 24 ) ) & 0x0f )


#define HC_CAP_SV( _caps )			( ( ( ( _caps ) >> 28 ) ) & 0x0f )
#define HC_CAP_SV_1_2V				(1ULL << 28)	// 1.2V signal voltage supported
#define HC_CAP_SV_1_8V				(1ULL << 29)	// 1.8V signal voltage supported
#define HC_CAP_SV_3_0V				(1ULL << 30)	// 3.0V signal voltage supported
#define HC_CAP_SV_3_3V				(1ULL << 31)	// 3.3V signal voltage supported

#define	HC_CAP_CD_INTR				(1ULL << 32)	// card detect interrupt supported
#define HC_CAP_BSY					(1ULL << 33)	// card detect busy supported
#define	HC_CAP_HS400ES				(1ULL << 34)	// hs400 enhanced strobe supported
#define HC_CAP_CD_WP				(1ULL << 35)	// card detect write protect supported
	_Uint64t		caps;
	_Uint32t		version;
	_Uint32t		sg_max;
	_Uint32t		dtr_max;					// Maximum Data Transfer Rate
	_Uint32t		dtr;						// Current Data Transfer Rate
	_Uint32t		timing;						// Current Timing
	_Uint32t		bus_width;					// Current Bus Width
	_Uint32t		idle_time;					// PM Idle Time in ms
	_Uint32t		sleep_time;					// PM Sleep Time in ms
	_Uint32t		rsvd[12];
};

struct _sdio_funcs {
	int			nfuncs;

	void		(*insertion)( struct sdio_connection *, sdio_device_instance_t * );
	void		(*removal)( struct sdio_connection *, sdio_device_instance_t * );
#define SDIO_EVENT_RESET	1
	int			(*event)( struct sdio_connection *, sdio_device_instance_t *, int ev );
};

struct _sdio_connect_parm {
	const char			*path;
#define SDIO_VERSION	0
	_Uint32t			vsdio;
	_Uint32t			flags;
	_Int32t				argc;
	char				**argv;
	sdio_device_ident_t	ident;
	sdio_funcs_t		funcs;
	_Uint32t			rsvd[12];
};

extern void				*sdio_alloc( size_t size );
extern int				sdio_free( void *ptr, size_t size );
extern int				sdio_verbosity( struct sdio_device *device, int flags, int verbosity );
extern int				sdio_idle( struct sdio_device *device );
#define PM_IDLE		0
#define PM_ACTIVE	1
#define PM_SLEEP	2
#define PM_SUSPEND	3
extern int				sdio_pwrmgnt( struct sdio_device *device, int action );
extern int				sdio_reset( struct sdio_device *device );
extern int				sdio_bus_error( struct sdio_device *device );
extern int				sdio_hpi( struct sdio_device *device, uint32_t timeout);
extern int				sdio_send_status( struct sdio_device *device, _Uint32t *rsp, int hpi );
extern int				sdio_wait_card_status( struct sdio_device *device, uint32_t *rsp, uint32_t mask, uint32_t val, uint32_t msec );
extern int				sdio_stop_transmission( struct sdio_device *device, int hpi );
extern int				sdio_set_block_count( struct sdio_device *device, int blkcnt, uint32_t flgs );
extern int				sdio_set_block_length( struct sdio_device *device, int blklen );
extern int				sdio_lock_unlock( struct sdio_device *device, int action, uint8_t *pwd, int pwd_len );
extern int				sdio_set_partition( struct sdio_device *device, _Uint32t partition );
extern int				sdio_erase( struct sdio_device *device, int partition, int flgs,
							uint64_t lba, int nlba );
extern struct sdio_cmd	*sdio_alloc_cmd( void );
extern void				sdio_free_cmd( struct sdio_cmd *cmd );
extern int				sdio_cmd_status( struct sdio_cmd *cmd, _Uint32t *status, _Uint32t *rsp );
extern int				sdio_send_cmd( struct sdio_device *device, struct sdio_cmd *cmd,
							void (*func)( struct sdio_device *, struct sdio_cmd *, void *),
							_Uint32t timeout, int retries );
extern int				sdio_setup_cmd( struct sdio_cmd *cmd, _Uint32t flgs,
							_Uint32t op, _Uint32t arg );
extern int				sdio_setup_cmd_ext( struct sdio_cmd *cmd, _Uint32t flgs,
							_Uint32t op, _Uint32t arg, _Uint32t earg );
extern int				sdio_setup_cmd_io( struct sdio_cmd *cmd, _Uint32t flgs,
							int blks, int blksz, void *sgl, int sgc, void *mhdl );

extern void				*sdio_client_hdl( struct sdio_device *device );
extern void				*sdio_bs_hdl( struct sdio_device *device );
extern sdio_cid_t		*sdio_get_cid( struct sdio_device *device );
extern void				*sdio_get_raw_cid( struct sdio_device *device );
extern sdio_csd_t		*sdio_get_csd( struct sdio_device *device );
extern void				*sdio_get_raw_csd( struct sdio_device *device );
extern sdio_ecsd_t		*sdio_get_ecsd( struct sdio_device *device );
extern void				*sdio_get_raw_ecsd( struct sdio_device *device );
extern void				*sdio_get_raw_scr( struct sdio_device *device );
extern int				sdio_sd_switch( struct sdio_device *device, int mode, int grp, uint8_t val, uint8_t *switch_status );
extern int				sdio_mmc_switch( struct sdio_device *device, uint32_t cmdset, uint32_t mode, uint32_t index, uint32_t value, uint32_t timeout );
extern int				sdio_send_ext_csd( struct sdio_device *device, uint8_t *csd );
extern int				sdio_mmc_rpmb_rw( struct sdio_device *device, void *pf, int nf, int flgs );
extern int				sdio_mmc_gen_man( struct sdio_device *device, uint8_t op, void *buf, int blklen, int blkcnt, uint32_t arg, uint32_t flgs);
extern int				sdio_hc_info( struct sdio_device *device, sdio_hc_info_t *info );
extern int				sdio_dev_info( struct sdio_device *device, sdio_dev_info_t *info );
extern int				sdio_retune( struct sdio_device *device );
#define SDIO_CACHE_DISABLE	0
#define SDIO_CACHE_ENABLE	1
#define SDIO_CACHE_FLUSH	2
extern int				sdio_cache( struct sdio_device *device, int op, uint32_t timeout );
extern int				sdio_set_partition( struct sdio_device *device, _Uint32t partition );
extern struct sdio_device *sdio_device_lookup( struct sdio_connection *connection,
							sdio_device_instance_t *instance );
extern int				sdio_timer_settime( int tid, uint32_t sec, uint32_t nsec, int repeat );
extern int				sdio_connect( sdio_connect_parm_t *parm, struct sdio_connection **connection );
#define SDIO_ENUM_DISABLE	0
#define SDIO_ENUM_ENABLE	1
extern int				sdio_enum( struct sdio_connection *connection, int action );
extern int				sdio_disconnect( struct sdio_connection *connection );
extern int				sdio_attach( struct sdio_connection *connection, sdio_device_instance_t *instance, struct sdio_device **hdl, void *client_hdl );
extern int				sdio_detach( struct sdio_device *device );

#define SDIO_POWERED_OFF		0x00
#define SDIO_POWERED_ON			0x01
#define SDIO_POWER_OFF_SHORT	0x02
#define SDIO_POWER_OFF_LONG		0x03
extern int				sdio_pwroff_notify( struct sdio_device *device, int op, int timeout );

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/mainline/hardware/devb/sdmmc/sdiodi/include/sdiodi.h $ $Rev: 971272 $")
#endif