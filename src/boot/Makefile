workspaceDir:=$(CURDIR)
BOOTDIR := boot/
TARGET := bootsector.elf
OBJECT := $(TARGET:%.elf=%.o)

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJECTS_DIR)/$(OBJECT)
	$(LD) $(LDFLAGS) $< -o $(TARGET_DIR)/$@

$(OBJECTS_DIR)/%.o: %.asm
	nasm $< -f elf -I $(workspaceDir) -o $@
