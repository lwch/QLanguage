AS  = as
GCC = gcc
LD  = ld
OBJCOPY = objcopy

OBJCOPY_FLAGS = -R.pdr -R.comment -R.note -S -O binary

all: os.img

main.asm.o: main.asm
	$(AS) $< -o $@

head.asm.o: head.asm
	$(AS) $< -o $@

os.img: main.asm.o head.asm.o
	$(LD) main.asm.o -o main.elf -Tmain.ld
	$(OBJCOPY) $(OBJCOPY_FLAGS) main.elf main.bin
	$(LD) head.asm.o -o head.elf -Thead.ld
	$(OBJCOPY) $(OBJCOPY_FLAGS) head.elf head.bin

clean:
	@rm -f main.s.o os.elf os.img
