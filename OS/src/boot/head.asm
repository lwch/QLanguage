.include "pm.h"

.code16
.text
	jmp LABEL_BEGIN

LABEL_GDT:         Descriptor 0,       0,      0
LABEL_DESC_CODE32: Descriptor 0,       0xFFFF, (DA_C + DA_32)
LABEL_DESC_VIDEO:  Descriptor 0xB8000, 0xFFFF, DA_DRW

.set GdtLen, (. - LABEL_GDT)

GdtPtr: .short (GdtLen - 1)
        .int   0

.set SelectorCode32, (LABEL_DESC_CODE32 - LABEL_GDT)
.set SelectorVideo,  (LABEL_DESC_VIDEO  - LABEL_GDT)

LABEL_BEGIN:
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	
	mov $0, %sp
	
	xor  %eax, %eax
	mov  %cs, %ax
	shl  $4,  %eax
	addl $(LABEL_CODE32), %eax
	movw %ax, (LABEL_DESC_CODE32 + 2)
	shr  $16, %eax
	movb %al, (LABEL_DESC_CODE32 + 4)
	movb %ah, (LABEL_DESC_CODE32 + 7)
	
	xor  %eax, %eax
	mov  %ds, %ax
	shl  $4, %eax
	add  $(LABEL_GDT), %eax
	movl %eax, (GdtPtr + 2)
	
	lgdtw GdtPtr
	
	cli
	
	inb  $0x92, %al
	orb  $0b00000010, %al
	outb %al, $0x92
	
	movl %cr0, %eax
	orl  $1, %eax
	movl %eax, %cr0
	
	ljmpl $SelectorCode32, $0

LABEL_CODE32:
.code32
	mov $10000, %esp