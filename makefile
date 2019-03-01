BOOTDIR=./src/boot
CC=gcc
LD=ld
CFLAGS=-W -Wall -pedantic -std=c11 -m32 -ffreestanding
LDFLAGS=--oformat binary -Ttext 0x1000 -m elf_i386
SOURCES=$(shell find . -name "*.c")
HEADERS=$(shell find . -name "*.h")
OBJECTS=$(SOURCES:%.c=%.o)
TARGET_DIR=./src/bin
TARGET=kernel.bin
KERNEL_SOURCE=./src/kernel

.PHONY: all
all: os

# Make os rely on setup first
# os: | setup bootsector.bin
	# qemu-system-x86_64 ./boot/bootsector.bin 
os: bootsector.bin kernel.bin
	cat $(TARGET_DIR)/bootsector.bin $(TARGET_DIR)/kernel.bin > $(TARGET_DIR)/os_image
	qemu-system-x86_64 $(TARGET_DIR)/os_image

%.bin: $(BOOTDIR)/%.asm
		mkdir $(TARGET_DIR)
		nasm $< -f bin -I $(BOOTDIR)/ -o $(TARGET_DIR)/$@

$(TARGET): $(KERNEL_SOURCE)/kernel_entry.o $(OBJECTS) # The order here is important, We always want to have kernel_entry first 
	$(LD) -o $(TARGET_DIR)/$@ $(LDFLAGS) $^ 	

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf32 -I boot/ -o $@

.PHONY: setup
setup:
	qemu-img create -f raw -o size=4G hard_disk.raw
	echo "w" | fdisk -C 208 -H 16 -S 64 hard_disk.raw

.PHONY: clean
clean:
	rm -rf $(TARGET_DIR) $(shell find ./src -name "*.o")

.PHONY: run
run:
	qemu-system-x86_64 $(TARGET_DIR)/os_image

.PHONY: rebuild
rebuild: | clean all