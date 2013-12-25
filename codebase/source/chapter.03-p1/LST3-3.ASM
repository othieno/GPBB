; Test file;
; Measures the performance of 1000 loads of AL from
; memory. (Use by renaming to TESYCODE, which is
; included by PZTEST.ASM (Listing 3.2). PZTIME.BAT
; (Listing 3.4) does this, along with all assembly
; and linking.)
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
     rept 1000
     mov al,[MemVar]
     endm
;
; Stop timing.
;
     call ZTimerOff
