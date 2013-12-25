; Listing 4.9. LST4-9.ASM
; Measures the performance of repeated MUL instructions,
; which allow the prefetch queue to be full at all times,
; to demonstrate a case in which DRAM refresh has no impact
; on code performance.
;
	sub	ax,ax
	call	ZTimerOn
	rept	1000
	mul	ax
	endm
	call	ZTimerOff
