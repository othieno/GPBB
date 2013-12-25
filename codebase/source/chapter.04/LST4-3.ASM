; Listing 4.3. LST4-3.ASM
; Measures the performance of reading 1000 words
; from memory with 1000 word-sized accesses.
;
	sub	si,si
	mov	cx,1000
	call	ZTimerOn
    rep	lodsw
	call	ZTimerOff
