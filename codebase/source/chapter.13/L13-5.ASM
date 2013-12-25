	mov	cx,[initialskip]
	shl	ecx,16	;put skip value in upper half of ECX
	mov	cx,100	;put loop count in CX
looptop:
          :
	ror	ecx,16	;make skip value word accessible in CX
	add	bx,cx	;skip BX ahead
	inc	cx	;set next skip value
	ror	ecx,16	;put loop count in CX
	dec	cx	;count down loop
	jnz	looptop

