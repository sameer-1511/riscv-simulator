.text
main: li x3, 0x10000
    li x4, 1024
    sd x4, 0(x3)
    addi x4, x4, -110
    sd x4, 8(x3)

addi x5, x3, 512
li x4, 512
sd x4, 0(x5)
addi x4, x4, -114
sd x4, 8(x5)

addi x6, x5, 120

ldbm x0, x3, x5
bigmul x0, 0(x6)

ecall