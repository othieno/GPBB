; Divides an arbitrarily long unsigned dividend by a 16-bit unsigned
; divisor. C near-callable as:
;	unsigned int Div(unsigned int * Dividend,
;		int DividendLength, unsigned int Divisor,
;		unsigned int * Quotient);
;         
; Returns the remainder of the division.
;
; Tested with TASM.

parms	struc
	dw	2 dup (?) ;pushed BP & return address
Dividend dw	?	;pointer to value to divide, stored in Intel
			; order, with lsb at lowest address, msb at
			; highest. Must be composed of an integral
			; number of words
DividendLength dw ?	;# of bytes in Dividend. Must be a multiple
			; of 2
Divisor	dw	?	;value by which to divide. Must not be zero,
			; or a Divide By Zero interrupt will occur
Quotient dw	?	;pointer to buffer in which to store the
			; result of the division, in Intel order.
			; The quotient returned is of the same
			; length as the dividend
parms	ends

	.model	small
	.code
	public	_Div
_Div	proc	near
        push    bp      ;preserve caller's stack frame
        mov     bp,sp   ;point to our stack frame
        push    si      ;preserve caller's register variables
        push    di

	std		;we're working from msb to lsb
	mov	ax,ds
	mov	es,ax	;for STOS
	mov	cx,[bp+DividendLength]
	sub	cx,2
	mov	si,[bp+Dividend]
	add	si,cx	;point to the last word of the dividend
			; (the most significant word)
	mov	di,[bp+Quotient]
	add	di,cx	;point to the last word of the quotient
			; buffer (the most significant word)
	mov	bx,[bp+Divisor]
	shr	cx,1
	inc	cx	;# of words to process
	sub	dx,dx	;convert initial divisor word to a 32-bit
			; value for DIV
DivLoop:
	lodsw		;get next most significant word of divisor
	div	bx
	stosw		;save this word of the quotient
			;DX contains the remainder at this point,
			; ready to prepend to the next divisor word
	loop	DivLoop
	mov	ax,dx	;return the remainder

	cld		;restore default Direction flag setting
        pop     di      ;restore caller's register variables
        pop     si
        pop     bp      ;restore caller's stack frame
        ret
_Div	endp
	end
