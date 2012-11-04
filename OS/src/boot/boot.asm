.code16
    .set BaseOfStack, 0x7C00
    .set SYSSEG, 0x1000
	.set SYSOFFSET, 0
    .set SYSLEN, 4
.text
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $BaseOfStack, %sp
    
LoadSystem:
	mov $SYSSEG, %ax
    mov %ax, %es
	mov $SYSOFFSET, %bx # %es : %bx
	mov $2, %ah # ������
	mov $0, %dx # ��ͷ / ������
    mov $2, %cx # ���� / ����
    mov $SYSLEN, %al # ������
    int $0x13
    jnc Success
    mov $0x0000, %dx
    mov $0x0000, %ax
    int $0x13
    jmp LoadSystem

Success:
    jmp $SYSSEG, $SYSOFFSET
    .org   510
    .word  0xAA55
