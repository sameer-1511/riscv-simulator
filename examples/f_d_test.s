.data
.float -2.3
.float 3.2
.double -4.7
.word 0

.text

lui x3, 0x10000

fld f7, 8(x3)

flw f10, 0(x3)
flw f11, 4(x3)

fcvt.w.s x17, f10
fcvt.wu.s x17, f10
fcvt.s.w f17, x10


fadd.s f12, f10, f11

lw x10, 0(x0)

feq.s x10, f1, f2 
fadd.s f10, f11, f12, rne
fmv.x.w x10, f10

fmadd.s f0, f3, f6, f27

flw f10, 0(x10)
fsw f10, 0(x10)
