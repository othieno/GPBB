;
; *** Listing 11-4 ***
;
; Measures the performance of adding an immediate value
; to a register, for comparison with Listing 11-5, which
; adds an immediate value to a memory variable.
;
        call    ZTimerOn
        rept    1000
        add     dx,100h
        endm
        call    ZTimerOff