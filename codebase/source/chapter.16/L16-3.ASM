; Assembly subroutine for Listing 16.2. Scans through Buffer, of
; length BufferLength, counting words and updating WordCount as
; appropriate. BufferLength must be > 0. *CharFlag and *WordCount
; should equal 0 on the first call. Tested with TASM.
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
        .code
        public  _ScanBuffer
_ScanBuffer     proc    near
        push    bp              ;preserve caller's stack frame
        mov     bp,sp           ;set up local stack frame
        push    si              ;preserve caller's register vars
        push    di

        mov     si,[bp+Buffer]  ;point to buffer to scan
        mov     bx,[bp+WordCount]
        mov     cx,[bx]         ;get current 32-bit word count
        mov     dx,[bx+2]
        mov     bx,[bp+CharFlag]
        mov     bl,[bx]         ;get current CharFlag
        mov     di,[bp+BufferLength] ;get # of bytes to scan
ScanLoop:
        mov     bh,bl           ;PredCharFlag = CharFlag;
        lodsb                   ;Ch = *BufferPtr++ & 0x7F;
        and     al,7fh          ;strip high bit for word processors
                                ; that set it as an internal flag
        mov     bl,1            ;assume this is a char; CharFlag = 1;
        cmp     al,'a'          ;it is a char if between a and z
        jb      CheckAZ
        cmp     al,'z'
        jna     IsAChar
CheckAZ:
        cmp     al,'A'          ;it is a char if between A and Z
        jb      Check09
        cmp     al,'Z'
        jna     IsAChar
Check09:
        cmp     al,'0'          ;it is a char if between 0 and 9
        jb      CheckApostrophe
        cmp     al,'9'
        jna     IsAChar
CheckApostrophe:
        cmp     al,27h          ;it is a char if an apostrophe
        jz      IsAChar
        sub     bl,bl           ;not a char; CharFlag = 0;
        and     bh,bh
        jz      ScanLoopBottom  ;if ((!CharFlag) && PredCharFlag) {
        add     cx,1            ;    (WordCount)++;
        adc     dx,0            ;}
IsAChar:
ScanLoopBottom:
        dec     di              ;} while (--BufferLength);
        jnz     ScanLoop

        mov     si,[bp+CharFlag]
        mov     [si],bl         ;set new CharFlag
        mov     bx,[bp+WordCount]
        mov     [bx],cx         ;set new word count
        mov     [bx+2],dx

        pop     di              ;restore caller's register vars
        pop     si
        pop     bp              ;restore caller's stack frame
        ret
_ScanBuffer     endp
        end

