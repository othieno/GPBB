
; *** Listing 17.2 ***
;
; Program to illustrate searching through a buffer of a specified
; length until either a specified byte or a zero byte is encountered.
; A loop unrolled four times and terminated with LOOP is used.

	.model	small
	.stack	100h
	.data
; Sample string to search through.
SampleString	label	byte
	db	'This is a sample string of a long enough length '
	db	'so that raw searching speed can outweigh any '
	db	'extra set-up time that may be required.',0
SAMPLE_STRING_LENGTH	equ	$-SampleString

; User prompt.
Prompt	db	'Enter character to search for:$'

; Result status messages.
ByteFoundMsg	db	0dh,0ah
		db	'Specified byte found.',0dh,0ah,'$'
ZeroByteFoundMsg db	0dh,0ah
		db	'Zero byte encountered.',0dh,0ah,'$'
NoByteFoundMsg	db	0dh,0ah
		db	'Buffer exhausted with no match.',0dh,0ah,'$'

; Table of initial, possibly partial loop entry points for
; SearchMaxLength.
SearchMaxLengthEntryTable	label	word
	dw	SearchMaxLengthEntry4
	dw	SearchMaxLengthEntry1
	dw	SearchMaxLengthEntry2
	dw	SearchMaxLengthEntry3

	.code
Start	proc	near
	mov	ax,@data	;point to standard data segment
	mov	ds,ax

	mov	dx,offset Prompt
	mov	ah,9		;DOS print string function
	int	21h		;prompt the user

	mov	ah,1		;DOS get key function
	int	21h		;get the key to search for

	mov	ah,al		;put character to search for in AH
	mov	cx,SAMPLE_STRING_LENGTH ;# of bytes to search
	mov	si,offset SampleString ;point to buffer to search
	call	SearchMaxLength	;search the buffer
	mov	dx,offset ByteFoundMsg ;assume we found the byte
	jc	PrintStatus	;we did find the byte
				;we didn't find the byte, figure out
				; whether we found a zero byte or
				; ran out of buffer
	mov	dx,offset NoByteFoundMsg
				;assume we didn't find a zero byte
	jcxz	PrintStatus	;we didn't find a zero byte
	mov	dx,offset ZeroByteFoundMsg ;we found a zero byte
PrintStatus:
	mov	ah,9		;DOS print string function
	int	21h		;report status

	mov	ah,4ch		;return to DOS
	int	21h
Start	endp

; Function to search a buffer of a specified length until either a
; specified byte or a zero byte is encountered.
;
; Input:
;	AH = character to search for
;	CX = maximum length to be searched (must be > 0)
;	DS:SI = pointer to buffer to be searched
;
; Output:
;	CX = 0 if and only if we ran out of bytes without finding
;		either the desired byte or a zero byte
;	DS:SI = pointer to searched-for byte if found, otherwise byte
;		after zero byte if found, otherwise byte after last
;		byte checked if neither searched-for byte nor zero
;		byte is found
;	Carry Flag = set if searched-for byte found, reset otherwise

SearchMaxLength	proc	near
	cld
	mov	bx,cx
	add	cx,3		;calculate the maximum # of passes
	shr	cx,1		; through the loop, which is
	shr	cx,1		; unrolled 4 times
	and	bx,3		;calculate the index into the entry
				; point table for the first,
				; possibly partial loop
	shl	bx,1		;prepare for a word-sized look-up
	jmp	SearchMaxLengthEntryTable[bx]
				;branch into the unrolled loop to do
				; the first, possibly partial loop

SearchMaxLengthLoop:
SearchMaxLengthEntry4:
	lodsb			;get the next byte
	cmp	al,ah		;is this the byte we want?
	jz	ByteFound	;yes, we're done with success
	and	al,al		;is this the terminating 0 byte?
	jz	ByteNotFound	;yes, we're done with failure
SearchMaxLengthEntry3:
	lodsb			;get the next byte
	cmp	al,ah		;is this the byte we want?
	jz	ByteFound	;yes, we're done with success
	and	al,al		;is this the terminating 0 byte?
	jz	ByteNotFound	;yes, we're done with failure
SearchMaxLengthEntry2:
	lodsb			;get the next byte
	cmp	al,ah		;is this the byte we want?
	jz	ByteFound	;yes, we're done with success
	and	al,al		;is this the terminating 0 byte?
	jz	ByteNotFound	;yes, we're done with failure
SearchMaxLengthEntry1:
	lodsb			;get the next byte
	cmp	al,ah		;is this the byte we want?
	jz	ByteFound	;yes, we're done with success
	and	al,al		;is this the terminating 0 byte?
	jz	ByteNotFound	;yes, we're done with failure
	loop	SearchMaxLengthLoop ;it's neither, so check the next
				; four bytes, if any
ByteNotFound:
	clc			;return "not found" status
	ret
ByteFound:
	dec	si		;point back to the location at which
				; we found the searched-for byte
	stc			;return "found" status
	ret
SearchMaxLength	endp
	end	Start

