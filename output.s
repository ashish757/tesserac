.global _main
.align 2
_main:
    mov w0, #5
    str w0, [sp, #-16]!
    mov w0, #10
    mov w1, w0
    ldr w0, [sp], #16
    add w0, w0, w1
    str w0, [sp, #-16]!
    mov w0, #3
    mov w1, w0
    ldr w0, [sp], #16
    sub w0, w0, w1
    ret
