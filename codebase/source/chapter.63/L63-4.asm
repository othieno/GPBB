; Listing 4: unoptimized cross product; 36 cycles
        fld     [vec0+4]	;starts & ends on cycle 0
        fmul    [vec1+8]	;starts on cycle 1
        fld     [vec0+8]	;starts & ends on cycle 2
        fmul    [vec1+4]	;starts on cycle 3
				;stalls for cycles 4-5
	fsubrp	st(1),st(0)	;starts on cycle 6
				;stalls for cycles 7-9
	fstp	[vec2+0]	;starts on cycle 10,
				; ends on cycle 11

        fld     [vec0+8]	;starts & ends on cycle 12
        fmul    [vec1+0]	;starts on cycle 13
        fld     [vec0+0]	;starts & ends on cycle 14
        fmul    [vec1+8]	;starts on cycle 15
				;stalls for cycles 16-17
	fsubrp	st(1),st(0)	;starts on cycle 18
				;stalls for cycles 19-21
	fstp	[vec2+4]	;starts on cycle 22,
				; ends on cycle 23

        fld     [vec0+0]	;starts & ends on cycle 24
        fmul    [vec1+4]	;starts on cycle 25
        fld     [vec0+4]	;starts & ends on cycle 26
        fmul    [vec1+0]	;starts on cycle 27
				;stalls for cycles 28-29
	fsubrp	st(1),st(0)	;starts on cycle 30
				;stalls for cycles 31-33
        fstp    [vec2+8]	;starts on cycle 34,
				; ends on cycle 35
