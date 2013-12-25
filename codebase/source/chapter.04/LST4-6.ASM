; Listing 4.6. LST4-6.ASM
; Measures the performance of 1000 MUL/SHR instruction
; pairs in a row. The lengthy execution time of MUL
; should keep the prefetch queue from ever emptying.
;
	mov	cx,1000
	sub	ax,ax
	call	ZTimerOn
	rept	1000
	mul	ax
	shr	ax,1
	endm
	call	ZTimerOff
