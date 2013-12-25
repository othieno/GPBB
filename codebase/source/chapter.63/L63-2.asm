; Listing 2: unoptimized dot product; 17 cycles
        fld     [vec0+0]	;starts & ends on cycle 0
        fmul    [vec1+0]	;starts on cycle 1
        fld     [vec0+4]	;starts & ends on cycle 2
        fmul    [vec1+4]	;starts on cycle 3
        fld     [vec0+8]	;starts & ends on cycle 4
        fmul    [vec1+8]	;starts on cycle 5
				;stalls for cycles 6-7
        faddp   st(1),st(0)	;starts on cycle 8
				;stalls for cycles 9-10
        faddp   st(1),st(0)	;starts on cycle 11
				;stalls for cycles 12-14
        fstp    [dot]		;starts on cycle 15,
				; ends on cycle 16
