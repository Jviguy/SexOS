#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/SexOS.kernel isodir/boot/SexOS.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "SexOS" {
	multiboot /boot/SexOS.kernel
}
EOF
grub-mkrescue -o SexOS.iso isodir
