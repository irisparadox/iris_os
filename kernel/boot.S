section .multiboot
    align 4
    dd 0x1BADB002            ; magic
    dd 0x00                  ; flags
    dd -(0x1BADB002)         ; checksum = -(magic + flags)

section .text
    extern kernel_main
    global _start
_start:
    cli
    mov esp, 0x90000
    call kernel_main
hang:
    hlt
    jmp hang
