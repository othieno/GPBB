;
; *** Listing 11-2 ***
;
; Measures the performance of accesses to word-sized
; variables that start at odd addresses (are not
; word-aligned).
;
Skip:
        push    ds
        pop     es
        mov     si,1    ;source and destination are the same
        mov     di,si   ; and both are not word-aligned
        mov     cx,1000 ;move 1000 words
        cld
        call    ZTimerOn
        rep     movsw
        call    ZTimerOff