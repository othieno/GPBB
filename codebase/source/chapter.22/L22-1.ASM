; Listing 22.1.  L22-1.ASM

OnStack	struc	;data that's stored on the stack after PUSH BP
OldBP	dw	?	;caller's BP
RetAddr	dw	?	;return address
Filler	dw	?	;character to fill the buffer with
Attrib	dw	?	;attribute to fill the buffer with
BufSize	dw	?	;number of character/attribute pairs to fill
BufOfs	dw	?	;buffer offset
BufSeg	dw	?	;buffer segment
EndMrk	db	?	;marker for the end of the stack frame
OnStack	ends
;
ClearS	proc	near
	push	bp			;save caller's BP
	mov	bp,sp			;point to stack frame
	cmp	word ptr [bp].BufSeg,0	;skip the fill if a null
	jne	Start			; pointer is passed
	cmp	word ptr [bp].BufOfs,0
	je	Bye
Start: cld					;make STOSW count up
	mov	ax,[bp].Attrib		;load AX with attribute parameter
	and	ax,0ff00h			;prepare for merging with fill char
	mov	bx,[bp].Filler		;load BX with fill char
	and	bx,0ffh			;prepare for merging with attribute
	or	ax,bx			;combine attribute and fill char
	mov	bx,[bp].BufOfs		;load DI with target buffer offset
	mov	di,bx
	mov	bx,[bp].BufSeg		;load ES with target buffer segment
	mov	es,bx
	mov	cx,[bp].BufSize		;load CX with buffer size
	rep	stosw			;fill the buffer
Bye:	mov	sp,bp			;restore original stack pointer
	pop	bp			; and caller's BP
	ret	EndMrk-RetAddr-2		;return, clearing the parms from the stack
ClearS	endp
