army
====

ARM emulator written in C++

Building
========

You need CMake and cppunit to build army. On OS X with Homebrew, you can just use:

```brew install cmake cppunit```

Then run CMake in the root directory of the repository:

```cmake .```

Then you can build army just using:

`make`

Demo
====

After building army, you can test it using 2 ready demo examples:

## fibonacci
This demo takes an integer `n` as single command-line argument and outputs `n`-th Fibonacci number. You can run it using:

```
$ ./armyexec 0 test/images/fibonacci/fibonacci.elf 7
Loaded image, starting execution
13
Execution ended.
Executed 201 instructions.
Execution took 0.46 ms (0.44 "MHz")
```

I.e. 7th Fibonacci number is 13.

Source code for the fibonacci demo is at `test/images/fibonacci/fibonacci.c`.

## prec
This demo receives a single integer command-line argument `n` and outputs all permutations of the sequence (1, 2, ..., n). You can run it using:

```
$ ./armyexec 0 test/images/prec/prec2 3
Loaded image, starting execution
Permutace 1: 1 2 3 OK
Permutace 2: 1 3 2 OK
Permutace 3: 2 1 3 OK
Permutace 4: 2 3 1 OK
Permutace 5: 3 1 2 OK
Permutace 6: 3 2 1 OK
Execution ended.
Executed 4895 instructions.
Execution took 4.46 ms (1.10 "MHz")
```

Source code for the prec demo is at `test/images/prec/prec2.c`.

## Instructions logging
It is possible to run army in a mode where every instruction is logged as it is executed. That is what the first argument to `armyexec` is for:

 - `0`: no instructions are logged
 - `1`: all instructions are logged in instruction format consistent with ARM documentation
 - `2`: same as `1`, but for each logged instruction, it also prints the state of all registers (R0-R15 and CPSR)
 
So logging with level `1` looks like this:

```
$ ./armyexec 1 test/images/prec/prec2 3
Loaded image, starting execution
81ac: CMP R0, #1, ROR #0
81b0: STMDA R13! {R4, R5, R6, R7, R8, R9, R10, R14}
81b4: BEQ #312
81b8: CMP R0, #2, ROR #0
81bc: BEQ #20
81d8: LDR R0, [R1, +#4]
81dc: BL #468
83b8: LDRB R1, [R0, +#0]
83bc: CMP R1, #0, ROR #0
83c0: MOVEQ R0, R1, LSL #0
83c4: BXEQ R14
83c8: MOV R12, R0, LSL #0
83cc: MOV R0, #0, ROR #0
...
8360: MOV R7, #3, ROR #0
8364: SWI #0

8368: LDMIB R13! {R7}
836c: BX R14
82d0: CMP R7, R8, LSL #0
82d4: BLE #-200
82d8: BL #144
8370: STMDA R13! {R7}
8374: MOV R7, #4, ROR #0
8378: SWI #0
Execution ended.
Executed 4895 instructions.
Execution took 164.25 ms (0.03 "MHz")
```

And logging level `2` looks likes this:

```
$ ./armyexec 2 test/images/prec/prec2 3
Loaded image, starting execution
81ac: CMP R0, #1, ROR #0
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000002 0x37240000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x000081ac 
CPSR: User | C

81b0: STMDA R13! {R4, R5, R6, R7, R8, R9, R10, R14}
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000002 0x37240000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0xffffffe0 0x00000000 0x000081b0 
CPSR: User | C

81b4: BEQ #312
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000002 0x37240000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0xffffffe0 0x00000000 0x000081b4 
CPSR: User | C

81b8: CMP R0, #2, ROR #0
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000002 0x37240000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0xffffffe0 0x00000000 0x000081b8 
CPSR: User | Z | C

81bc: BEQ #20
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000002 0x37240000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0x00000000 0xffffffe0 0x00000000 0x000081d8 
CPSR: User | Z | C

...

82d8: BL #144
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000001 0x00008c24 0x00000001 0x00000000 0x00000006 0x90000008 0x00000002 0x00000007 0x00000006 0x90000008 0x00000003 0x00000000 0x00000001 0xffffffe0 0x000082dc 0x00008370 
CPSR: User | C

8370: STMDA R13! {R7}
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000001 0x00008c24 0x00000001 0x00000000 0x00000006 0x90000008 0x00000002 0x00000007 0x00000006 0x90000008 0x00000003 0x00000000 0x00000001 0xffffffdc 0x000082dc 0x00008370 
CPSR: User | C

8374: MOV R7, #4, ROR #0
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000001 0x00008c24 0x00000001 0x00000000 0x00000006 0x90000008 0x00000002 0x00000004 0x00000006 0x90000008 0x00000003 0x00000000 0x00000001 0xffffffdc 0x000082dc 0x00008374 
CPSR: User | C

8378: SWI #0
R0         R1         R2         R3         R4         R5         R6         R7         R8         R9         R10        R11        R12        R13        R14        R15        
0x00000001 0x00008c24 0x00000001 0x00000000 0x00000006 0x90000008 0x00000002 0x00000004 0x00000006 0x90000008 0x00000003 0x00000000 0x00000001 0xffffffdc 0x000082dc 0x00008378 
CPSR: User | C

Execution ended.
Executed 4895 instructions.
Execution took 2287.57 ms (0.00 "MHz")

```
