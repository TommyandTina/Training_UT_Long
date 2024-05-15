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


/* return true (!0) or false (0) depending on whether this was a UEFI boot */
unsigned is_uefi_boot(void)
{
    static unsigned uefi = 0x55555555;    /* a value that a boolean will never be */

    /* Assign the standard arguments for an UEFI application */
    efi_image_handle = (void*)boot_regs[0];
    efi_system_table = (void*)boot_regs[1];

    if (uefi == 0x55555555)
    {
        uefi = 0;

        do {
            /* Check validity of the UEFI service pointers */
            if (efi_system_table == NULL) {
                break;
            }
            if (efi_system_table->BootServices == NULL) {
                break;
            }
            if (efi_system_table->RuntimeServices == NULL) {
                break;
            }

            /* Verify EFI_SYSTEM_TABLE signature */
            if (efi_system_table->Hdr.Signature != EFI_SYSTEM_TABLE_SIGNATURE) {
                break;
            }

            /* Verify EFI_BOOT_SERVICES signature */
            if (efi_system_table->BootServices->Hdr.Signature != EFI_BOOT_SERVICES_SIGNATURE) {
                break;
            }

            /* Verify EFI_RUNTIME_SERVICES signature */
            if (efi_system_table->RuntimeServices->Hdr.Signature != EFI_RUNTIME_SERVICES_SIGNATURE) {
                break;
            }
            uefi = 1;

        } while (0);

        if (uefi == 0) {
            efi_image_handle = NULL;
            efi_system_table = NULL;
        }
    }

    return uefi;
}



#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
