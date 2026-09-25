.section ".text.boot"
.global _start

_start:
    /* Set stack pointer directly at active boot execution level */
    adrp    x0, _stack_top
    add     x0, x0, :lo12:_stack_top
    mov     sp, x0

    /* Zero out .bss section cleanly using aligned 64-bit store loop */
    adrp    x0, _bss_start
    add     x0, x0, :lo12:_bss_start
    adrp    x1, _bss_end
    add     x1, x1, :lo12:_bss_end

bss_loop:
    cmp     x0, x1
    b.ge    bss_done
    str     xzr, [x0], #8
    b       bss_loop

bss_done:
    /* Branch to main firmware entry point */
    bl      main

halt:
    wfe
    b       halt
