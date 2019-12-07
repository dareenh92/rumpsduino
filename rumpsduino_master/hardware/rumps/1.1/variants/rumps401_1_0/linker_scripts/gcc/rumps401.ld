ENTRY(Reset_Handler)

/* Memory configuration for RUMPS401 */

MEMORY
{
    flash (rx)  :   ORIGIN = 0x00000000, LENGTH = 32k
    sram  (rwx) :   ORIGIN = 0x20000000, LENGTH = 8k
}

_end_stack = 0x20002000;

SECTIONS {
    . = ORIGIN(flash);

    /*vectors :
    {
        KEEP(*(.vectors .vectors.*))
    } >flash*/

    .text :
    {
		KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.vfp11_veneer)
        *(.v4_bx)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        _end_text = .;
    } >flash
	
	/* for exception handling/unwind - some Newlib functions (in
    common with C++ and STDC++) use this. */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)

    } > flash

     __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > flash
        __exidx_end = .;

    . = ALIGN(4);
     _etext = .;
    /* This is used by the startup in order to initialize the .data section */
    _sidata = _etext;

    .data :
    {
        _start_data = .;
        *(.data)
        _end_data = .;
    } >sram AT >flash

    . = ALIGN(4);

    .bss :
    {
        _start_bss = .;
        *(.bss)
        _end_bss = .;
    } >sram

    . = ALIGN(4);

    _start_stack = .;
}

_end = .;
PROVIDE(end = .);