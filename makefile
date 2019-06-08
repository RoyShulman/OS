workspaceDir=$(CURDIR)
export CC=gcc
export LD=ld
export CFLAGS=-nostdlib -Werror -g -Wextra -Wall -pedantic -std=c11 -m32 -ffreestanding -I$(workspaceDir)/src/ -fno-PIC
export LDFLAGS=--oformat binary -m elf_i386  -T $(workspaceDir)/linker.ld
export TARGET_DIR=$(workspaceDir)/bin
export OBJECTS_DIR=$(workspaceDir)/objects
SOURCE_DIRS := $(wildcard $(workspaceDir)/src/*/.)
TARGET=$(TARGET_DIR)/kernel.bin

.PHONY: clean debug build

all: $(TARGET)

clean:
	rm -r $(TARGET_DIR)
	rm -r $(OBJECTS_DIR)

debug: $(TARGET_DIR)/kernel.elf


$(TARGET):
	mkdir -p $(TARGET_DIR)
	mkdir -p $(OBJECTS_DIR)
	for dir in $(SOURCE_DIRS); do \
		$(MAKE) -C $$dir; \
	done

$(TARGET_DIR)/kernel.elf: $(KERNEL_SOURCE)/kernel_entry.o $(OBJECTS)
	$(LD) -o $@ -Ttext 0x1000 -m elf_i386 $^

$(TARGET_DIR)/os_image: $(TARGET_DIR)/bootsector.bin $(TARGET)
	cat $^ > $@