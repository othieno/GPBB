;
; *** Listing 11-3 ***
;
; Measures the performance of accesses to word-sized
; variables that start at even addresses (are word-aligned).
;
Skip:
        push    ds
        pop     es
        sub     si,si   ;source and destination are the same
        mov     di,si   ; and both are word-aligned
        mov     cx,1000 ;move 1000 words
        cld
        call    ZTimerOn
        rep     movsw
        call    ZTimerOff