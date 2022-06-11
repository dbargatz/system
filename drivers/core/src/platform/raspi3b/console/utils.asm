.globl get_el
get_el:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	ret

.globl get_tpidr_el2
get_tpidr_el2:
	mrs x0, TPIDR_EL2
	ret

.globl set_tpidr_el2
set_tpidr_el2:
	msr TPIDR_EL2, x0
	ret

.globl put32
put32:
	str w1,[x0]
	ret

.globl get32
get32:
	ldr w0,[x0]
	ret

.globl delay_cycles
delay_cycles:
	subs x0, x0, #1
	bne delay_cycles
	ret