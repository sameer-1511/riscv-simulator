li a7, 63       # syscall number for read
li a0, 0        # file descriptor: stdin
lui a1, 0x0000 # 
li a2, 10       # read 10 bytes
ecall           # perform syscall

# li a7, 1
# addi a0, x0, 23
# ecall

# addi x10, x0, 1
# addi x10, x10, 1
# addi x10, x10, 1
# addi x10, x10, 1
# addi x10, x10, 1