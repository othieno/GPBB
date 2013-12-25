;
; *** Listing 11-1 ***
;
; Measures the performance of an immediate move to
; memory, in order to demonstrate that the prefetch
; queue cycle-eater is alive and well on the AT.
;
        jmp     Skip
;
        even            ;always make sure word-sized memory
                        ; variables are word-aligned!
WordVar dw      0
;
Skip:
        call    ZTimerOn
        rept    1000
        mov     [WordVar],0
        endm
        call    ZTimerOff