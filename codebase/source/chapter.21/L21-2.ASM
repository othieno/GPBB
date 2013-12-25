; Calculates TCP/IP (16-bit carry-wrapping) checksum for buffer
;  starting at ESI, of length ECX words.
; Returns checksum in AX.
; High word of EAX, DX, ECX and ESI destroyed.
; All cycle counts assume 32-bit protected mode.
; Assumes buffer length > 0.

        sub     eax,eax         ;initialize the checksum
        mov     dx,[esi]        ;first word to checksum
        dec     ecx             ;we'll do 1 checksum outside the loop
        jz      short ckloopend ;only 1 checksum to do
        add     esi,2           ;point to the next word to checksum

ckloop:
        add     al,dl           ;cycle 1 U-pipe
        mov     dl,[esi]        ;cycle 1 V-pipe
        adc     ah,dh           ;cycle 2 U-pipe
        mov     dh,[esi+1]      ;cycle 2 V-pipe
        adc     eax,0           ;cycle 3 U-pipe
        add     esi,2           ;cycle 3 V-pipe
        dec     ecx             ;cycle 4 U-pipe
        jnz     ckloop          ;cycle 4 V-pipe

ckloopend:
        add     ax,dx           ;checksum the last word
        adc     eax,0

