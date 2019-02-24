BOOTDIR=./boot


.PHONY: all
all: os

os: bootsector.bin

%.bin: $(BOOTDIR)/%.asm
		nasm $< -f bin -o $(BOOTDIR)/$@

