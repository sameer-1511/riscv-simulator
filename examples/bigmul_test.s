# ============================================================
# Test case for BIGMUL on >64-bit operands (compatible parser)
# ============================================================

.data
# ------------------------------------------------------------
# Define two 128-bit numbers (16 bytes each) as raw bytes
# ------------------------------------------------------------

A:  byte 1,0,0,0,0
B:  byte 4
RES: zero 64       # Reserve 64 bytes for 256-bit result

.text
# ------------------------------------------------------------
# main program
# ------------------------------------------------------------
main:
    la x3, A          # load address of first operand
    la x4, B          # load address of second operand
    la x5, RES        # load address of result buffer

    ldbm x0, x3, x4   # load both operands into BIGMUL unit
    bigmul x0, 0(x5)  # multiply A * B, store result at RES

    # Load first two words of result to verify output
    ld x6, 0(x5)
    ld x7, 8(x5)

    # Exit
    li a7, 93
    li a0, 0
    ecall
