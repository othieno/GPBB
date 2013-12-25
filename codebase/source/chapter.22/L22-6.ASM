; Listing 22.6.  L22-6.ASM

ClearS	proc	near
	push	bp			;save caller's BP
	mov	bp,sp			;point to stack frame
	les	di,dword ptr [bp].BufOfs	;load ES:DI with target buffer segment:offset
	mov	ax,es			;put segment where we can test it
	or	ax,di			;is it a null pointer?
	je	Bye			;yes, so we're done
Start: cld					;make STOSW count up
	mov	ah,byte ptr [bp].Attrib[1] ;load AH with attribute
	mov	al,byte ptr [bp].Filler	;load AL with fill char
	mov	cx,[bp].BufSize		;load CX with buffer size
	rep	stosw			;fill the buffer
Bye:
	pop	bp			;restore caller's BP
	ret	EndMrk-RetAddr-2	;return, clearing the parms from the stack
ClearS	endp
