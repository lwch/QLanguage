AS=as
CC=gcc
LD=ld
OBJCOPY=objcopy

all: os.img

main.asm.o: main.asm
	$(AS) $< -o $@

head.asm.o: head.asm
	$(AS) $< -o $@

os.img: main.asm.o head.asm.o
	$(LD) main.asm.o -o main.elf -Tmain.ld
	$(OBJCOPY) -R .pdr -R .comment -R.note -S -O binary main.elf main.bin
	$(LD) head.asm.o -o head.elf -Thead.ld
	$(OBJCOPY) -R .pdr -R .comment -R.note -S -O binary head.elf head.binos.img

clean:
	@rm -f main.s.o os.elf os.img
