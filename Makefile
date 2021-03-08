build:
	i686-elf-as -I lib boot.s -o target/boot.o
	i686-elf-g++ -I lib -c kernel.cpp -o target/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	i686-elf-g++ -I lib -T linker.ld -o target/SexOS.bin -ffreestanding -O2 -nostdlib target/boot.o target/kernel.o -lgcc 
	cp target/SexOS.bin target/isodir/boot/SexOS.bin 
	cp grub.cfg target/isodir/boot/grub/grub.cfg
	grub-mkrescue -o target/SexOS.iso target/isodir
