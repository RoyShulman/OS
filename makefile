BOOTDIR=./boot


.PHONY: all
all: os

# Make os rely on setup first
os: | setup bootsector.bin
	qemu-system-x86_64 ./boot/bootsector.bin

%.bin: $(BOOTDIR)/%.asm
		nasm $< -f bin -I $(BOOTDIR)/ -o $(BOOTDIR)/$@

.PHONY: setup
setup:
	qemu-img create -f raw -o size=4G hard_disk.raw
	echo "w" | fdisk -C 208 -H 16 -S 64 hard_disk.raw
