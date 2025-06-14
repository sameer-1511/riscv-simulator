.data
.double -2.3, 3.2
.double 2.3, -3.2


.text
# addi x10, x0, 0x123

# addi a7, x0, 63       # syscall number for read
# addi a0, x0, 0        # file descriptor: stdin
# lui a1, 0x10001       # 
# addi a2, x0, 15       # read 10 bytes
# ecall         

# addi a7, x0, 64
# addi a0, x0, 1        # file descriptor: stdout
# lui a1, 0x10001
# # addi a1, a1, 0x1
# addi a2, x0, 15       # write 14 bytes
# ecall

# addi a7, x0, 64
# addi a0, x0, 1        # file descriptor: stdout
# lui a1, 0x10000
# addi a2, x0, 1        # write 1 byte
# ecall

# li a7, 1
# addi a0, x0, 23
# ecall

lui x3, 0x10000        # Load upper immediate to set base address
fld f0, 16(x3)          # Load the first double from memory into f0
fld f1, 24(x3)          # Load the second double from memory into f1
fadd.d f2, f0, f1      # Add the two doubles and store the result in f2
fmv.x.d a0, f2          # Move the result from f2 to a0 (integer register)
li a7, 3    # custom syscall number for print double floating point
ecall



