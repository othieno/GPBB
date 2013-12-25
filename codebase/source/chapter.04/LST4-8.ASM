; Listing 4.8. LST4-8.ASM
; Measures the performance of repeated SUB AL,AL instructions,
; which take 3 cycles each according to Intel's official
; specifications.
;
	sub	ax,ax
	call	ZTimerOn
	rept	1000
	sub	al,al
	endm
	call	ZTimerOff
