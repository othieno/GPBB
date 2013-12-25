; Listing 4.11. LST4-11.ASM
; Times speed of memory access to Enhanced Graphics
; Adapter graphics mode display memory at A000:0000.
;
	mov	ax,0010h
	int	10h		;select hi-res EGA graphics
				; mode 10 hex (AH=0 selects
				; BIOS set mode function,
				; with AL=mode to select)
;
	mov	ax,0a000h
	mov	ds,ax
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
;
	mov	ax,0003h
	int	10h		;return to text mode
