; Calculates TCP/IP (16-bit carry-wrapping) checksum for buffer
;  starting at ESI, of length ECX words.
; Returns checksum in AX.
; High word of EAX, ECX, EDX, and ESI destroyed.
; All cycle counts assume 32-bit protected mode.
; Assumes buffer starts on a dword boundary, is a dword multiple
;  in length, and length > 0.

        sub     eax,eax         ;initialize the checksum
        shr     ecx,1           ;we'll do two words per loop
        mov     edx,[esi]       ;preload the first dword
        add     esi,4           ;point to the next dword
        dec     ecx             ;we'll do 1 checksum outside the loop
        jz      short ckloopend ;only 1 checksum to do

ckloop:
        add     eax,edx         ;cycle 1 U-pipe
        mov     edx,[esi]       ;cycle 1 V-pipe
        adc     eax,0           ;cycle 2 U-pipe
        add     esi,4           ;cycle 2 V-pipe
        dec     ecx             ;cycle 3 U-pipe
        jnz     ckloop          ;cycle 3 V-pipe

ckloopend:
        add     eax,edx         ;checksum the last dword
        adc     eax,0
        mov     edx,eax         ;compress the 32-bit checksum
        shr     edx,16          ; into a 16-bit checksum
        add     ax,dx
        adc     eax,0

