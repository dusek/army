all: syscalls.o string.o

syscalls.o: syscalls.s
	arm-elf-as -march=armv5te -o syscalls.o syscalls.s

string.o: string.c string.h
	arm-elf-gcc -march=armv5te -ffreestanding -nostdlib -I. -O2 -c -o string.o string.c

clean:
	rm syscalls.o string.o
