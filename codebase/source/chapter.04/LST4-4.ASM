; Listing 4.4. LST4-4.ASM
; Measures the performance of reading 1000 words
; from memory with 2000 byte-sized accesses.
;
	sub	si,si
	mov	cx,2000
	call	ZTimerOn
    rep	lodsb
	call	ZTimerOff
