; Finds and returns the greatest common divisor of two integers.
; Uses Euclid's Algorithm: divides the larger integer by the
; smaller; if the remainder is 0, the smaller integer is the gcd,
; otherwise the smaller integer becomes the larger integer, the
; remainder becomes the smaller integer, and the process is
; repeated. Avoids code recursion.
;
; Tested with MASM and TASM.
;
; C near-callable as:
;        unsigned int gcd(unsigned int int1, unsigned int int2);

; Parameter structure:
parms	struc
	dw	?		;pushed BP
	dw	?		;pushed return address
int1	dw	?		;integers for which to find
int2	dw	?		; the gcd
parms	ends

	.model	small
	.code
	public	_gcd
	align	2
_gcd	proc	near
	push	bp	;preserve caller's stack frame
	mov	bp,sp	;set up our stack frame
	push	si	;preserve caller's register variables
	push	di

;Swap if necessary to make sure that int1 >= int2
	mov	ax,int1[bp]
	mov	bx,int2[bp]
	cmp	ax,bx		;is int1 >= int2?
	jnb	IntsSet		;yes, so we're all set
	xchg	ax,bx		;no, so swap int1 and int2
IntsSet:

; Now loop, dividing int1 by int2 and checking the remainder, until
; the remainder is 0. At each step, if the remainder isn't 0, assign
; int2 to int1, and the remainder to int2, then repeat.
GCDLoop:
				;if the remainder of int1 divided by
				; int2 is 0, then int2 is the gcd
	sub	dx,dx		;prepare int1 in DX:AX for division
	div	bx		;int1/int2; remainder is in DX
	and	dx,dx		;is the remainder zero?
	jz	Done		;yes, so int2 (BX) is the gcd
				;no, so move int2 to int1 and the
                                ; remainder to int2, and repeat the
                                ; process
	mov	ax,bx		;int1 = int2;
	mov	bx,dx		;int2 = remainder from DIV

;---start of loop unrolling; the above is repeated three times---
	sub	dx,dx		;prepare int1 in DX:AX for division
	div	bx		;int1/int2; remainder is in DX
	and	dx,dx		;is the remainder zero?
	jz	Done		;yes, so int2 (BX) is the gcd
	mov	ax,bx		;int1 = int2;
	mov	bx,dx		;int2 = remainder from DIV
;---
	sub	dx,dx		;prepare int1 in DX:AX for division
	div	bx		;int1/int2; remainder is in DX
	and	dx,dx		;is the remainder zero?
	jz	Done		;yes, so int2 (BX) is the gcd
	mov	ax,bx		;int1 = int2;
	mov	bx,dx		;int2 = remainder from DIV
;---
	sub	dx,dx		;prepare int1 in DX:AX for division
	div	bx		;int1/int2; remainder is in DX
	and	dx,dx		;is the remainder zero?
	jz	Done		;yes, so int2 (BX) is the gcd
	mov	ax,bx		;int1 = int2;
	mov	bx,dx		;int2 = remainder from DIV
;---end of loop unrolling---
	jmp	GCDLoop

	align	2
Done:
	mov	ax,bx		;return the gcd
	pop	di		;restore caller's register variables
	pop	si
	pop	bp		;restore caller's stack frame
	ret
_gcd	endp
	end

