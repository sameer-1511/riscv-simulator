.section .data
.double 2.3, -0.2
dnum1: .double 1.5
dnum2: .double 3.7
str: .string "\n"
big_num: .dword 0x100000000






.text
ld a0, big_num       # Load the big number into a0
auipc x10, 0x10000
ld x10, 34(x10)
# la a0, str          # Load address of the string into a0

addi a7, x0, 63       # syscall number for read
addi a0, x0, 0        # file descriptor: stdin
lui a1, 0x10001       # addi a2, x0, 15       # read 10 bytes
addi a2, x0,15
ecall         

addi a7, x0, 64
addi a0, x0, 1        # file descriptor: stdout
lui a1, 0x10001
addi a2, x0, 15       # write 14 bytes
ecall

addi a7, x0, 64
addi a0, x0, 1        # file descriptor: stdout
la a1, str          # Load address of the string into a1
addi a2, x0, 1        # write 1 byte
ecall

li a7, 1
addi a0, x0, 23
ecall

jal ra, print_newline

test:
# lui x3, 0x10000        # Load upper immediate to set base address
la x3, dnum1          # Load address of dnum1 into x3
fld f0, 0(x3)          # Load the first double from memory into f
la x3, dnum2          # Load address of dnum2 into x3
fld f1, 0(x3)          # Load the second double from memory into f

# fld f0, 0(x3)          # Load the first double from memory into f0
# fld f1, 8(x3)          # Load the second double from memory into f1
fadd.d f2, f0, f1      # Add the two doubles and store the result in f2
fmv.x.d a0, f2          # Move the result from f2 to a0 (integer register)
li a7, 3    # custom syscall number for print double floating point
ecall
jal x0, exit


print_newline:
    addi a7, x0, 64
    addi a0, x0, 1        # file descriptor: stdout
    la a1, str 
    addi a2, x0, 1        # write 1 byte
    ecall
    jal x0, test

exit:
    nop
