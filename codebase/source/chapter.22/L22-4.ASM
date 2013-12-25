; Listing 22.4.  L22-4.ASM

ClearS	proc	near
	push	bp			;save caller's BP
	mov	bp,sp			;point to stack frame
	cmp	word ptr [bp].BufSeg,0	;skip the fill if a null
	jne	Start			; pointer is passed
	cmp	word ptr [bp].BufOfs,0	
	je	Bye
Start: cld					;make STOSW count up
	mov	ax,[bp].Attrib		;load AX with attribute parameter
	sub	al,al			;prepare for merging with fill char
	mov	bx,[bp].Filler		;load BX with fill char
	sub	bh,bh			;prepare for merging with attribute
	or	ax,bx			;combine attribute and fill char
	les	di,dword ptr [bp].BufOfs	;load ES:DI with target buffer segment:offset
	mov	cx,[bp].BufSize		;load CX with buffer size
	rep	stosw			;fill the buffer
Bye:
	pop	bp			;restore caller's BP
	ret	EndMrk-RetAddr-2		;return, clearing the parms from the stack
ClearS	endp
