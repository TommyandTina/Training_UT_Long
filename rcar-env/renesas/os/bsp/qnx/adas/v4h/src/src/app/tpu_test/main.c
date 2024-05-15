/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <pthread.h>
#include <sys/resmgr.h>
#include <hw/rcar_tpu.h>
#include <hw/rcar_cpg.h>
#include <aarch64/r-car.h>


/*  
 * TPU channel 1 is used in the test 
 */

#define RCAR_H3_TPU_INTERNAL_CLK   66.66 * 1000000
#define RCAR_M3_TPU_INTERNAL_CLK   66.66 * 1000000
#define RCAR_V3_TPU_INTERNAL_CLK   66.66 * 1000000
#define RCAR_D3_TPU_INTERNAL_CLK   62.50 * 1000000 /* Check MD12(SW12.1) for exact clock value */
#define RCAR_E3_TPU_INTERNAL_CLK   66.66 * 1000000 /* Check MD12(SW12.4) for exact clock value */
#define RCAR_GEN4_TPU_INTERNAL_CLK 66.66 * 1000000

#define TPU_CLOCK_DIV              TPU_CLOCK_DIV_64

#define TPU_PRIORITY  	21
#define TPU_PULSE 		100

#define COUNTER_OPERATION_TESTING  1
#define OUTPUT_A_WAVEFORM_TESTING 1

void * tpu_hdl;
int coid;
int chid;
struct sigevent event;
static tpu_functions_t tpu_hwfuncs;
int tpu_iid = -1;
int tpu_irq;
int tpu_verbose = 0;
int tpu_mode = 0;
int tpu_chan = 1;
int waveform_duty = 100000;
int waveform_cycle = 200000;
uint32_t  prr_val;

/* For exit  */
struct sigaction act;
int tpu_test_exit = 0;


static void tpu_test_shutdown(int signo)
{
    tpu_test_exit = 1;
}


void *tpu_intr_thread(void *data) {
    struct _pulse   pulse;
    iov_t           iov;
    int	            rcvid;
    uint16_t        tsr = 0;
	uint64_t        n_overflow = 0;
	float           time = 0;

    SETIOV(&iov, &pulse, sizeof(pulse));
    memset(&pulse, 0, sizeof( pulse));

	fprintf(stdout, "Wait a moment, then press Ctrl-C to stop the test\n");

    while (!tpu_test_exit)  {
        if ((rcvid = MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL)) == -1)
            continue;

        switch (pulse.code) {
            case TPU_PULSE:
                tpu_hwfuncs.intr_isr(tpu_hdl, &tsr);
                /* TODO : process tsr */
				n_overflow++;
                InterruptUnmask(tpu_irq, tpu_iid);
                break;
            default:
                if (rcvid)
                    MsgReplyv(rcvid, ENOTSUP, &iov, 1);
                break;
        }
    }

	if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_H3) {
		time = (1 / (RCAR_H3_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_M3W) {
		time = (1 / (RCAR_M3_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_M3N) {
		time = (1 / (RCAR_M3_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_V3U) {
		time = (1 / (RCAR_GEN4_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
    } else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_V4H) {
		time = (1 / (RCAR_GEN4_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_V3M) {
		time = (1 / (RCAR_V3_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_V3H) {
		time = (1 / (RCAR_V3_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_D3) {
		time = (1 / (RCAR_D3_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_E3) {
		time = (1 / (RCAR_E3_TPU_INTERNAL_CLK / TPU_CLOCK_DIV)) * (0xFFFF) * n_overflow;
	}
	
	fprintf(stdout, "  Num Overflow = %ld\n", n_overflow);
	fprintf(stdout, "      Measured = %f sec, %f nsec\n", time, time * 1000000000);

    return (NULL);
}

/* 
 * PFC setting for TPU0TO1 pin 
 */
void pfc_setting_for_tpu_tpu0t01() 
{
	uint32_t tmp;
	uintptr_t PFC_vbase;

	if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_V3U) {
        PFC_vbase  = mmap_device_io(RCAR_GEN4_GPIO_SIZE,  RCAR_GEN4_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP2);
        if (PFC_vbase != (uintptr_t)MAP_FAILED) {
        
			tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_GPSR);
			tmp |= (1 << 22);
			
			out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
			out32(PFC_vbase + RCAR_GEN4_GPIO_GPSR, tmp);
			
			tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_IP2SR);
			tmp &= ~(0xF << 24);
			
			out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
			out32(PFC_vbase + RCAR_GEN4_GPIO_IP2SR, tmp);
			
			/* Unmap register */
			munmap_device_io(PFC_vbase, RCAR_GEN4_GPIO_SIZE);
		}
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_D3) {
		PFC_vbase  = mmap_device_io(RCAR_GEN3_GPIO_SIZE,  RCAR_GEN3_PFC_BASE);
		if (PFC_vbase != (uintptr_t)MAP_FAILED) {
			
		    tmp  = in32(PFC_vbase + RCAR_GEN3_PFC_GPSR5);
            tmp |= (1 << 1);
			
            out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
            out32(PFC_vbase + RCAR_GEN3_PFC_GPSR5, tmp);
			
			tmp = in32(PFC_vbase + RCAR_GEN3_PFC_IPSR12);
            tmp &= ~(0xF << 16);
	        
	        out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
            out32(PFC_vbase + RCAR_GEN3_PFC_IPSR12, tmp);
			
			/* Unmap register */
			munmap_device_io(PFC_vbase, RCAR_GEN3_GPIO_SIZE);
		}
	/* On WhiteHawk board, TPU0TO1 is GP2_07 */
	} else if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_V4H) {
        PFC_vbase  = mmap_device_io(RCAR_GEN4_GPIO_SIZE,  RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP2);
        if (PFC_vbase != (uintptr_t)MAP_FAILED) {
        
            if (tpu_chan == 0) {
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_GPSR);
                tmp |= (1 << 8 );
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_GPSR, tmp);
                
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_IP0SR);
                tmp &= ~(0xF << 28);
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_IP0SR, tmp);
            } else if (tpu_chan == 1) {
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_GPSR);
                tmp |= (1 << 7 );
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_GPSR, tmp);
                
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_IP1SR);
                tmp &= ~(0xF << 0);
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_IP0SR, tmp);
            } else if (tpu_chan == 2) {
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_GPSR);
                tmp |= (1 << 12 );
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_GPSR, tmp);
                
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_IP1SR);
                tmp &= ~(0xF << 16);
                tmp |= (1 << 16);
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_IP0SR, tmp);
            } else if (tpu_chan == 3) {
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_GPSR);
                tmp |= (1 << 13 );
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_GPSR, tmp);
                
                tmp = in32(PFC_vbase + RCAR_GEN4_GPIO_IP1SR);
                tmp &= ~(0xF << 20);
                tmp |= (1 << 20);
                
                out32(PFC_vbase + RCAR_GEN4_GPIO_PMMR, ~tmp);
                out32(PFC_vbase + RCAR_GEN4_GPIO_IP0SR, tmp);
            }
			/* Unmap register */
			munmap_device_io(PFC_vbase, RCAR_GEN4_GPIO_SIZE);
		}
	}
}

