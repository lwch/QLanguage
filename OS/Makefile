AS  = as
GCC = gcc
LD  = ld
OBJCOPY = objcopy

OBJCOPY_FLAGS = -R.pdr -R.comment -R.note -S -O binary

all: final/os.bin

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

out/make_bin: tools/make_bin.cpp
	$(GCC) $< -o $@
	
final/os.bin: out/make_bin out/boot.bin out/head.bin
	@out/make_bin out/boot.bin out/head.bin final/os.bin

clean:
	@rm -fr out/*

run: final/os.bin
	@bochs
