; Assembly subroutine for Listing 16.2. Scans through Buffer, of
; length BufferLength, counting words and updating WordCount as
; appropriate, using a lookup table-based approach. BufferLength
; must be > 0. *CharFlag and *WordCount should equal 0 on the
; first call. Tested with TASM.
; C near-callable as:
;       void ScanBuffer(char *Buffer, unsigned int BufferLength,
;               char *CharFlag, unsigned long *WordCount);

parms   struc
        dw      2 dup(?)        ;pushed return address & BP
Buffer  dw      ?               ;buffer to scan
BufferLength dw ?               ;length of buffer to scan
CharFlag dw     ?               ;pointer to flag for state of last
                                ; char processed on entry (0 on
                                ; initial call). Updated on exit
WordCount dw    ?               ;pointer to 32-bit count of words
                                ; found (0 on initial call)
parms   ends

        .model  small
        .data
; Table of char/not statuses for byte values 0-255 (128-255 are
; duplicates of 0-127 to effectively mask off bit 7, which some
; word processors set as an internal flag).
CharStatusTable label   byte
        REPT    2
        db      39 dup(0)
        db      1               ;apostrophe
        db      8 dup(0)
        db      10 dup(1)       ;0-9
        db      7 dup(0)
        db      26 dup(1)       ;A-Z
        db      6 dup(0)
        db      26 dup(1)       ;a-z
        db      5 dup(0)
        ENDM

        .code
        public  _ScanBuffer
_ScanBuffer     proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;set up local stack frame
        push    si              ;preserve caller's register vars
        push    di

        mov     si,[bp+Buffer]  ;point to buffer to scan
        mov     bx,[bp+WordCount]
        mov     di,[bx]         ;get current 32-bit word count
        mov     dx,[bx+2]
        mov     bx,[bp+CharFlag]
        mov     al,[bx]         ;get current CharFlag
        mov     cx,[bp+BufferLength] ;get # of bytes to scan
        mov     bx,offset CharStatusTable
ScanLoop:
        and     al,al           ;ZF=0 if last byte was a char,
                                ; ZF=1 if not
        lodsb                   ;get the next byte
                                ;***doesn't change flags***
        xlat                    ;look up its char/not status
                                ;***doesn't change flags***
        jz      ScanLoopBottom  ;don't count a word if last byte was
                                ; not a character
        and     al,al           ;last byte was a character; is the
                                ; current byte a character?
        jz      CountWord       ;no, so count a word
ScanLoopBottom:
        dec     cx              ;count down buffer length
        jnz     ScanLoop
Done:
        mov     si,[bp+CharFlag]
        mov     [si],al         ;set new CharFlag
        mov     bx,[bp+WordCount]
        mov     [bx],di         ;set new word count
        mov     [bx+2],dx

        pop     di              ;restore caller's register vars
        pop     si
        pop     bp              ;restore caller's stack frame
        ret

        align   2
CountWord:
        add     di,1            ;increment the word count
        adc     dx,0
        dec     cx              ;count down buffer length
        jnz     ScanLoop
        jmp     Done
_ScanBuffer     endp
        end

