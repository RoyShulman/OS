BOOTDIR=./src/boot
CC=gcc
LD=ld
CFLAGS=-W -Wall -pedantic -std=c11 -m32 -ffreestanding
LDFLAGS=--oformat binary -Ttext 0x1000 -m elf_i386
SOURCES=$(shell find . -name "*.c")
OBJECTS=$(SOURCES:%.c=%.o)
TARGET_DIR=./src/bin
TARGET=kernel.bin

.PHONY: all
all: os

# Make os rely on setup first
# os: | setup bootsector.bin
	# qemu-system-x86_64 ./boot/bootsector.bin 
os: bootsector.bin kernel.bin
	qemu-system-x86_64 $(BOOTDIR)/bootsector.bin

%.bin: $(BOOTDIR)/%.asm
		nasm $< -f bin -I $(BOOTDIR)/ -o $(BOOTDIR)/$@

$(TARGET): $(OBJECTS)
	mkdir $(TARGET_DIR)
	$(LD) $(LDFLAGS) $^ -o $(TARGET_DIR)/$@

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: setup
setup:
	qemu-img create -f raw -o size=4G hard_disk.raw
	echo "w" | fdisk -C 208 -H 16 -S 64 hard_disk.raw

.PHONY: clean
clean:
	rm -rf $(TARGET_DIR) $(OBJECTS)