static int _option(char* options)
{
    int         opt;
    char        *value;
    static char	*opts[] = {
                            "chan",
                            "mode",
                            "duty",
                            "cycle",
                            "verbose",
                            NULL
                          };

    while( *options != '\0' ) {
		if( ( opt = getsubopt( &options, opts, &value ) ) == -1 ) {
            continue;
		}

		switch( opt ) {
			case 0:			// channel
                tpu_chan = strtoul(value, NULL, 0);
                break;
			case 1:			// mode
                tpu_mode = strtoul(value, NULL, 0);
                break;
			case 2:			// duty
                waveform_duty = strtoul(value, NULL, 0);
                break;
			case 3:			// cycle
                waveform_cycle = strtoul(value, NULL, 0);
                break;
			case 4:			// verbose
                tpu_verbose = strtoul(value, NULL, 0);
                break;
            default:
               break;
        }
    }
    
    return EOK;
}


int main(int argc, char **argv) 
{
    static cpg_mgr_funcs_t cpg_hwfuncs;
    pthread_t thread_id; 
    int error = 0;
    char verbose[64];
	uintptr_t              prr;

    if (argc > 1) _option(argv[1]);

	if((prr = mmap_device_io(4, RCAR_PRODUCT_REGISTER)) == (uintptr_t)MAP_FAILED) {
		tpu_slogf( _SLOG_ERROR,  0, "Couldn't mapping for PRR register: %s\n", strerror(errno));
		return -1;
	}
	
	prr_val = in32(prr);
	
	if(prr != (uintptr_t)MAP_FAILED)
		munmap_device_io(prr, 4);

    if ((cpg_mgr_getfuncs(&cpg_hwfuncs, sizeof(cpg_mgr_funcs_t))) != EOK) {
		tpu_slogf( _SLOG_ERROR,  0, "tpu_test : Unable to get cpg hw funcs");
		return -1;
	}
    else {
        cpg_hwfuncs.init(0);
		if((RCAR_PRODUCT_ID(prr_val) != PRODUCT_ID_RCAR_V3U) && (RCAR_PRODUCT_ID(prr_val) != PRODUCT_ID_RCAR_V4H)) {
		    cpg_hwfuncs.mstp_enable(CPG_MSTP_GEN3_TPU0);
			tpu_irq = RCAR_GEN3_INTCSYS_TPU;
		}
		else {
            cpg_hwfuncs.mstp_enable(CPG_MSTP_GEN4_TPU);
            if(RCAR_PRODUCT_ID(prr_val) == PRODUCT_ID_RCAR_V3U) {
			    tpu_irq = RCAR_GEN4_INTCSYS_SPI_515;
			} else {
				tpu_irq = RCAR_GEN4_INTCSYS_SPI_308;
			}
		}
        cpg_hwfuncs.deinit();
    }

    if ((get_tpufuncs(&tpu_hwfuncs, sizeof(tpu_functions_t))) != EOK) {
		tpu_slogf( _SLOG_ERROR,  0, "tpu_test : Unable to get tpu hw funcs");
        return -1;
	}

    if((chid = ChannelCreate(0)) == -1) {
       tpu_slogf( _SLOG_ERROR,  0, "tpu_test : ChannelCreate failed (%s)", strerror(errno ));
       return -1;
    }

    if((coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
       tpu_slogf( _SLOG_ERROR,  0, "tpu_test : ConnectAttach failed (%s)", strerror(errno ));
       ChannelDestroy(chid);
       return -1;
    }    

    sprintf(verbose, "verbose=%d", tpu_verbose);
    tpu_hwfuncs.init(verbose);

    /* Attach TPU Timer channel 1 with enable interrupts */
    if((tpu_hdl = tpu_hwfuncs.channel_attach(tpu_chan, TPU_ATTACH_PRIORITY_STRICT | TPU_ATTACH_INTERRUPT_ENABLE)) == NULL) {
       tpu_slogf( _SLOG_ERROR,  0, "tpu_test : channel_attach failed (%s)", strerror(errno ));
       ConnectDetach(coid);
       ChannelDestroy(chid);
       return -1;
    }

    if (tpu_mode == 0) {
        printf("----------------------------------------\n");
        printf("    TPU free-running counter testing\n");
        printf("----------------------------------------\n");
        if (!error && (tpu_hwfuncs.set_clock(tpu_hdl, TPU_CLOCK_DIV_64, TPU_CLOCK_EDGE_RISE) != EOK))
        error++;
    }
    else if (tpu_mode == 1) {
        printf("----------------------------------------\n");
        printf("     TPU output waveform testing\n");
        printf("----------------------------------------\n");
        error = tpu_hwfuncs.config_waveform(tpu_hdl, waveform_duty, waveform_cycle);
    }
    else {
        /* Counter counts at rising edge of Internal clock/64 */
        if (!error && (tpu_hwfuncs.set_clock(tpu_hdl, TPU_CLOCK_DIV_64, TPU_CLOCK_EDGE_RISE) != EOK))
            error++;

        /* Counter is cleared on TGRB match */
        if (!error && (tpu_hwfuncs.set_clear_src(tpu_hdl, TPU_TCNT_CLR_ON_MATCH_B) != EOK))
            error++;

        /* Ouput 1 on TGRA match and Intial output is 0  */
        if (!error && (tpu_hwfuncs.set_pin_state(tpu_hdl, TPU_PIN_OUTPUT_1, TPU_PIN_INITIAL_0) != EOK))
            error++;

        /* Set period to TGRA, TGRB and TGRC */
        if (!error && (tpu_hwfuncs.set_general_reg(tpu_hdl, TPU_REG_GENERAL_A, 0x01FF) != EOK))
            error++;
        if (!error && (tpu_hwfuncs.set_general_reg(tpu_hdl, TPU_REG_GENERAL_B, 0x3FFF) != EOK))
            error++;
        if (!error && (tpu_hwfuncs.set_general_reg(tpu_hdl, TPU_REG_GENERAL_C, 0x0FFF) != EOK))
            error++;

        /* Set PWM mode, TGRA and TGRC as buffered operation, ...  */
        if (!error && (tpu_hwfuncs.set_mode(tpu_hdl, TPU_MODE_PWM | TPU_MODE_TGRA_BUF_OP | TPU_MODE_BUF_WR_ON_MATCH) != EOK))
            error++;
    }

    if (!error) {

        /* Setup for exit */
        act.sa_handler = tpu_test_shutdown;
        sigaction(SIGHUP, &act, NULL);
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGQUIT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);
        
        if (pthread_create( &thread_id, NULL, tpu_intr_thread, NULL)) {
            tpu_slogf( _SLOG_ERROR,  0, "tpu_test : pthread_create failed (%s)", strerror(errno ));
            goto fail1;
        }

        SIGEV_INTR_INIT(&event);
        event.sigev_notify   = SIGEV_PULSE;
        event.sigev_coid     = coid;
        event.sigev_code     = TPU_PULSE;
        event.sigev_priority = TPU_PRIORITY;

        /* Attach TPU interrupt, it can be an InterruptAttachEvent or an InterruptAttach 
         * Here is use InterruptAttachEvent */
        if ((tpu_iid = InterruptAttachEvent(tpu_irq, &event, _NTO_INTR_FLAGS_TRK_MSK)) == -1) {
            tpu_slogf( _SLOG_ERROR,  0, "tpu_test : InterruptAttachEvent failed (%s)", strerror(errno ));
            goto fail1;
        }

        pfc_setting_for_tpu_tpu0t01();

        /* Clear TCNT */
        tpu_hwfuncs.set_count(tpu_hdl, 0);

        /* Start Timer */
        tpu_hwfuncs.start(tpu_hdl, NULL);

        pthread_join(thread_id, NULL);
    }

    InterruptDetach(tpu_iid);
	
fail1:
    tpu_hwfuncs.channel_release(tpu_hdl);
    ConnectDetach(coid);
    ChannelDestroy(chid);
}


 
