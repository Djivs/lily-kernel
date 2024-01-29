GPPPARAMS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib
LDPARAMS = -ffreestanding -O2 -nostdlib  -lgcc

objects = boot.o kernel.o

kernel.o: kernel.cpp
	/usr/local/bin/i686-elf-tools-linux/bin/i686-elf-g++ -c $< -o $@ $(GPPPARAMS)

boot.o: boot.s
	/usr/local/bin/i686-elf-tools-linux/bin/i686-elf-as $< -o $@ 

lilyos.bin: linker.ld kernel.o boot.o
	/usr/local/bin/i686-elf-tools-linux/bin/i686-elf-gcc -T $< -o $@ $(objects) $(LDPARAMS)

	sh/verify-multiboot.sh

local_linux_install: lilyos.bin 
	sudo cp $< /boot/lilyos.bin

iso: lilyos.bin
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$<
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o lilyos.iso isodir