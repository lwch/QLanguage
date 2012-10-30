.code16
    .set BaseOfStack, 0x7C00
    .set SYSSEG, 0x1000
    .set SYSLEN, 50
.text
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $BaseOfStack, %sp
    
LoadSystem:
    mov $0x0000, %dx
    mov $0x0001, %cx
    mov $SYSSEG, %ax
    mov %ax, %es
    mov $0x0000, %bx
    mov $0x200 + SYSLEN, %ax
    int $0x13
    jnc Success
    mov $0x0000, %dx
    mov $0x0000, %ax
    int $0x13
    jmp LoadSystem

Success:
    jmp $0x1000
    .org   510
    .word  0xAA55
