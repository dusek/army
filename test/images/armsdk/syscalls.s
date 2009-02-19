.text
    .align 4
.globl open
.globl close
.globl read
.globl write
.globl exit

open:
    MOV R10, #0
    SWI #0
    BX LR

close:
    MOV R10, #1
    SWI #0
    BX LR

read:
    MOV R10, #2
    SWI #0
    BX LR

write:
    MOV R10, #3
    SWI #0
    BX LR

exit:
    MOV R10, #4
    SWI #0
    BX LR
