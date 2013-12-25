; Listing 4.12. LST4-12.ASM
; Times speed of memory access to normal system
; memory.
;
	mov	ax,ds
	mov	es,ax		;move to & from same segment
	sub	si,si		;move to & from same offset
	mov	di,si
	mov	cx,800h		;move 2K words
	cld
	call	ZTimerOn
	rep	movsw		;simply read each of the first
				; 2K words of the destination segment,
				; writing each byte immediately back
				; to the same address. No memory
				; locations are actually altered; this
				; is just to measure memory access
				; times
	call	ZTimerOff
