/* Linker script for generic-m
 *
 * Version:Sourcery G++ Lite 2009q1-161
 * BugURL:https://support.codesourcery.com/GNUToolchain/
 *
 * Copyright 2007, 2008 CodeSourcery, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 * */
OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
ENTRY(_start)
SEARCH_DIR(.)
GROUP(-lgcc -lc -lcs3 -lcs3unhosted -lcs3micro)

MEMORY
{
  /* ROM is a readable (r), executable region (x)      */
  rom (rx)   : ORIGIN = 0, LENGTH = 32k

  /* RAM is a readable (r), writable (w) and           */
  /* executable region (x)                             */
  ram (rwx)  : ORIGIN = 0x20000000, LENGTH = 8k
}

/* These force the linker to search for particular symbols from
 * the start of the link process and thus ensure the user's
 * overrides are picked up
 */
EXTERN(__cs3_reset_generic_m)
INCLUDE micro-names.inc
EXTERN(__cs3_interrupt_vector_micro)
EXTERN(__cs3_start_c main __cs3_stack __cs3_heap_end)

/*Original code  */
PROVIDE(__cs3_heap_start = _end);
PROVIDE(__cs3_heap_end = __cs3_region_start_ram + __cs3_region_size_ram);


/* Heap is defined in startup code.
EXTERN(__cs3_heap_start)
EXTERN(__cs3_heap_end)
*/

PROVIDE(__cs3_region_num = (__cs3_regions_end - __cs3_regions) / 20);
PROVIDE(__cs3_stack = __cs3_region_start_ram + __cs3_region_size_ram);

SECTIONS
{

  .text :
  {
    CREATE_OBJECT_SYMBOLS
    __cs3_region_start_rom = .;
    *(.cs3.region-head.rom)
    ASSERT (. == __cs3_region_start_rom, ".cs3.region-head.rom not permitted");

    /* Vector table */
    __cs3_interrupt_vector = __cs3_interrupt_vector_micro; 
    *(.cs3.interrupt_vector)  /* vector table */
    /* Make sure we pulled in an interrupt vector.  */
    ASSERT (. != __cs3_interrupt_vector_micro, "No interrupt vector");

    . = . + 0x80; /* skip the vector table */
    
    /* Map CS3 vector symbols to handler names in C */
    _start = __cs3_start_c;
    __cs3_reset = __cs3_start_c;
    
    *(.cs3.reset) /* Reset handler */
    
    /*--------------------------------------------*/
    /*__cs3_stack = 0x20002000;*/
    __cs3_isr_nmi = NMI_Handler;
    __cs3_isr_hard_fault = HARD_FAULT_Handler;
    __cs3_isr_systick = SYSTICK_Handler;
    __cs3_isr_external_0 = EXTERNAL_0_Handler;
    __cs3_isr_external_1 = EXTERNAL_1_Handler;
    __cs3_isr_external_2 = EXTERNAL_2_Handler;
    __cs3_isr_external_3 = EXTERNAL_3_Handler;
    __cs3_isr_external_4 = EXTERNAL_4_Handler;
    __cs3_isr_external_5 = EXTERNAL_5_Handler;
    __cs3_isr_external_6 = EXTERNAL_6_Handler;
    __cs3_isr_external_7 = EXTERNAL_7_Handler;
    __cs3_isr_external_8 = EXTERNAL_8_Handler;
    __cs3_isr_external_9 = EXTERNAL_9_Handler;
    __cs3_isr_external_10 = EXTERNAL_10_Handler;
    __cs3_isr_external_11 = EXTERNAL_11_Handler;
    __cs3_isr_external_12 = EXTERNAL_12_Handler;
    __cs3_isr_external_13 = EXTERNAL_13_Handler;
    __cs3_isr_external_14 = EXTERNAL_14_Handler;
    __cs3_isr_external_15 = EXTERNAL_15_Handler;


    *(.text .text.* .gnu.linkonce.t.*)
    *(.plt)
    *(.gnu.warning)
    *(.glue_7t) *(.glue_7) *(.vfp11_veneer)

    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.gcc_except_table)
  } >rom
  .eh_frame_hdr : ALIGN (4)
  {
    KEEP (*(.eh_frame_hdr))
  } >rom
  .eh_frame : ALIGN (4)
  {
    KEEP (*(.eh_frame))
  } >rom
  /* .ARM.exidx is sorted, so has to go in its own output section.  */
  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } >rom
  __exidx_end = .;
  .rodata : ALIGN (4)
  {
    *(.rodata .rodata.* .gnu.linkonce.r.*)

    . = ALIGN(4);
    KEEP(*(.init))

    . = ALIGN(4);
    __preinit_array_start = .;
    KEEP (*(.preinit_array))
    __preinit_array_end = .;

    . = ALIGN(4);
    __init_array_start = .;
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    __init_array_end = .;

    . = ALIGN(4);
    KEEP(*(.fini))

    . = ALIGN(4);
    __fini_array_start = .;
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    __fini_array_end = .;

    . = ALIGN(0x4);
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend.o(.ctors))

    . = ALIGN(0x4);
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*crtend.o(.dtors))

    /* Add debug information
    . = ALIGN(4);
    __my_debug_regions = .;
    LONG (__cs3_heap_start)
    LONG (__cs3_heap_end)
    LONG (__cs3_stack) */

    . = ALIGN(4);
    __cs3_regions = .;
    LONG (0)
    LONG (__cs3_region_init_ram)
    LONG (__cs3_region_start_ram)
    LONG (__cs3_region_init_size_ram)
    LONG (__cs3_region_zero_size_ram)
    __cs3_regions_end = .;

    . = ALIGN (8);
    *(.rom)
    *(.rom.b)
    _etext = .;
  } >rom

  .data : ALIGN (8)
  {
    __cs3_region_start_ram = .;
    _data = .;
    *(.cs3.region-head.ram)
    KEEP(*(.jcr))
    *(.got.plt) *(.got)
    *(.shdata)
    *(.data .data.* .gnu.linkonce.d.*)
    . = ALIGN (8);
    *(.ram)
    _edata = .;
  } >ram AT>rom
  .bss :
  {
    _bss = .;
    *(.shbss)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN (8);
    *(.ram.b)
    _ebss = .;
    _end = .;
    __end = .;
  } >ram AT>rom
  __cs3_region_init_ram = LOADADDR (.data);
  __cs3_region_init_size_ram = _edata - ADDR (.data);
  __cs3_region_zero_size_ram = _end - _edata;
  __cs3_region_size_ram = LENGTH(ram);


  .stab 0 (NOLOAD) : { *(.stab) }
  .stabstr 0 (NOLOAD) : { *(.stabstr) }
  /* DWARF debug sections.
   * Symbols in the DWARF debugging sections are relative to the beginning
   * of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* DWARF 2.1 */
  .debug_ranges   0 : { *(.debug_ranges) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }

  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) }
}

