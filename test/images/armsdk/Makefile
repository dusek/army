all: syscalls.elf.o string.elf.o syscalls.pe.o string.pe.o

syscalls.elf.o: syscalls.s
	arm-elf-as -march=armv5te -o syscalls.elf.o syscalls.s

string.elf.o: string.c string.h
	arm-elf-gcc -march=armv5te -nostdlib -fno-builtin -I. -O2 -c -o string.elf.o string.c

syscalls.pe.o: syscalls.s
	arm-pe-as -march=armv5te -o syscalls.pe.o syscalls.s

string.pe.o: string.c string.h
	arm-pe-gcc -march=armv5te -nostdlib -fno-builtin -I. -O2 -c -o string.pe.o string.c

clean:
	rm syscalls.elf.o string.elf.o syscalls.pe.o string.pe.o
