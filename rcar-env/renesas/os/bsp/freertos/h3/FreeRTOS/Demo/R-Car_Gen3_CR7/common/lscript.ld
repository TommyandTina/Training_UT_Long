
/* These values may need changing depending on your app */
_STACK_SIZE       = 0x4000;
_IRQ_STACK_SIZE   = 0x400;
_FIQ_STACK_SIZE   = 0x400;
_SVC_STACK_SIZE   = 0x400;
_ABORT_STACK_SIZE = 0x400;
_UNDEF_STACK_SIZE = 0x400;
_HEAP_SIZE        = 0x8000;
/* Heap uses HeapBase global var, see configTOTAL_HEAP_SIZE */

/* Note: See RCAR_VIDEO_BUFFER_START for additional memory used */

MEMORY
{
   ddr_base_addr : ORIGIN = 0x40040000, LENGTH = 0x060000
}

ENTRY(_start)

SECTIONS
{
.text : {
   *(.isr_vector)
   *(.vectors)
   *(.boot)
   *(.text)
   *(.text.*)
   *(.rawpcm)
   *(.gnu.linkonce.t.*)
   *(.plt)
   *(.gnu_warning)
   *(.gcc_execpt_table)
   *(.glue_7)
   *(.glue_7t)
   *(.ARM.extab)
   *(.gnu.linkonce.armextab.*)
} > ddr_base_addr

.init : {
   KEEP (*(.init))
} > ddr_base_addr

.fini : {
   KEEP (*(.fini))
} > ddr_base_addr

.interp : {
   KEEP (*(.interp))
} > ddr_base_addr

.rodata : {
   __rodata_start = .;
   *(.rodata)
   *(.rodata.*)
   *(.gnu.linkonce.r.*)
   __rodata_end = .;
} > ddr_base_addr

.rodata1 : {
   __rodata1_start = .;
   *(.rodata1)
   *(.rodata1.*)
   __rodata1_end = .;
} > ddr_base_addr

.data : {
   __data_start = .;
   *(.data)
   *(.data.*)
   *(.gnu.linkonce.d.*)
   *(.jcr)
   *(.got)
   *(.got.plt)
   __data_end = .;
} > ddr_base_addr

.data1 : {
   __data1_start = .;
   *(.data1)
   *(.data1.*)
   __data1_end = .;
} > ddr_base_addr

.got : {
   *(.got)
} > ddr_base_addr


.ctors : {
   __CTOR_LIST__ = .;
   ___CTORS_LIST___ = .;
   KEEP (*crtbegin.o(.ctors))
   KEEP (*(EXCLUDE_FILE(*crtend.o) .ctors))
   KEEP (*(SORT(.ctors.*)))
   KEEP (*(.ctors))
   __CTOR_END__ = .;
   ___CTORS_END___ = .;
} > ddr_base_addr

.dtors : {
   __DTOR_LIST__ = .;
   ___DTORS_LIST___ = .;
   KEEP (*crtbegin.o(.dtors))
   KEEP (*(EXCLUDE_FILE(*crtend.o) .dtors))
   KEEP (*(SORT(.dtors.*)))
   KEEP (*(.dtors))
   __DTOR_END__ = .;
   ___DTORS_END___ = .;
} > ddr_base_addr

.fixup : {
   __fixup_start = .;
   *(.fixup)
   __fixup_end = .;
} > ddr_base_addr

.eh_frame : {
   *(.eh_frame)
} > ddr_base_addr

.eh_framehdr : {
   __eh_framehdr_start = .;
   *(.eh_framehdr)
   __eh_framehdr_end = .;
} > ddr_base_addr

.gcc_except_table : {
   *(.gcc_except_table)
} > ddr_base_addr


.ARM.exidx : {
   __exidx_start = .;
   *(.ARM.exidx*)
   *(.gnu.linkonce.armexidix.*.*)
   __exidx_end = .;
} > ddr_base_addr

.preinit_array : {
   __preinit_array_start = .;
   KEEP (*(SORT(.preinit_array.*)))
   KEEP (*(.preinit_array))
   __preinit_array_end = .;
} > ddr_base_addr

.init_array : {
   __init_array_start = .;
   KEEP (*(SORT(.init_array.*)))
   KEEP (*(.init_array))
   __init_array_end = .;
} > ddr_base_addr


.fini_array : {
   __fini_array_start = .;
   KEEP (*(SORT(.fini_array.*)))
   KEEP (*(.fini_array))
   __fini_array_end = .;
} > ddr_base_addr

.ARM.attributes : {
   __ARM.attributes_start = .;
   *(.ARM.attributes)
   __ARM.attributes_end = .;
} > ddr_base_addr

.sdata : {
   __sdata_start = .;
   *(.sdata)
   *(.sdata.*)
   *(.gnu.linkonce.s.*)
   __sdata_end = .;
} > ddr_base_addr

.sbss (NOLOAD) : {
   __sbss_start = .;
   *(.sbss)
   *(.sbss.*)
   *(.gnu.linkonce.sb.*)
   __sbss_end = .;
} > ddr_base_addr

.tdata : {
   __tdata_start = .;
   *(.tdata)
   *(.tdata.*)
   *(.gnu.linkonce.td.*)
   __tdata_end = .;
} > ddr_base_addr

.tbss : {
   __tbss_start = .;
   *(.tbss)
   *(.tbss.*)
   *(.gnu.linkonce.tb.*)
   __tbss_end = .;
} > ddr_base_addr

.bss (NOLOAD) : {
   . = ALIGN(4);
   __bss_start__ = .;
   *(.bss)
   *(.bss.*)
   *(.gnu.linkonce.b.*)
   *(COMMON)
   . = ALIGN(4);
   __bss_end__ = .;
} > ddr_base_addr

/* Generate Stack and Heap definitions */

.heap (NOLOAD) : {
   . = ALIGN(16);
   HeapBase = .;
   __heap_start__ = .;
   end = __heap_start__;
   _end = end;
   __end = end;
   . += _HEAP_SIZE;
   _heap_end = .;
   HeapLimit = .;
} > ddr_base_addr

.stack (NOLOAD) : {
   . = ALIGN(16);
   _stack_end = .;
   . += _STACK_SIZE;
   _stack = .;
   __stack = _stack;
   . = ALIGN(16);
   _irq_stack_end = .;
   . += _IRQ_STACK_SIZE;
   __irq_stack = .;
   _supervisor_stack_end = .;
   . += _SVC_STACK_SIZE;
   . = ALIGN(16);
   __supervisor_stack = .;
   _abort_stack_end = .;
   . += _ABORT_STACK_SIZE;
   . = ALIGN(16);
   __abort_stack = .;
   _fiq_stack_end = .;
   . += _FIQ_STACK_SIZE;
   . = ALIGN(16);
   __fiq_stack = .;
   _undef_stack_end = .;
   . += _UNDEF_STACK_SIZE;
   . = ALIGN(16);
   __undef_stack = .;
} > ddr_base_addr

_end = .;
}
