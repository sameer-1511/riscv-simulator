.text
main: li x3, 0x1000
    li x4, 42
    sd x4, 0(x3)
    li x5, 0x1200
    li x4, 24
    sd x4, 0(x5)

    li x6, 0x1400

    ldbm x0, x3, x5
    bigmul x0, 0(x6)

    ld x7,0(x6)

    li a7, 93
    li a0,0
    ecall