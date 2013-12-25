; Listing 4.10. LST4-10.ASM
; Measures the performance of repeated SHR instructions,
; which empty the prefetch queue, to demonstrate the
; worst-case impact of DRAM refresh on code performance.
;
	call	ZTimerOn
	rept	1000
	shr	ax,1
	endm
	call	ZTimerOff
