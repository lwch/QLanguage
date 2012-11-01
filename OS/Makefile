AS  = as
GCC = gcc
LD  = ld
OBJCOPY = objcopy

OBJCOPY_FLAGS = -R.pdr -R.comment -R.note -S -O binary

all: out/boot.bin out/head.bin

out/boot.asm.o: src/boot/boot.asm
	$(AS) $< -o $@

out/head.asm.o: src/boot/head.asm
	$(AS) $< -o $@

out/boot.elf: out/boot.asm.o
	$(LD) $< -o $@ -Tboot.ld

out/boot.bin: out/boot.elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@

out/head.elf: out/head.asm.o
	$(LD) $< -o $@ -Thead.ld
	
out/head.bin: out/head.elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@

clean:
	@rm -fr out/*
