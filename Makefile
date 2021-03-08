build:
	i686-elf-as boot.s -o target/boot.o
	i686-elf-g++ -c kernel.cpp -o target/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	i686-elf-g++ -T linker.ld -o target/SexOS.bin -ffreestanding -O2 -nostdlib target/boot.o target/kernel.o -lgcc 
	cp target/SexOS.bin target/isodir/boot/SexOS.bin 
	cp grub.cfg target/isodir/boot/grub/grub.cfg
	grub-mkrescue -o target/myos.iso target/isodir
