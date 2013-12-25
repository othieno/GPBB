; Measures the performance of 20000 loads of AL from
; memory. (Use by renaming to TESYCODE, which is
; included by LZTEST.ASM (Listing 3.6). LZTIME.BAT
; (Listing 3.7) does this, along with all assembly
; and linking.)
;
; Note: takes about 10 minutes to assemble on a slow PC
;    if you are using MASM
;
     jmp  Skip ;jump around defined data
;
MemVar   db   ?
;
Skip:
;
; Start timing.
;
     call ZTimerOn
;
     rept 20000
     mov al,[MemVar]
     endm
;
; Stop timing.
;
     call ZTimerOff
