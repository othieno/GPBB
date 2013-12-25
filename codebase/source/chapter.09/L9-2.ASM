; Searches a text buffer for a text string. Uses REPNZ SCASB to scan
; the buffer for locations that match a specified character of the
; searched-for string, then uses REPZ CMPS to check fully only those
; locations that REPNZ SCASB has identified as potential matches.
;     
; C small model-callable as:
;    unsigned char * FindString(unsigned char * Buffer,
;    	unsigned int BufferLength, unsigned char * SearchString,
;	unsigned int SearchStringLength,
;	unsigned int ScanCharOffset);
;
; Returns a pointer to the first match for SearchString in Buffer, or
; a NULL pointer if no match is found. Buffer should not start at
; offset 0 in the data segment to avoid confusing a match at 0 with
; no match found.

Parms	struc
		dw	2 dup(?) ;pushed BP/return address
Buffer		dw	?	;pointer to buffer to search
BufferLength	dw	?	;length of buffer to search
SearchString	dw	?	;pointer to string for which to search
SearchStringLength dw	?	;length of string for which to search
ScanCharOffset	dw	?	;offset in string of character for
				; which to scan
Parms	ends
	.model	small
	.code
	public _FindString
_FindString	proc	near
	push	bp	;preserve caller's stack frame
	mov	bp,sp	;point to our stack frame
	push	si	;preserve caller's register variables
	push	di
	cld		;make string instructions increment pointers
	mov	si,[bp+SearchString] ;pointer to string to search for
	mov	cx,[bp+SearchStringLength] ;length of string
	jcxz	FindStringNotFound ;no match if string is 0 length
	mov	dx,[bp+BufferLength] ;length of buffer
	sub	dx,cx	;difference between buffer and search lengths
	jc	FindStringNotFound ;no match if search string is
			; longer than buffer
	inc	dx	;difference between buffer and search string
			; lengths, plus 1 (# of possible string start
			; locations to check in the buffer)
	mov	di,ds
	mov	es,di
	mov	di,[bp+Buffer]	;point ES:DI to buffer to search thru
	mov	bx,[bp+ScanCharOffset] ;offset in string of character
			; on which to scan
	add	di,bx	;point ES:DI to first buffer byte to scan
	mov	al,[si+bx] ;put the scan character in AL
	inc	bx	;set BX to the offset back to the start of the
			; potential full match after a scan match,
			; accounting for the 1-byte overrun of
			; REPNZ SCASB
FindStringLoop:
	mov	cx,dx	;put remaining buffer search length in CX
	repnz	scasb	;scan for the scan byte
	jnz	FindStringNotFound ;not found, so there's no match
			;found, so we have a potential match-check the
			; rest of this candidate location
	push	di	;remember the address of the next byte to scan
	mov	dx,cx	;set aside the remaining length to search in
			; the buffer
	sub	di,bx	;point back to the potential start of the
			; match in the buffer
	mov	si,[bp+SearchString] ;point to the start of the string
	mov	cx,[bp+SearchStringLength] ;string length
	shr	cx,1	;convert to word for faster search
	jnc	FindStringWord ;do word search if no odd byte
	cmpsb		;compare the odd byte
	jnz	FindStringNoMatch ;odd byte doesn't match, so we
			; haven't found the search string here
FindStringWord:
	jcxz	FindStringFound ;if the string is only 1 byte long,
			; we've found a match
	repz	cmpsw	;check the rest of the string a word at a time
	jz	FindStringFound ;it's a match
FindStringNoMatch:
	pop	di	;get back pointer to the next byte to scan
	and	dx,dx	;is there anything left to check?
	jnz	FindStringLoop ;yes-check next byte
FindStringNotFound:
	sub	ax,ax	;return a NULL pointer indicating that the
	jmp	FindStringDone ; string was not found
FindStringFound:
	pop	ax	;point to the buffer location at which the
	sub	ax,bx	; string was found (earlier we pushed the
			; address of the byte after the scan match)
FindStringDone:
	pop	di	;restore caller's register variables
	pop	si
	pop	bp	;restore caller's stack frame
	ret
_FindString	endp
	end
