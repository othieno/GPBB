; Listing 22.7.  L22-7.ASM

ClearS	proc	near
	pop	dx		;get the return address
	pop	ax		;put fill char into AL
	pop	bx		;get the attribute
	mov	ah,bh		;put attribute into AH
	pop	cx		;get the buffer size
	pop	di		;get the offset of the buffer origin
	pop	es		;get the segment of the buffer origin
	mov	bx,es		;put the segment where we can test it
	or	bx,di		;null pointer?
	je	Bye		;yes, so we're done
	cld				;make STOSW count up
	rep	stosw		;do the string store
Bye:
	jmp	dx		;return to the calling code
ClearS	endp
