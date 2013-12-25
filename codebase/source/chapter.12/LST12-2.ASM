; Measures the effect of loading a byte register 1 cycle before
; using a register to address memory.
	mov	bp,2	;run the test code twice to make sure				; it's cached	sub	bx,bxCacheFillLoop:	call	ZTimerOn ;start timing	rept	1000	nop	mov	dl,cl	mov	ax,[bx]	endm	call	ZTimerOff ;stop timing	dec	bp	jz	Done	jmp	CacheFillLoopDone:
