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

    vectors :
    {
        *(.vectors)
    } >flash

    .text :
    {
        *(.text)
        *(.rodata)
        *(.rodata*)
        _end_text = .;
    } >flash

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