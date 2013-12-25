	mov	cx,[initialskip]
	bswap	ecx	;put skip value in upper half of ECX
	mov	cx,100	;put loop count in CX
looptop:
          :
	bswap	ecx	;make skip value word accessible in CX
	add	bx,cx	;skip BX ahead
	inc	cx	;set next skip value
	bswap	ecx	;put loop count in CX
	dec	cx	;count down loop
	jnz	looptop


