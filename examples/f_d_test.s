feq.s x10, f1, f2 
fadd.s f10, f11, f12, rne
fmv.x.w x10, f10

fmadd.s f0, f3, f6, f27

flw f10, 0(x10)
fsw f10, -1(x10)
