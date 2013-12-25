; Listing 6: optimized transformation: 34 cycles
        fld     [vec0+0]	;starts & ends on cycle 0
        fmul    [matrix+0]	;starts on cycle 1
	fld	[vec0+0]	;starts & ends on cycle 2
	fmul	[matrix+16]	;starts on cycle 3
	fld	[vec0+0]	;starts & ends on cycle 4
	fmul	[matrix+32]	;starts on cycle 5
	fld	[vec0+4]	;starts & ends on cycle 6
	fmul	[matrix+4]	;starts on cycle 7
	fld	[vec0+4]	;starts & ends on cycle 8
	fmul	[matrix+20]	;starts on cycle 9
	fld	[vec0+4]	;starts & ends on cycle 10
	fmul	[matrix+36]	;starts on cycle 11
	fxch	st(2)		;no cost
	faddp	st(5),st(0)	;starts on cycle 12
	faddp	st(3),st(0)	;starts on cycle 13
	faddp	st(1),st(0)	;starts on cycle 14
	fld	[vec0+8]	;starts & ends on cycle 15
	fmul	[matrix+8]	;starts on cycle 16
	fld	[vec0+8]	;starts & ends on cycle 17
	fmul	[matrix+24]	;starts on cycle 18
	fld	[vec0+8]	;starts & ends on cycle 19
	fmul	[matrix+40]	;starts on cycle 20
	fxch	st(2)		;no cost
	faddp	st(5),st(0)	;starts on cycle 21
	faddp	st(3),st(0)	;starts on cycle 22
	faddp	st(1),st(0)	;starts on cycle 23
	fxch	st(2)		;no cost
	fadd	[matrix+12]	;starts on cycle 24
	fxch	st(1)		;starts on cycle 25
	fadd	[matrix+28]	;starts on cycle 26
	fxch	st(2)		;no cost
	fadd	[matrix+44]	;starts on cycle 27
	fxch	st(1)		;no cost
	fstp	[vec1+0]	;starts on cycle 28,
				; ends on cycle 29
	fstp	[vec1+8]	;starts on cycle 30,
				; ends on cycle 31
	fstp	[vec1+4]	;starts on cycle 32,
				; ends on cycle 33
