.data
bb: .dword 0x1234567890
aa: .word 0x12345
cc: .word -32

gg: .string "Hello, World!\n"

.text

#lw x0, aa
lw x10, aa
ld x11, bb

lw x12, gg
nop

add x0, x1, x2
sub x0, x1, x2
xor x0, x1, x2
or x0, x1, x2
and x0, x1, x2
sll x0, x1, x2
srl x0, x1, x2
sra x0, x1, x2
slt x0, x1, x2
sltu x0, x1, x2

nop 
add x0,x0,x0

llll:



main: 

fadd.s s0, s1, s2

addi x0, x1, 1
xori x0, x1, 1
ori x0, x1, 1
andi x0, x1, 1
slli x0, x1, 1
srli x0, x1, 1
srai x0, x1, 1
slti x0, x1, 1
sltiu x0, x1, 1


fdsff:
jal x1, lol
lol:

#
#lb x0, 0(x1)
#lh x0, 0(x1)
#lw x0, 0(x1)
#ld x0, 0(x1)
#lbu x0, 0(x1)
#lhu x0, 0(x1)
#lwu x0, 0(x1)
#
sb x0, 0(x1)
#sh x0, 0(x1)
#sw x0, 0(x1)
#sd x0, 0(x1)
#
beq x0, x0, lol
#beq x0, x1, 0
#bne x0, x1, 0
#blt x0, x1, 0
#bge x0, x1, 0
#bltu x0, x1, 0
#bgeu x0, x1, 0
#
#jal x0, 0
#jalr x0, 0(x1)
#
#lui x0, 0x12345
#auipc x0, 0x12345






