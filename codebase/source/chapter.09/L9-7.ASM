; Listing 9.7
; Program to perform a direct far jump to address 1000:5.
; *** Do not run this program! It's just an example of how ***
; *** to build a direct far jump to an absolute address    ***
;
; Tested with TASM 2 and MASM 5.

FarSeg	segment	at 01000h
	org	5
FarLabel label	far
FarSeg	ends

	.model	small
	.code
start:
	jmp	FarLabel
	end	start

