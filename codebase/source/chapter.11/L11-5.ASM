;
; *** Listing 11.5 ***
;
; Measures the performance of adding an immediate value
; to a memory variable, for comparison with Listing 11.4,
; which adds an immediate value to a register.
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
        add     dx,100h
        endm
        call    ZTimerOff