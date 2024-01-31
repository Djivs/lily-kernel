CPP_COMPILER = /usr/local/bin/i686-elf-tools-linux/bin/i686-elf-g++
AS_COMPILER = /usr/local/bin/i686-elf-tools-linux/bin/i686-elf-as
C_COMPILER = /usr/local/bin/i686-elf-tools-linux/bin/i686-elf-gcc

GPPPARAMS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib -r -std=c++11
LDPARAMS = -ffreestanding -O2 -nostdlib  -lgcc

CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$( $(CPP_COMPILER) $(GPP_PARAMS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$( $(CPP_COMPILER) $(GPP_PARAMS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

objects = boot.o kernel.o

OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(objects) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:=$(CRTI_OBJ) $(objects) $(CRTN_OBJ)

kernel.o: kernel.cpp VGAPrinter.cpp
	$(CPP_COMPILER) kernel.cpp VGAPrinter.cpp -o $@ $(GPPPARAMS)

boot.o: boot.s
	$(AS_COMPILER) $< -o $@ 

# if crti.o and crtn.o build instructions are not provided,
# system compiler is generally used
# so it compiles x86 crti.s and crtn.s as x64 assembly code
crti.o: crti.s
	$(AS_COMPILER) $< -o $@

crtn.o: crtn.s
	$(AS_COMPILER) $< -o $@


lilyos.bin: linker.ld $(OBJ_LINK_LIST)
	$(C_COMPILER) -T $< -o $@ $(OBJ_LINK_LIST) $(LDPARAMS)

	sh/verify-multiboot.sh

local_linux_install: lilyos.bin 
	sudo cp $< /boot/lilyos.bin

iso: lilyos.bin
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$<
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o lilyos.iso isodir

clean:
	rm lilyos.ios lilyos.bin $(INTERNAL_OBJS)