workspaceDir=$(CURDIR)
BOOTDIR=$(workspaceDir)/src/boot
CC=gcc
LD=ld
CFLAGS=-nostdlib -Werror -g -Wextra -Wall -pedantic -std=c11 -m32 -ffreestanding -I$(workspaceDir)/src/
LDFLAGS=--oformat binary -Ttext 0x1000 -m elf_i386
SOURCES=$(shell find $(workspaceDir) -name "*.c")
ASM_SOURCES=$(shell find $(workspaceDir) -name "*.asm" -not -path "$(workspaceDir)/src/boot/*")
HEADERS=$(shell find $(workspaceDir) -name "*.h")
OBJECTS=$(SOURCES:%.c=%.o)
OBJECTS+=$(ASM_SOURCES:%.asm=%.o)
TARGET_DIR=$(workspaceDir)/src/bin
TARGET=kernel.bin
KERNEL_SOURCE=$(workspaceDir)/src/kernel
GDB=gdb

.PHONY: run
run: $(TARGET_DIR)/os_image
	qemu-system-x86_64 $<

.PHONY: all
all: $(TARGET_DIR)/os_image

.PHONY: debug
debug: $(TARGET_DIR)/os_image $(TARGET_DIR)/kernel.elf
	qemu-system-x86_64 -s $< &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file $(TARGET_DIR)/kernel.elf"

$(TARGET_DIR)/kernel.elf:  $(OBJECTS)
	$(LD) -o $@ -Ttext 0x1000 -m elf_i386 $^

$(TARGET_DIR)/os_image: $(TARGET_DIR)/bootsector.bin $(TARGET_DIR)/$(TARGET)
	cat $^ > $@

$(TARGET_DIR)/%.bin: $(BOOTDIR)/%.asm
		mkdir $(TARGET_DIR)
		nasm $< -f bin -I $(BOOTDIR)/ -o $@

$(TARGET_DIR)/$(TARGET): $(KERNEL_SOURCE)/kernel_entry.o $(OBJECTS) # The order here is important, We always want to have kernel_entry first 
	$(LD) -o $@ $(LDFLAGS) $^ 	

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


.PHONY: rebuild
rebuild: | clean all