; Listing 4.7. LST4-7.ASM
; Measures the performance of repeated MOV AL,0 instructions,
; which take 4 cycles each according to Intel's official
; specifications.
;
	sub	ax,ax
	call	ZTimerOn
	rept	1000
	mov	al,0
	endm
	call	ZTimerOff
