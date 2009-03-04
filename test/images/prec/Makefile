all: prec prec2

prec: prec.o ../armsdk/syscalls.elf.o ../armsdk/string.elf.o
	arm-elf-ld -e main -o prec prec.o ../armsdk/syscalls.elf.o ../armsdk/string.elf.o `arm-elf-gcc -print-libgcc-file-name`

prec.o: prec.c ../armsdk/syscalls.h ../armsdk/string.h
	arm-elf-gcc -march=armv5te -I.. -O2 -nostdlib -fno-builtin -c -o prec.o prec.c

clean:
	-rm prec.o prec2.o

prec2: prec2.o ../armsdk/syscalls.elf.o ../armsdk/string.elf.o
	arm-elf-ld -e main -o prec2 prec2.o ../armsdk/syscalls.elf.o ../armsdk/string.elf.o `arm-elf-gcc -print-libgcc-file-name`

prec2.o: prec2.c ../armsdk/syscalls.h ../armsdk/string.h
	arm-elf-gcc -march=armv5te -I.. -O2 -nostdlib -fno-builtin -c -o prec2.o prec2.c
