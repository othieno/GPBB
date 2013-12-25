; Listing 4.5. LST4-5.ASM
; Measures the performance of 1000 SHR instructions
; in a row. Since SHR executes in 2 cycles but is
; 2 bytes long, the prefetch queue is always empty,
; and prefetching time determines the overall
; performance of the code.
;
	call	ZTimerOn
	rept	1000
	shr	ax,1
	endm
	call	ZTimerOff
