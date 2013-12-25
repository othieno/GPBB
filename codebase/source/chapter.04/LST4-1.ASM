; Listing 4.1. LST4-1.ASM
; Measures the performance of a loop which uses a
; byte-sized memory variable as the loop counter.
;
	jmp	Skip
;
Counter	db	100
;
Skip:
	call	ZTimerOn
LoopTop:
	dec	[Counter]
	jnz	LoopTop
	call	ZTimerOff
