; Calculates TCP/IP (16-bit carry-wrapping) checksum for buffer
;  starting at ESI, of length ECX words.
; Returns checksum in AX.
; ECX and ESI destroyed.
; All cycle counts assume 32-bit protected mode.
; Assumes buffer length > 0.
; Note that timing indicates that the pipe sequence and
;  cycle counts shown (based on documented execution rules)
;  differ from the actual execution sequence and cycle counts;
;  this loop has been measured to execute in 5 cycles; apparently,
;  the 1st half of ADD somehow pairs with the prefix byte, or the
;  prefix byte gets executed ahead of time.

        sub     ax,ax           ;initialize the checksum

ckloop:
        add     ax,[esi]        ;cycle 1 U-pipe prefix byte
                                ;cycle 1 V-pipe idle (no pairing w/prefix)
                                ;cycle 2 U-pipe 1st half of ADD
                                ;cycle 2 V-pipe idle (register contention)
                                ;cycle 3 U-pipe 2nd half of ADD
                                ;cycle 3 V-pipe idle (register contention)
        adc     ax,0            ;cycle 4 U-pipe prefix byte
                                ;cycle 4 V-pipe idle (no pairing w/prefix)
                                ;cycle 5 U-pipe ADC AX,0
        add     esi,2           ;cycle 5 V-pipe
        dec     ecx             ;cycle 6 U-pipe
        jnz     ckloop          ;cycle 6 V-pipe

