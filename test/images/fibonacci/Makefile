all: fibonacci

fibonacci: fibonacci.o ../armsdk/syscalls.o ../armsdk/string.o
	arm-elf-ld -nostdlib -e Fib -o fibonacci fibonacci.o ../armsdk/syscalls.o ../armsdk/string.o `arm-elf-gcc -print-libgcc-file-name`

fibonacci.o: fibonacci.c ../armsdk/string.h ../armsdk/syscalls.h
	arm-elf-gcc -nostdlib -fno-builtin -I.. -O2 -march=armv5te -c -o fibonacci.o fibonacci.c

clean:
	-rm fibonacci.o fibonacci
