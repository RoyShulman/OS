workspaceDir:=$(CURDIR)
TARGET := bootsector.bin
OBJECT := $(TARGET:%.bin=%.o)

build: $(TARGET)

$(TARGET): $(OBJECTS_DIR)/$(OBJECT)
	$(LD) $(LDFLAGS) $< -o $(TARGET_DIR)/$@

$(OBJECTS_DIR)/%.o: %.asm
	nasm $< -f elf -I $(workspaceDir) -o $@
