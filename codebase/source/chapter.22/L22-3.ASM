; Listing 22.3.  L22-3.ASM

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
	les	di,dword ptr [bp].BufOfs	;load ES:DI with target buffer segment:offset
	mov	cx,[bp].BufSize		;load CX with buffer size
	rep	stosw			;fill the buffer
Bye:
	pop	bp			;restore caller's BP
	ret	EndMrk-RetAddr-2		;return, clearing the parms from the stack
ClearS	endp
