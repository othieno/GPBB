; Listing 3: optimized dot product; 15 cycles
        fld     [vec0+0]	;starts & ends on cycle 0
        fmul    [vec1+0]	;starts on cycle 1
        fld     [vec0+4]	;starts & ends on cycle 2
        fmul    [vec1+4]	;starts on cycle 3
        fld     [vec0+8]	;starts & ends on cycle 4
        fmul    [vec1+8]	;starts on cycle 5
	fxch	st(1)		;no cost
        faddp   st(2),st(0)	;starts on cycle 6
				;stalls for cycles 7-8
        faddp   st(1),st(0)	;starts on cycle 9
				;stalls for cycles 10-12
        fstp    [dot]		;starts on cycle 13,
				; ends on cycle 14
