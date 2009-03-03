.text
    .align 4
.globl open
.globl close
.globl read
.globl write
.globl exit

open:
    PUSH {R7}
    MOV R7, #0
    SWI #0
    POP {R7}
    BX LR

close:
    PUSH {R7}
    MOV R7, #1
    SWI #0
    POP {R7}
    BX LR

read:
    PUSH {R7}
    MOV R7, #2
    SWI #0
    POP {R7}
    BX LR

write:
    PUSH {R7}
    MOV R7, #3
    SWI #0
    POP {R7}
    BX LR

exit:
    PUSH {R7}
    MOV R7, #4
    SWI #0
    POP {R7}
    BX LR
