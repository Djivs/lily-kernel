#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/lilyos.kernel isodir/boot/lilyos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Lily OS" {
	multiboot /boot/lilyos.kernel
}
EOF
grub-mkrescue -o lilyos.iso isodir
