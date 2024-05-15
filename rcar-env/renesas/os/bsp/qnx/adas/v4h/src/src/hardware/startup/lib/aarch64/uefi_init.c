/*
 * Copyright (c) 2022, BlackBerry Limited. All rights reserved.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

#include "startup.h"
#include <hw/uefi.h>

/**
 * @brief Configure the global UEFI service pointers
 *
 * The EFI_IMAGE_ENTRY_POINT provides the two arguments:
 *  1. EFI_HANDLE ImageHandle
 *  2. EFI_SYSTEM_TABLE *SystemTable
 * The SystemTable is a pointer to the image's system table which contains
 * the standard console input and output protocols and the services pointers
 * such as EFI_BOOT_SERVICES and EFI_RUNTIME_SERVICES tables.
 *
 * This routine is responsible to verify the validity of the service pointers,
 * and set the global UEFI services pointers that are used in the rest of the
 * startup code.
 * A failure in this routine should assert the boot since a system without
 * a valid EFI_SYSTEM_TABLE would not be compliant with the UEFI specification,
 * and crash later in the code.
 */
void uefi_init(void)
{
    static unsigned uefi_init_done = 0;

    /* Prevent reentry */
    ASSERT(uefi_init_done == 0);
    uefi_init_done = 1;

    /* Assign the standard arguments for an UEFI application */
    efi_image_handle = (void*)boot_regs[0];
    efi_system_table = (void*)boot_regs[1];

    /* validation of  UEFI service pointers and EFI_SYSTEM_TABLE signature has been done in is_uefi_boot() */

    /* Assign the UEFI services pointers */
    efi_boot_services = efi_system_table->BootServices;
    efi_runtime_services = efi_system_table->RuntimeServices;

    /* Configure the function pointer to exit the EFI Boot Services */
    uefi_exit_init();
}



#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
