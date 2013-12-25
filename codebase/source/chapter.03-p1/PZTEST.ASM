;
; *** Listing 3-2 ***
;
; Program to measure performance of code that takes less than
; 54 ms to execute. (PZTEST.ASM)
;
; Link with PZTIMER.ASM (Listing 3-1). PZTEST.BAT (Listing 3-4)
; can be used to assemble and link both files. Code to be
; measured must be in the file TESTCODE; Listing 3-3 shows
; a sample TESTCODE file.
;
; By Michael Abrash
;
mystack	segment	para stack 'STACK'
	db	512 dup(?)
mystack	ends
;
Code	segment	para public 'CODE'
	assume	cs:Code, ds:Code
	extrn	ZTimerOn:near, ZTimerOff:near, ZTimerReport:near
Start	proc	near
	push	cs
	pop	ds	;set DS to point to the code segment,
			; so data as well as code can easily
			; be included in TESTCODE
;
	include	TESTCODE ;code to be measured, including
			; calls to ZTimerOn and ZTimerOff
;
; Display the results.
;
	call	ZTimerReport
;
; Terminate the program.
;
	mov	ah,4ch
	int	21h
Start	endp
Code	ends
	end	Start
