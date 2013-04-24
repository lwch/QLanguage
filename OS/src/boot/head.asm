.include "pm.h"

.code16
.text
	jmp LABEL_BEGIN

LABEL_GDT:         Descriptor 0,       0,          0
LABEL_DESC_DATA:   Descriptor 0,       0xFFFFFFFF, DA_DRW
LABEL_DESC_CODE32: Descriptor 0,       0xFFFF,     (DA_CR + DA_32)
LABEL_DESC_KERNEL: Descriptor 0x12000, 0xFFFF,     (DA_CR + DA_32)

.set GdtLen, (. - LABEL_GDT)

GdtPtr: .short (GdtLen - 1)
        .int   0

.set SelectorData,   (LABEL_DESC_DATA   - LABEL_GDT)
.set SelectorCode32, (LABEL_DESC_CODE32 - LABEL_GDT)
.set SelectorKernel, (LABEL_DESC_KERNEL - LABEL_GDT)

LABEL_BEGIN:
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	
	mov $0, %sp
	
	mov $0x13, %al
	mov $0, %ah
	int $0x10
	
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
	mov $SelectorData, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	mov $0x20000, %esp
	
	mov $(init + 0x10000), %esi
	mov $0x12000, %edi
	mov $0xFFFF, %ecx
	call memcpy
	
	ljmpl $SelectorKernel, $0x0
memcpy:
	mov (%esi), %eax
	inc %esi
	mov %eax, (%edi)
	inc %edi
	dec %ecx
	jnz memcpy
	ret
init:
	mov $SelectorData, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	mov $0x20000, %esp
