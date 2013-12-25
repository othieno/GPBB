; Listing 1: use of fxch to allow addition of first two
; products to start while third multiplication finishes
        fld     [vec0+0]        ;starts & ends on cycle 0
        fmul    [vec1+0]        ;starts on cycle 1
        fld     [vec0+4]        ;starts & ends on cycle 2
        fmul    [vec1+4]        ;starts on cycle 3
        fld     [vec0+8]        ;starts & ends on cycle 4
        fmul    [vec1+8]        ;starts on cycle 5
        fxch    st(1)           ;no cost
        faddp   st(2),st(0)     ;starts on cycle 6
