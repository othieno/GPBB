; Calculates TCP/IP (16-bit carry-wrapping) checksum for buffer
;  starting at ESI, of length ECX words.
; Returns checksum in AX.
; High word of EAX, EBX, ECX, EDX, and ESI destroyed.
; All cycle counts assume 32-bit protected mode.
; Assumes buffer starts on a dword boundary, is a dword multiple
;  in length, and length > 0.

        sub     eax,eax         ;initialize the checksum
        shr     ecx,2           ;we'll do two dwords per loop
        jnc     short noodddword ;is there an odd dword in buffer?
        mov     eax,[esi]       ;checksum the odd dword
        jz      short ckloopdone ;no, done
        add     esi,4           ;point to the next dword
noodddword:
        mov     edx,[esi]       ;preload the first dword
        mov     ebx,[esi+4]     ;preload the second dword
        dec     ecx             ;we'll do 1 checksum outside the loop
        jz      short ckloopend ;only 1 checksum to do
        add     esi,8           ;point to the next dword

ckloop:
        add     eax,edx         ;cycle 1 U-pipe
        mov     edx,[esi]       ;cycle 1 V-pipe
        adc     eax,ebx         ;cycle 2 U-pipe
        mov     ebx,[esi+4]     ;cycle 2 V-pipe
        adc     eax,0           ;cycle 3 U-pipe
        add     esi,8           ;cycle 3 V-pipe
        dec     ecx             ;cycle 4 U-pipe
        jnz     ckloop          ;cycle 4 V-pipe

ckloopend:
        add     eax,edx         ;checksum the last two dwords
        adc     eax,ebx
        adc     eax,0
ckloopdone:
        mov     edx,eax         ;compress the 32-bit checksum
        shr     edx,16          ; into a 16-bit checksum
        add     ax,dx
        adc     eax,0

