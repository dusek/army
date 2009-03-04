all: fibonacci.elf #fibonacci.pe

fibonacci.elf: fibonacci.elf.o ../armsdk/syscalls.elf.o ../armsdk/string.elf.o
	arm-elf-ld -nostdlib -e Fib -o fibonacci.elf fibonacci.elf.o ../armsdk/syscalls.elf.o ../armsdk/string.elf.o `arm-elf-gcc -print-libgcc-file-name`

fibonacci.elf.o: fibonacci.c ../armsdk/string.h ../armsdk/syscalls.h
	arm-elf-gcc -nostdlib -fno-builtin -I.. -O2 -march=armv5te -c -o fibonacci.elf.o fibonacci.c

#fibonacci.pe: fibonacci.pe.o ../armsdk/syscalls.pe.o ../armsdk/string.pe.o
#	arm-pe-ld -nostdlib -e Fib -o fibonacci.pe fibonacci.pe.o ../armsdk/syscalls.pe.o ../armsdk/string.pe.o `arm-pe-gcc -print-libgcc-file-name`
#
#fibonacci.pe.o: fibonacci.c ../armsdk/string.h ../armsdk/syscalls.h
#	arm-pe-gcc -nostdlib -fno-builtin -I.. -O2 -march=armv5te -c -o fibonacci.pe.o fibonacci.c
#
clean:
	-rm fibonacci.elf.o fibonacci.elf #fibonacci.pe.o fibonacci.pe
