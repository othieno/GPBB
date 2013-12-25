; Listing 4.2. LST4-2.ASM
; Measures the performance of a loop which uses a
; word-sized memory variable as the loop counter.
;
	jmp	Skip
;
Counter	dw	100
;
Skip:
	call	ZTimerOn
LoopTop:
	dec	[Counter]
	jnz	LoopTop
	call	ZTimerOff
