/*
 * $QNXLicenseC:
 * Copyright 2015, QNX Software Systems. 
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


#include "startup.h"
#include "hw/uefi.h"

static _Uintn
do_raminfo(void) {
	const EFI_BOOT_SERVICES	*const bs = efi_system_table->BootServices;
	_Uintn					map_size = 0;
	EFI_MEMORY_DESCRIPTOR	*desc = NULL;
	EFI_STATUS				status = 0;
	_Uintn					map_key;
	_Uintn					desc_size;
	uint32_t				desc_version;

	// Get the UEFI memory map size
	status = bs->GetMemoryMap(&map_size, desc, &map_key, &desc_size, &desc_version);
	if (status != EFI_BUFFER_TOO_SMALL) {
		crash("Failed to query the UEFI memory map with error %l\n", status);
	}

	do {
		// Allocating the memory creates at least one new descriptor
		map_size += desc_size;
		status = bs->AllocatePool(EfiLoaderData, map_size, (void **)&desc);
		if ((desc == NULL) || (status != EFI_SUCCESS)) {
			crash("Fatal error while allocating the UEFI memory map buffer\n");
		}

		// Get the UEFI memory map
		status = bs->GetMemoryMap(&map_size, desc, &map_key, &desc_size, &desc_version);
		if (status != EFI_SUCCESS) {
			bs->FreePool(desc);
			desc = NULL;
		}
	} while (status == EFI_BUFFER_TOO_SMALL);
	if (status != EFI_SUCCESS) {
		crash("Failed to get the UEFI memory map with error %l\n", status);
	}

	paddr_t					starting_paddr = NULL_PADDR;
	paddr_t					ending_paddr = NULL_PADDR;
	EFI_MEMORY_DESCRIPTOR	*avoid_list = NULL;
	EFI_MEMORY_DESCRIPTOR	**owner = &avoid_list;

	// Tell the startup library about all the ram
	while(map_size != 0) {
		// Make sure the start address fits in a paddr_t (think 32bit systems)
		if((paddr_t)desc->PhysicalStart == desc->PhysicalStart) {
			uint64_t 		nbytes  = desc->NumberOfPages * 0x1000u;
			paddr64_t const end_paddr = desc->PhysicalStart + nbytes;
			if((paddr_t)end_paddr != end_paddr) {
				// have to shrink size
				nbytes =  (~(paddr_t)0 + 1) - desc->PhysicalStart;
				desc->NumberOfPages = nbytes / 0x1000u;
			}
			switch(desc->Type) {
			case EfiReservedMemoryType:
			case EfiUnusableMemory:
			case EfiMemoryMappedIO:
			case EfiMemoryMappedIOPortSpace:
				break;
			case EfiACPIReclaimMemory:
			case EfiACPIMemoryNVS:
			case EfiPalCode:
			case EfiLoaderCode:
			case EfiLoaderData:
			case EfiBootServicesCode:
			case EfiBootServicesData:	
			case EfiRuntimeServicesCode:
			case EfiRuntimeServicesData:	
				if((PADDR_T)desc->PhysicalStart == desc->PhysicalStart) {
					// Maintain the list in a low to high paddr order
					// so that we're more likely to mark the unusable
					// regions as such before alloc_ram() hands them
					// out to somebody else.
					*owner = desc;
					owner = (void *)&desc->VirtualStart;
					*owner = NULL;
				}
				/* FALLTHROUGH */
			case EfiConventionalMemory:	
				if(desc->PhysicalStart != ending_paddr) {
					if(starting_paddr != NULL_PADDR) {
						add_ram(starting_paddr, ending_paddr - starting_paddr);
					}
					starting_paddr = desc->PhysicalStart;
					ending_paddr = starting_paddr;
				}
				ending_paddr += nbytes;
				break;
			default:
				crash("Unexpected memory type: %d\n", desc->Type);
			}
		}
		desc = (void *)((uintptr_t)desc + desc_size);
		map_size -= desc_size;
	}
	if(starting_paddr != ending_paddr) {
		add_ram(starting_paddr, ending_paddr - starting_paddr);
	}
	//Tell the library about all the areas we need to avoid
	while(avoid_list != NULL) {
		size_t const	nbytes = avoid_list->NumberOfPages*0x1000u;
		switch(avoid_list->Type) {
		case EfiACPIReclaimMemory:
		case EfiACPIMemoryNVS:
		case EfiPalCode:
		case EfiRuntimeServicesCode:
		case EfiRuntimeServicesData:
			// This memory we can't use at all
			alloc_ram(avoid_list->PhysicalStart, nbytes, 0);
			break;
		case EfiLoaderCode:
		case EfiLoaderData:
		case EfiBootServicesCode:
		case EfiBootServicesData:
			// This memory, we can use after startup is done
			avoid_ram(avoid_list->PhysicalStart, nbytes);
			break;
		}
		avoid_list = (void *)(uintptr_t)avoid_list->VirtualStart;
	}
	return map_key;
}

int
init_raminfo_uefi(void) {

	if(efi_system_table == NULL) {
		return 0;
	}

	do_raminfo();

	uefi_exit_boot_services();

	return 1;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/init_raminfo_uefi.c $ $Rev: 915201 $")
#endif
