BOOTDIR = boot/
SOURCES := $(wildcard *.asm)
OBJECTS := $(SOURCES:%.asm=%.o)
# OBJECTS := $(OBJECTS_DIR)/$(OBJECTS)

build: $(OBJECTS)

.PHONY: clean
clean:
	rm $(OBJECTS)

%.o: %.asm
	nasm $< -f elf32 -I $(OBJECTS_DIR) -o $(OBJECTS_DIR)$@