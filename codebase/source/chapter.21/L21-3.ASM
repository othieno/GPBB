; Calculates TCP/IP (16-bit carry-wrapping) checksum for buffer
;  starting at ESI, of length ECX words.
; Returns checksum in AX.
; High word of EAX, BX, EDX, ECX and ESI destroyed.
; All cycle counts assume 32-bit protected mode.
; Assumes buffer length > 0.

        sub     eax,eax         ;initialize the checksum
        sub     edx,edx		;prepare for later ORing
        shr     ecx,1           ;we'll do two words per loop
        jnc     short ckloopsetup ;even number of words
        mov     ax,[esi]        ;do the odd word
        jz      short ckloopdone ;no more words to checksum
        add     esi,2		;point to the next word
ckloopsetup:
	mov	dx,[esi]	;load most of 1st word to
	mov	bl,[esi+2]	; checksum (last byte loaded in loop)
	dec	ecx		;any more dwords to checksum?
	jz	short ckloopend	;no

ckloop:
        mov     bh,[esi+3]      ;cycle 1 U-pipe
        add     esi,4           ;cycle 1 V-pipe
        shl     ebx,16          ;cycle 2 U-pipe
				;cycle 2 V-pipe idle
				; (register contention)
        or      ebx,edx         ;cycle 3 U-pipe
        mov     dl,[esi]        ;cycle 3 V-pipe
        add     eax,ebx         ;cycle 4 U-pipe
        mov     bl,[esi+2]      ;cycle 4 V-pipe
        adc     eax,0           ;cycle 5 U-pipe
        mov     dh,[esi+1]      ;cycle 5 V-pipe
        dec     ecx             ;cycle 6 U-pipe
        jnz     ckloop          ;cycle 6 V-pipe

ckloopend:
        mov     bh,[esi+3]      ;checksum the last dword
	add	ax,dx
	adc	ax,bx
	adc	ax,0

        mov     edx,eax         ;compress the 32-bit checksum
        shr     edx,16          ; into a 16-bit checksum
        add     ax,dx
        adc     eax,0
ckloopdone:

