#!/bin/bash

sudo apt install bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo

mkdir ~/src

wget https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz
sudo tar -xzf binutils-2.37.tar.gz -C ~/src
wget https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz
sudo tar -xzf gcc-11.2.0.tar.gz -C ~/src

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

cd $HOME/src
 
mkdir build-binutils
cd build-binutils
../binutils-2.37/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd $HOME/src

which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
