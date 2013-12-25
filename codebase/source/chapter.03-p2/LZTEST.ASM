;
; *** Listing 3-6 ***
;
; Program to measure performance of code that takes longer than
; 54 ms to execute. (LZTEST.ASM)
;
; Link with LZTIMER.ASM (Listing 3-5). LZTEST.BAT (Listing 3-7)
; can be used to assemble and link both files. Code to be
; measured must be in the file TESTCODE; Listing 3-8 shows
; a sample file (LST3-8.ASM) which should be named TESTCODE.
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
	pop	ds	;point DS to the code segment,
			; so data as well as code can easily
			; be included in TESTCODE
;
; Delay for 6-7 seconds, to let the Enter keystroke that started the
; program come back up.
;
	mov	ah,2ch
	int	21h		;get the current time
	mov	bh,dh		;set the current time aside
DelayLoop:
	mov	ah,2ch
	push	bx		;preserve start time
	int	21h		;get time
	pop	bx		;retrieve start time
	cmp	dh,bh		;is the new seconds count less than
				; the start seconds count?
	jnb	CheckDelayTime	;no
	add	dh,60		;yes, a minute must have turned over,
				; so add one minute
CheckDelayTime:
	sub	dh,bh		;get time that's passed
	cmp	dh,7		;has it been more than 6 seconds yet?
	jb	DelayLoop	;not yet
;
	include	TESTCODE 	;code to be measured, including calls
				; to ZTimerOn and ZTimerOff
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
