#include <armsdk/syscalls.h>
#include <armsdk/string.h>

void usage(int fd)
{
    writes(fd, "Usage: \"fibonacci N\" (computes Fib(N))\n");
}

void Fib(int argc, char **argv) {
    int x,y,z;

    if (argc < 2) {
        usage(stderr);
        exit();
    }

    int N = strtou32(argv[1]);
    
    x = 1;           //MOV R1, #1
    y = 0;           //MOV R2, #0
    if (N < 2)       //CMP R0, #2
        x = N;       //MOVMI R1, R0; BMI #5
    else {
        N -= 2;
        do {
            z = y;       //MOV R3, R2
            y = x;       //MOV R2, R1
            x = y + z;   //ADD R1, R2, R3
            N--;         //SUBS R0, R0, 1
        } while (N >= 0);//BPL #-6
    }

    writei(stdout, x);
    writes(stdout, "\n");
    exit();
}

