.code16
.text
	call DispStr
	jmp .

DispStr:
    mov $BootMessage, %ax
    mov %ax, %bp
    mov $16, %cx
    mov $0x1301, %ax
    mov $0x000C, %bx
    mov $0, %dl
    int $0x10
    ret

BootMessage:
    .ascii "Hello, OS world!"
