.data
.string "Hello, World!\n"


.text
# addi x10, x0, 0x123

# addi a7, x0, 63       # syscall number for read
# addi a0, x0, 0        # file descriptor: stdin
# lui a1, 0x10000       # 
# addi a2, x0, 14       # read 10 bytes
# ecall         

addi a7, x0, 64
addi a0, x0, 1        # file descriptor: stdout
lui a1, 0x10000
addi a2, x0, 14       # write 14 bytes
ecall

# li a7, 1
# addi a0, x0, 23
# ecall

# addi x10, x0, 1
# addi x10, x10, 1
# addi x10, x10, 1
# addi x10, x10, 1
# addi x10, x10, 1