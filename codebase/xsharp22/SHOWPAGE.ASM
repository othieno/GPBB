; Shows the page at the specified offset in the bitmap. Page is
; displayed when this routine returns. This code first appeared in
; PC Techniques.
;
; C near-callable as: void ShowPage(unsigned int StartOffset);

INPUT_STATUS_1  equ     03dah   ;Input Status 1 register
CRTC_INDEX      equ     03d4h   ;CRT Controller Index reg
START_ADDRESS_HIGH equ  0ch     ;bitmap start address high byte
START_ADDRESS_LOW equ   0dh     ;bitmap start address low byte

ShowPageParms   struc
        dw      2 dup (?) ;pushed BP and return address
StartOffset dw  ?       ;offset in bitmap of page to display
ShowPageParms   ends

        .model  small
        .code
        public  _ShowPage
_ShowPage       proc    near
        push    bp      ;preserve caller's stack frame
        mov     bp,sp   ;point to local stack frame
; Wait for display enable to be active (status is active low), to be
; sure both halves of the start address will take in the same frame.
        mov     bl,START_ADDRESS_LOW        ;preload for fastest
        mov     bh,byte ptr StartOffset[bp] ; flipping once display
        mov     cl,START_ADDRESS_HIGH       ; enable is detected
        mov     ch,byte ptr StartOffset+1[bp]
        mov     dx,INPUT_STATUS_1
WaitDE:
        in      al,dx
        test    al,01h
        jnz     WaitDE  ;display enable is active low (0 = active)
; Set the start offset in display memory of the page to display.
        mov     dx,CRTC_INDEX
        mov     ax,bx
        out     dx,ax   ;start address low
        mov     ax,cx
        out     dx,ax   ;start address high
; Now wait for vertical sync, so the other page will be invisible when
; we start drawing to it.
        mov     dx,INPUT_STATUS_1
WaitVS:
        in      al,dx
        test    al,08h
        jz      WaitVS  ;vertical sync is active high (1 = active)
        pop     bp      ;restore caller's stack frame
        ret
_ShowPage       endp
        end
