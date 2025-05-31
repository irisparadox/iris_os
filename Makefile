CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = nasm

BUILD = build
ISO = $(BUILD)/iso
KERNEL = $(BUILD)/iris.bin

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Ikernel/sys
LDFLAGS = -m elf_i386 -n -Ttext 0x100000

SRC_C = kernel/main.c kernel/video.c kernel/io.c
SRC_ASM = kernel/boot.S

OBJ_C = $(patsubst kernel/%.c,$(BUILD)/%.o,$(SRC_C))
OBJ_ASM = $(BUILD)/boot.o

all: iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: kernel/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_ASM): $(SRC_ASM) | $(BUILD)
	$(AS) -f elf32 $< -o $@

$(KERNEL): $(OBJ_ASM) $(OBJ_C)
	$(LD) $(LDFLAGS) -o $@ $(OBJ_ASM) $(OBJ_C)

iso: $(KERNEL)
	mkdir -p $(ISO)/boot/grub
	cp $(KERNEL) $(ISO)/boot/iris.bin
	cp grub/grub.cfg $(ISO)/boot/grub
	grub-mkrescue -o $(BUILD)/iris.iso $(ISO)

run: iso
	qemu-system-x86_64 -cdrom $(BUILD)/iris.iso

clean:
	rm -rf $(BUILD)