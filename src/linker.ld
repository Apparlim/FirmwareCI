ENTRY(_start)

SECTIONS
{
    . = 0x80000;

    .text : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss ALIGN(16) : {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    }

    . = ALIGN(16);
    . += 0x10000; /* 64 KB Stack */
    _stack_top = .;
}
