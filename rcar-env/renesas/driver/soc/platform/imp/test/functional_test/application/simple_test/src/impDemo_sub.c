/******************************************************************************
     IMP Sample Code (How to use IMP Framework with IMP Driver)
     Copyright (c) 2017-2019 Renesas Electronics Corporation.

    [File] impDemo_sub.c
******************************************************************************/ 
#include <stdio.h>
#include <stdint.h>
//#include <sys/mman.h>
/* Demo */
#include "impDemo.h"


void FillMemory( void *mem, uint8_t data, uint32_t size )
{
	//uint32_t *m8 = (uint32_t*)mem;
	uint8_t *m8 = (uint8_t*)mem;
	size_t i;

	for(i=0;i<size;i++){
		m8[i] = data;
	}
}
void FillMemory_demo( void *mem, uint8_t *data, uint32_t size)
{
	size_t i;
	uint8_t *m8 = (uint8_t*)mem;

	for(i=0;i<size;i++){
		m8[i] = data[i];
	}
	
	return ;
}

void OutputMemory( void *mem, uint32_t xsize, uint32_t ysize, uint32_t bytepp )
{
	size_t	i, j;
	uint8_t	*m8;
	uint16_t	*m16;
	uint32_t	*m32;

	printf("-- OutPutMemory --\n");

	switch(bytepp){
		case 1U:
			m8 = (uint8_t*)mem;
			for(i=0;i<ysize;i++){
				for(j=0;j<xsize;j++){
					printf("%02X ", m8[j+(i*xsize)]);
				}
				printf("\n");
			}
			break;

		case 2U:
			m16 = (uint16_t*)mem;
			for(i=0;i<ysize;i++){
				for(j=0;j<xsize;j++){
					printf("%04X ", m16[j+(i*xsize)]);
				}
				printf("\n");
			}
			break;

		case 4U:
			m32 = (uint32_t*)mem;
			for(i=0;i<ysize;i++){
				for(j=0;j<xsize;j++){
					printf("%08X ", m32[j+(i*xsize)]);
				}
				printf("\n");
			}
			break;
	}

	return ;
}

void Output_check( void *mem, uint32_t *data, uint32_t size, uint32_t bytepp )
{
	size_t	i;
	uint32_t err_cnt = 0;
	uint8_t	*m8_data1;
	uint8_t	*m8_data2;
	uint16_t	*m16_data1;
	uint16_t	*m16_data2;
	uint32_t	*m32_data1;
	uint32_t	*m32_data2;
	
	printf("\nCheck to OUTPUT Image\n");
	switch(bytepp){
		case 1U:
			m8_data1 = (uint8_t*)mem;
			m8_data2 = (uint8_t*)data;
			for(i=0;i<size;i++){
				//m8_data2[i] = (uint8_t)data[i];
				if( m8_data1[i] != m8_data2[i]) {
					if (err_cnt < 100) {
						printf("Ouput Image Data [%d] : %02X\n", i, m8_data1[i]);
						printf("Expected_value   [%d] : %02X\n", i, m8_data2[i]);
						if (err_cnt == 99) {
							printf("Too many diffs. stop logging\n");
						}
					}
					err_cnt++;
				}
			}
			break;

		case 2U:
			m16_data1 = (uint16_t*)mem;
			m16_data2 = (uint16_t*)data;
			for(i=0;i<size;i++){
//				m16_data2[i] = (uint16_t)data[i];
				if( m16_data1[i] != m16_data2[i]) {
					if (err_cnt < 100) {
						printf("Ouput Image Data [%d]   : %04X\n", i, m16_data1[i]);
						printf("Expected_value   [%d]   : %04X\n", i, m16_data2[i]);
						if (err_cnt == 99) {
							printf("Too many diffs. stop logging\n");
						}
					}
					err_cnt++;
				}
			}
			break;

		case 3U:
			m32_data1 = (uint32_t*)mem;
			m32_data2 = (uint32_t*)data;
			for(i=0;i<size;i++){
				//m32_data2[i] = (uint32_t)data[i];
				if( m32_data1[i] != m32_data2[i]) {
					if (err_cnt < 100) {
						printf("Ouput Image Data : %08X\n", m32_data1[i]);
						printf("Expected_value : %08X\n", m32_data2[i]);
						if (err_cnt == 98) {
							printf("Too many diffs. stop logging\n");
						}
					}
					err_cnt++;
					err_cnt++;
				}
			}
			break;
	}
	
	if(err_cnt == 0 )
	{
		printf("OutPut Image OK!!\n");
	}

	return ;
}