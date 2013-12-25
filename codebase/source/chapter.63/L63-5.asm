; Listing 5: optimized cross product; 22 cycles
        fld     [vec0+4]	;starts & ends on cycle 0
        fmul    [vec1+8]	;starts on cycle 1
        fld     [vec0+8]	;starts & ends on cycle 2
        fmul    [vec1+0]	;starts on cycle 3
        fld     [vec0+0]	;starts & ends on cycle 4
        fmul    [vec1+4]	;starts on cycle 5
        fld     [vec0+8]	;starts & ends on cycle 6
        fmul    [vec1+4]	;starts on cycle 7
        fld     [vec0+0]	;starts & ends on cycle 8
        fmul    [vec1+8]	;starts on cycle 9
        fld     [vec0+4]	;starts & ends on cycle 10
        fmul    [vec1+0]	;starts on cycle 11
	fxch	st(2)		;no cost
	fsubrp	st(5),st(0)	;starts on cycle 12
	fsubrp	st(3),st(0)	;starts on cycle 13
	fsubrp	st(1),st(0)	;starts on cycle 14
	fxch	st(2)		;no cost
				;stalls for cycle 15
	fstp	[vec2+0]	;starts on cycle 16,
				; ends on cycle 17
	fstp	[vec2+4]	;starts on cycle 18,
				; ends on cycle 19
        fstp    [vec2+8]	;starts on cycle 20,
				; ends on cycle 21
