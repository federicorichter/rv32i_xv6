.section .text
.global _entry
_entry:
    # Set up a stack for C.
    la sp, stack0
    li a0, 1024*4
    # Assume no hartid (multi-core) support for simplicity
    add sp, sp, a0
    # Jump to start() in start.c
    call start
spin:
    j spin
