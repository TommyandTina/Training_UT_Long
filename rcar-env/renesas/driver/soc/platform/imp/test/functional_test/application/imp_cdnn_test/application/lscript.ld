/* Linker script to place sections and symbol values.
 * It references following symbols, which must be defined in code:
 *   start64 : Entry point
 * 
 * It defines following symbols, which code can use without definition:
 *   __cs3_peripherals
 *   __code_start
 *   __exidx_start
 *   __exidx_end
 *   __data_start
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   __stack
 *   __el3_stack
 *   __ttb0_l1
 *   __ttb0_l2_ram
 *   __ttb0_l2_private
 *   __ttb0_l2_periph
 *   __top_of_ram
 */


_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x2000;
_HEAP_SIZE = DEFINED(_HEAP_SIZE) ? _HEAP_SIZE : 0x2000000;

_EL1_STACK_SIZE = DEFINED(_EL1_STACK_SIZE) ? _EL1_STACK_SIZE : 0x2000;
_EL2_STACK_SIZE = DEFINED(_EL2_STACK_SIZE) ? _EL2_STACK_SIZE : 1024;
_EL3_STACK_SIZE = DEFINED(_EL3_STACK_SIZE) ? _EL3_STACK_SIZE : 1024;

MEMORY
{
   psu_ddr_0_S_AXI_BASEADDR : ORIGIN = 0x54C00000, LENGTH = 0x2B400000
}


ENTRY(start64)

SECTIONS
{
	__peripherals_start = 0xE6000000;
	__peripherals_end   = 0x100000000;

    /*
     * GICv3 distributor
     */
    .gicd 0xF1010000 (NOLOAD):
    {
        *(.gicd)
    }

    /*
     * GICv3 redistributors
     * 128KB for each redistributor in the system
     */
    .gicr 0xF1020000 (NOLOAD):
    {
        *(.gicr)
    }

    .vectors 0x54C00000:
    {
        __code_start = .;
        KEEP(*(StartUp))
        KEEP(*(EL1VECTORS EL2VECTORS EL3VECTORS _vector_table))
    } > psu_ddr_0_S_AXI_BASEADDR
	


    .init :
    {
        KEEP (*(SORT_NONE(.init)))
    } > psu_ddr_0_S_AXI_BASEADDR

    .text :
    {
        *(.text*)
		. = ALIGN(1024*1024*2);
		__text_end = .;
    } > psu_ddr_0_S_AXI_BASEADDR
	
	.fixedPlace 0x49300000 (NOLOAD) : {
		__fixedPlace_RVX_MEM_start = .;
		*(.fixedPlace_RVX_MEM)
		__fixedPlace_RVX_MEM_end = .;
		__fixedPlace_start = .;
		*(.fixedPlace)
		*(.fixedPlace.*)
		KEEP (*(.fixedPlace))
		__fixedPlace_end = .;
	} > psu_ddr_0_S_AXI_BASEADDR
	
    .fini :
    {
        KEEP (*(SORT_NONE(.fini)))
    } > psu_ddr_0_S_AXI_BASEADDR

    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    } > psu_ddr_0_S_AXI_BASEADDR

    .eh_frame :
    {
        KEEP (*(.eh_frame))
    } > psu_ddr_0_S_AXI_BASEADDR

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > psu_ddr_0_S_AXI_BASEADDR

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > psu_ddr_0_S_AXI_BASEADDR

    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } > psu_ddr_0_S_AXI_BASEADDR

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array ))
        PROVIDE_HIDDEN (__init_array_end = .);
    } > psu_ddr_0_S_AXI_BASEADDR

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array ))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } > psu_ddr_0_S_AXI_BASEADDR

    .ctors :
    {
        /* gcc uses crtbegin.o to find the start of
           the constructors, so we make sure it is
           first.  Because this is a wildcard, it
           doesn't matter if the user does not
           actually link against crtbegin.o; the
           linker won't look for a file to match a
           wildcard.  The wildcard also means that it
           doesn't matter which directory crtbegin.o
           is in.  */
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        /* We don't want to include the .ctor section from
           the crtend.o file until after the sorted ctors.
           The .ctor section from the crtend file contains the
           end of ctors marker and it must be last */
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    } > psu_ddr_0_S_AXI_BASEADDR

    .dtors :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    } > psu_ddr_0_S_AXI_BASEADDR

    .jcr :
    {
        KEEP (*(.jcr))
    } > psu_ddr_0_S_AXI_BASEADDR

    .data :
    {
        __data_start = . ;
        *(.data .data.* .gnu.linkonce.d.*)
        SORT(CONSTRUCTORS)
    } > psu_ddr_0_S_AXI_BASEADDR

    .bss :
    {
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
    } > psu_ddr_0_S_AXI_BASEADDR

    .stack (NOLOAD):
    {
		. = ALIGN(64);
		_stack_end = .;
		. += _STACK_SIZE;
		__stack = .;
    } > psu_ddr_0_S_AXI_BASEADDR

    .el3_stack (NOLOAD):
    {
		. = ALIGN(64);
		_el3_stack_end = .;
		. += _EL3_STACK_SIZE;
		__el3_stack = .;
    } > psu_ddr_0_S_AXI_BASEADDR
    
    .el2_stack (NOLOAD):
    {
		. = ALIGN(64);
		_el2_stack_end = .;
		. += _EL2_STACK_SIZE;
		__el2_stack = .;
    } > psu_ddr_0_S_AXI_BASEADDR
    .el1_stack (NOLOAD):
    {
		. = ALIGN(64);
		_el1_stack_end = .;
		. += _EL1_STACK_SIZE;
		__el1_stack = .;
    } > psu_ddr_0_S_AXI_BASEADDR

    .ttb0 (NOLOAD):
    {
		. = ALIGN(1024*1024*2);
		__ttb0_l1_base = .;
		__ttb0_l1 = .;
		. = . + 0x1000;
		__ttb0_l1_end = .;

		. = ALIGN(4096);
		__ttb0_l2_base = .;
		. = . + 0x1000 * 4; /* Tables for 4GB */
		. = ALIGN(1024*1024*2);
		__ttb0_l2_end = .;
    } > psu_ddr_0_S_AXI_BASEADDR


	.note.gnu.build-id :
	{
	  . = ALIGN(4);
	  PROVIDE( _note = .);
	  *(.note.gnu.build-id)
	  . = ALIGN(4);
	  *(.note.gnu.build-id.*);
	  . = ALIGN(4);
	  PROVIDE( _enote = .);
	} > psu_ddr_0_S_AXI_BASEADDR
	
	.heap (NOLOAD):
    {
       . = ALIGN(64);
	   _heap = .;
	   HeapBase = .;
	   _heap_start = .;
	   /*. += _HEAP_SIZE;*/
	   . = ORIGIN(psu_ddr_0_S_AXI_BASEADDR) + LENGTH(psu_ddr_0_S_AXI_BASEADDR);
	   _heap_end = .;
	   HeapLimit = .;
    } > psu_ddr_0_S_AXI_BASEADDR
	
    /*
     * The startup code uses the end of this region to calculate
     * the top of memory - don't place any RAM regions after it
     */
    __top_of_ram = .;
}
