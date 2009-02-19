all: fibonacci

fibonacci: fibonacci.o
	arm-elf-ld -nostdlib -e Fib -o fibonacci fibonacci.o ../armsdk/syscalls.o ../armsdk/string.o `arm-elf-gcc -print-libgcc-file-name`

fibonacci.o: fibonacci.c
	arm-elf-gcc -nostdlib -fno-builtin -I../armsdk -O1 -march=armv5te -c -o fibonacci.o fibonacci.c

clean:
	-rm fibonacci.o fibonacci
